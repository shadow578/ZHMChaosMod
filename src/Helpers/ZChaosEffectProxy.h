#pragma once
#include "IChaosEffect.h"

#include <Logging.h>

#include "Helpers/Utils.h"

class ZChaosEffectProxy final : public IChaosEffect
{
public:
	ZChaosEffectProxy(IChaosEffect* pEffect)
		: m_pEffect(pEffect)
	{
	}

public: // Lifecycle
	void OnModInitialized() override
	{
		if (LifecycleMethodAllowed(ELifecycleMethodFlag::OnModInitialized))
		{
			Logger::Debug("[{}] OnModInitialized()", m_pEffect->GetName());
			m_pEffect->OnModInitialized();
		}
	}

	void OnModUnload() override
	{
		if (LifecycleMethodAllowed(ELifecycleMethodFlag::OnModUnload))
		{
			Logger::Debug("[{}] OnModUnload()", m_pEffect->GetName());
			m_pEffect->OnModUnload();
		}
	}

	void OnEngineInitialized() override
	{
		if (LifecycleMethodAllowed(ELifecycleMethodFlag::OnEngineInitialized))
		{
			Logger::Debug("[{}] OnEngineInitialized()", m_pEffect->GetName());
			m_pEffect->OnEngineInitialized();
		}
	}

	void OnEnterScene() override
	{
		if (LifecycleMethodAllowed(ELifecycleMethodFlag::OnEnterScene))
		{
			Logger::Debug("[{}] OnEnterScene()", m_pEffect->GetName());
			m_pEffect->OnEnterScene();
		}
	}

	void OnClearScene() override
	{
		if (LifecycleMethodAllowed(ELifecycleMethodFlag::OnClearScene))
		{
			Logger::Debug("[{}] OnClearScene()", m_pEffect->GetName());
			m_pEffect->OnClearScene();
		}
	}

	void LoadResources() override
	{
		if (LifecycleMethodAllowed(ELifecycleMethodFlag::LoadResources))
		{
			Logger::Debug("[{}] LoadResources()", m_pEffect->GetName());
			m_pEffect->LoadResources();
		}
	}

	void OnDrawDebugUI() override
	{
		if (LifecycleMethodAllowed(ELifecycleMethodFlag::OnDrawDebugUI))
		{
			m_pEffect->OnDrawDebugUI();
		}
	}

public: // Effect logic
	void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override
	{
		if (EffectLogicMethodAllowed())
		{
			m_pEffect->OnFrameUpdate(p_UpdateEvent, p_fEffectTimeRemaining);
		}
	}

	void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override
	{
		if (EffectLogicMethodAllowed())
		{
			m_pEffect->OnSlowUpdate(p_fDeltaTime, p_fEffectTimeRemaining);
		}
	}

	void OnDrawUI(const bool p_HasFocus) override
	{
		if (EffectLogicMethodAllowed())
		{
			m_pEffect->OnDrawUI(p_HasFocus);
		}
	}

	void Start() override
	{
		if (EffectLogicMethodAllowed())
		{
			Logger::Debug("[{}] Start()", m_pEffect->GetName());
			m_pEffect->Start();
		}
	}

	void Stop() override
	{
		if (EffectLogicMethodAllowed())
		{
			Logger::Debug("[{}] Stop()", m_pEffect->GetName());
			m_pEffect->Stop();
		}
	}

public: // Metadata
	bool Available() const override
	{
		return m_pEffect->Available();
	}

	std::string GetName() const override
	{
		return m_pEffect->GetName();
	}

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return m_pEffect->GetDisplayName(p_bVoting);
	}

	EDuration GetDuration() const override
	{
		return m_pEffect->GetDuration();
	}

	std::string GetAuthor() const override
	{
		return m_pEffect->GetAuthor();
	}

	bool IsCompatibleWith(const IChaosEffect* p_pOther) const override
	{
		if (Utils::IsInstanceOf<ZChaosEffectProxy>(p_pOther))
		{
			Logger::Error("Compatibility check called with p_pOther being a ZChaosEffectProxy instance! This will not work corrrectly!");
		}

		return m_pEffect->IsCompatibleWith(p_pOther);
	}

	ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
	{
		return m_pEffect->AlwaysActiveLifecycleMethods();
	}

private:
	IChaosEffect* m_pEffect;

	inline bool LifecycleMethodAllowed(const ELifecycleMethodFlag p_eMethod) const
	{
		auto s_eAllowed = AlwaysActiveLifecycleMethods();
		if ((s_eAllowed & p_eMethod) == p_eMethod)
		{
			return true;
		}

		return Available();
	}

	inline bool EffectLogicMethodAllowed() const
	{
		return Available();
	}
};
