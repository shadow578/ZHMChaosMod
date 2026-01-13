#include "ChaosMod.h"

#include "Logging.h"

#include <IconsMaterialDesign.h>
#include <imgui.h>

#include "EffectRegistry.h"
#include "Helpers/ImGuiExtras.h"

#include "BuildInfo.h"

#include <set>

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
    std::set<std::string> s_AuthorNames;

    // core authors
	s_AuthorNames.insert("shadow578");

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
    ForeachEffect(false, [p_HasFocus](IChaosEffect* p_pEffect)
        {
            p_pEffect->OnDrawUI(p_HasFocus);
        }
    );

    DrawMainUI(p_HasFocus);
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
	ImGui::SetNextWindowSize({ 400.0f, 350.0f }, ImGuiCond_FirstUseEver);

    ImGui::PushFont(SDK()->GetImGuiBlackFont());
    const auto s_ConfigShowing = ImGui::Begin(ICON_MD_QUESTION_MARK "CHAOS MOD CONFIGURATION", &m_bMenuActive);
    ImGui::PushFont(SDK()->GetImGuiRegularFont());

    if (s_ConfigShowing)
    {
        ImGui::SeparatorText("Settings");
        DrawConfigurationContents();

        ImGui::SeparatorText("Unlockers");
        DrawUnlockersContents();

        ImGui::SeparatorText("About");
        ImGui::TextWrapped(fmt::format(
            "ZHMChaosMod Version {}, developed by {}.",
            BuildInfo::GetDisplayVersion(),
            m_sAuthorNames
        ).c_str());
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

    ImGui::TextUnformatted("Chaos Interval");
    ImGui::SameLine();
    ImGuiEx::DragFloat(
        "##Chaos Interval",
        &m_EffectTimer.m_fIntervalSeconds,
        5.0f,
        120.0f);

    ImGui::TextUnformatted("Effect Duration");
    ImGui::SameLine();
    ImGuiEx::DragFloat(
        "##Effect Duration",
        &m_fFullEffectDuration,
        5.0,
        120.0
    );

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

                Logger::Debug(TAG "Selected voting option {}", s_pVoting->GetName());
            }
        }

        ImGui::EndCombo();
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
            m_qDeferredFrameUpdateActions.push([this, s_Unlocker = s_Unlocker.get()]()
                {
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
    ImGui::SetNextWindowPos({ s_ImgGuiIO.DisplaySize.x - 300.0f, 100.0f }, ImGuiCond_FirstUseEver);

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
    ImGui::SetNextWindowSize({ 700, 800 }, ImGuiCond_FirstUseEver);

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

        ImGui::TextUnformatted(fmt::format(
            "ZHMChaosMod version {} ({}) (targeting ZHMModSDK {}, running on {}); {} effects loaded, {} effects available",
			BuildInfo::GetDisplayVersion(),
            BuildInfo::GetInternalVersion(),
            BuildInfo::c_sTargetSDKVersion,
			SDKVersion(),
            s_aEffects.size(),
            s_nAvailableEffects
        ).c_str());

        ImGui::TextUnformatted(fmt::format(
            "Enable States: MOD={}, USER={}, TIMER={}",
			m_bModEnabled ? "True" : "False",
            m_bUserEnabled ? "True" : "False",
			m_EffectTimer.m_bEnable ? "True" : "False"
        ).c_str());

		auto* s_pVoting = GetCurrentVotingIntegration();
        ImGui::TextUnformatted(fmt::format(
            "Using Voting Integration: {}",
			s_pVoting ? s_pVoting->GetName() : "<null>"
        ).c_str());

		ImGui::Checkbox("Menu Always Visible", &m_bDebugMenuAlwaysVisible);

        ImGui::Separator();

        ImGui::BeginChild("##effect_list_pane", ImVec2(300, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto& s_Effect : s_aEffects)
        {
            if (s_Effect)
            {
				auto s_sEffectName = s_Effect->GetName();
				const auto s_bAvailable = s_Effect->Available();
                if (!s_bAvailable)
                {
                    s_sEffectName += "*";
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.0f, 0.0f, 1.0f));
				}

                if (ImGui::Selectable(
                    s_sEffectName.c_str(),
                    m_pEffectForDebug == s_Effect.get()))
                {
                    m_pEffectForDebug = s_Effect.get();
                    Logger::Debug(TAG "Selected '{}' for debug", s_Effect->GetName());
                }

                if (!s_bAvailable)
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
        m_qDeferredFrameUpdateActions.push([this]()
            {
                Logger::Info(TAG "Calling Start() for '{}'", m_pEffectForDebug->GetName());
                m_pEffectForDebug->Start();
                m_fDebugEffectRemainingTime = 30.0f;
            });
    }

    if (ImGui::Button("Stop() now"))
    {
        m_qDeferredFrameUpdateActions.push([this]()
            {
                Logger::Info(TAG "Calling Stop() for '{}'", m_pEffectForDebug->GetName());
                m_pEffectForDebug->Stop();
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

    if (ImGui::Button("Print Compatibility"))
    {
        Logger::Info(TAG "Compatibility for A='{}':", m_pEffectForDebug->GetName());
        for (const auto& s_pOtherEffect : EffectRegistry::GetInstance().GetEffects())
        {
            constexpr int c_nNamePadding = 30;

            auto s_sOtherName = s_pOtherEffect->GetName();
            s_sOtherName.resize(c_nNamePadding, ' ');

            const auto s_bCompatibleA = m_pEffectForDebug->IsCompatibleWith(s_pOtherEffect.get());
            const auto s_bCompatibleB = s_pOtherEffect->IsCompatibleWith(m_pEffectForDebug);

            Logger::Info(
                TAG " - with B='{}': A>B={} \t| B>A={}",
                s_sOtherName,
                s_bCompatibleA ? "Compatible" : "Incompatible",
                s_bCompatibleB ? "Compatible" : "Incompatible"
            );
        }
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
