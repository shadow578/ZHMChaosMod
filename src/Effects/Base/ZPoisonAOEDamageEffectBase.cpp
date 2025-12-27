#include "ZPoisonAOEDamageEffectBase.h"

#include "IPluginInterface.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZHitman5.h>
#include <Glacier/SExternalReferences.h>

#include "Helpers/ZQuickEntityLoader.h"

#define TAG "[ZPoisonAOEDamageEffectBase] "


#pragma region g_AOECloudProp
// [assembly:/_pro/chaosmod/effectsphere_poison.entitytemplate].pc_entitytype
// [assembly:/_pro/chaosmod/effectsphere_poison.entitytemplate].pc_entityblueprint
static ZQuickEntityLoader g_AOECloudProp(
    "ZPoisonAOEDamageEffectBase_Prop",
    R"(
{
  "tempHash": "00F81BA3CAACA51C",
  "tbluHash": "000F7FE97BDD6CE8",
  "rootEntity": "0000",
  "entities": {
    "0000": {
      "parent": null,
      "name": "EffectSphere_Poison_ROOT",
      "factory": "[modules:/zspatialentity.class].pc_entitytype",
      "blueprint": "[modules:/zspatialentity.class].pc_entityblueprint",
      "propertyAliases": {
        "m_vGlobalSize": [
          {
            "originalEntity": "0005",
            "originalProperty": "m_vGlobalSize"
          }
        ],
        "m_ParticleColorRangeStart": [
          {
            "originalEntity": "0008",
            "originalProperty": "m_ParticleColorRangeStart"
          }
        ],
        "m_ParticleColorRangeEnd": [
          {
            "originalEntity": "0008",
            "originalProperty": "m_ParticleColorRangeEnd"
          }
        ],
        "m_rTarget": [
          {
            "originalEntity": "0001",
            "originalProperty": "m_rTarget"
          }
        ]
      },
      "inputCopying": {
        "Start": {
          "In": [
            "00f1"
          ]
        }
      }
    },
    "0001": {
      "parent": "0000",
      "name": "ActHelper_Poison01",
      "factory": "[assembly:/_pro/design/gamecore/acthelpers.template?/acthelper_poison.entitytemplate].pc_entitytype",
      "blueprint": "[assembly:/_pro/design/gamecore/acthelpers.template?/acthelper_poison.entitytemplate].pc_entityblueprint",
      "properties": {
        "m_nTimeLimit": {
          "type": "float32",
          "value": 2.0
        },
        "m_nTimeLimit2": {
          "type": "float32",
          "value": 2.0
        },
        "m_nTimeLimit3": {
          "type": "float32",
          "value": 2.0
        },
        "m_nTimeLimit4": {
          "type": "float32",
          "value": 2.0
        },
        "m_bValueresetpoison": {
          "type": "bool",
          "value": false
        },
        "m_rTarget": {
          "type": "SEntityTemplateReference",
          "value": "0021"
        }
      }
    },
    "0002": {
      "parent": "0000",
      "name": "VariantConverter_void_",
      "factory": "[modules:/zhm5variantconverterentity_void.class].pc_entitytype",
      "blueprint": "[modules:/zhm5variantconverterentity_void.class].pc_entityblueprint",
      "events": {
        "Value": {
          "TriggerPoison": [
            "0001"
          ]
        }
      }
    },
    "0003": {
      "parent": "0000",
      "name": "SignalFork_IActor",
      "factory": "[modules:/zsignalforkentity_iactor.class].pc_entitytype",
      "blueprint": "[modules:/zsignalforkentity_iactor.class].pc_entityblueprint",
      "events": {
        "Out01": {
          "SetIActor": [
            "0001"
          ]
        },
        "Out02": {
          "Variant": [
            "0002"
          ]
        }
      }
    },
    "0004": {
      "parent": "0000",
      "name": "ActorPickerEffectReference",
      "factory": "[modules:/zactorpickereffectreference.class].pc_entitytype",
      "blueprint": "[modules:/zactorpickereffectreference.class].pc_entityblueprint",
      "events": {
        "OnIActor": {
          "In": [
            "0003"
          ]
        }
      }
    },
    "0005": {
      "parent": "0000",
      "name": "VolumeBox",
      "factory": "[modules:/zboxvolumeentity.class].pc_entitytype",
      "blueprint": "[modules:/zboxvolumeentity.class].pc_entityblueprint",
      "properties": {
        "m_mTransform": {
          "type": "SMatrix43",
          "value": {
            "rotation": {
              "x": -0.0,
              "y": 0.0,
              "z": -0.0
            },
            "position": {
              "x": 0.0,
              "y": 0.0,
              "z": 0.013276999816298485
            }
          }
        },
        "m_eidParent": {
          "type": "SEntityTemplateReference",
          "value": "0000"
        },
        "m_vGlobalSize": {
          "type": "SVector3",
          "value": {
            "x": 4.0,
            "y": 4.0,
            "z": 4.0
          }
        }
      }
    },
    "0006": {
      "parent": "0000",
      "name": "ActorPicker",
      "factory": "[modules:/zactorpicker.class].pc_entitytype",
      "blueprint": "[modules:/zactorpicker.class].pc_entityblueprint",
      "properties": {
        "m_aEnterEffects": {
          "type": "TArray<SEntityTemplateReference>",
          "value": [
            "0004"
          ]
        },
        "m_aVolumes": {
          "type": "TArray<SEntityTemplateReference>",
          "value": [
            "0005"
          ]
        },
        "m_aCurrentEffects": {
          "type": "TArray<SEntityTemplateReference>",
          "value": [
            "0004"
          ]
        }
      }
    },
    "0007": {
      "parent": "0000",
      "name": "ActorCounter",
      "factory": "[modules:/zactorcounter.class].pc_entitytype",
      "blueprint": "[modules:/zactorcounter.class].pc_entityblueprint",
      "properties": {
        "m_mTransform": {
          "type": "SMatrix43",
          "value": {
            "rotation": {
              "x": -0.0,
              "y": 0.0,
              "z": -0.0
            },
            "position": {
              "x": 0.0,
              "y": 0.0,
              "z": 0.07042799890041351
            }
          }
        },
        "m_aVolumes": {
          "type": "TArray<SEntityTemplateReference>",
          "value": [
            "0005"
          ]
        },
        "m_eidParent": {
          "type": "SEntityTemplateReference",
          "value": "0000",
          "postInit": true
        }
      },
      "events": {
        "ActorEntered": {
          "Pick": [
            "0006"
          ]
        }
      }
    },
    "0008": {
      "parent": "0000",
      "name": "FX_PB_Smoke",
      "factory": "[assembly:/_pro/effects/templates/smoke/fx_smoke.template?/fx_pb_smoke.entitytemplate].pc_entitytype",
      "blueprint": "[assembly:/_pro/effects/templates/smoke/fx_smoke.template?/fx_pb_smoke.entitytemplate].pc_entityblueprint",
      "properties": {
        "m_vGravity": {
          "type": "SVector3",
          "value": {
            "x": 0.0,
            "y": 0.0,
            "z": 0.05000000074505806
          }
        },
        "m_fFriction": {
          "type": "float32",
          "value": 100.0
        },
        "m_fParticleSizeMin": {
          "type": "float32",
          "value": 20.0
        },
        "m_fParticleSizeMax": {
          "type": "float32",
          "value": 30.0
        },
        "m_FixedAlpha": {
          "type": "float32",
          "value": 0.5
        },
        "SizeAddMax": {
          "type": "float32",
          "value": 90.0
        },
        "m_fTimeToLiveSecondsMin": {
          "type": "float32",
          "value": 2.0
        },
        "m_fTimeToLiveSecondsMax": {
          "type": "float32",
          "value": 3.0
        },
        "m_fMotionStretchFactor": {
          "type": "float32",
          "value": 5.0
        },
        "m_ParticleColorRangeStart": {
          "type": "SColorRGB",
          "value": "#acd373"
        },
        "m_ParticleColorRangeEnd": {
          "type": "SColorRGB",
          "value": "#acd373"
        },
        "SizeAddMin": {
          "type": "float32",
          "value": 10.0
        },
        "AlphaCurve": {
          "type": "ZCurve",
          "value": {
            "data": [
              [
                0.0,
                0.0,
                0.0,
                0.0,
                -0.21460182964801788,
                0.7146018147468567,
                4.8090179775077144e-17,
                0.0
              ],
              [
                0.02083333395421505,
                0.5,
                0.0,
                0.0,
                -0.21460182964801788,
                -0.07079632580280304,
                0.7853981852531433,
                0.5
              ],
              [
                0.0416666679084301,
                1.0,
                0.0,
                0.0,
                0.0,
                0.0,
                0.0,
                1.0
              ],
              [
                0.6995614171028137,
                1.0,
                0.0,
                0.0,
                0.21460182964801788,
                -0.7146018147468567,
                -4.8090179775077144e-17,
                1.0
              ],
              [
                0.8497806787490845,
                0.5,
                0.0,
                0.0,
                0.21460182964801788,
                0.07079632580280304,
                -0.7853981852531433,
                0.5
              ],
              [
                1.0,
                0.0,
                0.0,
                0.0,
                0.0,
                0.0,
                0.0,
                0.0
              ]
            ]
          }
        }
      }
    },
    "0009": {
      "parent": "0000",
      "name": "FX_PR_Smoke_Wispy_Generic_4t_8x4",
      "factory": "[assembly:/_pro/effects/templates/particlerenderers/fx_pr_smoke.template?/fx_pr_smoke_wispy_generic_4t_8x4.entitytemplate].pc_entitytype",
      "blueprint": "[assembly:/_pro/effects/templates/particlerenderers/fx_pr_smoke.template?/fx_pr_smoke_wispy_generic_4t_8x4.entitytemplate].pc_entityblueprint",
      "properties": {
        "Translucency": {
          "type": "float32",
          "value": 1.0
        },
        "SortParticles": {
          "type": "bool",
          "value": false
        },
        "Diffusion": {
          "type": "float32",
          "value": 0.0
        },
        "Detail": {
          "type": "float32",
          "value": 0.0
        },
        "FlowTurbulenceStart": {
          "type": "float32",
          "value": 1.0
        },
        "FlowTurbulenceEnd": {
          "type": "float32",
          "value": 3.0
        },
        "AlphaErosion": {
          "type": "float32",
          "value": 0.0
        },
        "AlphaIntersection": {
          "type": "float32",
          "value": 15.0
        }
      }
    },
    "000a": {
      "parent": "0000",
      "name": "ParticleEmitterBox_Steam_Rolling",
      "factory": "[modules:/zparticleemitterboxentity.class].pc_entitytype",
      "blueprint": "[modules:/zparticleemitterboxentity.class].pc_entityblueprint",
      "properties": {
        "m_mTransform": {
          "type": "SMatrix43",
          "value": {
            "rotation": {
              "x": -0.0,
              "y": 0.0,
              "z": -0.0
            },
            "position": {
              "x": 0.0,
              "y": 0.0,
              "z": 0.0
            }
          }
        },
        "m_ParticleController": {
          "type": "SEntityTemplateReference",
          "value": "0008"
        },
        "m_ParticleRender": {
          "type": "SEntityTemplateReference",
          "value": "0009"
        },
        "m_vGlobalSize": {
          "type": "SVector3",
          "value": {
            "x": 0.0,
            "y": 0.0,
            "z": 0.0
          }
        },
        "m_fSpeedMax": {
          "type": "float32",
          "value": 1.0
        },
        "m_fSpeedMin": {
          "type": "float32",
          "value": 1.0
        },
        "m_bEnabled": {
          "type": "bool",
          "value": false
        },
        "m_eRoomBehaviour": {
          "type": "ZSpatialEntity.ERoomBehaviour",
          "value": "ROOM_DYNAMIC"
        },
        "m_fParticlesPerSecond": {
          "type": "float32",
          "value": 32.0
        },
        "m_fFixedEmissionCount": {
          "type": "float32",
          "value": 0.0
        },
        "m_fParticlesPerSecondMax": {
          "type": "float32",
          "value": 40.0
        },
        "m_eSpawnMode": {
          "type": "eParticleEmitterBoxEntity_SpawnModes",
          "value": "BOX_SPAWNMODE_ELLIPSOID_INSIDE"
        },
        "m_fEmissionCameraProximity": {
          "type": "float32",
          "value": 30.0
        },
        "m_fOuterConeAngle": {
          "type": "float32",
          "value": 15.0
        },
        "m_ParticleShaderParameters": {
          "type": "SEntityTemplateReference",
          "value": "0009"
        },
        "m_eidParent": {
          "type": "SEntityTemplateReference",
          "value": "0000"
        },
        "m_fEmissionTimeSeconds": {
          "type": "float32",
          "value": 1.0
        }
      }
    },
    "0020": {
      "parent": "0000",
      "name": "Keywords",
      "factory": "[modules:/zentity.class].pc_entitytype",
      "blueprint": "[modules:/zentity.class].pc_entityblueprint"
    },
    "0021": {
      "parent": "0020",
      "name": "Keyword_ITEM_POISON_SICK",
      "factory": "[modules:/zkeywordentity.class].pc_entitytype",
      "blueprint": "[modules:/zkeywordentity.class].pc_entityblueprint",
      "properties": {
        "m_sKeyword": {
          "type": "ZString",
          "value": "ITEM_POISON_SICK"
        }
      }
    },
    "0022": {
      "parent": "0020",
      "name": "Keyword_ITEM_POISON_SEDATIVE",
      "factory": "[modules:/zkeywordentity.class].pc_entitytype",
      "blueprint": "[modules:/zkeywordentity.class].pc_entityblueprint",
      "properties": {
        "m_sKeyword": {
          "type": "ZString",
          "value": "ITEM_POISON_SEDATIVE"
        }
      }
    },
    "0023": {
      "parent": "0020",
      "name": "Keyword_ITEM_POISON_SLOW",
      "factory": "[modules:/zkeywordentity.class].pc_entitytype",
      "blueprint": "[modules:/zkeywordentity.class].pc_entityblueprint",
      "properties": {
        "m_sKeyword": {
          "type": "ZString",
          "value": "ITEM_POISON_SLOW"
        }
      }
    },
    "0024": {
      "parent": "0020",
      "name": "Keyword_ITEM_POISON_FAST",
      "factory": "[modules:/zkeywordentity.class].pc_entitytype",
      "blueprint": "[modules:/zkeywordentity.class].pc_entityblueprint",
      "properties": {
        "m_sKeyword": {
          "type": "ZString",
          "value": "ITEM_POISON_FAST"
        }
      }
    },
    "00f0": {
      "parent": "0000",
      "name": "DisableTimer",
      "factory": "[modules:/ztimerentity.class].pc_entitytype",
      "blueprint": "[modules:/ztimerentity.class].pc_entityblueprint",
      "properties": {
        "m_bEnabled": {
          "type": "bool",
          "value": false
        },
        "m_nInterval": {
          "type": "int32",
          "value": 5000
        }
      },
      "events": {
        "TimeOut": {
          "Disable": [
            "0007"
          ],
          "Deactivate": [
            "000a"
          ]
        }
      }
    },
    "00f1": {
      "parent": "0000",
      "name": "CentralTrigger",
      "factory": "[modules:/zsignalpipeentity_void.class].pc_entitytype",
      "blueprint": "[modules:/zsignalpipeentity_void.class].pc_entityblueprint",
      "events": {
        "Out": {
          "Enable": [
            "0007"
          ],
          "Activate": [
            "000a"
          ],
          "Start": [
            "00f0"
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

void ZPoisonAOEDamageEffectBase::LoadResources()
{
    g_AOECloudProp.LoadAsync();
}

bool ZPoisonAOEDamageEffectBase::Available() const
{
    return IChaosEffect::Available() &&
        g_AOECloudProp.Ready();
}

void ZPoisonAOEDamageEffectBase::OnDrawDebugUI()
{
    static const std::map<EPoisonType, std::string> c_mPoisonTypesToNames{
        { EPoisonType::SICK, "Sick" },
        { EPoisonType::SEDATIVE, "Sedative" },
        { EPoisonType::LETHAL_SLOW, "Lethal Slow" },
        { EPoisonType::LETHAL_FAST, "Lethal Fast" },
    };

    ImGui::TextUnformatted(fmt::format("Prop Loading: {}", g_AOECloudProp.Loading() ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Prop Ready:   {}", g_AOECloudProp.Ready() ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Prop Failed:  {}", g_AOECloudProp.Failed() ? "Yes" : "No").c_str());

    ImGui::Separator();

    ImGui::BeginDisabled(!g_AOECloudProp.Ready());

    if (ImGui::BeginCombo("DBG Poison Type", c_mPoisonTypesToNames.at(m_eDebugPoisonType).c_str()))
    {
        for (auto [s_eType, s_sName] : c_mPoisonTypesToNames)
        {
            if (ImGui::Selectable(
                s_sName.c_str(),
                s_eType == m_eDebugPoisonType
            ))
            {
                m_eDebugPoisonType = s_eType;
            }
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Spawn DBG Nearby"))
    {
        if (const auto s_Player = SDK()->GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetWorldMatrix();

                // ~10 forward
                const auto s_Forward = (-s_WM.Backward).Normalized();
                s_WM.Trans += s_Forward * 10.0f;

                SParams s_Params{
                    .m_Position = s_WM,
                    .m_eType = m_eDebugPoisonType
                };
                Spawn(s_Params);
            }
        }
    }

    ImGui::EndDisabled();
}

void ZPoisonAOEDamageEffectBase::Spawn(const SParams& p_Params)
{
    ZEntityRef s_RootEntity;
    if (!SpawnEntity(s_RootEntity))
    {
        return;
    }

    // set transform
    auto* s_pSpatialEntity = s_RootEntity.QueryInterface<ZSpatialEntity>();
    if (!s_pSpatialEntity)
    {
        Logger::Debug(TAG "Root entity is not a ZSpatialEntity.");
        return;
    }

    s_pSpatialEntity->SetWorldMatrix(p_Params.m_Position);

    // set poison type
    ZEntityRef s_KeywordEntity;
    if (!GetPoisonKeywordEntity(p_Params.m_eType, s_RootEntity, s_KeywordEntity))
    {
        Logger::Debug(TAG "Could not find poison keyword entity for type {}", static_cast<int>(p_Params.m_eType));
        return;
    }

    s_RootEntity.SetProperty("m_rTarget", s_KeywordEntity);

    // misc. properties
    s_RootEntity.SetProperty("m_vGlobalSize", p_Params.m_AreaSize);
    s_RootEntity.SetProperty("m_ParticleColorRangeStart", p_Params.m_ParticleColorRangeStart);
    s_RootEntity.SetProperty("m_ParticleColorRangeEnd", p_Params.m_ParticleColorRangeEnd);

    // trigger
    s_RootEntity.SignalInputPin("Start");
}

bool ZPoisonAOEDamageEffectBase::SpawnEntity(ZEntityRef& p_RootEntity)
{
    const auto s_Scene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_Scene)
    {
        Logger::Debug(TAG "Cannot spawn prop, no scene loaded.");
        return false;
    }

    if (!g_AOECloudProp.Ready())
    {
        Logger::Debug(TAG "Explosion prop factory not ready.");
        return false;
    }

    TResourcePtr<ZTemplateEntityFactory> s_PropRTFactory = g_AOECloudProp.GetFactory();
    TResourcePtr<ZTemplateEntityBlueprintFactory> s_PropRTBpFactory = g_AOECloudProp.GetBlueprintFactory();

    SExternalReferences s_ExternalRefs;

    Functions::ZEntityManager_NewEntity->Call(
        Globals::EntityManager,
        p_RootEntity,
        "ZAOEDamageEffectBase_Prop",
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

bool ZPoisonAOEDamageEffectBase::GetPoisonKeywordEntity(const EPoisonType p_eType, ZEntityRef p_RootEntity, ZEntityRef& p_KeywordEntity)
{
    if (!g_AOECloudProp.Ready())
    {
        Logger::Debug(TAG "Explosion prop factory not ready.");
        return false;
    }

    TResourcePtr<ZTemplateEntityBlueprintFactory> s_PropRTBpFactory = g_AOECloudProp.GetBlueprintFactory();

    if (const auto idx = s_PropRTBpFactory.GetResource()->GetSubEntityIndex(static_cast<int>(p_eType)); idx != -1)
    {
        if (const auto s_Ent = s_PropRTBpFactory.GetResource()->GetSubEntity(p_RootEntity.m_pEntity, idx))
        {
            p_KeywordEntity = s_Ent;
            return true;
        }
    }

    return false;
}
