#pragma once
#include <string>

constexpr std::string GetPageBase(const std::string p_sBody)
{
    std::string s_sHtml = R"html(
<!DOCTYPE html>
<html>
<head>
    <title>ZHM Chaos Mod - YouTube Sign-in</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; background: #1a1a2e; color: #eee; }
        .success { color: #4CAF50; font-size: 48px; 
        .failure { color: #e73a3a; font-size: 48px; }
        .notice { color: #ccc; font-size: 14px; }

        .spinner { border: 4px solid #333; border-top: 4px solid #9146FF; border-radius: 50%; width: 40px; height: 40px; animation: spin 1s linear infinite; margin: 20px auto; }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
    </style>
</head>
<body>
)html";

	s_sHtml += p_sBody;

    s_sHtml += R"html(
</body>
</html>
)html";

    return s_sHtml;
}

constexpr std::string GetTokenCapturePage()
{
    return GetPageBase(R"html(
<h1>Connecting to YouTube...</h1>
<div class="spinner"></div>
<p>Please wait while we complete the sign-in.</p>
<script>
    const params = new URLSearchParams(window.location.search);
    const code = params.get('code');
    const state = params.get('state');
    if (code && state) {
        window.location.href = '/auth?code=' + code + '&state=' + state; // invalid, but expected by server
    } else {
        document.body.innerHTML = '<h1>Sign-in Failed</h1><p>No access token or state received. Please try again.</p>';
    }
</script>
)html");
}

constexpr std::string GetTokenSuccessPage()
{
    return GetPageBase(R"html(
<div class="success">&#10004;</div>
<h1>Sign-in Complete!</h1>
<p>You can close this window and return to the game.</p>
<p class="notice">YouTube voting is now available.</p>
)html");
}

constexpr std::string GetTokenErrorPage(const std::string s_sError = "")
{
    return GetPageBase(R"html(
<div class="failure">&#x2718;</div>
<h1>Sign-in Failed!</h1>
<p>You may want to retry.</p>
<p class="notice"> 
)html"
+ s_sError +
R"html(
</p>
)html");
}