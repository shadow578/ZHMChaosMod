#pragma once
#include "IChaosEffect.h"

#include "Helpers/EntityBindings/SHM5SingleDoor2Binding.h"

#include <vector>

/**
 * Reusable base class for effects that manipulate doors.
 * Handles door discovery and provides access to the door bindings.
 */
class ZDoorsEffectBase : public virtual IChaosEffect
{
public:
	ZDoorsEffectBase(size_t p_nMinDoors = 50)
		: m_nMinDoors(p_nMinDoors)
	{
	}

	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void OnDrawDebugUI() override;

protected:
	std::vector<SHM5SingleDoor2Binding>& GetDoors() { return m_aDoors; }

private:
	const size_t m_nMinDoors;
	std::vector<SHM5SingleDoor2Binding> m_aDoors;
};
