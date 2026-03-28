#pragma once
#include "Entity/Bindings/EntityBinding.h"

class IRenderPostfilterParametersEntity;
class ZPostfilterGraphBaseNode;
class ZPostfilterBlendEntity;

// [modules:/zpostfilterlayerentity.class].pc_entitytype
struct SPostfilterLayerEntityBinding : SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(TEntityRef<IRenderPostfilterParametersEntity>, m_parametersEntity);
    PROPERTY(TEntityRef<ZPostfilterGraphBaseNode>, m_postfilterGraphNode);
    PROPERTY(TEntityRef<ZPostfilterBlendEntity>, m_blendNode);

    PROPERTY(float32, m_fBlendInDuration);
    PROPERTY(float32, m_fBlendOutDuration);
    PROPERTY(float32, m_fBlendSustainDuration);

    INPUT_PIN(BlendIn);
    INPUT_PIN(BlendOut);
    INPUT_PIN(BlendInOut);
};
