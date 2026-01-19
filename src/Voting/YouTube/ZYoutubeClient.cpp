#include "ZYoutubeClient.h"
#include "HttpPages.h"
#include "ZYoutubeBroadcastConnection.h"

#include <Logging.h>

#include "Helpers/Net/UrlUtils.h"
#include "Helpers/Utils.h"

#include <nlohmann/json.hpp>
#include <ixwebsocket/IXHttpServer.h>
#include <ixwebsocket/IXHttpClient.h>

#define TAG "[ZYoutubeClient] "

using json = nlohmann::json;

ZYoutubeClient::ZYoutubeClient(const std::string p_sClientId, const bool p_bReadOnly, const int p_nTokenCapturePort)
	: m_sClientId(p_sClientId),
	m_nTokenCapturePort(p_nTokenCapturePort),
	m_bReadOnlyScope(p_bReadOnly)
{

}

ZYoutubeClient::~ZYoutubeClient()
{
	Disconnect();
}

std::string ZYoutubeClient::GetAuthorizationUrl()
{
	std::string s_sScope = "https://www.googleapis.com/auth/youtube";
	if (m_bReadOnlyScope)
	{
		s_sScope += ".readonly";
	}

	return UrlUtils::BuildQueryUrl("https://accounts.google.com/o/oauth2/v2/auth", {
		{ "client_id", m_sClientId },
		{ "redirect_uri", ("http://localhost:" + std::to_string(m_nTokenCapturePort)) },
		{ "scope", s_sScope },
		{ "response_type", "code" },
		{ "access_type", "offline" },
		{ "prompt", "consent" }
	});
}

void ZYoutubeClient::Connect(const bool p_bOpenBrowser)
{
	Disconnect();
	StartTokenCaptureServer();

	if (p_bOpenBrowser)
	{
		std::string s_sAuthUrl = GetAuthorizationUrl();
		Logger::Info(TAG "Opening Twitch authorization URL: {}", s_sAuthUrl);
		Utils::OpenBrowser(s_sAuthUrl);
	}
}

void ZYoutubeClient::Disconnect()
{
	StopTokenCaptureServer();
	m_pBroadcastConnection = nullptr;
}

bool ZYoutubeClient::IsConnected() const
{
	std::lock_guard s_Lock(m_BroadcastConnectionMutex);
	return m_pBroadcastConnection && m_pBroadcastConnection->IsConnected();
}

ZYoutubeBroadcastConnection* ZYoutubeClient::GetBroadcastConnection()
{
	std::lock_guard s_Lock(m_BroadcastConnectionMutex);
	return m_pBroadcastConnection ? m_pBroadcastConnection.get() : nullptr;
}

void ZYoutubeClient::StartTokenCaptureServer()
{
	m_TokenCaptureServerThread = std::thread(&ZYoutubeClient::RunTokenCaptureServer, this);
}

void ZYoutubeClient::StopTokenCaptureServer()
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

void ZYoutubeClient::RunTokenCaptureServer()
{
	bool s_bTokenReceived = false;

	{
		std::lock_guard s_Lock(m_TokenCaptureServerMutex);
		if (m_pTokenCaptureServer)
		{
			return;
		}

		m_pTokenCaptureServer = std::make_unique<ix::HttpServer>(m_nTokenCapturePort, "127.0.0.1");

		m_pTokenCaptureServer->setOnConnectionCallback(
			[this, &s_bTokenReceived](ix::HttpRequestPtr p_pRequest, std::shared_ptr<ix::ConnectionState>) -> ix::HttpResponsePtr
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


					YT::SAuthToken s_Token;
					if (!GetAuthTokenFromCode(s_sCode, s_Token))
					{
						return std::make_shared<ix::HttpResponse>(
							500, "Internal Server Error",
							ix::HttpErrorCode::Ok,
							ix::WebSocketHttpHeaders{ {"Content-Type", "text/html"} },
							GetTokenErrorPage("Failed to exchange authorization code for access token.")
						);
					}

					{
						Logger::Debug(TAG "Obtained access token, init connection");

						std::lock_guard s_BroadcastLock(m_BroadcastConnectionMutex);
						m_pBroadcastConnection = std::make_unique<ZYoutubeBroadcastConnection>(m_sClientId, s_Token);

						if (!m_pBroadcastConnection->IsConnected())
						{
							m_pBroadcastConnection = nullptr;
							return std::make_shared<ix::HttpResponse>(
								500, "Internal Server Error",
								ix::HttpErrorCode::Ok,
								ix::WebSocketHttpHeaders{ {"Content-Type", "text/html"} },
								GetTokenErrorPage("Failed to connect to the YouTube Live Broadcast.")
							);
						}
					}

					s_bTokenReceived = true;

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
	while (!s_bTokenReceived)
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

bool ZYoutubeClient::GetAuthTokenFromCode(const std::string& p_sAuthCode, YT::SAuthToken& p_Token)
{
	const json s_RequestJson = {
		{"code", p_sAuthCode},
		{"client_id", m_sClientId},
		{"redirect_uri", ("http://localhost:" + std::to_string(m_nTokenCapturePort))},
		{"grant_type", "authorization_code"}
	};

	ix::HttpClient s_Client;
	ix::HttpRequestArgsPtr s_pRequest = s_Client.createRequest();

	s_pRequest->extraHeaders["Content-Type"] = "application/json";
	s_pRequest->extraHeaders["Accept"] = "application/json";

	const auto s_pResponse = s_Client.post(
		"https://oauth2.googleapis.com/token",
		s_RequestJson.dump(),
		s_pRequest
	);

	if (!s_pResponse)
	{
		Logger::Error(TAG "OAuth token exchange request failed!");
		return false;
	}

	if (s_pResponse->statusCode < 200 || s_pResponse->statusCode >= 300)
	{
		Logger::Error(TAG "OAuth token exchange failed: {} {}", s_pResponse->statusCode, s_pResponse->body);
		return false;
	}

	const auto s_ResponseJson = json::parse(s_pResponse->body);

	// only accept bearer token
	p_Token = YT::SAuthToken::FromJson(s_ResponseJson);
	if (!p_Token)
	{
		Logger::Error(TAG "Invalid OAuth token response: {}", s_pResponse->body);
		return false;
	}

	Logger::Debug(TAG "Got access token for scope {}. Expiration in {} seconds", p_Token.m_sScope, p_Token.m_nExpiresIn);
	return true;
}
