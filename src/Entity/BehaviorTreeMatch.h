#pragma once
#include <Glacier/ZString.h>

/// Behavior tree node names for match properties of spsystem / legacy behaviors.
/// Extract using: strings HITMAN3.exe | grep '^BT_'
namespace BehaviorTreeMatch
{
    inline const ZString BT_AMBIENCE("BT_AMBIENCE"); // lowest priority

    inline const ZString BT_OVERRIDE("BT_OVERRIDE");
    inline const ZString BT_OVERRIDE_STANDING("BT_OVERRIDE_STANDING");
    inline const ZString BT_OVERRIDE_CURIOUS("BT_OVERRIDE_CURIOUS");
    inline const ZString BT_OVERRIDE_CAUTIOUS("BT_OVERRIDE_CAUTIOUS");
    inline const ZString BT_OVERRIDE_SENTRY("BT_OVERRIDE_SENTRY");
    inline const ZString BT_CLOSECOMBAT_ONLY("BT_CLOSECOMBAT_ONLY");

    inline const ZString BT_OVERRIDE_ALL("BT_OVERRIDE_ALL"); // highest priority, will make the actor ignore every other behavior
} // namespace BehaviorTreeMatch