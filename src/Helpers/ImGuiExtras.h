#pragma once
#include <imgui.h>

namespace ImGuiEx
{
    /**
     * Drop-in replacement of ImGui::SliderFloat with ImGui::DragScalar, for floats. 
     */
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

    /**
     * Like ImGui::ProgressBar, but automatically resizes so text fits fully.
     * @param p_fFraction Progress fraction. 0.0 - 1.0.
     * @param p_sOverlayText Text to overlay in the progress bar.
     * @param p_fTextPadding Horizontal padding of the overlay text.
     */
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
