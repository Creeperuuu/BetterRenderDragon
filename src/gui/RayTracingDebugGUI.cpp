// RayTracingDebugGUI.cpp
#include "RayTracingDebugGUI.h"
#include "imgui.h"
#include "mc/client/bgfx/bgfx.h"

template <typename T>
bool ImGui_EnumCombo(const char *label, T *value, const char *const *items,
                     int count) {
  int v = int(*value);
  bool changed = ImGui::Combo(label, &v, items, count);
  if (changed)
    *value = T(v);
  return changed;
}

static const char *DLSSModes[] = {"DLSS", "Fallback", "Off"};
static const char *DLSSQualities[] = {"Performance",  "Balanced",
                                      "Quality",      "UltraPerformance",
                                      "UltraQuality", "Off"};
static const char *RtDebugModeNames[] = {"None", "Diffuse", "Specular",
                                         "Refraction"};
static const char *RtRenderMethodNames[] = {"Game", "GamePathTracer",
                                            "ReferencePathTracer"};
static const char *RtLevelCullerNames[] = {"None", "Perspective", "Invalid"};
static const char *RtFrustumCullerNames[] = {"Perspective", "Raytracing"};

void DrawRayTracingDebugWindow(bgfx::RayTracingConfiguration *cfg,
                               bool *p_open) {
  if (!cfg)
    return;
  if (!ImGui::Begin("RayTracing Debug", p_open)) {
    ImGui::End();
    return;
  }

  ImGui::Checkbox("Rtx On", &cfg->mRtxOn);

  if (ImGui::TreeNode("DLSS Options")) {
    ImGui_EnumCombo("Mode", &cfg->mDLSSOptions.mMode, DLSSModes, 3);
    ImGui_EnumCombo("Scaling", &cfg->mDLSSOptions.mQuality, DLSSQualities, 6);
    ImGui::InputFloat("Sharpness", &cfg->mDLSSOptions.mSharpness, 0.01f, 0.1f);
    ImGui::InputFloat("Mipmap Bias", &cfg->mDLSSOptions.mMipmapBias, 0.01f,
                      0.1f);
    ImGui::TreePop();
  }

  ImGui::InputFloat("Emissive Brightness Multiplier",
                    &cfg->mEmissiveBrightnessMultiplier, 0.01f, 0.1f);
  ImGui::InputFloat("Emissive Desaturation", &cfg->mEmissiveDesaturation, 0.01f,
                    0.1f);
  ImGui::InputFloat("Indirect Emissive Boost Multiplier",
                    &cfg->mIndirectEmissiveBoostMultiplier, 0.01f, 0.1f);
  ImGui::InputFloat("Surface Wetness", &cfg->mSurfaceWetness, 0.01f, 0.1f);
  ImGui::InputInt("Irradiance Cache Max History Length",
                  (int *)&cfg->mIrradianceCacheMaxHistoryLength, 1, 10);
  ImGui::InputFloat("Ray Count Multiplier", &cfg->mRayCountMultiplier, 0.01f,
                    0.1f);
  ImGui::InputInt("Double Buffer Write Idx", (int *)&cfg->mDoubleBufferWriteIdx,
                  1, 1);
  ImGui::InputFloat2("SubPixel Jitter Offset", cfg->mSubPixelJitterOffset);
  ImGui::InputFloat2("Previous SubPixel Jitter Offset",
                     cfg->mPreviousSubPixelJitterOffset);
  ImGui::Checkbox("Camera Is Under Water", &cfg->mCameraIsUnderWater);
  ImGui::Checkbox("Camera Is Under Lava", &cfg->mCameraIsUnderLava);
  ImGui::InputInt("Dimension Idx", (int *)&cfg->mDimensionIdx, 1, 10);
  ImGui::Checkbox("Override Light Absorption Scattering",
                  &cfg->mOverrideLightAbsorptionScattering);
  ImGui::InputFloat3("Fog Absorbtion", cfg->mFogAbsorbtion);
  ImGui::InputFloat3("Fog Scattering", cfg->mFogScattering);
  ImGui::InputFloat3("Water Absorbtion", cfg->mWaterAbsorbtion);
  ImGui::InputFloat3("Water Scattering", cfg->mWaterScattering);
  ImGui::InputFloat3("Cloud Absorbtion", cfg->mCloudAbsorbtion);
  ImGui::InputFloat3("Cloud Scattering", cfg->mCloudScattering);
  ImGui::Checkbox("Override Sun Direction", &cfg->mOverrideSunDirection);
  ImGui::InputFloat3("Direction To Sun", cfg->mDirectionToSun);
  ImGui::InputFloat("Tweakable Sun Azimuth Rotation",
                    &cfg->mTweakableSunAzimuthRotation, 0.01f, 0.1f);
  ImGui::InputFloat("HeightMap Pixel Edge Width",
                    &cfg->mHeightMapPixelEdgeWidth, 0.01f, 0.1f);
  ImGui::InputFloat("HeightMap Depth", &cfg->mHeightMapDepth, 0.01f, 0.1f);
  ImGui::Checkbox("Override Fog", &cfg->mOverrideFog);
  ImGui::InputFloat("Height Fog Falloff Height", &cfg->mHeightFogFalloffHeight,
                    0.1f, 1.0f);
  ImGui::InputFloat("Height Fog Start", &cfg->mHeightFogStart, 1.0f, 10.0f);
  ImGui::InputFloat("Fog Henyey Greenstein G", &cfg->mFogHenyeyGreensteinG,
                    0.01f, 0.1f);
  ImGui::InputFloat("Water Henyey Greenstein G", &cfg->mWaterHenyeyGreensteinG,
                    0.01f, 0.1f);
  ImGui::InputFloat("Fog Density Multiplier", &cfg->mFogDensityMultiplier,
                    0.01f, 0.1f);
  ImGui::Checkbox("Inject Global Illumination Into Fog",
                  &cfg->mInjectGlobalIlluminationIntoFog);
  ImGui::InputFloat("Fog Start", &cfg->mFogStart, 1.0f, 10.0f);
  ImGui::InputFloat("Fog End", &cfg->mFogEnd, 1.0f, 10.0f);
  ImGui::InputFloat("Render Distance", &cfg->mRenderDistance, 1.0f, 10.0f);
  ImGui::InputFloat("Weather Fog Level", &cfg->mWeatherFogLevel, 0.01f, 0.1f);
  ImGui::InputFloat("Rain Level", &cfg->mRainLevel, 0.01f, 0.1f);

  ImGui::InputInt("Acceleration Structure Idx",
                  (int *)&cfg->mAccelerationStructure.idx, 1, 1);
  ImGui::InputInt("Terrain Atlas Texture Idx",
                  (int *)&cfg->mTerrainAtlasTexture.idx, 1, 1);
  ImGui::InputInt("Sky Texture Idx", (int *)&cfg->mSkyTexture.idx, 1, 1);

  ImGui::Checkbox("Override Sky", &cfg->mOverrideSky);
  ImGui::ColorEdit4("Sky Color", cfg->mSkyColor);
  ImGui::InputFloat2("Sky UV Scale", cfg->mSkyUVScale);
  ImGui::ColorEdit3("Final Combine Sky Color Override",
                    cfg->mFinalCombineSkyColourOverride);
  ImGui::InputFloat("Final Combine Sky Colour Override Strength",
                    &cfg->mFinalCombineSkyColourOverrideStrength, 0.01f, 0.1f);
  ImGui::InputFloat3("Previous Frame World Origin",
                     cfg->mPreviousFrameWorldOrigin);
  ImGui::InputFloat3("World Origin", cfg->mWorldOrigin);

  ImGui::Checkbox("Night Vision Enabled", &cfg->mNightVisionEnabled);
  ImGui::InputFloat("Night Vision Scale", &cfg->mNightVisionScale, 0.01f, 0.1f);

  ImGui::InputFloat("Exposure Modifier Ev", &cfg->mExposureModifierEv, 0.01f,
                    0.1f);
  ImGui::InputFloat("Auto Exposure Temporal Filter Alpha",
                    &cfg->mAutoExposureTemporalFilterAlpha, 0.01f, 0.1f);
  ImGui::InputFloat("Auto Exposure Min EV", &cfg->mAutoExposureMinEV, 0.01f,
                    0.1f);
  ImGui::InputFloat("Auto Exposure Max EV", &cfg->mAutoExposureMaxEV, 0.01f,
                    0.1f);
  ImGui::InputFloat("Auto Exposure Lobe Difference Threshold",
                    &cfg->mAutoExposureLobeDifferenceThreshold, 0.01f, 0.1f);
  ImGui::InputFloat("Auto Exposure Lobe Mixing Min",
                    &cfg->mAutoExposureLobeMixingMin, 0.01f, 0.1f);
  ImGui::InputFloat("Auto Exposure Lobe Mixing Max",
                    &cfg->mAutoExposureLobeMixingMax, 0.01f, 0.1f);
  ImGui::InputFloat("Sky Brightness Adjustment", &cfg->mSkyBrightnessAdjustment,
                    0.01f, 0.1f);
  ImGui::InputFloat("Moon Mesh Intensity", &cfg->mMoonMeshIntensity, 0.01f,
                    0.1f);
  ImGui::InputFloat("Sun Mesh Intensity", &cfg->mSunMeshIntensity, 0.01f, 0.1f);

  ImGui_EnumCombo("Debug Mode", &cfg->mDebugMode, RtDebugModeNames, 4);
  ImGui_EnumCombo("Render Method", &cfg->mRenderMethod, RtRenderMethodNames, 3);

  ImGui::InputFloat("Smoothertron", &cfg->mSmoothertron, 0.01f, 0.1f);
  ImGui::Checkbox("Enable Denoising", &cfg->mEnableDenoising);
  ImGui::Checkbox("Enable Glass Opacity Hack", &cfg->mEnableGlassOpacityHack);
  ImGui::Checkbox("Request Recompile Shaders", &cfg->mRequestRecompileShaders);
  ImGui::Checkbox("Enable Profiling", &cfg->mEnableProfiling);
  ImGui::Checkbox("Enable Irradiance Cache", &cfg->mEnableIrradianceCache);
  ImGui::Checkbox("Probability Based Raycasts",
                  &cfg->mProbabilityBasedRaycasts);
  ImGui::Checkbox("Enable Caustics Stabilization In RefMode",
                  &cfg->mEnableCausticsStabilizationInRefMode);
  ImGui::Checkbox("Enable Ray Reordering", &cfg->mEnableRayReordering);
  ImGui::Checkbox("Enable SH Diffuse", &cfg->mEnableSHDiffuse);
  ImGui::Checkbox("Enable WFTest", &cfg->mEnableWFTest);
  ImGui::Checkbox("Visualize Lights", &cfg->mVisualizeLights);
  ImGui::Checkbox("Enable Explicit Light Sampling",
                  &cfg->mEnableExplicitLightSampling);
  ImGui::InputFloat("Explicit Lights Intensity Bias",
                    &cfg->mExplicitLightsIntensityBias, 0.01f, 0.1f);
  ImGui::InputInt("Max Light Count", (int *)&cfg->mMaxLightCount, 1, 1);

  ImGui_EnumCombo("Level Culler", &cfg->mLevelCuller, RtLevelCullerNames, 3);
  ImGui_EnumCombo("Frustum Culler", &cfg->mFrustumCuller, RtFrustumCullerNames,
                  2);

  ImGui::InputFloat("Dynamic Render Scale", &cfg->mDynamicRenderScale, 0.01f,
                    0.1f);
  ImGui::InputFloat("Focal Distance", &cfg->mFocalDistance, 0.01f, 0.1f);
  ImGui::InputFloat("Aperture Size", &cfg->mApertureSize, 0.01f, 0.1f);
  ImGui::InputInt("Aperture Type", (int *)&cfg->mApertureType, 1, 1);

  ImGui::InputFloat("Diffuse Despeckle Filter Relative Difference Epsilon",
                    &cfg->mDiffuseDespeckleFilterRelativeDifferenceEpsilon,
                    0.01f, 0.1f);
  ImGui::InputFloat("Specular Despeckle Filter Relative Difference Epsilon",
                    &cfg->mSpecularDespeckleFilterRelativeDifferenceEpsilon,
                    0.01f, 0.1f);
  ImGui::InputFloat("SVGF Diffuse Alpha", &cfg->mSVGFDiffuseAlpha, 0.01f, 0.1f);
  ImGui::InputFloat("SVGF Diffuse Alpha Moments",
                    &cfg->mSVGFDiffuseAlphaMoments, 0.01f, 0.1f);
  ImGui::InputFloat("SVGF Diffuse Phi Luminance",
                    &cfg->mSVGFDiffusePhiLuminance, 0.01f, 0.1f);
  ImGui::InputFloat("SVGF Specular Alpha", &cfg->mSVGFSpecularAlpha, 0.01f,
                    0.1f);
  ImGui::InputFloat("SVGF Specular Alpha Moments",
                    &cfg->mSVGFSpecularAlphaMoments, 0.01f, 0.1f);
  ImGui::InputFloat("SVGF Specular Phi Luminance",
                    &cfg->mSVGFSpecularPhiLuminance, 0.01f, 0.1f);
  ImGui::InputFloat("SVGF Diffuse Phi Normal", &cfg->mSVGFDiffusePhiNormal,
                    0.01f, 0.1f);
  ImGui::InputFloat("SVGF Specular Phi Normal", &cfg->mSVGFSpecularPhiNormal,
                    0.01f, 0.1f);
  ImGui::InputFloat("SVGF Phi Depth", &cfg->mSVGFPhiDepth, 0.01f, 0.1f);

  ImGui::InputInt("SVGF Atrous Iteration Count Diffuse",
                  &cfg->mSVGFAtrousIterationCountDiffuse, 1, 1);
  ImGui::InputInt("SVGF Atrous Iteration Count Specular",
                  &cfg->mSVGFAtrousIterationCountSpecular, 1, 1);
  ImGui::InputInt("SVGF Diffuse Temporal Feedback Iteration Idx",
                  &cfg->mSVGFDiffuseTemporalFeedbackIterationIdx, 1, 1);
  ImGui::InputInt("SVGF Specular Temporal Feedback Iteration Idx",
                  &cfg->mSVGFSpecularTemporalFeedbackIterationIdx, 1, 1);
  ImGui::Checkbox("SVGF Enable Firefly Filter Diffuse",
                  &cfg->mSVGFEnableFireflyFilterDiffuse);
  ImGui::Checkbox("SVGF Enable Firefly Filter Specular",
                  &cfg->mSVGFEnableFireflyFilterSpecular);
  ImGui::Checkbox("Enable Adaptive Denoiser", &cfg->mEnableAdaptiveDenoiser);

  ImGui::LabelText("Point Lights Count", "%zu", cfg->mPointLightsCount);
  ImGui::LabelText("Point Lights", "%p", (void *)cfg->mPointLights);
  ImGui::LabelText("Light Distance Histogram", "%p",
                   (void *)cfg->mLightDistanceHistogram);
  ImGui::LabelText("PBR Texture Data", "%p",
                   (void *)(cfg->mPBRTextureData.get()));

  ImGui::InputFloat("Tone Mapping Shadow Contrast",
                    &cfg->mToneMappingShadowContrast, 0.01f, 0.1f);
  ImGui::InputFloat("Tone Mapping Shadow Contrast End",
                    &cfg->mToneMappingShadowContrastEnd, 0.01f, 0.1f);
  ImGui::InputFloat("Tone Mapping Curve Shift", &cfg->mToneMappingCurveShift,
                    0.01f, 0.1f);
  ImGui::InputFloat("Tone Mapping Dynamic Range",
                    &cfg->mToneMappingDynamicRange, 0.01f, 0.1f);
  ImGui::InputFloat("Tone Mapping Shadow Min Slope",
                    &cfg->mToneMappingShadowMinSlope, 0.01f, 0.1f);
  ImGui::InputFloat("Tone Mapping Max Exposure Increase",
                    &cfg->mToneMappingMaxExposureIncrease, 0.01f, 0.1f);
  ImGui::InputFloat("Tone Mapping Filmic Saturation Correction",
                    &cfg->mToneMappingFilmicSaturationCorrection, 0.01f, 0.1f);
  ImGui::Checkbox("Prioritize Visible Chunks For Irradiance Cache",
                  &cfg->mPrioritizeVisibleChunksForIrradianceCache);
  ImGui::InputInt("MissingTexture Idx", (int *)&cfg->mMissingTexture.idx, 1, 1);

  ImGui::InputFloat4("Generic Debug Sliders", cfg->mGenericDebugSliders);

  ImGui::LabelText("LUT Memory", "%p",
                   cfg->mLUTMemory ? cfg->mLUTMemory : nullptr);
  ImGui::InputInt("LUT Stride", &cfg->mLUTStride, 1, 1);

  ImGui::End();
}