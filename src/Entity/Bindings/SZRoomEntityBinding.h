#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [modules:/zroomentity.class].pc_entitytype (partial)
struct SZRoomEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(bool, m_bVisible);
    PROPERTY(bool, m_bForceVisible);
    PROPERTY(bool, m_bIsPrivate);
    PROPERTY(bool, m_bVisibleInBoxReflection);

    PROPERTY(ZEntityRef, m_eidParent);

    PROPERTY(bool, m_bOutside);
    PROPERTY(bool, m_bOutsideMaster);

    INPUT_PIN(MakeVisible);
    INPUT_PIN(MakeInvisible);
    INPUT_PIN(Detach);
};
