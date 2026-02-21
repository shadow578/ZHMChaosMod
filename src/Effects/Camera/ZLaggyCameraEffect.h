#pragma once
#include "Effects/Base/ZCameraEffectBase.h"

#include <queue>

class ZLaggyCameraEffect : public ZCameraEffectBase 
{
public:
    ZLaggyCameraEffect(const std::string p_sNameSuffix, const std::string p_sDisplayName, const int p_nDelayFrames, const float32 m_fApplyPercent) :
        ZCameraEffectBase(),
        m_sNameSuffix(p_sNameSuffix),
		m_sDisplayName(p_sDisplayName),
		m_nDelayFrames(p_nDelayFrames),
		m_fTransformApplyPercent(m_fApplyPercent)
    {
    }

    void OnDrawDebugUI() override;

    void Start() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetName() const override
    {
        return ZCameraEffectBase::GetName() + "_" + m_sNameSuffix;
	}

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return m_sDisplayName;
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short;
    }

private:
	const std::string m_sNameSuffix;
	const std::string m_sDisplayName;

    std::queue<SMatrix> m_qTransformHistory;

    int m_nDelayFrames;
	float32 m_fTransformApplyPercent;
};
