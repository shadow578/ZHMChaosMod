#include "ZGravityModifierEffectBase.h"

#include "IPluginInterface.h"
#include "Logging.h"

#include "Helpers/Utils.h"

#define TAG "[ZGravityModifierEffectBase] "

/**
 * Static Entity ID for PhysicsWorld entity.
 * There should only ever be one of these, so we can just lookup by ID.
 * [assembly:/_pro/effects/templates/logic/fx_logic_physicsworld.template?/fx_logic_physicsworld.entitytemplate].pc_entitytype, PhysicsWorld
 */
constexpr uint64_t c_nPhysicsWorldId = 0x859611037148f21b;

/**
 * Property name for gravity SVector3 property.
 */
const std::string c_sGravityPropertyName = "m_vGravity";

const SVector3 c_fDefaultGravity = SVector3(0.f, 0.f, -9.81f);

void ZGravityModifierEffectBase::RestoreDefaultGravity() 
{
    SetGravity(c_fDefaultGravity);
}

void ZGravityModifierEffectBase::SetGravity(const SVector3 p_vGravity)
{
    if (auto p_PhysicsWorld = SDK()->GetEntityById(c_nPhysicsWorldId))
    {
        if (!p_PhysicsWorld.SetProperty(c_sGravityPropertyName, p_vGravity))
        {
            Logger::Debug(TAG "Failed to set gravity property on PhysicsWorld entity.");
        }
    }
}

bool ZGravityModifierEffectBase::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
        // all gravity modifiers are incompatible with each other
        && !Utils::IsInstanceOf<ZGravityModifierEffectBase>(p_pOther);
}
