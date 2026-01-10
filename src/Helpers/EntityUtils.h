#pragma once

#include <Glacier/ZEntity.h>
#include <Glacier/EntityFactory.h>

#include <Logging.h>

#include <vector>
#include <string>
#include <optional>

#define TAG "[EntityUtils] "

namespace Utils
{

    class ZEntityFinder
    {
    public:
        ZEntityFinder() {}

        /**
         * Search for entity with matching id, as per entity template.
         * e.g. 0x859611037148f21b
         */
        ZEntityFinder& EntityID(const uint64_t p_nEntityId)
        {
            m_nEntityId = p_nEntityId;
            return *this;
        }

        /**
         * Search for entity with matching name, as per entity template.
         * e.g. "LD_MapTracker_NPCActor"
         */
        ZEntityFinder& EntityName(const std::string& p_sEntityName)
        {
            m_sEntityName = p_sEntityName;
            return *this;
        }

        /**
         * Search for entity with matching (primary) type name.
         * e.g. "ZLightEntity"
         */
        ZEntityFinder& EntityType(const std::string& p_sEntityType)
        {
            m_sEntityType = p_sEntityType;
            return *this;
        }

        /**
         * Search for (sub-) entity with matching blueprint resource id.
         * This refers to a resource id of a .pc_entityblueprint resource, not .pc_entitytype!
         * e.g. ResId<"[assembly:/_pro/vehicles/templates/vehicle_logic.template?/vehicle_fueltank_a.entitytemplate].pc_entityblueprint">
         */
        template<detail::StringLiteral ResPath>
        ZEntityFinder& BlueprintResource()
        {
            return BlueprintResource(ResId<ResPath>);
        }
        
        ZEntityFinder& BlueprintResource(const ZRuntimeResourceID& p_ridBlueprint)
        {
            m_ridBlueprint = p_ridBlueprint;
            return *this;
        }

        /**
         * Search for entities matching all given parameters.
         * @param p_nMaxResults Limit to a maximum number of results. 0 = no limit.
         */
        std::vector<ZEntityRef> Find(const size_t p_nMaxResults = 0) const;
        ZEntityRef FindFirst() const
        {
            auto s_aResults = Find(1);
            if (!s_aResults.empty())
            {
                return s_aResults[0];
            }
            
            return {};
        }

    private:
        std::optional <uint64_t> m_nEntityId = std::nullopt;
        std::optional <std::string> m_sEntityName = std::nullopt;
        std::optional <std::string> m_sEntityType = std::nullopt;
        std::optional <ZRuntimeResourceID> m_ridBlueprint = std::nullopt;

        bool Evaluate(const ZEntityRef& p_rEntity, ZEntityBlueprintFactoryBase* p_pParentFactory, ZEntityBlueprintFactoryBase* p_pSubFactory, int p_nSubIndex) const;
    };

    /**
     * Attempt to get the name (as per template) of an entity.
     * @param p_Entity Entity reference.
     * @param p_pFactory Entity Blueprint pointer. If nullptr, attempt to find from entity ref.
     * @param p_nSubIndex Entity sub-index in blueprint. If -1, attempt to find from entity ref.
     */
    std::string GetEntityName(const ZEntityRef& p_Entity, ZEntityBlueprintFactoryBase* p_pFactory = nullptr, int p_nSubIndex = -1);

    /**
     * Attempt to get the (primary) type name of an entity.
     * @param p_Entity Entity reference.
     */
    std::string GetEntityTypeName(const ZEntityRef& p_Entity);

    /**
	 * Attempt to get the blueprint factory of an entity, even if it's a sub-entity.
     * @param p_rEntity Entity reference.
     */
    ZEntityBlueprintFactoryBase* GetEntityBlueprintFactoryFor(ZEntityRef p_rEntity);

    /**
     * safely get a property from an entity. Returns std::nullopt if failed or invalid.
	 * @template T The property type.
	 * @param p_rEntity The entity to get the property from. if invalid, returns std::nullopt.
	 * @param p_sProperty The property name. If not found or of wrong type, returns std::nullopt.
     */
    template <typename T>
    std::optional<T> GetProperty(const ZEntityRef& p_rEntity, const std::string& p_sProperty)
    {
        if (!p_rEntity)
        {
            return std::nullopt;
		}

        auto s_PropValue = p_rEntity.GetProperty<T>(p_sProperty);
        if (!s_PropValue.Is<T>())
        {
            Logger::Warn(TAG "Failed to get '{}' on EID {:016X}",
                p_sProperty,
                p_rEntity ? p_rEntity.GetEntity()->GetType()->m_nEntityId : 0
            );
            return std::nullopt;
        }

        return s_PropValue.Get();
    }

    /**
	 * safely set a property on an entity. Returns false if failed.
	 * @template T The property type.
	 * @param p_rEntity The entity to set the property on. if invalid, returns false.
	 * @param p_sProperty The property name. If not found or of wrong type, returns false.
	 * @param p_Value The value to set.
     * @param p_bInvokeChangeHandlers Invoke property change handlers.
     */
    template <typename T>
    inline bool SetProperty(ZEntityRef& p_rEntity, const std::string& p_sProperty, const T& p_Value, bool p_bInvokeChangeHandlers = true)
    {
        if (!p_rEntity)
        {
            return false;
        }

        if (!p_rEntity.SetProperty<T>(p_sProperty, p_Value, p_bInvokeChangeHandlers))
        {
            Logger::Warn(TAG "Failed to set '{}' on EID {:016X}",
                p_sProperty,
                p_rEntity ? p_rEntity.GetEntity()->GetType()->m_nEntityId : 0
            );
            return false;
        }

		return true;
    }
};

#undef TAG
