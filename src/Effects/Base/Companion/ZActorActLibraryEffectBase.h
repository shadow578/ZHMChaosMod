#pragma once
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

#include "Helpers/EntityBindings/ActLibrary/SActLibraryBinding.h"

/**
 * Base class for effects that use the NPCActor custom act libarary follow helper from Companion Mod.
 * Use GetActLibraryFor to get the act library binding for a specific actor, then use the sub-entity bindings on that to start custom acts.
 */
class ZActorActLibraryEffectBase : public virtual ZCompanionModDependentEffectBase
{
  public:
    ZActorActLibraryEffectBase()
        : ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 3, 0)) // TODO currently indev, will be included in 1.4.0?
    {
    }

  protected:
    /**
     * Gets the act library binding for a specific actor.
     */
    SActLibraryBinding GetActLibraryFor(const TEntityRef<ZActor> p_rActor) const;

    SDanceMatActBinding GetDanceMatActFor(const TEntityRef<ZActor> p_rActor) const;
};
