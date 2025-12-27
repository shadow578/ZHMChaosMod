#pragma once

namespace InputActionNames
{
    namespace Keyboard
    {
        // Keyboard "W" Key (Digital)
        static inline const std::string c_sUp = "KBMButtonUp";

        // Keyboard "S" Key (Digital)
        static inline const std::string c_sDown = "KBMButtonDown";

        // Keyboard "A" Key (Digital)
        static inline const std::string c_sLeft = "KBMButtonLeft";

        // Keyboard "D" Key (Digital)
        static inline const std::string c_sRight = "KBMButtonRight";

        // Keyboard Controls, Vertical Movement Axis (W/S) (Analog)
        static inline const std::string c_sVertical = "eIAKBMMoveVertical";

        // Keyboard Controls, Horizontal Movement Axis (A/D) (Analog)
        static inline const std::string c_sHorizontal = "eIAKBMMoveHorizontal";

        // Keyboard Controls, Vertical Look Axis (Mouse Y) (Analog)
        static inline const std::string c_sLookVertical = "eIAKBMLookVertical";

        // Keyboard Controls, Horizontal Look Axis (Mouse X) (Analog)
        static inline const std::string c_sLookHorizontal = "eIAKBMLookHorizontal";

        // Keyboard Controls, Pause Button ("ESC" key) (Digital)
        static inline const std::string c_sStart = "KBMButtonStart";

        // Keyboard Controls, Left Mouse Button (Shoot) (Digital)
        static inline const std::string c_sMouseLeft = "KBMMouseLeft";

        // Keyboard Controls, Right Mouse Button (Aim) (Digital)
        static inline const std::string c_sMouseRight = "KBMMouseRight";
    };

    namespace Controller
    {
        // Controller, Left Stick, Vertical Axis (Up/Down) (Analog)
        static inline const std::string c_sLeftStickVertical = "eIAStickLeftVertical_Analog";

        // Controller, Left Stick, Horizontal Axis (Left/Right) (Analog)
        static inline const std::string c_sLeftStickHorizontal = "eIAStickLeftHorizontal_Analog";

        // Controller, Right Stick, Vertical Axis (Up/Down) (Analog)
        static inline const std::string c_sRightStickVertical = "eIAStickRightVertical_Analog";

        // Controller, Right Stick, Horizontal Axis (Left/Right) (Analog)
        static inline const std::string c_sRightStickHorizontal = "eIAStickRightHorizontal_Analog";

        // Controller, Right Trigger (R2) (Analog)
        static inline const std::string c_sTriggerLeft = "eIATriggerLeft_Analog";

        // Controller, Left Trigger (L2) (Analog)
        static inline const std::string c_sTriggerRight = "eIATriggerRight_Analog";

        // Controller, Pause Button ("Options" on DS4) (Digital)
        static inline const std::string c_sStart = "ButtonStart";
    };
};