#include "ZYoutubeAuthHandler.h"
#include "ZAuthToken.h"
#include "HttpPages.h"

#include <Logging.h>

#include "Helpers/Net/UrlUtils.h"
#include "Helpers/Utils.h"

#include <ixwebsocket/IXHttpServer.h>


#define TAG "[ZYoutubeAuthHandler] "

ZYoutubeAuthHandler::ZYoutubeAuthHandler(const std::string p_sClientId, const bool p_bReadOnly, const int p_nTokenCapturePort)
	: m_sClientId(p_sClientId),
	m_sApiScope(p_bReadOnly ? YT::c_sReadOnlyScope : YT::c_sReadWriteScope),
	m_nTokenCapturePort(p_nTokenCapturePort)
{
}

ZYoutubeAuthHandler::~ZYoutubeAuthHandler()
{
	StopTokenCaptureServer();
	ClearAuthToken();
}

std::string ZYoutubeAuthHandler::GetAuthorizationUrl()
{
	return UrlUtils::BuildQueryUrl("https://accounts.google.com/o/oauth2/v2/auth", {
		{ "client_id", m_sClientId },
		{ "redirect_uri", ("http://localhost:" + std::to_string(m_nTokenCapturePort)) },
		{ "scope", m_sApiScope },
		{ "response_type", "code" },
		{ "access_type", "offline" },
		{ "prompt", "consent" }
		});
}

void ZYoutubeAuthHandler::StartAuthorization(const bool p_bOpenBrowser)
{
	StartTokenCaptureServer();

	if (p_bOpenBrowser)
	{
		std::string s_sAuthUrl = GetAuthorizationUrl();
		Logger::Info(TAG "Opening Twitch authorization URL: {}", s_sAuthUrl);
		Utils::OpenBrowser(s_sAuthUrl);
	}
}

void ZYoutubeAuthHandler::StopAuthorization()
{
	StopTokenCaptureServer();
}

std::shared_ptr<ZAuthToken> ZYoutubeAuthHandler::GetAuthToken()
{
	std::lock_guard s_Lock(m_AuthTokenMutex);
	if (m_pAuthToken)
	{
		return m_pAuthToken;
	}

	return nullptr;
}

void ZYoutubeAuthHandler::StartTokenCaptureServer()
{
	StopTokenCaptureServer();

	m_TokenCaptureServerThread = std::thread(&ZYoutubeAuthHandler::RunTokenCaptureServer, this);
}

void ZYoutubeAuthHandler::StopTokenCaptureServer()
{
	{
		std::lock_guard s_Lock(m_TokenCaptureServerMutex);

		if (m_pTokenCaptureServer)
		{
			m_pTokenCaptureServer->stop();
			m_pTokenCaptureServer = nullptr;
		}
	}

	if (m_TokenCaptureServerThread.joinable())
	{
		m_TokenCaptureServerThread.join();
	}
}

void ZYoutubeAuthHandler::RunTokenCaptureServer()
{
	m_bTokenReceived = false;

	{
		std::lock_guard s_Lock(m_TokenCaptureServerMutex);
		if (m_pTokenCaptureServer)
		{
			return;
		}

		m_pTokenCaptureServer = std::make_unique<ix::HttpServer>(m_nTokenCapturePort, "127.0.0.1");

		m_pTokenCaptureServer->setOnConnectionCallback(
			[this](ix::HttpRequestPtr p_pRequest, std::shared_ptr<ix::ConnectionState>) -> ix::HttpResponsePtr
			{
				const std::string s_sUri = p_pRequest->uri;

				// get uri without query params
				std::string s_sUriPath = s_sUri;
				const auto s_nQueryStart = s_sUri.find_first_of("#?");
				if (s_nQueryStart != std::string::npos)
				{
					s_sUriPath = s_sUri.substr(0, s_nQueryStart);
				}


				Logger::Debug(TAG "HTTP {} {} ({})", p_pRequest->method, s_sUri, s_sUriPath);

				if (s_sUriPath == "/") // /?<bunch of crap from google>
				{
					Logger::Debug(TAG "Serving token capture page");
					return std::make_shared<ix::HttpResponse>(
						200, "OK",
						ix::HttpErrorCode::Ok,
						ix::WebSocketHttpHeaders{ {"Content-Type", "text/html"} },
						GetTokenCapturePage()
					);
				}

				if (s_sUriPath == "/auth") // /auth?code=<code>
				{
					std::string s_sCode;

					auto s_nStartPos = s_sUri.find("code=");
					if (s_nStartPos != std::string::npos)
					{
						s_nStartPos += 5; // len("code=")

						auto s_nEndPos = s_sUri.find('&', s_nStartPos);
						if (s_nEndPos == std::string::npos)
						{
							s_nEndPos = s_sUri.length();
						}

						s_sCode = s_sUri.substr(s_nStartPos, s_nEndPos - s_nStartPos);
					}

					if (s_sCode.empty())
					{
						return std::make_shared<ix::HttpResponse>(
							400, "Bad Request",
							ix::HttpErrorCode::Ok,
							ix::WebSocketHttpHeaders{ {"Content-Type", "text/html"} },
							GetTokenErrorPage("No authorization code received.")
						);
					}

					Logger::Debug(TAG "Received authorization code: {}", s_sCode);

					const auto s_pAuthToken = ZAuthToken::FromAuthCode(
						m_sClientId, 
						s_sCode,
						("http://localhost:" + std::to_string(m_nTokenCapturePort))
					);

					if (!s_pAuthToken)
					{
						return std::make_shared<ix::HttpResponse>(
							500, "Internal Server Error",
							ix::HttpErrorCode::Ok,
							ix::WebSocketHttpHeaders{ {"Content-Type", "text/html"} },
							GetTokenErrorPage("Failed to exchange authorization code for access token.")
						);
					}

					SetAuthToken(s_pAuthToken);

					m_bTokenReceived = true;

					return std::make_shared<ix::HttpResponse>(
						200, "OK",
						ix::HttpErrorCode::Ok,
						ix::WebSocketHttpHeaders{ {"Content-Type", "text/html"} },
						GetTokenSuccessPage()
					);
				}

				return std::make_shared<ix::HttpResponse>(
					404, "Not Found",
					ix::HttpErrorCode::Ok,
					ix::WebSocketHttpHeaders{ {"Content-Type", "text/plain"} },
					"Not Found"
				);
			}
		);


		Logger::Debug(TAG "Starting HTTP server on port {}", m_nTokenCapturePort);
		const auto s_Res = m_pTokenCaptureServer->listen();
		if (!s_Res.first)
		{
			Logger::Error(TAG "Failed to start HTTP server: {}", s_Res.second);
			m_pTokenCaptureServer = nullptr;
			return;
		}

		m_pTokenCaptureServer->start();
	}

	// wait for token to be received
	while (!m_bTokenReceived)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		std::lock_guard s_Lock(m_TokenCaptureServerMutex);
		if (!m_pTokenCaptureServer)
		{
			break;
		}
	}

	// stop the server
	{
		std::lock_guard s_Lock(m_TokenCaptureServerMutex);
		if (m_pTokenCaptureServer)
		{
			m_pTokenCaptureServer->stop();
			m_pTokenCaptureServer = nullptr;
			Logger::Debug(TAG "HTTP server stopped");
		}
	}
}

void ZYoutubeAuthHandler::SetAuthToken(std::shared_ptr<ZAuthToken> p_pToken)
{
	Logger::Debug(TAG "Obtained auth token!");

	{
		std::lock_guard s_Lock(m_AuthTokenMutex);
		m_pAuthToken = p_pToken;
	}

	{
		std::lock_guard s_Lock(m_OnAuthTokenReceivedCallbackMutex);
		if (m_OnAuthTokenReceivedCallback)
		{
			m_OnAuthTokenReceivedCallback(p_pToken);
		}
	}
}
