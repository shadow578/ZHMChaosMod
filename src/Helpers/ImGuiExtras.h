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
};
