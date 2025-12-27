#include "ZSoundFXEffectBase.h"

#include "IPluginInterface.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZHitman5.h>
#include <Glacier/SExternalReferences.h>
#include <Glacier/CompileReflection.h>

#include "Helpers/ZQuickEntityLoader.h"

#define TAG "[ZSoundFXEffectBase] "


#pragma region g_SFXPlayerProp
// [assembly:/_pro/chaosmod/sfx_player.entitytemplate].pc_entitytype
// [assembly:/_pro/chaosmod/sfx_player.entitytemplate].pc_entityblueprint
static ZQuickEntityLoader g_SFXPlayerProp(
    "ZPoisonAOEDamageEffectBase_Prop",
    R"(
{
	"tempHash": "00CEB751A091E960",
	"tbluHash": "00F3F4625ECFFC4E",
	"rootEntity": "0000",
	"entities": {
		"0000": {
			"parent": null,
			"name": "SFXPlayer_ROOT",
			"factory": "[modules:/zspatialentity.class].pc_entitytype",
			"blueprint": "[modules:/zspatialentity.class].pc_entityblueprint",
			"propertyAliases": {
				"m_pMainEvent": [
					{
						"originalEntity": "0002",
						"originalProperty": "m_pMainEvent"
					}
				]
			},
			"inputCopying": {
				"Start": {
					"Send": [
						"0002"
					]
				},
				"Stop": {
					"Stop": [
						"0002"
					]
				}
			}
		},
		"0001": {
			"parent": "0000",
			"name": "MOCKUP_SFX_DEVICE_AUDIO_EMITTER",
			"factory": "[assembly:/templates/sound/wwise/soundentities.template?/3daudioemitter.entitytemplate].pc_entitytype",
			"blueprint": "[assembly:/templates/sound/wwise/soundentities.template?/3daudioemitter.entitytemplate].pc_entityblueprint",
			"properties": {
				"m_mTransform": {
					"type": "SMatrix43",
					"value": {
						"rotation": {
							"x": 0.0,
							"y": 0.0,
							"z": 0.0
						},
						"position": {
							"x": 0.0,
							"y": 0.0,
							"z": 0.0
						}
					}
				},
				"m_eidParent": {
					"type": "SEntityTemplateReference",
					"value": "0000"
				},
				"m_bOneShotEmitter": {
					"type": "bool",
					"value": false
				},
				"m_bUseRaycastObstruction": {
					"type": "bool",
					"value": true
				}
			},
			"subsets": {
				"AudioEmitters": [
					"0001"
				]
			}
		},
		"0002": {
			"parent": "0000",
			"name": "play_sfx",
			"factory": "[modules:/zaudioevententity.class].pc_entitytype",
			"blueprint": "[modules:/zaudioevententity.class].pc_entityblueprint",
			"properties": {
				"m_pEmitter": {
					"type": "SEntityTemplateReference",
					"value": "0001"
				},
				"m_eCullingBehavior": {
					"type": "AudioEventCullingBehavior",
					"value": "NoAutoSendStop"
				}
			}
		}
	},
	"propertyOverrides": [],
	"overrideDeletes": [],
	"pinConnectionOverrides": [],
	"pinConnectionOverrideDeletes": [],
	"externalScenes": [],
	"subType": "brick",
	"quickEntityVersion": 3.1,
	"extraFactoryDependencies": [],
	"extraBlueprintDependencies": [],
	"comments": []
}
)"
);
#pragma endregion


void ZSoundFXEffectBase::LoadResources()
{
    g_SFXPlayerProp.LoadAsync();
}

bool ZSoundFXEffectBase::Available()
{
    return IChaosEffect::Available() &&
        g_SFXPlayerProp.Ready();
}

void ZSoundFXEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop Loading: {}", g_SFXPlayerProp.Loading() ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Prop Ready:   {}", g_SFXPlayerProp.Ready() ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Prop Failed:  {}", g_SFXPlayerProp.Failed() ? "Yes" : "No").c_str());

    ImGui::BeginDisabled(!g_SFXPlayerProp.Ready());

    if (ImGui::Button("Play Test SFX"))
    {
        if (const auto s_Player = SDK()->GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetWorldMatrix();

                const auto s_Forward = (-s_WM.Backward).Normalized();
                s_WM.Trans += s_Forward * 5.0f;

                const auto s_RuntimeResourceId = ResId<"[assembly:/sound/wwise/exportedwwisedata/events/item_events/sfx_explosives/proximityexplosive_rubberduck/play_duckarmed.wwiseevent].pc_wwisebank">;
                PlayAt(s_WM, s_RuntimeResourceId);
            }
        }
    }

    ImGui::EndDisabled();
}

void ZSoundFXEffectBase::PlayAt(const SMatrix& p_Position, const ZRuntimeResourceID& p_SoundResource)
{
    ZEntityRef s_RootEntity;
    if (!SpawnEntity(s_RootEntity))
    {
        return;
    }

    auto* s_pSpatialEntity = s_RootEntity.QueryInterface<ZSpatialEntity>();
    if (!s_pSpatialEntity)
    {
        Logger::Debug(TAG "Root entity is not a ZSpatialEntity.");
        return;
    }

    s_pSpatialEntity->SetWorldMatrix(p_Position);
    s_RootEntity.SetProperty("m_pMainEvent", p_SoundResource);

    s_RootEntity.SignalInputPin("Start");
}

bool ZSoundFXEffectBase::SpawnEntity(ZEntityRef& p_RootEntity)
{
    const auto s_Scene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_Scene)
    {
        Logger::Debug(TAG "Cannot spawn prop, no scene loaded.");
        return false;
    }

    if (!g_SFXPlayerProp.Ready())
    {
        Logger::Debug(TAG "prop factory not ready.");
        return false;
    }

    TResourcePtr<ZTemplateEntityFactory> s_PropRTFactory = g_SFXPlayerProp.GetFactory();
    TResourcePtr<ZTemplateEntityBlueprintFactory> s_PropRTBpFactory = g_SFXPlayerProp.GetBlueprintFactory();

    SExternalReferences s_ExternalRefs;

    Functions::ZEntityManager_NewEntity->Call(
        Globals::EntityManager,
        p_RootEntity,
        "ZSFXPlayer_Prop",
        s_PropRTFactory,
        s_Scene.m_ref,
        s_ExternalRefs,
        -1
    );

    if (!p_RootEntity)
    {
        Logger::Debug(TAG "Spawning entity failed.");
        return false;
    }

    Logger::Debug(TAG "Spawned entity successfully.");
    return true;
}
