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
     * Text is always left-aligned.
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
        const auto s_fBarWidth = max(s_fWindowWidth, s_fTextWidth);

        // Draw progress bar without overlay text
        ImGui::ProgressBar(p_fFraction, ImVec2(s_fBarWidth, 0.0f), "");

        // Draw left-aligned text on top of the progress bar
        const auto s_vBarMin = ImGui::GetItemRectMin();
        const auto s_vBarMax = ImGui::GetItemRectMax();
        const auto s_fTextX = s_vBarMin.x + p_fTextPadding;
        const auto s_fTextY = s_vBarMin.y + (s_vBarMax.y - s_vBarMin.y - s_vTextSize.y) * 0.5f;

        ImGui::GetWindowDrawList()->AddText(
            ImVec2(s_fTextX, s_fTextY),
            ImGui::GetColorU32(ImGuiCol_Text),
            p_sOverlayText
        );
    }
};
