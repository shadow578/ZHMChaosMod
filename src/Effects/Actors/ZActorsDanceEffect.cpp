#include "ZActorsDanceEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/ActorUtils.h"

void ZActorsDanceEffect::LoadResources()
{
    ZActorActLibraryEffectBase::LoadResources();
    ZSoundFXEffectBase::LoadResources();

    m_pSFXResource = ZResourceProvider::Create<"[assembly:/sound/wwise/exportedwwisedata/events/levelspecific_music/music_hokkaido/snowcrane/play_dancemat.wwiseevent].pc_wwisebank">();
}

void ZActorsDanceEffect::OnClearScene()
{
    ZActorActLibraryEffectBase::OnClearScene();
    ZSoundFXEffectBase::OnClearScene();

    m_pSFXResource = nullptr;
    m_aActiveActBindings.clear();
}

void ZActorsDanceEffect::OnDrawDebugUI()
{
    ZActorActLibraryEffectBase::OnDrawDebugUI();
    ZSoundFXEffectBase::OnDrawDebugUI();
}

bool ZActorsDanceEffect::Available() const
{
    return ZActorActLibraryEffectBase::Available() && ZSoundFXEffectBase::Available()
           && m_pSFXResource && m_pSFXResource->IsAvailable();
}

void ZActorsDanceEffect::Start()
{
    // get player position
    const auto s_rPlayer = Utils::GetLocalPlayer();
    if (!s_rPlayer)
    {
        return;
    }

    const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>();
    if (!s_pPlayerSpatial)
    {
        return;
    }

    const auto s_mPlayerTransform = s_pPlayerSpatial->GetObjectToWorldMatrix();
    const auto s_vPlayerPos = s_mPlayerTransform.Pos;

    // get six closest actors to player
    const auto s_aDanceActors = Utils::GetNearbyActors(s_vPlayerPos, 6);
    const auto s_aDancePositions = GetDancePositions(s_vPlayerPos);

    // start dancing
    for (size_t i = 0; i < s_aDanceActors.size(); i++)
    {
        const auto s_rActor = s_aDanceActors[i];
        const auto s_mDancePos = s_aDancePositions[i];

        if (auto s_rDanceMatAct = GetDanceMatActFor(s_rActor.first))
        {
            if (auto s_rActSpatialOpt = s_rDanceMatAct.m_rWaypointSpatial; s_rActSpatialOpt.has_value())
            {
                s_rActSpatialOpt.value().m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mDancePos);

                s_rDanceMatAct.m_MovementType = ZActBehaviorEntity_EMovementType::MT_SNAP; // TP to target, no walking
                s_rDanceMatAct.m_bExpertMode = true;                                       // all are pros :P

                s_rDanceMatAct.Start();
                m_aActiveActBindings.push_back(s_rDanceMatAct);
            }
        }
    }

    // play SFX at first dancer's position
    PlayAt(s_aDancePositions[0], m_pSFXResource->GetResourceID());
}

void ZActorsDanceEffect::Stop()
{
    for (auto& s_Biding : m_aActiveActBindings)
    {
        s_Biding.Cancel();
    }
    m_aActiveActBindings.clear();
}

std::array<SMatrix, 6> ZActorsDanceEffect::GetDancePositions(const float4& p_vBase) const
{
    constexpr float32 c_fDistanceH = 1.0f;
    constexpr float32 c_fDistanceV = 0.8f;

    // build dancer pattern ([0] == p_vBase):
    // [3]   [4]   [5]
    //    [1]   [2]
    //       [0]
    std::array<SMatrix, 6> s_aPositions;

    SMatrix s_mPos = SMatrix();
    s_mPos.Pos = p_vBase;
    s_aPositions[0] = s_mPos;

    s_mPos = SMatrix();
    s_mPos.Pos = p_vBase + float4(-c_fDistanceH, c_fDistanceV, 0.f, 0.f);
    s_aPositions[1] = s_mPos;

    s_mPos = SMatrix();
    s_mPos.Pos = p_vBase + float4(c_fDistanceH, c_fDistanceV, 0.f, 0.f);
    s_aPositions[2] = s_mPos;

    s_mPos = SMatrix();
    s_mPos.Pos = p_vBase + float4(-2.f * c_fDistanceH, 2.f * c_fDistanceV, 0.f, 0.f);
    s_aPositions[3] = s_mPos;

    s_mPos = SMatrix();
    s_mPos.Pos = p_vBase + float4(0.f, 2.f * c_fDistanceV, 0.f, 0.f);
    s_aPositions[4] = s_mPos;

    s_mPos = SMatrix();
    s_mPos.Pos = p_vBase + float4(2.f * c_fDistanceH, 2.f * c_fDistanceV, 0.f, 0.f);
    s_aPositions[5] = s_mPos;

    return s_aPositions;
}

REGISTER_CHAOS_EFFECT(ZActorsDanceEffect);
