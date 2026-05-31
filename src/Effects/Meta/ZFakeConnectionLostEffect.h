#pragma once
#include "IChaosEffect.h"

#include "Entity/Bindings/SGenericModalDialogTwoButtonsEntityBinding.h"

class ZFakeConnectionLostEffect : public IChaosEffect
{
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;

    void Start() override;

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Unstable Internet";
    }

  private:
    SGenericModalDialogTwoButtonsEntityBinding m_FakeDisconnectDialog;
};