#pragma once
#include "Effects/Base/ZCompanionModDependentEffectBase.h"

#include "Helpers/EntityBindings/SFollowPlayerHelperBinding.h"

/**
 * Base class for effects that use the NPCActor custom player follow helper from Companion Mod.
 * Use GetFollowHelperFor to get the follow player helper binding for a specific actor, and use the input pins on the binding to control the actor.
 */
class ZActorFollowPlayerHelperEffectBase : public virtual ZCompanionModDependentEffectBase
{
public:
	ZActorFollowPlayerHelperEffectBase()
		: ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 3, 0)) // NPCActor custom follow player helper
	{
	}

protected:
	/**
	 * Gets the follow helper binding for a specific actor.
	 */
	SFollowPlayerHelperBinding GetFollowHelperFor(const TEntityRef<ZActor> p_rActor) const;
};
