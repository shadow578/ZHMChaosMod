#include "ChaosMod.h"

#include "Logging.h"

#include <IconsMaterialDesign.h>
#include <imgui.h>

#include "EffectRegistry.h"
#include "ZEffectConfigurationAccessor.h"
#include "Helpers/ImGuiExtras.h"
#include "Helpers/CompanionMod.h"
#include "Helpers/ZPerfCounter.h"

#include "BuildInfo.h"

#include <unordered_set>

#define TAG "[ChaosModUI] "

static std::string EffectDurationToString(const IChaosEffect::EDuration p_Duration)
{
    switch (p_Duration)
    {
    case IChaosEffect::EDuration::OneShot:
        return "One-Shot";
    case IChaosEffect::EDuration::Short:
        return "Short";
    case IChaosEffect::EDuration::Full:
        return "Full";
    default:
        return "<INVALID>";
    }
}

void ChaosMod::InitAuthorNames()
{
    std::unordered_set<std::string> s_AuthorNames;

    // core authors
    s_AuthorNames.insert("shadow578");
    s_AuthorNames.insert("OrfeasZ");

    // gather effect authors
    for (const auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect)
        {
            const auto s_sAttribution = s_Effect->GetAttribution();
            for (const auto& s_sName : s_sAttribution)
            {
                s_AuthorNames.insert(s_sName);
            }
        }
    }

    m_sAuthorNames.clear();
    for (const auto& s_AuthorName : s_AuthorNames)
    {
        if (!m_sAuthorNames.empty())
        {
            m_sAuthorNames += ", ";
        }
        m_sAuthorNames += s_AuthorName;
    }
}

void ChaosMod::OnDrawMenu()
{
    if (ImGui::Button(ICON_MD_QUESTION_MARK " CHAOS MOD"))
    {
        m_bMenuActive = !m_bMenuActive;
    }

    if (ImGui::Button(ICON_MD_BUG_REPORT " CHAOS MOD DEBUG"))
    {
        m_bDebugMenuActive = !m_bDebugMenuActive;
    }
}

void ChaosMod::OnDrawUI(const bool p_HasFocus)
{
    ForeachEffect(false, [p_HasFocus](IChaosEffect* p_pEffect) {
        p_pEffect->OnDrawUI(p_HasFocus);
    });

    DrawMainUI(p_HasFocus);
    DrawEffectConfigUI(p_HasFocus);
    DrawOverlayUI(p_HasFocus);
    DrawDebugUI(p_HasFocus);
}

#pragma region Main UI
void ChaosMod::DrawMainUI(const bool p_bHasFocus)
{
    if (!m_bMenuActive || !p_bHasFocus)
    {
        return;
    }

    // start at a sensible size
    ImGui::SetNextWindowSize({450.0f, 500.0f}, ImGuiCond_FirstUseEver);

    ImGui::PushFont(SDK()->GetImGuiBlackFont());
    const auto s_ConfigShowing = ImGui::Begin(ICON_MD_QUESTION_MARK "CHAOS MOD CONFIGURATION", &m_bMenuActive);
    ImGui::PushFont(SDK()->GetImGuiRegularFont());

    if (s_ConfigShowing)
    {
        const auto& s_CompanionMeta = CompanionModUtil::LoadCompanionModInfo(false);
        if (!s_CompanionMeta.m_bPresent)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Companion Mod not detected! Consider installing it to unlock more effects.");
        }

        ImGui::SeparatorText("Settings");
        DrawConfigurationContents();

        ImGui::SeparatorText("Unlockers");
        DrawUnlockersContents();

        ImGui::SeparatorText("About");
        ImGui::TextWrapped(fmt::format("ZHMChaosMod Version {}, developed by {}.", BuildInfo::GetDisplayVersion(), m_sAuthorNames).c_str());
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
}

void ChaosMod::DrawConfigurationContents()
{
    if (ImGui::Checkbox("Enabled", &m_bUserEnabled))
    {
        UpdateEffectTimerEnabled();
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Enable the Chaos Mod.");
    }

    ImGui::TextUnformatted("Chaos Interval");
    ImGui::SameLine();
    if (ImGuiEx::DragFloat(
        "##Chaos Interval",
        &m_EffectTimer.m_fIntervalSeconds,
        5.0f,
        120.0f
        ))
    {
        m_pConfiguration->SetDouble("EffectInterval", m_EffectTimer.m_fIntervalSeconds);
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("At which interval effects trigger.");
    }

    ImGui::TextUnformatted("Effect Duration");
    ImGui::SameLine();
    if(ImGuiEx::DragFloat(
        "##Effect Duration",
        &m_fFullEffectDuration,
        5.0,
        120.0
        ))
    {
        m_pConfiguration->SetDouble("FullEffectDuration", m_fFullEffectDuration);
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("How long a full-time effect lasts. Short effects use half of this.");
    }

    if (ImGui::Checkbox("Use Real-Time Timer", &m_bEffectTimersUseRealtime))
    {
        m_EffectTimer.m_eTimeMode = m_bEffectTimersUseRealtime ? ZTimer::ETimeMode::RealTime : ZTimer::ETimeMode::GameTime;
        m_pConfiguration->SetBool("EffectTimersUseRealtime", m_bEffectTimersUseRealtime);
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Should the effect timer use realtime or in-game time?");
    }

    if (ImGui::Button("Open Effects Configuration"))
    {
        m_bEffectConfigOpen = true;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Configure which effects should be enabled during gameplay");
    }

    ImGui::SeparatorText("Voting");

    auto* s_pVoting = GetCurrentVotingIntegration();

    ImGui::TextUnformatted("Voting Mode");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##Voting Mode", s_pVoting ? s_pVoting->GetDisplayName().c_str() : ""))
    {
        for (auto& s_pOption : EffectRegistry::GetInstance().GetVotingIntegrations())
        {
            if (ImGui::Selectable(
                    s_pOption->GetDisplayName().c_str(),
                    s_pVoting == s_pOption.get()
                ))
            {
                if (s_pVoting)
                {
                    s_pVoting->Deactivate();
                }

                s_pVoting = s_pOption.get();

                m_pVotingIntegration = s_pVoting;
                m_pVotingIntegration->Activate();

                Logger::Debug(TAG "Selected voting option {}", s_pVoting ? s_pVoting->GetName() : "<null>");
            }
        }

        ImGui::EndCombo();
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Select how new effects should be selected.");
    }

    if (s_pVoting)
    {
        s_pVoting->DrawConfigUI();
    }
}

void ChaosMod::DrawUnlockersContents()
{
    for (auto& s_Unlocker : EffectRegistry::GetInstance().GetUnlockers())
    {
        ImGui::BeginDisabled(!s_Unlocker->Available());

        if (ImGui::Button(s_Unlocker->GetDisplayName().c_str()))
        {
            m_qDeferredFrameUpdateActions.push([this, s_Unlocker = s_Unlocker.get()]() {
                Logger::Info(TAG "Starting unlocker '{}'", s_Unlocker->GetName());
                s_Unlocker->Start();
            });
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(s_Unlocker->GetDescription().c_str());
        }

        ImGui::EndDisabled();
    }
}
#pragma endregion

#pragma region Effect Config UI
void ChaosMod::DrawEffectConfigUI(const bool p_bHasFocus)
{
    if (!m_bEffectConfigOpen || !p_bHasFocus)
    {
        return;
    }

    // start at a sensible size
    ImGui::SetNextWindowSize({450.0f, 500.0f}, ImGuiCond_FirstUseEver);

    ImGui::PushFont(SDK()->GetImGuiBlackFont());
    const auto s_ConfigShowing = ImGui::Begin(ICON_MD_SETTINGS "CHAOS MOD EFFECTS CONFIGURATION", &m_bEffectConfigOpen);
    ImGui::PushFont(SDK()->GetImGuiRegularFont());

    if (s_ConfigShowing)
    {
        ImGui::Separator();

        ImGui::BeginChild("##effect_cfg_list_pane", ImVec2(300, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto& s_pEffect : EffectRegistry::GetInstance().GetEffects())
        {
            if (!s_pEffect)
                continue;

            if (ImGui::Selectable(
                    s_pEffect->GetDisplayName(false).c_str(),
                    m_pEffectForConfig == s_pEffect.get()
                ))
            {
                m_pEffectForConfig = s_pEffect.get();
                Logger::Debug(TAG "Selected '{}' for config", s_pEffect->GetName());
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::BeginChild("##effect_cfg_pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

        if (m_pEffectForConfig)
        {
            DrawEffectConfigPane();
        }
        else
        {
            ImGui::TextUnformatted("Select an effect to configure it.");
        }

        ImGui::EndChild();
        ImGui::EndGroup();
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
}

void ChaosMod::DrawEffectConfigPane()
{
    if (!m_pEffectForConfig)
        return;

    std::string s_sAttribution = "";
    for (const auto& s_sName : m_pEffectForConfig->GetAttribution())
    {
        if (!s_sAttribution.empty())
        {
            s_sAttribution += ", ";
        }
        s_sAttribution += s_sName;
    }
    if (s_sAttribution.empty())
    {
        s_sAttribution = "The Chaos Mod Team";
    }

    // center title horizontally in content pane
    const auto s_sTitle = fmt::format("Configuring '{}'", m_pEffectForConfig->GetDisplayName(false));
    const auto s_nContentWidth = ImGui::GetContentRegionAvail().x;
    const auto s_nTitleWidth = ImGui::CalcTextSize(s_sTitle.c_str()).x;

    ImGui::SetCursorPosX((s_nContentWidth - s_nTitleWidth) * 0.5f);
    ImGui::TextUnformatted(s_sTitle.c_str());

    // sub-title with attribution
    const auto s_sSubtitle = fmt::format("by {}", s_sAttribution);
    const auto s_nSubtitleWidth = ImGui::CalcTextSize(s_sSubtitle.c_str()).x;

    ImGui::SetCursorPosX((s_nContentWidth - s_nSubtitleWidth) * 0.5f);
    ImGui::TextDisabled(s_sSubtitle.c_str());

    ImGui::Separator();

    ZEffectConfigurationAccessor s_ConfigAccessor(this, m_pEffectForConfig->GetName());
    m_pEffectForConfig->DrawConfigUI(&s_ConfigAccessor);
}
#pragma endregion

#pragma region Overlay UI
void ChaosMod::DrawOverlayUI(const bool p_bHasFocus)
{
    // show when menu shown or chaos activated by user
    if (!m_bMenuActive && !m_bUserEnabled)
    {
        return;
    }

    // configure overlay window to auto-size and have transparent background
    ImGuiWindowFlags s_OverlayFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoTitleBar;
    if (!p_bHasFocus)
    {
        s_OverlayFlags |= ImGuiWindowFlags_NoBackground;
    }

    // start out at a sensible position
    const auto& s_ImgGuiIO = ImGui::GetIO();
    ImGui::SetNextWindowPos({s_ImgGuiIO.DisplaySize.x - 300.0f, 100.0f}, ImGuiCond_FirstUseEver);

    // overlay has full opacity, even without focus
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

    // progress bar has transparent background and red fill (default is yellow)
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.55f, 0.11f, 0.13f, 1.00f));

    ImGui::PushFont(SDK()->GetImGuiBlackFont());
    const auto s_OvelayShowing = ImGui::Begin(ICON_MD_QUESTION_MARK "##CHAOS MOD OVERLAY", NULL, s_OverlayFlags);
    ImGui::PushFont(SDK()->GetImGuiRegularFont());

    if (s_OvelayShowing)
    {
        DrawOverlayContents();
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void ChaosMod::DrawOverlayContents()
{
    const float32 s_fRemainingToNext = m_EffectTimer.m_fIntervalSeconds - m_EffectTimer.GetElapsedSeconds();
    ImGuiEx::ProgressBarTextFit(
        s_fRemainingToNext / m_EffectTimer.m_fIntervalSeconds,
        fmt::format("Next Effect in {:.0f} Seconds", s_fRemainingToNext).c_str()
    );

    ImGui::SeparatorText("Current Vote");
    GetCurrentVotingIntegration()->DrawOverlayUI();

    ImGui::SeparatorText("Active Effects");
    for (const auto& s_ActiveEffect : m_aActiveEffects)
    {
        float32 s_fPercentRemaining = 0.0f;
        std::string s_sOverlayText = s_ActiveEffect.m_pEffect->GetDisplayName(false);
        if (s_ActiveEffect.m_pEffect->GetDuration() != IChaosEffect::EDuration::OneShot)
        {
            s_fPercentRemaining = s_ActiveEffect.m_fTimeRemaining / s_ActiveEffect.m_fDuration;
            s_sOverlayText += fmt::format(" - {:.0f}s", s_ActiveEffect.m_fTimeRemaining);
        }

        ImGuiEx::ProgressBarTextFit(s_fPercentRemaining, s_sOverlayText.c_str());
    }
}
#pragma endregion

#pragma region Debug UI
void ChaosMod::DrawDebugUI(const bool p_bHasFocus)
{
    if (!m_bDebugMenuActive || (!m_bDebugMenuAlwaysVisible && !p_bHasFocus))
    {
        return;
    }

    // start at a sensible size
    ImGui::SetNextWindowSize({700, 800}, ImGuiCond_FirstUseEver);

    ImGui::PushFont(SDK()->GetImGuiBlackFont());
    const auto s_Showing = ImGui::Begin(ICON_MD_BUG_REPORT "CHAOS MOD DEBUG", &m_bDebugMenuActive);
    ImGui::PushFont(SDK()->GetImGuiRegularFont());

    if (s_Showing)
    {
        const auto& s_aEffects = EffectRegistry::GetInstance().GetEffects();
        size_t s_nAvailableEffects = 0;
        for (const auto& s_Effect : s_aEffects)
        {
            if (s_Effect && s_Effect->Available())
            {
                s_nAvailableEffects++;
            }
        }

        ImGui::TextUnformatted(fmt::format("ZHMChaosMod version {} ({}) (targeting ZHMModSDK {}, running on {}); {} effects loaded, {} effects available", BuildInfo::GetDisplayVersion(), BuildInfo::GetInternalVersion(), BuildInfo::c_sTargetSDKVersion, SDKVersion(), s_aEffects.size(), s_nAvailableEffects).c_str());

        const auto& s_CompanionMeta = CompanionModUtil::LoadCompanionModInfo(false);
        if (s_CompanionMeta.m_bPresent)
        {
            ImGui::TextUnformatted(fmt::format("Companion Mod version {} ({}.{}.{}) detected!", s_CompanionMeta.m_sVersion, s_CompanionMeta.m_Version.m_nMajor, s_CompanionMeta.m_Version.m_nMinor, s_CompanionMeta.m_Version.m_nPatch).c_str());
        }
        else
        {
            ImGui::TextUnformatted("Companion Mod not detected");
        }

        ImGui::TextUnformatted(fmt::format("Enable States: MOD={}, USER={}, TIMER={}", m_bModEnabled ? "True" : "False", m_bUserEnabled ? "True" : "False", m_EffectTimer.m_bEnable ? "True" : "False").c_str());

        auto* s_pVoting = GetCurrentVotingIntegration();
        ImGui::TextUnformatted(fmt::format("Using Voting Integration: {}", s_pVoting ? s_pVoting->GetName() : "<null>").c_str());

        ImGui::Checkbox("Menu Always Visible", &m_bDebugMenuAlwaysVisible);

#if defined(_DEBUG)
        ImGui::Separator();

        ImGui::Checkbox("Test Mode", &m_bTestmodeEnabled);
        ImGui::SameLine();
        ImGui::SliderFloat("##tm_interval", &m_fTestmodeInterval, 2.0f, 30.0f, "Interval: %.1f s");

        if (m_bTestmodeEnabled)
        {
            ImGui::TextUnformatted(fmt::format("Next Effect in {:.1f} seconds. Last effect: '{}' (#{})", m_fTestmodeTimeToNextEffect, m_pEffectForDebug ? m_pEffectForDebug->GetName() : "<none>", m_nTestmodeEffectIndex).c_str());
        }

        ImGui::Separator();

        if (ImGui::Button("Generate Compatibility Matrix"))
        {
            GenerateAndOpenCompatibilityMatrix();
        }
#endif // _DEBUG

        ImGui::Separator();

        ImGui::BeginChild("##effect_list_pane", ImVec2(300, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto& s_Effect : s_aEffects)
        {
            if (s_Effect)
            {
                auto s_sEffectName = s_Effect->GetName();
                const auto s_sEffectDisplayName = s_Effect->GetDisplayName(false);

                const auto s_bHasDisplayName = !s_sEffectDisplayName.empty() && s_sEffectDisplayName != s_sEffectName;

                const auto s_bAvailable = s_Effect->Available();
                if (!s_bAvailable)
                {
                    s_sEffectName += "*";
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.0f, 0.0f, 1.0f));
                }
                else if (!s_bHasDisplayName)
                {
                    s_sEffectName += "?";
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
                }

                if (ImGui::Selectable(
                        s_sEffectName.c_str(),
                        m_pEffectForDebug == s_Effect.get()
                    ))
                {
                    m_pEffectForDebug = s_Effect.get();
                    Logger::Debug(TAG "Selected '{}' for debug", s_Effect->GetName());
                }

                if (!s_bAvailable || !s_bHasDisplayName)
                {
                    ImGui::PopStyleColor();
                }
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::BeginChild("##effect_debug_pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

        if (m_pEffectForDebug)
        {
            DrawEffectDebugPane();
        }
        else
        {
            ImGui::TextUnformatted("No effect selected for debug.");
        }

        ImGui::EndChild();
        ImGui::EndGroup();
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
}

void ChaosMod::DrawEffectDebugPane()
{
    std::string s_sAttribution = "";
    for (const auto& s_sName : m_pEffectForDebug->GetAttribution())
    {
        if (!s_sAttribution.empty())
        {
            s_sAttribution += ", ";
        }
        s_sAttribution += s_sName;
    }

    ImGui::TextUnformatted(fmt::format("Name:         {}", m_pEffectForDebug->GetName()).c_str());
    ImGui::TextUnformatted(fmt::format("Display Name: {} / {}", m_pEffectForDebug->GetDisplayName(false), m_pEffectForDebug->GetDisplayName(true)).c_str());
    ImGui::TextUnformatted(fmt::format("Attribution:  {}", s_sAttribution).c_str());
    ImGui::TextUnformatted(fmt::format("Duration:     {}", EffectDurationToString(m_pEffectForDebug->GetDuration())).c_str());
    ImGui::TextUnformatted(fmt::format("Available:    {}", m_pEffectForDebug->Available() ? "Yes" : "No").c_str());

    ImGui::BeginDisabled(!m_pEffectForDebug->Available());
    if (ImGui::Button("Start() now"))
    {
        m_qDeferredFrameUpdateActions.push([this]() {
            Logger::Info(TAG "Calling Start() for '{}'", m_pEffectForDebug->GetName());
            m_fDebugEffectRemainingTime = 30.0f;

            ZPerfCounter s_Counter;
            s_Counter.Start();

            m_pEffectForDebug->Start();

            const auto s_fElapsedUs = s_Counter.Stop();
            Logger::Debug(TAG "Start() for '{}' took {:.2f} us", m_pEffectForDebug->GetName(), s_fElapsedUs);
        });
    }

    if (ImGui::Button("Stop() now"))
    {
        m_qDeferredFrameUpdateActions.push([this]() {
            Logger::Info(TAG "Calling Stop() for '{}'", m_pEffectForDebug->GetName());

            ZPerfCounter s_Counter;
            s_Counter.Start();

            m_pEffectForDebug->Stop();

            const auto s_fElapsedUs = s_Counter.Stop();
            Logger::Debug(TAG "Stop() for '{}' took {:.2f} us", m_pEffectForDebug->GetName(), s_fElapsedUs);
        });
    }

    ImGui::TextUnformatted("Debug Effect Remaining Time:");
    ImGui::SameLine();
    if (ImGui::Button("0.5 s"))
    {
        m_fDebugEffectRemainingTime = 0.5f;
    }
    ImGui::SameLine();
    ImGuiEx::DragFloat(
        "##Debug Effect Remaining",
        &m_fDebugEffectRemainingTime,
        0.0f,
        60.0f
    );

    if (ImGui::Button("Trigger Effect Activation"))
    {
        m_qDeferredFrameUpdateActions.push([this]() {
            ActivateEffect(m_pEffectForDebug);
        });
    }

    ImGui::EndDisabled();

    ImGui::Separator();

    if (m_pEffectForDebug->Available())
    {
        m_pEffectForDebug->OnDrawDebugUI();
    }
    else
    {
        ImGui::TextUnformatted("Effect Debug UI not drawn, as effect is not available.");
    }
}
#pragma endregion
