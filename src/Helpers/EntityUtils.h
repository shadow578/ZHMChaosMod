#pragma once

#include <Glacier/ZEntity.h>
#include <Glacier/EntityFactory.h>

#include <vector>
#include <string>

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
};