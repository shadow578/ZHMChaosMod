#pragma once
#include "Base/ZSpawnRepositoryItemEffectBase.h"

class ZItemRainEffect : public ZSpawnRepositoryItemEffectBase
{
public:
    ZItemRainEffect(
		const std::string p_sNameSuffix, 
		const std::string p_sDisplayName, 
		const std::string p_sFixedRepositoryId = "", 
		const float32 p_fSpawnInterval = 0.5f,
		const size_t p_nItemsPerBatch = 1
	)
        : ZSpawnRepositoryItemEffectBase(),
		m_sNameSuffix(p_sNameSuffix),
		m_sDisplayName(p_sDisplayName),
		m_sFixedRepositoryId(p_sFixedRepositoryId),
		m_fSpawnInterval(p_fSpawnInterval),
		m_nItemsPerBatch(p_nItemsPerBatch)
    {
    }

	void Start() override { m_bActive = true; }
	void Stop() override { m_bActive = false; }
	void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

	std::string GetName() const override
	{
		return ZSpawnRepositoryItemEffectBase::GetName() + "_" + m_sNameSuffix;
	}

    std::string GetDisplayName(const bool p_bVoting) const override
    {
		return m_sDisplayName;
    }

private:
	const std::string m_sNameSuffix;
	const std::string m_sDisplayName;
	const std::string m_sFixedRepositoryId;
	const float32 m_fSpawnInterval;
	const size_t m_nItemsPerBatch;
	
	bool m_bActive = false;
	float32 m_fTimeSinceLastSpawn = 0.0f;
};
