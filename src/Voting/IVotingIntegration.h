#pragma once
#include "IChaosEffect.h"

#include <imgui.h>

#include <vector>
#include <string>

/**
 * Interface for voting integration modules.
 */
class IVotingIntegration
{
public:
	/**
	 * Initialize the voting integration.
	 * Called once on mod load.
	 */
	virtual void Initialize() {};

	/**
	 * Called when the voting integration is selected by the user.
	 * After this call, the integration must be ready to handle voting operations until Deactivate is called.
	 */
	virtual void Activate() {};

	/**
	 * Called when the voting integration is deselected by the user.
	 */
	virtual void Deactivate() {};

	/**
	 * Start a vote for the given options.
	 * Voting integrations may only call metadata methods on the IChaosEffect. 
	 * Any other operations are to be considered illegal.
	 * The voting integration shall assume that the options in the vector are valid and compatible with one another.
	 * @param p_aOptions The options to be voted on.
	 */
	virtual void StartVote(const std::vector<IChaosEffect*>& p_aOptions) = 0;

	/**
	 * End the current vote and return the winning option.
	 * If the vote resulted in a tie, the integration may select any of the tied options at random.
	 * After EndVote(), any internal voting options shall be cleared. 
	 * No vote options shall be shown in the Overlay UI after End().
	 * @return The winning option. If no vote was active, nullptr shall be returned.
	 */
	virtual IChaosEffect* EndVote() = 0;

	/**
	 * Draw the configuration UI for the integration.
	 * This is called from the mod's configuration window, so it should only draw ImGui elements.
	 */
	virtual void DrawConfigUI() {};

	/**
	 * Draw the vote options in the overlay UI.
	 * This is called from the mod's overlay window, so it should only draw ImGui elements.
	 */
	virtual void DrawOverlayUI() = 0;

	/**
	 * Get the unique name of the integration.
	 */
	virtual std::string GetName() const;

	/**
	 * Get the display name of the integration.
	 */
	virtual std::string GetDisplayName() const { return GetName(); }
};
