#include "ChaosMod.h"

#include "Logging.h"

#include <IconsMaterialDesign.h>
#include <imgui_internal.h>

#include "EffectRegistry.h"
#include "Helpers/ImGuiExtras.h"

#define TAG "[ChaosModUI] "

std::string EffectDurationToString(const IChaosEffect::EDuration p_Duration)
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

void ChaosMod::OnDrawMenu()
{
    if (ImGui::Button(ICON_MD_QUESTION_MARK " CHAOS MOD"))
    {
        m_bMenuActive = !m_bMenuActive;
    }
}

void ChaosMod::OnDrawUI(const bool p_HasFocus)
{
    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            s_Effect->OnDrawUI(p_HasFocus);
        }
    }

    if (!m_bMenuActive)
    {
        return;
    }

    DrawDebugWindow();

    ImGui::PushFont(SDK()->GetImGuiBlackFont());
    const auto s_Showing = ImGui::Begin("CHAOS MOD", &m_bMenuActive);
    ImGui::PushFont(SDK()->GetImGuiRegularFont());

    if (s_Showing)
    {
        // this is shown even without focus to give user feedback at all times
        const float32 s_fRemainingToNext = m_EffectTimer.m_fIntervalSeconds - m_EffectTimer.GetElapsedSeconds();
        ImGui::ProgressBar(
            s_fRemainingToNext / m_EffectTimer.m_fIntervalSeconds,
            ImVec2(-1.0f, 0.0f),
            fmt::format("Next Effect In {:.1f} Seconds", s_fRemainingToNext).c_str()
        );

        ImGui::SeparatorText("Current Vote");
        for (const auto& s_Effect : m_aCurrentVote)
        {
            ImGui::BulletText(s_Effect->GetDisplayName(true).c_str());
        }

        ImGui::SeparatorText("Active Effects");
        for (const auto& s_ActiveEffect : m_aActiveEffects)
        {
            const float32 s_fPercentRemaining = s_ActiveEffect.m_fTimeRemaining / s_ActiveEffect.m_fDuration;
            ImGui::BulletText(s_ActiveEffect.m_pEffect->GetDisplayName(false).c_str());

            if (s_ActiveEffect.m_pEffect->GetDuration() != IChaosEffect::EDuration::OneShot)
            {
                ImGui::SameLine();
                ImGui::ProgressBar(
                    s_fPercentRemaining,
                    ImVec2(-1.0f, 0.0f),
                    fmt::format("{:.1f}", s_ActiveEffect.m_fTimeRemaining).c_str()
                );
            }
        }

        if (!p_HasFocus)
        {
            ImGui::PopFont();
            ImGui::End();
            ImGui::PopFont();
            return;
        }

        ImGui::SeparatorText("Settings");

        if (ImGui::Checkbox("Enabled", &m_EffectTimer.m_bEnable))
        {
            if (m_EffectTimer.m_bEnable)
            {
                // on enable, prepare first vote
                m_aCurrentVote.clear();
                OnEffectTimerTrigger();
            }
        }


        ImGui::TextUnformatted("Chaos Interval");
        ImGui::SameLine();

        const float32 p_fMinInterval = 5.0f;
        const float32 p_fMaxInterval = 120.0f;

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

        if (ImGui::Button("Open Debug Menu"))
        {
            m_bDebugMenuActive = true;
        }

        ImGui::TextUnformatted(fmt::format("Effects Loaded: {}", EffectRegistry::GetInstance().GetEffects().size()).c_str());

        ImGui::SeparatorText("Unlockers");
        DrawUnlockersSection();
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
}

void ChaosMod::DrawUnlockersSection()
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

void ChaosMod::DrawDebugWindow()
{
    if (!m_bDebugMenuActive)
    {
        return;
    }

    ImGui::PushFont(SDK()->GetImGuiBlackFont());
    const auto s_Showing = ImGui::Begin("CHAOS DEBUG", &m_bDebugMenuActive);
    ImGui::PushFont(SDK()->GetImGuiRegularFont());

    if (s_Showing)
    {
        ImGui::BeginChild("chaos left pane", ImVec2(300, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
        {
            if (s_Effect)
            {
                if (ImGui::Selectable(
                    s_Effect->GetName().c_str(),
                    m_pEffectForDebug == s_Effect.get()))
                {
                    m_pEffectForDebug = s_Effect.get();
                    Logger::Debug(TAG "Selected '{}' for debug", s_Effect->GetName());
                }
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::BeginChild("effect debug pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

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
    ImGui::TextUnformatted(fmt::format("Name:         {}", m_pEffectForDebug->GetName()).c_str());
    ImGui::TextUnformatted(fmt::format("Display Name: {} / {}", m_pEffectForDebug->GetDisplayName(false), m_pEffectForDebug->GetDisplayName(true)).c_str());
    ImGui::TextUnformatted(fmt::format("Available:    {}", m_pEffectForDebug->Available() ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Duration:     {}", EffectDurationToString(m_pEffectForDebug->GetDuration())).c_str());

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