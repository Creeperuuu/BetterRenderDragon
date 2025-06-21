#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace bgfx {

struct DLSSOptions {
  enum class Mode : int {
    DLSS = 0,
    Fallback = 1,
    Off = 2,
    Count = 3,
  };

  enum class Scaling : int {
    Performance = 0,
    Balanced = 1,
    Quality = 2,
    UltraPerformance = 3,
    UltraQuality = 4,
    Off = 5,
    Count = 6,
  };

  bgfx::DLSSOptions::Mode mMode;       
  bgfx::DLSSOptions::Scaling mQuality; 
  float mSharpness;                    
  float mMipmapBias;                   
};

struct AccelerationStructureHandle {
  uint16_t idx; 
};

struct TextureHandle {
  uint16_t idx; 
};

struct RtLightInfo {
  float mWorldSpacePosition[3]; 
  uint32_t packedData;          
};

enum class RtDebugMode : int {
  None = 0,
  Diffuse = 1,
  Specular = 2,
  Refraction = 3,
  Count = 4,
};

enum class RtRenderMethod : uint32_t {
  Game = 0,
  GamePathTracer = 1,
  ReferencePathTracer = 2,
  Count = 3,
};

enum class RtLevelCuller : int {
  None = 0,
  Perspective = 1,
  Invalid = 2,
  Count = 3,
};

enum class RtFrustumCuller : int {
  Perspective = 0,
  Raytracing = 1,
  Count = 2,
};

struct PBRTextureData {
  float colourToMaterialUvScale[2]; 
  float colourToMaterialUvBias[2];  
  float colourToNormalUvScale[2];   
  float colourToNormalUvBias[2];    
  int flags;                        
  float uniformRoughness;           
  float uniformEmissive;            
  float uniformMetalness;           
  float uniformSubsurface;          
  float maxMipColour;               
  float maxMipMer;                  
  float maxMipNormalOrHeight;       
  static const int kFlagHasMaterialTexture = 1;
  static const int kFlagHasSubsurfaceChannel = 2;
  static const int kFlagHasNormalTexture = 4;
  static const int kFlagHasHeightMapTexture = 8;
};

struct ProgramHandle {
  uint16_t idx; 
};

struct RayTracingConfiguration {
public:
  bool mRtxOn;                                              
  bgfx::DLSSOptions mDLSSOptions;                           
  float mEmissiveBrightnessMultiplier;                      
  float mEmissiveDesaturation;                              
  float mIndirectEmissiveBoostMultiplier;                   
  float mSurfaceWetness;                                    
  unsigned int mIrradianceCacheMaxHistoryLength;            
  float mRayCountMultiplier;                                
  unsigned int mDoubleBufferWriteIdx;                       
  float mSubPixelJitterOffset[2];                           
  float mPreviousSubPixelJitterOffset[2];                   
  bool mCameraIsUnderWater;                                 
  bool mCameraIsUnderLava;                                  
  unsigned int mDimensionIdx;                               
  bool mOverrideLightAbsorptionScattering;                  
  float mFogAbsorbtion[3];                                  
  float mFogScattering[3];                                  
  float mWaterAbsorbtion[3];                                
  float mWaterScattering[3];                                
  float mCloudAbsorbtion[3];                                
  float mCloudScattering[3];                                
  bool mOverrideSunDirection;                               
  float mDirectionToSun[3];                                 
  float mTweakableSunAzimuthRotation;                       
  float mHeightMapPixelEdgeWidth;                           
  float mHeightMapDepth;                                    
  bool mOverrideFog;                                        
  float mHeightFogFalloffHeight;                            
  float mHeightFogStart;                                    
  float mFogHenyeyGreensteinG;                              
  float mWaterHenyeyGreensteinG;                            
  float mFogDensityMultiplier;                              
  bool mInjectGlobalIlluminationIntoFog;                    
  float mFogStart;                                          
  float mFogEnd;                                            
  float mRenderDistance;                                    
  float mWeatherFogLevel;                                   
  float mRainLevel;                                         
  bgfx::AccelerationStructureHandle mAccelerationStructure; 
  bgfx::TextureHandle mTerrainAtlasTexture;                 
  bgfx::TextureHandle mSkyTexture;                          
  bool mOverrideSky;                                        
  float mSkyColor[4];                                       
  float mSkyUVScale[2];                                     
  float mFinalCombineSkyColourOverride[3];                  
  float mFinalCombineSkyColourOverrideStrength;             
  float mPreviousFrameWorldOrigin[3];                       
  float mWorldOrigin[3];                                    
  bool mNightVisionEnabled;                                 
  float mNightVisionScale;                                  
  float mExposureModifierEv;                                
  float mAutoExposureTemporalFilterAlpha;                   
  float mAutoExposureMinEV;                                 
  float mAutoExposureMaxEV;                                 
  float mAutoExposureLobeDifferenceThreshold;               
  float mAutoExposureLobeMixingMin;                         
  float mAutoExposureLobeMixingMax;                         
  float mSkyBrightnessAdjustment;                           
  float mMoonMeshIntensity;                                 
  float mSunMeshIntensity;                                  
  bgfx::RtDebugMode mDebugMode;                             
  bgfx::RtRenderMethod mRenderMethod;                       
  float mSmoothertron;                                      
  bool mEnableDenoising;                                    
  bool mEnableGlassOpacityHack;                             
  bool mRequestRecompileShaders;                            
  bool mEnableProfiling;                                    
  bool mEnableIrradianceCache;                              
  bool mProbabilityBasedRaycasts;                           
  bool mEnableCausticsStabilizationInRefMode;               
  bool mEnableRayReordering;                                
  bool mEnableSHDiffuse;                                    
  bool mEnableWFTest;                                       
  bool mVisualizeLights;                                    
  bool mEnableExplicitLightSampling;                        
  float mExplicitLightsIntensityBias;                       
  unsigned int mMaxLightCount;                              
  bgfx::RtLevelCuller mLevelCuller;                         
  bgfx::RtFrustumCuller mFrustumCuller;                     
  float mDynamicRenderScale;                                
  float mFocalDistance;                                     
  float mApertureSize;                                      
  uint32_t mApertureType;                                   
  float mDiffuseDespeckleFilterRelativeDifferenceEpsilon;   
  float mSpecularDespeckleFilterRelativeDifferenceEpsilon;  
  float mSVGFDiffuseAlpha;                                  
  float mSVGFDiffuseAlphaMoments;                           
  float mSVGFDiffusePhiLuminance;                           
  float mSVGFSpecularAlpha;                                 
  float mSVGFSpecularAlphaMoments;                          
  float mSVGFSpecularPhiLuminance;                          
  float mSVGFDiffusePhiNormal;                              
  float mSVGFSpecularPhiNormal;                             
  float mSVGFPhiDepth;                                      
  int mSVGFAtrousIterationCountDiffuse;                     
  int mSVGFAtrousIterationCountSpecular;                    
  int mSVGFDiffuseTemporalFeedbackIterationIdx;             
  int mSVGFSpecularTemporalFeedbackIterationIdx;            
  bool mSVGFEnableFireflyFilterDiffuse;                     
  bool mSVGFEnableFireflyFilterSpecular;                    
  bool mEnableAdaptiveDenoiser;                             
  const bgfx::RtLightInfo *mPointLights;                    
  size_t mPointLightsCount;                                 
  static const size_t kReducedSetPointLightsCount = 4096;
  static const size_t kMaxLightSetReductionRatio = 24;
  static const size_t kMaxPointLightsCount = 98304;
  static const uint32_t kMaxHistogramDistance = 2048;
  static const uint32_t kHistogramBins = 512;
  uint32_t *mLightDistanceHistogram;                                  
  std::shared_ptr<std::vector<bgfx::PBRTextureData>> mPBRTextureData; 
  static const int kDefaultPbrTextureBufferSize = 4096;
  float mToneMappingShadowContrast;                             
  float mToneMappingShadowContrastEnd;                          
  float mToneMappingCurveShift;                                 
  float mToneMappingDynamicRange;                               
  float mToneMappingShadowMinSlope;                             
  float mToneMappingMaxExposureIncrease;                        
  float mToneMappingFilmicSaturationCorrection;                 
  bool mPrioritizeVisibleChunksForIrradianceCache;              
  bgfx::TextureHandle mMissingTexture;                          
  float mGenericDebugSliders[4];                                
  void *mLUTMemory;                                             
  int mLUTStride;                                               
  bgfx::ProgramHandle clearVertexIrradianceCache;               
  bgfx::ProgramHandle clearFaceIrradianceCache;                 
  bgfx::ProgramHandle resolveLightMeasurement;                  
  bgfx::ProgramHandle accumulateInscatter;                      
  bgfx::ProgramHandle accumulateGIInscatter;                    
  bgfx::ProgramHandle blurGIInscatter;                          
  bgfx::ProgramHandle reproject;                                
  bgfx::ProgramHandle reprojectSH;                              
  bgfx::ProgramHandle reprojectSpecularOnly;                    
  bgfx::ProgramHandle filterMoments;                            
  bgfx::ProgramHandle atrous;                                   
  bgfx::ProgramHandle blendCheckerboardFields;                  
  bgfx::ProgramHandle filterMomentsSH;                          
  bgfx::ProgramHandle atrousSH;                                 
  bgfx::ProgramHandle blendCheckerboardFieldsSH;                
  bgfx::ProgramHandle finalCombine;                             
  bgfx::ProgramHandle diffuseFireflyFilter;                     
  bgfx::ProgramHandle diffuseFireflyFilterSH;                   
  bgfx::ProgramHandle specularFireflyFilter;                    
  bgfx::ProgramHandle calculateFaceData;                        
  bgfx::ProgramHandle checkerboardUpscale;                      
  bgfx::ProgramHandle checkerboardUpscaleSH;                    
  bgfx::ProgramHandle temporalDenoising;                        
  bgfx::ProgramHandle shadowDenoising;                          
  bgfx::ProgramHandle finalizeDenoising;                        
  bgfx::ProgramHandle blendCheckerboardFieldsShadow;            
  bgfx::ProgramHandle wfTest;                                   
  bgfx::ProgramHandle drawLights;                               
  bgfx::ProgramHandle toneMappingHistogram;                     
  bgfx::ProgramHandle toneCurve;                                
  bgfx::ProgramHandle taa;                                      
  bgfx::ProgramHandle copyToFinal;                              
  bgfx::ProgramHandle checkerboardInterleave;                   
  bgfx::ProgramHandle blurGradients;                            
  bgfx::ProgramHandle tileClassification;                       
  bgfx::ProgramHandle preBlasSkinning;                          
  bgfx::ProgramHandle primaryCheckerboardRayGenInline;          
  bgfx::ProgramHandle sunShadowRayGenInline;                    
  bgfx::ProgramHandle refractionRayGenInline;                   
  bgfx::ProgramHandle diffuseRayGenCombinedInline;              
  bgfx::ProgramHandle adaptiveDenoiserCalculateGradientsInline; 
  bgfx::ProgramHandle adaptiveDenoiserGenerateReferenceInline;  
  bgfx::ProgramHandle specularRayGenInline;                     
  bgfx::ProgramHandle explicitLightSamplingInline;              
  bgfx::ProgramHandle pathTracingRayGenInline;                  
  bgfx::ProgramHandle updateVertexIrradianceCacheInline;        
  bgfx::ProgramHandle updateFaceIrradianceCacheInline;          
  bgfx::ProgramHandle calculateInscatterInline;                 
  bgfx::ProgramHandle calculateGIInscatterInline;               
  bgfx::ProgramHandle incidentLightMeterInline;                 
};
} 