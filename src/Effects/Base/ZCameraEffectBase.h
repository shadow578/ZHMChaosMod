#pragma once

#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZCameraEntity.h>

#include "Helpers/ZTemplateEntitySpawner.h"

class ZCameraEffectBase : public virtual IChaosEffect
{
public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
	bool Available() const override;
    
    void Start() override;
    void Stop() override;

    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

protected:
    inline ZEntityRef GetEffectCameraEntity() const
    {
        return m_EffectCameraEntity;
    }

    inline ZEntityRef GetOriginalCameraEntity() const
    {
        return m_OriginalCameraEntity;
    }

    inline bool IsEffectCameraActive() const
    {
        return m_bEffectCameraActive && m_EffectCameraEntity && m_OriginalCameraEntity;
    }

private:
    bool EnsureCameraEntity();
    std::unique_ptr<ZTemplateEntitySpawner> m_pCameraSpawner;

    bool m_bEffectCameraActive = false;
    ZEntityRef m_EffectCameraEntity;
    ZEntityRef m_OriginalCameraEntity;
};
