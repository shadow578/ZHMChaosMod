#pragma once
#include <Glacier/ZString.h>

/// Behavior tree node names for match properties of spsystem / legacy behaviors.
/// Extract using: strings HITMAN3.exe | grep '^BT_'
namespace BehaviorTreeMatch
{
    static const ZString BT_AMBIENCE("BT_AMBIENCE"); // lowest priority

    static const ZString BT_OVERRIDE("BT_OVERRIDE");
    static const ZString BT_OVERRIDE_STANDING("BT_OVERRIDE_STANDING");
    static const ZString BT_OVERRIDE_CURIOUS("BT_OVERRIDE_CURIOUS");
    static const ZString BT_OVERRIDE_CAUTIOUS("BT_OVERRIDE_CAUTIOUS");
    static const ZString BT_OVERRIDE_SENTRY("BT_OVERRIDE_SENTRY");
    static const ZString BT_CLOSECOMBAT_ONLY("BT_CLOSECOMBAT_ONLY");

    static const ZString BT_OVERRIDE_ALL("BT_OVERRIDE_ALL"); // highest priority, will make the actor ignore every other behavior
} // namespace BehaviorTreeMatch