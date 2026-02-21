#pragma once
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

#include "Helpers/EntityBindings/SShootAtHelperBinding.h"

/**
 * Base class for effects that use the NPCActor custom weapon control helper from Companion Mod.
 * Use GetShootAtHelperFor to get the shoot at helper binding for a specific actor, and use the input pins on the binding to control the actor's weapon.
 */
class ZActorWeaponCustomControlEffectBase : public virtual ZCompanionModDependentEffectBase
{
public:
	ZActorWeaponCustomControlEffectBase()
		: ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 3, 0)) // NPCActor custom weapon control helper
	{
	}

protected:
	/**
	 * Gets the shoot helper binding for a specific actor. 
	 */
	SShootAtHelperBinding GetShootAtHelperFor(const TEntityRef<ZActor> p_rActor) const;
};
