#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZItem.h>

#include "Helpers/ZTemplateEntitySpawner.h"

#include <vector>

/**
 * base class for effects that spawn repository items.
 * heavily based on repository prop spawning from Assets mod.
 */
class ZSpawnRepositoryItemEffectBase : public virtual IChaosEffect
{
public:
    ZSpawnRepositoryItemEffectBase()
    {
    }

    void LoadResources() override;
    void OnClearScene() override;
    bool Available() const override;
    void OnDrawDebugUI() override;

protected:
    struct SRepositoryPropInfo
    {
        /**
		 * repository id.
         */
        std::string m_sID;

        /**
		 * common name, stable for searching across game versions.
         */
        std::string m_sCommonName;

        /**
		 * display name, for showing in UI.
         */
        std::string m_sDisplayName;

        operator bool() const
        {
            return !m_sID.empty();
		}
	};

private:
	static std::vector<SRepositoryPropInfo> m_aRepositoryProps; // static to share between instances

    static bool LoadRepositoryProps();
    static std::string DynamicObjectToString(const ZDynamicObject& p_DynamicObject);

    std::unique_ptr<ZTemplateEntitySpawner> m_pItemSpawnerSpawner;
	std::unique_ptr<ZTemplateEntitySpawner> m_pRepositoryKeywordSpawner;

protected:
    /**
     * Get a list of all available repository props. 
     */
    const std::vector<SRepositoryPropInfo>& GetRepositoryProps() const
    {
        return m_aRepositoryProps;
	}

    /**
	 * Find repository prop by ID.
     */
    SRepositoryPropInfo GetRepositoryPropByID(const std::string& p_sID) const;

    /**
	 * Find repository prop by common name.
     */
	SRepositoryPropInfo GetRepositoryPropByCommonName(const std::string& p_sCommonName) const;

    /**
     * Spawn the given repository prop at the given position.
     * @param p_RepositoryProp Repository prop to spawn.
     * @param s_vPosition World position to spawn at.
	 * @param p_ePhysicsMode Spawned item physics mode. EPM_DYNAMIC to enable physics.
     * @return true if spawning succeeded, false otherwise.
	 */
    bool SpawnRepositoryPropAt(
        const SRepositoryPropInfo& p_RepositoryProp, 
        const float4 s_vPosition, 
        const ZItemSpawner::EPhysicsMode p_ePhysicsMode = ZItemSpawner::EPhysicsMode::EPM_DYNAMIC
    );
};
