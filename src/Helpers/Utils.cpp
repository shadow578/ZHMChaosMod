#include "Utils.h"

#include "IPluginInterface.h"

#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZContentKitManager.h>

SMatrix Utils::InterpolateAffine(const SMatrix& p_A, const SMatrix& p_B, const float32 p_fT)
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


std::vector<ZActor*> Utils::GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified)
{
    std::vector<ZActor*> s_aActors;

    if (!Globals::NextActorId || *Globals::NextActorId <= 0)
    {
        return s_aActors;
    }

    for (uint16 i = 0; i < *Globals::NextActorId; i++)
    {
        auto s_pActor = Globals::ActorManager->m_aActiveActors[i].m_pInterfaceRef;
        if (!s_pActor)
        {
            continue;
        }

        if ((!p_bIncludeDead && s_pActor->IsDead()) ||
            (!p_bIncludePacified && s_pActor->IsPacified()))
        {
            continue;
        }

        s_aActors.push_back(s_pActor);
    }

    return s_aActors;
}

ZActor *Utils::GetRandomActor(const bool p_bRequireAlive)
{
    auto s_aActors = GetActors(!p_bRequireAlive, !p_bRequireAlive);
    if (s_aActors.empty())
    {
        return nullptr;
    }

    return SelectRandomElement(s_aActors);
}

bool Utils::TeleportPlayerTo(const SMatrix p_Position)
{
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player.m_ref)
    {
        return false;
    }

    auto s_PlayerSpatialEntity = s_Player.m_ref.QueryInterface<ZSpatialEntity>();
    if (!s_PlayerSpatialEntity)
    {
        return false;
    }

    s_PlayerSpatialEntity->SetWorldMatrix(p_Position);
    return true;
}

bool Utils::SetPlayerOutfit(const std::string& p_sCommonName, TEntityRef<ZGlobalOutfitKit>& p_OutfitOut)
{
    const auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player)
    {
        return false;
    }

    const auto* s_pContentKitManager = Globals::ContentKitManager;
    if (!s_pContentKitManager)
    {
        return false;
    }

    for (const auto& s_KV : s_pContentKitManager->m_repositoryGlobalOutfitKits)
    {
        const auto& s_OutfitKit = s_KV.second;
        if (!s_OutfitKit || !s_OutfitKit.m_pInterfaceRef)
        {
            continue;
        }

        const auto s_sOutfitCommonName = s_OutfitKit.m_pInterfaceRef->m_sCommonName;
        if (p_sCommonName == s_sOutfitCommonName.c_str())
        {
            p_OutfitOut = s_OutfitKit;
            break;
        }
    }

    if (!p_OutfitOut || !p_OutfitOut.m_pInterfaceRef)
    {
        Logger::Error("SetPlayerOutfit: No outfit kit found with common name '{}'", p_sCommonName);
        return false;
    }


    Functions::ZHitman5_SetOutfit->Call(
        s_Player.m_pInterfaceRef,
        p_OutfitOut,
        0,
        0,
        false,
        false
    );

    return true;
}
