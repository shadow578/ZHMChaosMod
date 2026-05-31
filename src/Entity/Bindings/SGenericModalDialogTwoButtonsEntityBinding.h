#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [assembly:/_pro/design/tutorialsupport.template?/tutorial_ui_genericmodaldialogtwobuttons.entitytemplate].pc_entitytype
struct SGenericModalDialogTwoButtonsEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(ZRuntimeResourceID, m_pTitle);    // Dialog title LINE resource
    PROPERTY(ZRuntimeResourceID, m_pText);     // Dialog text LINE resource
    PROPERTY(ZRuntimeResourceID, PromptText);  // First button text LINE resource
    PROPERTY(ZRuntimeResourceID, PromptText2); // Second button text LINE resource

    INPUT_PIN(Show); // Show the dialog. after showing, there will be no way to check which button the user pressed (feedback handled via output pins, which binding does not support rn)
};
