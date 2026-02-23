#pragma once
#include <string>

/**
 * Base interface for Unlocker Effects.
 * Unlockers are special Effects that are responsible for restoring certain game states or features
 * that may have been broken by other effects.
 * Players have the option to trigger Unlockers manually to fix issues they encounter during gameplay.
 */
class IUnlocker
{
public:
    virtual void Start() = 0;
    virtual bool Available() const { return true; }
    
    virtual std::string GetName() const;
    virtual std::string GetDisplayName() const = 0;
    virtual std::string GetDescription() const = 0;
};
