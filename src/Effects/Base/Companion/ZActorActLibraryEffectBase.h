#pragma once
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

#include "Entity/Bindings/ActLibrary/SStandWaitingActEntityBinding.h"
#include "Entity/Bindings/ActLibrary/SStandDanceMatActEntityBinding.h"
#include "Entity/Bindings/ActLibrary/SLambicDanceActEntityBinding.h"

/**
 * Base class for effects that use the NPCActor custom act library from Companion Mod.
 */
class ZActorActLibraryEffectBase : public virtual ZCompanionModDependentEffectBase
{
  public:
    ZActorActLibraryEffectBase()
        : ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 4, 0)) // NPCActor effect library added
    {
    }

  protected:
    // Act_MR_Stand_Waiting
    SStandWaitingActEntityBinding GetStandWaitingBinding(const TEntityRef<ZActor> p_rActor) const;

    // Act_MR_Stand_Dance_Mat
    SStandDanceMatActEntityBinding GetStandDanceMatBinding(const TEntityRef<ZActor> p_rActor) const;

    // Act_MR_Lambic_Dance
    SLambicDanceActEntityBinding GetLambicDanceBinding(const TEntityRef<ZActor> p_rActor) const;
};
