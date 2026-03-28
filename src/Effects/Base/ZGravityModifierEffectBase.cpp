#include "ZGravityModifierEffectBase.h"

#include <Logging.h>

#include "Helpers/Utils.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

#define TAG "[ZGravityModifierEffectBase] "

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
    // there should only ever be one of these, so we can just lookup by ID.
    auto s_rPhysicsEntity = Utils::ZEntityFinder()
                                .EntityID(EntityId::HM3::FXLogicPhysicsWorld::PhysicsWorld)
                                .FindFirst();

    if (!s_rPhysicsEntity)
    {
        Logger::Debug(TAG "Failed to find PhysicsWorld entity.");
        return;
    }

    if (!Utils::SetProperty<SVector3>(s_rPhysicsEntity, c_sGravityPropertyName, p_vGravity))
    {
        Logger::Debug(TAG "Failed to set gravity property on PhysicsWorld entity.");
    }
}

bool ZGravityModifierEffectBase::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
           // all gravity modifiers are incompatible with each other
           && !Utils::IsInstanceOf<ZGravityModifierEffectBase>(p_pOther);
}
