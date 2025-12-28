#pragma once
#include <imgui.h>

namespace ImGuiEx
{
    inline bool DragFloat(
        const char* p_sLabel,
        float* p_fValue,
        const float p_fMin,
        const float p_fMax,
        const char* p_sFormat = "%.3f",
        ImGuiSliderFlags p_nFlags = 0)
    {
        return ImGui::DragScalar(
            p_sLabel,
            ImGuiDataType_Float,
            p_fValue,
            0.1f,
            &p_fMin,
            &p_fMax,
            p_sFormat,
            p_nFlags
        );
    }

    inline void ProgressBarTextFit(
        const float p_fFraction,
        const char* p_sOverlayText,
        const float p_fTextPadding = 10.0f)
    {
		const auto s_vTextSize = ImGui::CalcTextSize(p_sOverlayText);
        const auto s_fTextWidth = s_vTextSize.x + (p_fTextPadding * 2.0f);
        const auto s_fWindowWidth = ImGui::GetContentRegionAvail().x;

        ImGui::ProgressBar(
            p_fFraction,
            ImVec2(max(s_fWindowWidth, s_fTextWidth), 0.0f),
            p_sOverlayText
        );
    }
};
