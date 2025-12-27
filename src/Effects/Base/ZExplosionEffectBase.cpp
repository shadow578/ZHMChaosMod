#include "ZExplosionEffectBase.h"

#include "IPluginInterface.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZHitman5.h>
#include <Glacier/SExternalReferences.h>

#include "Helpers/ZQuickEntityLoader.h"

#define TAG "[ZExplosionEffectBase] "

#pragma region g_ExplosionProp
// [assembly:/_pro/chaosmod/explosion_standalone.entitytemplate].pc_entitytype
// [assembly:/_pro/chaosmod/explosion_standalone.entitytemplate].pc_entityblueprint
static ZQuickEntityLoader g_ExplosionProp(
    "ZExplosionEffectBase_Prop",
    R"(
{
	"tempHash": "000441B2E82EFD21",
	"tbluHash": "00D971A41C87B12B",
	"rootEntity": "feedfeedfeedfeed",
	"entities": {
		"feedfeedfeedfeed": {
			"parent": null,
			"name": "ZExplosionEffectBase_ROOT",
			"factory": "[modules:/zspatialentity.class].pc_entitytype",
			"blueprint": "[modules:/zspatialentity.class].pc_entityblueprint"
		},
		"feedfeedfeedfeec": {
			"parent": "feedfeedfeedfeed",
			"name": "PropHelper_Explosive",
			"factory": "[assembly:/_pro/design/items/prop_device_ica_modular_runtimes.template?/prophelper_ica_payload_explosion.entitytemplate].pc_entitytype",
			"blueprint": "[assembly:/_pro/design/items/prop_device_ica_modular_runtimes.template?/prophelper_ica_payload_explosion.entitytemplate].pc_entityblueprint",
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
					"value": "feedfeedfeedfeed"
				}
			}
		},
		"feedfeedfeedfeeb": {
			"parent": "feedfeedfeedfeed",
			"name": "FX_E_Explosion_C4_Large",
			"factory": "[assembly:/_pro/effects/templates/fire/fx_explosions.template?/fx_e_explosion_c4_large.entitytemplate].pc_entitytype",
			"blueprint": "[assembly:/_pro/effects/templates/fire/fx_explosions.template?/fx_e_explosion_c4_large.entitytemplate].pc_entityblueprint",
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
				"m_eRoomBehaviour": {
					"type": "ZSpatialEntity.ERoomBehaviour",
					"value": "ROOM_DYNAMIC"
				},
				"m_bForceVisible": {
					"type": "bool",
					"value": false
				},
				"m_eidParent": {
					"type": "SEntityTemplateReference",
					"value": "feedfeedfeedfeed"
				}
			}
		},
		"feedfeedfeedfeea": {
			"parent": "feedfeedfeedfeed",
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
					"value": "feedfeedfeedfeed"
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
					"feedfeedfeedfeed"
				]
			}
		},
		"feedfeedfeedfee9": {
			"parent": "feedfeedfeedfeed",
			"name": "play_sfx_explosion",
			"factory": "[modules:/zaudioevententity.class].pc_entitytype",
			"blueprint": "[modules:/zaudioevententity.class].pc_entityblueprint",
			"properties": {
				"m_pMainEvent": {
					"type": "ZRuntimeResourceID",
					"value": {
						"resource": "[assembly:/sound/wwise/exportedwwisedata/events/effects/sfx_explosions/s2_explosions/play_s2_explosion_generic.wwiseevent].pc_wwisebank",
						"flag": "5F"
					}
				},
				"m_pEmitter": {
					"type": "SEntityTemplateReference",
					"value": "feedfeedfeedfeea"
				},
				"m_eCullingBehavior": {
					"type": "AudioEventCullingBehavior",
					"value": "NoAutoSendStop"
				}
			}
		},
		"feedfeedfeedfee0": {
			"parent": "feedfeedfeedfeed",
			"name": "TimerRandom",
			"factory": "[assembly:/_pro/design/logic.template?/timerrandom.entitytemplate].pc_entitytype",
			"blueprint": "[assembly:/_pro/design/logic.template?/timerrandom.entitytemplate].pc_entityblueprint",
			"properties": {
				"m_bEnabled": {
					"type": "bool",
					"value": false
				},
				"m_fMinTime": {
					"type": "float32",
					"value": 0.1
				},
				"m_fMaxTime": {
					"type": "float32",
					"value": 0.3
				}
			},
			"events": {
				"TimeOut": {
					"Trigger": [
						"feedfeedfeedfeec"
					],
					"Activate": [
						"feedfeedfeedfeeb"
					],
					"Detach": [
						"feedfeedfeedfeeb"
					],
					"Send": [
						"feedfeedfeedfee9"
					],
					"Stop": [
						"feedfeedfeedfee0"
					]
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

void ZExplosionEffectBase::LoadResources()
{
    g_ExplosionProp.LoadAsync();
}

bool ZExplosionEffectBase::Available() const
{
    return IChaosEffect::Available() &&
        g_ExplosionProp.Ready();
}

void ZExplosionEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop Loading: {}", g_ExplosionProp.Loading() ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Prop Ready:   {}", g_ExplosionProp.Ready() ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Prop Failed:  {}", g_ExplosionProp.Failed() ? "Yes" : "No").c_str());

    ImGui::BeginDisabled(!g_ExplosionProp.Ready());

    if (ImGui::Button("Spawn Nearby"))
    {
        if (const auto s_Player = SDK()->GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetWorldMatrix();

                // ~10 forward
                const auto s_Forward = (-s_WM.Backward).Normalized();
                s_WM.Trans += s_Forward * 10.0f;

                SExplosionParams s_Params{
                    .m_Position = s_WM,
                };
                SpawnExplosion(s_Params);
            }
        }
    }

    ImGui::EndDisabled();
}

void ZExplosionEffectBase::SpawnExplosion(const SExplosionParams& p_Params)
{
    ZEntityRef s_RootEntity, s_ExplosionEntity, s_TriggerEntity;
    if (!SpawnExplosionEntity(s_RootEntity, s_ExplosionEntity, s_TriggerEntity))
    {
        return;
    }

    auto* s_pSpatialEntity = s_RootEntity.QueryInterface<ZSpatialEntity>();
    if (!s_pSpatialEntity)
    {
        Logger::Debug(TAG "Root entity is not a ZSpatialEntity.");
        return;
    }

    s_pSpatialEntity->SetWorldMatrix(p_Params.m_Position);
    s_TriggerEntity.SetProperty("m_fTimeMin", p_Params.m_fFuseTimeMin);
    s_TriggerEntity.SetProperty("m_fTimeMax", p_Params.m_fFuseTimeMax);
    s_ExplosionEntity.SetProperty("m_fTargetStrength", p_Params.m_fTargetStrength);
    s_ExplosionEntity.SetProperty("m_eDeathContext", p_Params.m_eDeathContext);
    s_ExplosionEntity.SetProperty("m_eDeathType", p_Params.m_eDeathContext);

    s_TriggerEntity.SignalInputPin("Start");
}

bool ZExplosionEffectBase::SpawnExplosionEntity(ZEntityRef& p_RootEntity, ZEntityRef& p_ExplosionEntity, ZEntityRef& p_TriggerEntity)
{
    const auto s_Scene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_Scene)
    {
        Logger::Debug(TAG "Cannot spawn prop, no scene loaded.");
        return false;
    }

    if (!g_ExplosionProp.Ready())
    {
        Logger::Debug(TAG "Explosion prop factory not ready.");
        return false;
    }

    TResourcePtr<ZTemplateEntityFactory> s_PropRTFactory = g_ExplosionProp.GetFactory();
    TResourcePtr<ZTemplateEntityBlueprintFactory> s_PropRTBpFactory = g_ExplosionProp.GetBlueprintFactory();

    SExternalReferences s_ExternalRefs;

    Functions::ZEntityManager_NewEntity->Call(
        Globals::EntityManager,
        p_RootEntity,
        "ZExplosionEffectBase_ROOT",
        s_PropRTFactory,
        s_Scene.m_ref,
        s_ExternalRefs,
        -1
    );

    if (!p_RootEntity)
    {
        Logger::Debug(TAG "Spawning explosion entity failed.");
        return false;
    }

    Logger::Debug(TAG "Spawned explosion entity successfully.");

    // "PropHelper_Explosive" does the explosion itself
    if (const auto idx = s_PropRTBpFactory.GetResource()->GetSubEntityIndex(0xfeedfeedfeedfeec); idx != -1)
    {
        if (const auto s_Ent = s_PropRTBpFactory.GetResource()->GetSubEntity(p_RootEntity.m_pEntity, idx))
        {
            p_ExplosionEntity = s_Ent;
        }
    }

    // "TimerRandom" acts as the trigger
    if (const auto idx = s_PropRTBpFactory.GetResource()->GetSubEntityIndex(0xfeedfeedfeedfee0); idx != -1)
    {
        if (const auto s_Ent = s_PropRTBpFactory.GetResource()->GetSubEntity(p_RootEntity.m_pEntity, idx))
        {
            p_TriggerEntity = s_Ent;
        }
    }

    if (!p_ExplosionEntity || !p_TriggerEntity)
    {
        Logger::Debug(TAG "Failed to get explosion sub-entities.");
        return false;
    }

    return true;
}
