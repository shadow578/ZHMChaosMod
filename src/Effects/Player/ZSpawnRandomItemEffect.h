#pragma once
#include "IChaosEffect.h"

#include "Glacier/ZResource.h"

#include <vector>

// heavily based on Assets mod
class ZSpawnRandomItemEffect : public IChaosEffect
{
public:
    ZSpawnRandomItemEffect(const std::string m_sDisplayName, const int p_nCount)
        : m_sDisplayName(std::move(m_sDisplayName)),
        m_pCount(p_nCount)
    {

    }

    void LoadResources() override;
    void Start() override;
    void OnDrawDebugUI() override;
    void OnClearScene() override;

    std::string GetName() const override
    {
        return IChaosEffect::GetName() + "_" + std::to_string(m_pCount);
    }

    std::string GetDisplayName() const override
    {
        return m_sDisplayName;
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

    ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
    {
        return IChaosEffect::AlwaysActiveLifecycleMethods() |
            ELifecycleMethodFlag::OnClearScene;
    }

private:
    std::string m_sDisplayName;
    int m_pCount;

    bool SpawnRepositoryPropAt(const ZRepositoryID& p_RepositoryId, const SMatrix s_Transform);

    bool LoadRepositoryProps();
    std::string DynamicObjectToString(const ZDynamicObject& p_DynamicObject);
};
