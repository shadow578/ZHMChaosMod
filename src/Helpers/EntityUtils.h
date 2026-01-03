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
            // by entity id. 0 = no search by id
            uint64_t m_nEntityId = 0;

            // by entity name. empty = no search by name
            std::string m_sEntityName = "";

            // by entity type. empty = no search by type
            std::string m_sEntityType = "";

			// by (sub-entity) blueprint resource id. 0 = no search by blueprint id
			// this is the id of a .pc_entityblueprint resource, not .pc_entitytype!
			ZRuntimeResourceID m_ridBlueprint = ZRuntimeResourceID(0);

			// maximum result count. 0 = no limit
			size_t m_nMaxResults = 0;
        };

        /**
         * Search for entities matching given parameters. Parameters are combined with OR logic. 
         */
        std::vector<ZEntityRef> FindEntities(const SSearchParams& p_Params);
    };

    std::string GetEntityName(const ZEntityRef& p_Entity, ZEntityBlueprintFactoryBase* p_pFactory = nullptr, int p_nSubIndex = -1);

    std::string GetEntityTypeName(const ZEntityRef& p_Entity);

    /**
	 * Get the blueprint factory for the given entity, even if it's a sub-entity.
     */
    ZEntityBlueprintFactoryBase* GetEntityBlueprintFactoryFor(ZEntityRef p_rEntity);

    /**
     * safely get a property from an entity. Returns std::nullopt if failed.
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
