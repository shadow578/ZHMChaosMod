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
    namespace EntityFinder
    {
        struct SSearchParams
        {
            /**
             * Search for entity with matching id, as per entity template.
             * e.g. 0x859611037148f21b
             */
            std::optional <uint64_t> m_nEntityId = std::nullopt;

            /**
             * Search for entity with matching name, as per entity template.
             * e.g. "LD_MapTracker_NPCActor"
             */
            std::optional <std::string> m_sEntityName = std::nullopt;

            /**
             * Search for entity with matching (primary) type name.
             * e.g. "ZLightEntity"
             */
            std::optional <std::string> m_sEntityType = std::nullopt;

            /**
             * Search for (sub-) entity with matching blueprint resource id.
             * This refers to a resource id of a .pc_entityblueprint resource, not .pc_entitytype!
             * e.g. ResId<"[assembly:/_pro/vehicles/templates/vehicle_logic.template?/vehicle_fueltank_a.entitytemplate].pc_entityblueprint">
             */
            std::optional <ZRuntimeResourceID> m_ridBlueprint = std::nullopt;

            /**
             * Limit to a maximum number of results.
             * When this number of results is reached, the search ends early.
             */
			std::optional<size_t> m_nMaxResults = std::nullopt;
        };

        /**
         * Search for entities matching given parameters. 
         * Parameters are combined with OR logic. 
         */
        std::vector<ZEntityRef> FindEntities(const SSearchParams& p_Params);
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
