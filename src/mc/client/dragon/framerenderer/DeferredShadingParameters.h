#pragma once

#include "glm/glm.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>

namespace dragon {
namespace framerenderer {
struct PointLightLODingParameters {
  std::vector<float> mLightingGridHierarchyRadii;
  float mBlockLightAmbientLightIntensity_BeforeDiffuseFadeOut;
  float mBlockLightAmbientLightIntensity_AfterDiffuseFadeOut;
  float mDistanceFromCamera_StartDiffuseFadeOut;
  float mDistanceFromCamera_EndDiffuseFadeOut;
  float mDistanceFromCamera_StartSpecularFadeOut;
  float mDistanceFromCamera_EndSpecularFadeOut;
  bool mEnableFadeOutPointLightDiffuse;
  bool mEnableFadeOutPointLightSpecular;
};
struct DeferredShadingParameters {
  enum class DirectionalShadowsMode : int {
    OFF = 0,
    CASCADED = 1,
  };
  std::array<float, 3UL> mBlockBaseAmbientLightColor;
  float mBlockLightIndirectContributionMultiplier;
  float mBlockLightIndirectSpecularFallbackMultiplier;
  float mSkyLightIndirectContributionMultiplier;
  float mIBLIndirectContributionMultiplier;
  float mVolumetricFogMinAmbientValue;
  uint32_t mIBLSkyLightFadeStart;
  uint32_t mIBLSkyLightFadeEnd;
  float mCameraLightIntensityLerpSpeed;
  float mEmissiveBrightnessMultiplier;
  int mEmissiveBrightnessVLUNormalizationFactor;
  float mDirectionalLightContributionFadeEndAngle;
  float mDirectionalLightContributionFadeBeginAngle;
  float mDirectionalLightContributionFadeEndAngleAtmospherics;
  float mDirectionalLightContributionFadeBeginAngleAtmospherics;
  float mSubsurfaceScatteringContribution;
  float mSubsurfaceScatteringDiffuseWrapValue;
  float mSubsurfaceScatteringFalloffDistance;
  float mEndPortalEmissiveMultiplier;
  float mSunTextureEmissiveMultiplier;
  float mMoonTextureEmissiveMultiplier;
  float mGlowInkSignStrength;
  float mLightningBoltAdaptiveEmissiveMultiplier;
  dragon::framerenderer::DeferredShadingParameters::DirectionalShadowsMode
      mDirectionalShadowsMode;
  bool mSpecularEnabled;
  bool mDiffuseEnabled;
  bool mEmissiveEnabled;
  bool mAmbientEnabled;
  bool mDirectionalLightContributionEnabled;
  bool mPointLightContributionEnabled;
  bool mEnableDeferredShadingSkyLightHeuristic;
  bool mEnableVolumetricSkyLightHeuristic;
  bool mPointLightManhattanDistanceAttenuationEnabled;
  bool mPointLightAttenuationWindowEnabled;
  float mPointLightAttenuationWindowSmoothstepMin;
  float mPointLightAttenuationWindowSmoothstepMax;
  float mPointLightAttenuationWindowYScale;
  float mPointLightAttenuationWindowYBias;
  dragon::framerenderer::PointLightLODingParameters mPointLightLODingParameters;
  glm::vec3 mAmbientLightColor;
  float mAmbientLightIlluminance;
  float mSkyIntensity;
  bool mTexelReflectionsEnabled;
  bool mTexelShadowsEnabled;
  bool mDisableShadowFiltering;
  int mQuantizedTexelSize;
  float mQuantizationSurfaceNormalPrecision;
};

} // namespace framerenderer
} // namespace dragon