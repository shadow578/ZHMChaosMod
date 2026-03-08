#pragma once
#include "Effects/Base/ZPlayerCrippleBoxEffectBase.h"

/**
 * Debug effect for testing Cripple Box flags.
 */
class ZPlayerCrippleBoxDbgEffect final : public virtual ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override {}

    void OnDrawDebugUI() override;

    bool IsEnabled() const override
    {
        return false;
    }

  private:
    bool m_bReapplyOnChange = true;
};