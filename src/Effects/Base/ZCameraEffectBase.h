#pragma once

#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZCameraEntity.h>

#include "Helpers/ZTemplateEntitySpawner.h"

/**
 * Reusable base for camera effects.
 * Handles spawning, enabling and disabling of a effect camera via Start() and Stop().
 */
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
    /**
     * Effect camera, if IsEffectCameraActive() == true.
     */
    inline ZEntityRef GetEffectCameraEntity() const
    {
        return m_EffectCameraEntity;
    }

    /**
     * Stock Player camera, if IsEffectCameraActive() == true.
     */
    inline ZEntityRef GetOriginalCameraEntity() const
    {
        return m_OriginalCameraEntity;
    }

    /**
     * Is the effect's camera spawned and currently active?
     */
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
