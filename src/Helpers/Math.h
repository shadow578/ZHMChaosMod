#pragma once
#include "Glacier/ZMath.h"
#include "Glacier/SColorRGB.h"

#include <vector>
#include <random>
#include <type_traits>

namespace Math
{
    template <typename T>
    static T GetRandomNumber(const T p_Min, const T p_Max)
    {
        static_assert(std::is_arithmetic<T>::value, "GetRandomNumber only supports arithmetic types.");

        static thread_local std::mt19937 s_Generator{ std::random_device{}() };

        if constexpr (std::is_integral<T>::value)
        {
            std::uniform_int_distribution<T> s_Distribution(p_Min, p_Max);
			return s_Distribution(s_Generator);
        }
        else
        {
            std::uniform_real_distribution<T> s_Distribution(p_Min, p_Max);
            return s_Distribution(s_Generator);
        }
    }

    template <typename T>
    static T SelectRandomElement(const std::vector<T>& p_aElements)
    {
        if (p_aElements.empty())
        {
            throw std::runtime_error("SelectRandomElement called with empty vector.");
        }

        const auto s_Index = GetRandomNumber<size_t>(0, p_aElements.size() - 1);
        return p_aElements[s_Index];
    }

    template <typename T>
    static T Interpolate(const T& p_A, const T& p_B, const float32 p_fT)
    {
        if (p_fT <= 0.0f)
        {
            return p_A;
        }
        else if (p_fT >= 1.0f)
        {
            return p_B;
        }

        return p_A + (p_B - p_A) * p_fT;
    }

    SMatrix InterpolateAffine(const SMatrix& p_A, const SMatrix& p_B, const float32 p_fT);

    static SColorRGB GetRainbowColor(const float32 p_fTime, const float32 p_fOffset)
    {
        constexpr float32 c_fTwoPi = 6.28318530718f;

        float32 s_fT = p_fTime + p_fOffset * c_fTwoPi;

        return SColorRGB{
            .r = (std::sin(s_fT) + 1.0f) * 0.5f,
            .g = (std::sin(s_fT + (c_fTwoPi / 3.0f)) + 1.0f) * 0.5f,
            .b = (std::sin(s_fT + (2.0f * c_fTwoPi / 3.0f)) + 1.0f) * 0.5f
        };
    }
};
