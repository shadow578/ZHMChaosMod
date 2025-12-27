#pragma once

#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZCameraEntity.h>

class ZCameraEffectBase : public virtual IChaosEffect
{
public:
    void Start() override;
    void Stop() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

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

    bool m_bEffectCameraActive = false;
    ZEntityRef m_EffectCameraEntity;
    ZEntityRef m_OriginalCameraEntity;
};
