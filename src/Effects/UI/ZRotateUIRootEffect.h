#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

class ZRotateUIRootEffect : public IChaosEffect
{
  public:
    ZRotateUIRootEffect(const std::string p_sNameSuffix, const std::string p_sDisplayName, const SVector3 p_vRotationRate) : m_sNameSuffix(p_sNameSuffix),
                                                                                                                             m_sDisplayName(p_sDisplayName),
                                                                                                                             m_vRotationRate(p_vRotationRate)
    {
    }

    void OnEngineInitialized() override;
    void OnModUnload() override;
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_Other) const override;

    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;

    std::string GetName() const override
    {
        return IChaosEffect::GetName() + "_" + m_sNameSuffix;
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return m_sDisplayName;
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short;
    }

  private:
    std::string m_sNameSuffix;
    std::string m_sDisplayName;
    SVector3 m_vRotationRate;

  private:
    bool m_bActive = false;
    float32 m_fTimeElapsed = 0.0f;
    ZEntityRef m_rUIRoot;

    void OnFrameUpdateAlways(const SGameUpdateEvent& p_UpdateEvent);
};
