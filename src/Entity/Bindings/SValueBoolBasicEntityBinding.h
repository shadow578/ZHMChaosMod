#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [modules:/zvalueboolbasic.class].pc_entitytype
struct SValueBoolBasicEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY_RO(bool, m_bValue);

    INPUT_PIN(SetTrue);
    INPUT_PIN(SetFalse);
    INPUT_PIN(Toggle);
};
