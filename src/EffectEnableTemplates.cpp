#include "ChaosMod.h"

std::vector<ChaosMod::SEffectEnableTemplate> ChaosMod::g_aEffectEnableTemplates = {
    {.m_sName = "SA/SO Compatible", //
     .m_sDescription = "Disables all effects that could ruin Silent Asassin or Suit Only runs.",
     .m_bDefaultEnabled = true,
     .m_mEffectEnableStates = {
         {"ZActorsFireRandomlyEffect", false},
         {"ZAuthoritativeFigureEffect", false},
         {"ZExplodeActorsOnTouchEffect", false},
         {"ZExplodeAllVehiclesEffect", false},
         {"ZExplodeNearbyActorsEffect", false},
         {"ZExplodeRandomActorEffect", false},
         {"ZExplosiveKnockoutEffect", false},
         {"ZSwapPlayerWithActorEffect", false},
     }},
    {.m_sName = "No Camera Effects", //
     .m_sDescription = "Disables all Camera Effects.",
     .m_bDefaultEnabled = true,
     .m_mEffectEnableStates = {
         {"ZBlurryVisionEffect", false},
         {"ZCameraWaveFOVEffect", false},
         {"ZCameraWideFOVEffect", false},
         {"ZCameraZoomFOVEffect", false},
         {"ZFirstPersonCameraEffect", false},
         {"ZFlipCameraEffect", false},
         {"ZFollowingActorCameraEffect", false},
         {"ZLaggyCameraEffect_delayed_normal", false},
         {"ZLaggyCameraEffect_lag_extreme", false},
         {"ZLaggyCameraEffect_lag_normal", false},
         {"ZOverheadCameraEffect", false},
     }}
};
