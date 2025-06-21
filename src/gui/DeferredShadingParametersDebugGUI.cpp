#include "DeferredShadingParametersDebugGUI.h"
#include "imgui.h"
#include "mc/client/dragon/framerenderer/DeferredShadingParameters.h"

static const char *DirectionalShadowsModeNames[] = {"Off", "Cascade", "Evsm"};

template <typename T>
bool ImGui_EnumCombo(const char *label, T *value, const char *const *items,
                     int count) {
  int v = int(*value);
  bool changed = ImGui::Combo(label, &v, items, count);
  if (changed)
    *value = static_cast<T>(v);
  return changed;
}

void DrawDeferredShadingParametersDebugWindow(
    dragon::framerenderer::DeferredShadingParameters *cfg, bool *p_open) {
  if (!cfg)
    return;
  if (!ImGui::Begin("Deferred Shading Parameters", p_open)) {
    ImGui::End();
    return;
  }

  ImGui::InputFloat3("Block Base Ambient Light Color",
                     cfg->mBlockBaseAmbientLightColor.data());
  ImGui::InputFloat("Block Light Indirect Contribution Multiplier",
                    &cfg->mBlockLightIndirectContributionMultiplier, 0.01f,
                    0.1f);
  ImGui::InputFloat("Block Light Indirect Specular Fallback Multiplier",
                    &cfg->mBlockLightIndirectSpecularFallbackMultiplier, 0.01f,
                    0.1f);
  ImGui::InputFloat("Sky Light Indirect Contribution Multiplier",
                    &cfg->mSkyLightIndirectContributionMultiplier, 0.01f, 0.1f);
  ImGui::InputFloat("IBL Indirect Contribution Multiplier",
                    &cfg->mIBLIndirectContributionMultiplier, 0.01f, 0.1f);
  ImGui::InputFloat("Volumetric Fog Min Ambient Value",
                    &cfg->mVolumetricFogMinAmbientValue, 0.01f, 0.1f);
  ImGui::InputInt("IBL SkyLight Fade Start", (int *)&cfg->mIBLSkyLightFadeStart,
                  1, 10);
  ImGui::InputInt("IBL SkyLight Fade End", (int *)&cfg->mIBLSkyLightFadeEnd, 1,
                  10);
  ImGui::InputFloat("Camera Light Intensity Lerp Speed",
                    &cfg->mCameraLightIntensityLerpSpeed, 0.01f, 0.1f);
  ImGui::InputFloat("Emissive Brightness Multiplier",
                    &cfg->mEmissiveBrightnessMultiplier, 0.01f, 0.1f);
  ImGui::InputInt("Emissive Brightness VLU Normalization Factor",
                  &cfg->mEmissiveBrightnessVLUNormalizationFactor, 1, 10);
  ImGui::InputFloat("Directional Light Contribution Fade End Angle",
                    &cfg->mDirectionalLightContributionFadeEndAngle, 0.01f,
                    0.1f);
  ImGui::InputFloat("Directional Light Contribution Fade Begin Angle",
                    &cfg->mDirectionalLightContributionFadeBeginAngle, 0.01f,
                    0.1f);
  ImGui::InputFloat(
      "Directional Light Contribution Fade End Angle Atmospherics",
      &cfg->mDirectionalLightContributionFadeEndAngleAtmospherics, 0.01f, 0.1f);
  ImGui::InputFloat(
      "Directional Light Contribution Fade Begin Angle Atmospherics",
      &cfg->mDirectionalLightContributionFadeBeginAngleAtmospherics, 0.01f,
      0.1f);
  ImGui::InputFloat("Subsurface Scattering Contribution",
                    &cfg->mSubsurfaceScatteringContribution, 0.01f, 0.1f);
  ImGui::InputFloat("Subsurface Scattering Diffuse Wrap Value",
                    &cfg->mSubsurfaceScatteringDiffuseWrapValue, 0.01f, 0.1f);
  ImGui::InputFloat("Subsurface Scattering Falloff Distance",
                    &cfg->mSubsurfaceScatteringFalloffDistance, 0.01f, 0.1f);
  ImGui::InputFloat("End Portal Emissive Multiplier",
                    &cfg->mEndPortalEmissiveMultiplier, 0.01f, 0.1f);
  ImGui::InputFloat("Sun Texture Emissive Multiplier",
                    &cfg->mSunTextureEmissiveMultiplier, 0.01f, 0.1f);
  ImGui::InputFloat("Moon Texture Emissive Multiplier",
                    &cfg->mMoonTextureEmissiveMultiplier, 0.01f, 0.1f);
  ImGui::InputFloat("Glow Ink Sign Strength", &cfg->mGlowInkSignStrength, 0.01f,
                    0.1f);
  ImGui::InputFloat("Lightning Bolt Adaptive Emissive Multiplier",
                    &cfg->mLightningBoltAdaptiveEmissiveMultiplier, 0.01f,
                    0.1f);

  ImGui_EnumCombo("Directional Shadows Mode", &cfg->mDirectionalShadowsMode,
                  DirectionalShadowsModeNames,
                  IM_ARRAYSIZE(DirectionalShadowsModeNames));

  ImGui::Checkbox("Specular Enabled", &cfg->mSpecularEnabled);
  ImGui::Checkbox("Diffuse Enabled", &cfg->mDiffuseEnabled);
  ImGui::Checkbox("Emissive Enabled", &cfg->mEmissiveEnabled);
  ImGui::Checkbox("Ambient Enabled", &cfg->mAmbientEnabled);
  ImGui::Checkbox("Directional Light Contribution Enabled",
                  &cfg->mDirectionalLightContributionEnabled);
  ImGui::Checkbox("Point Light Contribution Enabled",
                  &cfg->mPointLightContributionEnabled);
  ImGui::Checkbox("Enable Deferred Shading SkyLight Heuristic",
                  &cfg->mEnableDeferredShadingSkyLightHeuristic);
  ImGui::Checkbox("Enable Volumetric SkyLight Heuristic",
                  &cfg->mEnableVolumetricSkyLightHeuristic);
  ImGui::Checkbox("Point Light Manhattan Distance Attenuation Enabled",
                  &cfg->mPointLightManhattanDistanceAttenuationEnabled);
  ImGui::Checkbox("Point Light Attenuation Window Enabled",
                  &cfg->mPointLightAttenuationWindowEnabled);

  ImGui::InputFloat("Point Light Attenuation Window Smoothstep Min",
                    &cfg->mPointLightAttenuationWindowSmoothstepMin, 0.01f,
                    0.1f);
  ImGui::InputFloat("Point Light Attenuation Window Smoothstep Max",
                    &cfg->mPointLightAttenuationWindowSmoothstepMax, 0.01f,
                    0.1f);
  ImGui::InputFloat("Point Light Attenuation Window YScale",
                    &cfg->mPointLightAttenuationWindowYScale, 0.01f, 0.1f);
  ImGui::InputFloat("Point Light Attenuation Window YBias",
                    &cfg->mPointLightAttenuationWindowYBias, 0.01f, 0.1f);

  if (ImGui::TreeNode("Point Light LODing Parameters")) {
    auto &vec = cfg->mPointLightLODingParameters.mLightingGridHierarchyRadii;
    ImGui::Text("Lighting Grid Hierarchy Radii");
    for (size_t i = 0; i < vec.size(); ++i) {
      ImGui::PushID(int(i));
      ImGui::InputFloat("##Radius", &vec[i], 0.01f, 0.1f);
      ImGui::SameLine();
      if (ImGui::Button("-")) {
        vec.erase(vec.begin() + i);
        ImGui::PopID();
        break;
      }
      ImGui::PopID();
    }
    if (ImGui::Button("+ Add Radius")) {
      vec.push_back(1.0f);
    }

    ImGui::InputFloat(
        "Block Light Ambient Light Intensity Before Diffuse Fade Out",
        &cfg->mPointLightLODingParameters
             .mBlockLightAmbientLightIntensity_BeforeDiffuseFadeOut,
        0.01f, 0.1f);
    ImGui::InputFloat(
        "Block Light Ambient Light Intensity After Diffuse Fade Out",
        &cfg->mPointLightLODingParameters
             .mBlockLightAmbientLightIntensity_AfterDiffuseFadeOut,
        0.01f, 0.1f);
    ImGui::InputFloat("Distance From Camera Start Diffuse Fade Out",
                      &cfg->mPointLightLODingParameters
                           .mDistanceFromCamera_StartDiffuseFadeOut,
                      0.01f, 0.1f);
    ImGui::InputFloat(
        "Distance From Camera End Diffuse Fade Out",
        &cfg->mPointLightLODingParameters.mDistanceFromCamera_EndDiffuseFadeOut,
        0.01f, 0.1f);
    ImGui::InputFloat("Distance From Camera Start Specular Fade Out",
                      &cfg->mPointLightLODingParameters
                           .mDistanceFromCamera_StartSpecularFadeOut,
                      0.01f, 0.1f);
    ImGui::InputFloat("Distance From Camera End Specular Fade Out",
                      &cfg->mPointLightLODingParameters
                           .mDistanceFromCamera_EndSpecularFadeOut,
                      0.01f, 0.1f);
    ImGui::Checkbox(
        "Enable Fade Out Point Light Diffuse",
        &cfg->mPointLightLODingParameters.mEnableFadeOutPointLightDiffuse);
    ImGui::Checkbox(
        "Enable Fade Out Point Light Specular",
        &cfg->mPointLightLODingParameters.mEnableFadeOutPointLightSpecular);

    ImGui::TreePop();
  }

  ImGui::InputFloat3("Ambient Light Color", (float *)&cfg->mAmbientLightColor);
  ImGui::InputFloat("Ambient Light Illuminance", &cfg->mAmbientLightIlluminance,
                    0.01f, 0.1f);
  ImGui::InputFloat("Sky Intensity", &cfg->mSkyIntensity, 0.01f, 0.1f);

  ImGui::Checkbox("Texel Reflections Enabled", &cfg->mTexelReflectionsEnabled);
  ImGui::Checkbox("Texel Shadows Enabled", &cfg->mTexelShadowsEnabled);
  ImGui::Checkbox("Disable Shadow Filtering", &cfg->mDisableShadowFiltering);

  ImGui::InputInt("Quantized Texel Size", &cfg->mQuantizedTexelSize, 1, 10);
  ImGui::InputFloat("Quantization Surface Normal Precision",
                    &cfg->mQuantizationSurfaceNormalPrecision, 0.01f, 0.1f);

  ImGui::End();
}