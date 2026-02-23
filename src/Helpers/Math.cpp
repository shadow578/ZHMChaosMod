#include "Math.h"

SMatrix Math::InterpolateAffine(const SMatrix& p_A, const SMatrix& p_B, const float32 p_fT)
{
    if (p_fT <= 0.0f)
    {
        return p_A;
    }
    if (p_fT >= 1.0f)
    {
        return p_B;
    }

    // decompose to DirectX components
    DirectX::XMVECTOR s_ScaleA, s_RotationA, s_TranslationA,
        s_ScaleB, s_RotationB, s_TranslationB;
    DirectX::XMMatrixDecompose(&s_ScaleA, &s_RotationA, &s_TranslationA, p_A.DX());
    DirectX::XMMatrixDecompose(&s_ScaleB, &s_RotationB, &s_TranslationB, p_B.DX());

    if (DirectX::XMVectorGetX(DirectX::XMQuaternionDot(s_RotationA, s_RotationB)) < 0.0f)
        s_RotationB = DirectX::XMVectorNegate(s_RotationB);

    // lerp translation and scale
    const auto s_TranslationO = DirectX::XMVectorLerp(s_TranslationA, s_TranslationB, p_fT);
    const auto s_ScaleO = DirectX::XMVectorLerp(s_ScaleA, s_ScaleB, p_fT);

    // slerp rotation to avoid distortions
    const auto s_RotationO = DirectX::XMQuaternionSlerp(s_RotationA, s_RotationB, p_fT);

    // recompose and convert back to SMatrix
    const auto s_MatrixO = DirectX::XMMatrixAffineTransformation(
        s_ScaleO,
        DirectX::XMVectorZero(),
        s_RotationO,
        s_TranslationO
    );
    return SMatrix(s_MatrixO);
}
