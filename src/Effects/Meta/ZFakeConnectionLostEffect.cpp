#include "ZFakeConnectionLostEffect.h"

#include "Registry.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

void ZFakeConnectionLostEffect::OnEnterScene()
{
    auto s_rDialog = Utils::ZEntityFinder()
                         .EntityID(EntityId::CompanionMod::GameEssentials::UIModalDisconnectedFake)
                         .FindFirst();
    m_FakeDisconnectDialog = SGenericModalDialogTwoButtonsEntityBinding(s_rDialog);
}

void ZFakeConnectionLostEffect::OnClearScene()
{
    m_FakeDisconnectDialog = {};
}

bool ZFakeConnectionLostEffect::Available() const
{
    return IChaosEffect::Available() && m_FakeDisconnectDialog;
}

void ZFakeConnectionLostEffect::Start()
{
    m_FakeDisconnectDialog.Show();
}

REGISTER_CHAOS_EFFECT(ZFakeConnectionLostEffect)
