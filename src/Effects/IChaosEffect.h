#pragma once

#include "IPluginInterface.h"

#include <string>
#include <set>

/**
 * Interface for all chaos effects.
 * Effects must - at minimum - implement Start() for one-shot effects.
 */
class IChaosEffect
{
public:
    enum class EDuration
    {
        /**
         * Effect has no persistent effect.
         */
        OneShot,

        /**
         * Persistent Effectr running for shortened (1/2) duration.
         */
        Short,

        /**
         * Persistent Effect running for full duration.
         */
        Full
    };

public: // lifecycle
    /**
     * Called when the mod itself is initialized.
     * Forwards @see IPluginInterface::Init
     */
    virtual void OnModInitialized() {};

    /**
     * Called when the mod itself unloads.
     * Forwards @see IPluginInterface::~IPluginInterface
     */
    virtual void OnModUnload() {};

    /**
     * Called when the engine is fully initialized.
     * Forwards @see IPluginInterface::OnEngineInitialized
     */
    virtual void OnEngineInitialized() {};

    /**
     * Called when a new scene finished loading and enters play mode.
     */
    virtual void OnEnterScene() {};

    /**
     * Called when the current scene unloads.
     * Effects shall clean up any spawned entities, as they will no longer be valid.
     */
    virtual void OnClearScene() {};

    /**
     * Called at a suitable time for loading of dynamic resources to occur.
     * At this point, the engine is initialized (after OnEngineInitialized).
     * A Scene may or may not be loaded at this point.
     * Note that LoadResources may be called multiple times during the mod's lifetime.
	 * Effects should release loaded resources in OnClearScene as appropriate.
     */
    virtual void LoadResources() {};

public: // Effect logic
    /**
     * Called every frame during play mode (not when paused).
     * @param p_UpdateEvent Engine update event containing timing information.
     * @param p_fEffectTimeRemaining Time remaining for the effect in seconds, or 0 if effect is inactive.
     */
    virtual void OnFrameUpdate(const SGameUpdateEvent &p_UpdateEvent, const float32 p_fEffectTimeRemaining) {};

    /**
     * Called during frame update during play mode (not when paused), but at a lower frequency.
     * Expected to be called at least every 0.5 seconds.
     * Effects that need to do more expensive operations regularly should do so here instead of in OnFrameUpdate.
     * @param p_fDeltaTime Game time delta since the last call, in seconds.
     * @param p_fEffectTimeRemaining Time remaining for the effect in seconds, or 0 if effect is inactive.
     */
    virtual void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) {};

    /**
     * Called for drawing effect-specific UI in the ZHMModSDK menu, allowing
     * effects to create custom windows.
     * Forwards @see IPluginInterface::OnDrawUI
     */
    virtual void OnDrawUI(const bool p_HasFocus) {};

    /**
     * Called within the rendering pass of the ChaosMod debug menu, allowing
     * effects to display debug information (when selected for debug).
     * since a window is already created, effects only need to use ImGui controls.
     */
    virtual void OnDrawDebugUI() {};

    /**
     * Called to start the effect.
     * One-Shot effects should do their activities here.
     */
    virtual void Start() = 0;

    /**
     * Called shortly before the next effect.
     * Persistent effects should stop their activities here.
     */
    virtual void Stop() {};

public: // Metadata
    /**
     * Check whether the effect is available to be used.
     * Unavailable effects will not be selected by the ChaosMod.
	 * When unavailable, *effect logic* methods will not be called.
	 * *lifecycle* and *metadata* methods are unaffected.
     */
    virtual bool Available() const { return m_bIsAvailable; }

    /**
     * Check whether "other" can be activated or active at the same
     * time as this effect without causing issues.
     */
    virtual bool IsCompatibleWith(const IChaosEffect *p_pOther) const { return this != p_pOther; }

    /**
     * Get the internal name of the effect, e.g. for debug menu and settings.
     */
    virtual std::string GetName() const;

    /**
     * Get the display name of the effect, e.g. for enduser facing UI.
     */
    virtual std::string GetDisplayName(const bool p_bVoting) const { return GetName(); }

    /**
     * Get for how long this effect should remain active.
     * Duration does NOT affect calls to the effect, only how long it is remaining active.
     * OneShot effects DO have Stop() called after some delay.
     */
    virtual EDuration GetDuration() const { return EDuration::Full; }

    /**
     * Get the display name of the person who authored this effect.
     */
    virtual std::set<std::string> GetAttribution() const { return {}; }

protected:
    bool m_bIsAvailable = true;
};
