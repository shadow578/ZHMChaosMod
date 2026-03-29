#include "ActorUtils.h"
#include "Math.h"

#include <Logging.h>

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

#define TAG "[ActorUtils] "

std::vector<TEntityRef<ZActor>> Utils::GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified)
{
    std::vector<TEntityRef<ZActor>> s_aActors;

    if (!Globals::ActorManager)
    {
        return s_aActors;
    }

    for (auto& s_rActor : Globals::ActorManager->m_activatedActors)
    {
        if (!s_rActor)
        {
            continue;
        }

        if ((!p_bIncludeDead && s_rActor.m_pInterfaceRef->IsDead()) || (!p_bIncludePacified && s_rActor.m_pInterfaceRef->IsPacified()))
        {
            continue;
        }

        s_aActors.push_back(s_rActor);
    }

    return s_aActors;
}

TEntityRef<ZActor> Utils::GetRandomActor(const bool p_bRequireAlive)
{
    auto s_aActors = GetActors(!p_bRequireAlive, !p_bRequireAlive);
    if (s_aActors.empty())
    {
        return {};
    }

    return Math::SelectRandomElement(s_aActors);
}

std::vector<std::pair<TEntityRef<ZActor>, float32>> Utils::GetNearbyActors(
    const float4& p_vPosition,
    const int p_nMaxCount,
    const float32 p_fRadius,
    const bool p_bIncludeDead,
    const bool p_bIncludePacified
)
{
    if (p_nMaxCount == 0)
    {
        return {};
    }

    TEntityRef<ZActor> s_rClosestActor = {};
    float32 s_fClosestDistance = FLT_MAX;
    std::vector<std::pair<TEntityRef<ZActor>, float32>> s_aNearbyActors;
    for (auto& s_rActor : Utils::GetActors(p_bIncludeDead, p_bIncludePacified))
    {
        const auto* s_pActorSpatial = s_rActor.m_entityRef.QueryInterface<ZSpatialEntity>();
        if (!s_pActorSpatial)
        {
            continue;
        }

        auto s_ActorWM = s_pActorSpatial->GetObjectToWorldMatrix();
        const auto s_fDistance = float4::Distance(p_vPosition, s_ActorWM.Trans);

        // skip if out of range
        if (s_fDistance > p_fRadius)
        {
            continue;
        }

        // track closest actor for n==1 case
        if (s_fClosestDistance > s_fDistance)
        {
            s_fClosestDistance = s_fDistance;
            s_rClosestActor = s_rActor;
        }

        // only populate list if we need more than 1
        // we use a shortcut here to avoid unnecessary calculations
        if (p_nMaxCount > 1)
        {
            s_aNearbyActors.emplace_back(s_rActor, s_fDistance);
        }
    }

    if (p_nMaxCount == 1)
    {
        s_aNearbyActors.emplace_back(s_rClosestActor, s_fClosestDistance);
        return s_aNearbyActors;
    }

    // for n > 1, sort by distance and return the closest n
    std::sort(
        s_aNearbyActors.begin(),
        s_aNearbyActors.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    );

    if (s_aNearbyActors.size() > p_nMaxCount)
    {
        s_aNearbyActors.resize(p_nMaxCount);
    }

    return s_aNearbyActors;
}

TEntityRef<ZSpatialEntity> Utils::GetActorHeadAttachEntity(const TEntityRef<ZActor> p_rActor)
{
    return TEntityRef<ZSpatialEntity>(GetSubEntity(p_rActor.m_entityRef, EntityId::HM3::NPCActor::Head));
}

TEntityRef<ZHM5ItemWeapon> Utils::GetMainWeapon(const TEntityRef<ZActor> p_rActor)
{
    if (!p_rActor || !p_rActor.m_pInterfaceRef->m_pInventoryHandler)
    {
        return {};
    }

    return p_rActor.m_pInterfaceRef->m_pInventoryHandler->m_rMainWeapon;
}

class ZAddItemToActorInventoryHelper
{
  private:
    TEntityRef<ZActor> m_rActor;

  public:
    ZAddItemToActorInventoryHelper(TEntityRef<ZActor> p_rActor) : m_rActor(p_rActor)
    {
    }

    void ItemCreatedHandler(const uint32 p_nTicket, const TEntityRef<IItemBase> p_rItem)
    {
        if (p_rItem && m_rActor && m_rActor.m_pInterfaceRef->m_pInventoryHandler)
        {
            for (auto& s_PendingItem : m_rActor.m_pInterfaceRef->m_pInventoryHandler->m_aPendingItems)
            {
                if (s_PendingItem.m_nTicket == p_nTicket)
                {
                    const auto s_rIItem = TEntityRef<IItem>(p_rItem.m_entityRef);
                    const auto* s_pZHMItem = p_rItem.m_entityRef.QueryInterface<ZHM5Item>();
                    if (s_rIItem && s_pZHMItem && s_pZHMItem->m_pItemConfigDescriptor)
                    {
                        s_PendingItem.m_rItem = s_rIItem;
                        s_PendingItem.m_eAttachLocation = s_pZHMItem->m_pItemConfigDescriptor->m_ItemConfig.m_ItemHandsIdle == eItemHands::IH_TWOHANDED
                                                              ? EAttachLocation::eALRifle
                                                              : EAttachLocation::eALRightHand;
                    }

                    break;
                }
            }

            Functions::ZActorInventoryHandler_FinalizePendingItems->Call(m_rActor.m_pInterfaceRef->m_pInventoryHandler);

            Logger::Debug(TAG "[ZAddItemToActorInventoryHelper] Finalized Item add Ticket#{}", p_nTicket);
        }

        // this is somewhat hacky, as the delegate remains
        // however, the engine normally doesn't call the delegate more than once, so we should be fine.
        delete this;
    }
};

bool Utils::AddAndEquipWeapon(TEntityRef<ZActor> p_rActor, const ZRepositoryID& p_ridWeapon, bool p_bReplaceMainWeapon)
{
    if (!p_bReplaceMainWeapon && IsArmed(p_rActor))
    {
        return false;
    }

    if (!p_rActor || !p_rActor.m_pInterfaceRef->m_pInventoryHandler
        || !Globals::EntityManager || !Functions::ZEntityManager_GenerateDynamicObjectID
        || !Globals::WorldInventory || !Functions::ZWorldInventory_RequestNewItem
        || !Globals::WorldInventory_InvalidTicket
        || !Functions::ZActorInventoryHandler_FinalizePendingItems)
    {
        return false;
    }

    // make a copy of the actor ref bc for some reason GenerateDynamicObjectID clears it.
    auto s_rActorCopy = p_rActor.m_entityRef;
    const auto s_nNewObjectId = Functions::ZEntityManager_GenerateDynamicObjectID->Call(
        Globals::EntityManager,
        s_rActorCopy,
        EDynamicEntityType::eDET_CharacterInventoryItem,
        0
    );

    // NOTE: s_pInventoryAddHelper freed in ItemCreatedHandler
    auto* s_pInventoryAddHelper = new ZAddItemToActorInventoryHelper(p_rActor);
    const ZMemberDelegate<ZAddItemToActorInventoryHelper, void(uint32, TEntityRef<IItemBase>)> s_Delegate(
        s_pInventoryAddHelper,
        &ZAddItemToActorInventoryHelper::ItemCreatedHandler
    );

    // RequestNewItem also clears the actor ref for some reason
    s_rActorCopy = p_rActor.m_entityRef;
    const auto s_nTicket = Functions::ZWorldInventory_RequestNewItem->Call(
        Globals::WorldInventory,
        p_ridWeapon,
        s_Delegate,
        s_nNewObjectId,
        false,
        {},
        s_rActorCopy
    );

    if (s_nTicket == *Globals::WorldInventory_InvalidTicket)
    {
        // note: RequestNewItem should NEVER call the delegate if it fails, so we can
        // safely free the helper here.
        delete s_pInventoryAddHelper;
        return false;
    }

    // add pending item
    // ZAddItemToActorInventoryHelper will handle finalizing the item add
    ZActorInventoryHandler::SPendingItemInfo s_PendingItem{
        .m_nTicket = s_nTicket,
        .m_eAttachLocation = EAttachLocation::eALUndefined,
        .m_eMaxTension = EGameTension::EGT_Undefined,
        .m_bLeftHand = false,
        .m_bWeapon = true
    };
    p_rActor.m_pInterfaceRef->m_pInventoryHandler->m_aPendingItems.push_back(s_PendingItem);
    return true;
}

bool Utils::SetRagdollPhysicsEnabled(TEntityRef<ZActor> p_rActor, const bool p_bEnablePhysicsSystem)
{
    ZEntityRef s_rPhysicsSystem = Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::HM3::NPCActor::PhysicsSystem);
    if (!s_rPhysicsSystem)
    {
        Logger::Debug(TAG "could not get PhysicsSystem sub-entity of NPCActor {}", p_rActor.m_pInterfaceRef ? p_rActor.m_pInterfaceRef->GetActorName() : "<null>");
        return false;
    }

    return Utils::SetProperty<bool>(s_rPhysicsSystem, "m_bActive", p_bEnablePhysicsSystem);
}
