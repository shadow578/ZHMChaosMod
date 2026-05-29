#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include <Glacier/Enums.h>
#include <Glacier/ZMath.h>

// [modules:/zuicontrolentity.class].pc_entitytype
// + [modules:/zuicontrollayoutlegacyaspect.class].pc_entitytype
// also applicable to many other UI controls, e.g. [assembly:/templates/ui/controls/basiccontrols.template?/container.entitytemplate].pc_entitytype and most .uic
struct SZUIControlEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    // [modules:/zuicontrolentity.class].pc_entitytype
    PROPERTY(ZUIControlEntity_EAlignment, m_eAlignment);
    PROPERTY(SVector2, m_vAlignment);
    PROPERTY(SVector3, m_vPositionOffset);
    PROPERTY(SVector3, m_vRotation);
    PROPERTY(float32, m_fAlpha); // 0 - 100
    PROPERTY(SVector2, m_vLayoutSize);
    PROPERTY(bool, m_bUseContentSize);
    PROPERTY(ZUIControlEntity_EScaleMode, m_eScaleMode);
    PROPERTY(SVector2, m_vScaleManual);
    PROPERTY(SVector2, m_vScaleResolutionBase);
    PROPERTY(bool, m_bIsContainer);
    PROPERTY(ZEntityRef, m_pLayoutEntity);
    PROPERTY(bool, m_bIsVisible);

    // [modules:/zuicontrollayoutlegacyaspect.class].pc_entitytype
    PROPERTY(bool, m_bIsLegacyLayoutActive);
    PROPERTY(ZUIControlLayoutLegacyAspect_EAlignmentType, m_eAlignmentType);
    PROPERTY(ZUIControlLayoutLegacyAspect_ELayoutMode, m_eSizeMode);
    PROPERTY(SVector2, m_vSize);
};
