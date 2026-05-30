#include "Effects/Base/Companion/ZHUDImageVideoViewEffectBase.h"

#include "Helpers/ZResourceProvider.h"

#include <vector>

class ZLiveTvEffect : public ZHUDImageVideoViewEffectBase
{
  public:
    void LoadResources() override;
    void OnClearScene() override;
    bool Available() const override;

    void OnDrawDebugUI() override;

    void Start();
    void Stop();

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "On-Demand TV";
    }

  private:
    SVector2 m_vSize = SVector2(30.f, 30.f);
    SVector2 m_vPositionMin = SVector2(-370.f, -200.f);
    SVector2 m_vPositionMax = SVector2(950.f, 530.f);

  private:
    std::vector<std::shared_ptr<ZResourceProvider>> m_aVideoResources;
    std::vector<SImageVideoViewEntityBinding> m_aActiveVideoViews;

    void PlayVideo(const ZRuntimeResourceID& p_ridVideo);
};
