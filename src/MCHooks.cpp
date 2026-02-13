
#include "Core/Math/Vec4.h"
#include "mc/client/RenderDragon/Materials/MaterialResourceManager.h"
#include "mc/client/Renderdragon/Materials/MaterialUniformName.h"
#include "mc/client/Renderdragon/Materials/ShaderCodePlatform.h"
#include "mc/client/Renderdragon/Rendering/LightingModels.h"
#include "mc/client/bgfx/bgfx.h"
#include "mc/client/dragon/framerenderer/DeferredShadingParameters.h"
#include "mc/deps/core/resource/ResourceLocation.h"
#include <codecvt>
#include <cstdio>

#if defined(_WIN32)
#include "gui/Options.h"
#include < stacktrace>
#endif
#include <iostream>

#include <iostream>
//=====================================================Vanilla2Deferred=====================================================

char globalGraphicsMode = 0;
int MaterialResourceManagerOffset = 0;

bgfx::RayTracingFeatureConfiguration *gRayTracingFeatureConfiguration = nullptr;

dragon::framerenderer::DeferredShadingParameters *gDeferredParams = nullptr;

using dragon::rendering::LightingModels;
#if defined(_WIN32)
bool shouldForceEnableVibrantVisuals() {
  return brd::Options::forceEnableVibrantVisuals;
}
#endif
//======================================================CustomUniforms======================================================

//=====================================================MaterialBinLoader====================================================

typedef bool (*PFN_ResourcePackManager_load)(void *This,
                                             const ResourceLocation &location,
                                             std::string &resourceStream);

//==========================================================================================================================

#include "api/memory/Hook.h"
#if defined(_WIN32)
int NEW_VIDEO_SETTINGS = 0;

SKY_AUTO_STATIC_HOOK(getGameVersionString, memory::HookPriority::Normal,
                     std::initializer_list<const char *>(
                         {// Win 1.21.60
                          "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 "
                          "48 83 EC 60 48 8B F1 48 89 4C 24 ? 45 33 E4 48 8D "
                          "4C 24 ? E8 ? ? ? ? 48 8B D8 48 8B 48",
                          "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 "
                          "48 83 EC 60 48 8B F9 48 89 4C 24 ? 45 33 E4"}),
                     std::string, std::string *result) {
  auto version = origin(result);
  if (version.find("1.21.6") != std::string::npos) {
    NEW_VIDEO_SETTINGS = 723;
    MaterialResourceManagerOffset = 1008;
  } else if (version.find("1.21.7") != std::string::npos) {
    NEW_VIDEO_SETTINGS = 726;
    MaterialResourceManagerOffset = 1008;
  } else if (version.find("1.21.8") != std::string::npos) {
    NEW_VIDEO_SETTINGS = 730;
    MaterialResourceManagerOffset = 960;
  } else if (version.find("1.21.9") != std::string::npos) {
    MaterialResourceManagerOffset = 960;
  } else if (version.find("1.21.10") != std::string::npos) {
    MaterialResourceManagerOffset = 960;
  } else if (version.find("1.21.11") != std::string::npos) {
    MaterialResourceManagerOffset = 960;
  } else if (version.find("1.21.12") != std::string::npos) {
    MaterialResourceManagerOffset = 960;
  } else if (version.find("1.21.13") != std::string::npos) {
    MaterialResourceManagerOffset = 960;
  } else if (version.find("1.26.0") != std::string::npos) {
    MaterialResourceManagerOffset = 960;
  }
  return version;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void *resourcePackManager = nullptr;
PFN_ResourcePackManager_load ResourcePackManager_load;
// ResourcePackManager::ResourcePackManager
SKY_AUTO_STATIC_HOOK(
    ResourcePackManagerConstructor, memory::HookPriority::Normal,
    std::initializer_list<const char *>(
        {// 1.21.130
         "48 89 5C 24 ? 55 56 57 41 56 41 57 48 81 EC 90 00 00 00 48 8B 05 ? ? "
         "? ? 48 33 C4 48 89 84 24 ? ? ? ? 41 0F B6 E9 4D 8B F0",
         // 1.21.90
         "4C 8B DC 49 89 5B ? 49 89 53 ? 49 89 4B ? 55 56 57 41 56 41 57 48 83 "
         "EC 70 41 0F B6 E9 4D"}),
    void *, void *This, uintptr_t a2, uintptr_t a3, bool needsToInitialize) {

  void *result = origin(This, a2, a3, needsToInitialize);
  if (needsToInitialize && !resourcePackManager) {
    resourcePackManager = This;
    void **vptr = *(void ***)resourcePackManager;
    ResourcePackManager_load = (PFN_ResourcePackManager_load) * (vptr + 3);
  }
  return result;
}
class TempFileWrapper {
private:
  FILE *m_file;
  std::wstring m_path;

public:
  TempFileWrapper(const std::string &content) : m_file(nullptr) {
    wchar_t tempPath[MAX_PATH];
    wchar_t tempFile[MAX_PATH];

    if (GetTempPathW(MAX_PATH, tempPath) == 0) {
      return;
    }

    if (GetTempFileNameW(tempPath, L"brd", 0, tempFile) == 0) {
      return;
    }

    m_path = tempFile;

    HANDLE hFile = CreateFileW(m_path.c_str(), GENERIC_WRITE, 0, nullptr,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
      return;
    }

    DWORD written = 0;
    WriteFile(hFile, content.data(), static_cast<DWORD>(content.size()),
              &written, nullptr);
    CloseHandle(hFile);

    m_file = _wfsopen(m_path.c_str(), L"rb", _SH_DENYNO);
  }

  ~TempFileWrapper() {
    if (m_file) {
      fclose(m_file);
    }
    if (!m_path.empty()) {
      DeleteFileW(m_path.c_str());
    }
  }

  FILE *get() const { return m_file; }

  TempFileWrapper(const TempFileWrapper &) = delete;
  TempFileWrapper &operator=(const TempFileWrapper &) = delete;

  TempFileWrapper(TempFileWrapper &&other) noexcept
      : m_file(other.m_file), m_path(std::move(other.m_path)) {
    other.m_file = nullptr;
  }

  TempFileWrapper &operator=(TempFileWrapper &&other) noexcept {
    if (this != &other) {
      if (m_file)
        fclose(m_file);
      if (!m_path.empty())
        DeleteFileW(m_path.c_str());

      m_file = other.m_file;
      m_path = std::move(other.m_path);
      other.m_file = nullptr;
    }
    return *this;
  }
};
std::string utf16_to_utf8(const std::wstring &wstr) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
  return conv.to_bytes(wstr);
}

std::string strip_data(const std::string &s) {
  std::string tail;

  const std::string data_tag = "data/";
  size_t pos = s.rfind(data_tag);
  if (pos != std::string::npos) {
    return s.substr(pos + data_tag.size());
  }
  return s;
}

static void *GetWFSOpenPtr() {
  HMODULE hCRT = GetModuleHandleW(L"ucrtbase.dll");
  if (!hCRT)
    hCRT = GetModuleHandleW(L"msvcrt.dll");
  if (hCRT)
    return GetProcAddress(hCRT, "_wfsopen");
  return nullptr;
}

static void *true_wfsopen = GetWFSOpenPtr();

SKY_AUTO_STATIC_HOOK(readFileHOOK, memory::HookPriority::Normal, true_wfsopen,
                     FILE *, const wchar_t *wpath, const wchar_t *mode,
                     int shflag) {

  std::string p = utf16_to_utf8(wpath);
  std::string inner_p = strip_data(p);
  if (brd::Options::materialBinLoaderEnabled && brd::Options::redirectShaders &&
      resourcePackManager) {
    std::string out;
    if (inner_p.find("renderer/materials/") != std::string::npos) {
      ResourceLocation location(inner_p);
      ResourcePackManager_load(resourcePackManager, location, out);
    }

    if (!out.empty()) {
      static std::vector<TempFileWrapper> tempFiles;
      static std::mutex tempFilesMutex;

      tempFiles.emplace_back(out);
      auto &wrapper = tempFiles.back();

      if (wrapper.get()) {
        std::lock_guard<std::mutex> lock(tempFilesMutex);
        tempFiles.erase(std::remove_if(tempFiles.begin(), tempFiles.end(),
                                       [](const TempFileWrapper &w) {
                                         return w.get() == nullptr;
                                       }),
                        tempFiles.end());

        return wrapper.get();
      } else {
        tempFiles.pop_back();
      }
    }
  }
  return origin(wpath, mode, shflag);
}

///////////////////////////////////////////////////////////////////////////////////////

using dragon::materials::MaterialResourceManager;

typedef void (*PFN_mce_framebuilder_BgfxFrameBuilder_discardFrame)(
    uintptr_t This, bool waitForPreviousFrame);
typedef void (*PFN_dragon_materials_CompiledMaterialManager_freeShaderBlobs)(
    uintptr_t This);
PFN_mce_framebuilder_BgfxFrameBuilder_discardFrame discardFrame = nullptr;
PFN_dragon_materials_CompiledMaterialManager_freeShaderBlobs freeShaderBlobs =
    nullptr;

bool discardFrameAndClearShaderCaches(uintptr_t bgfxFrameBuilder) {
  uintptr_t compiledMaterialManager =
      *(uintptr_t *)(*(uintptr_t *)(bgfxFrameBuilder + 40) + 16) + 776;
  uintptr_t mExtractor = *(uintptr_t *)(bgfxFrameBuilder + 32);
  MaterialResourceManager *mMaterialsManager = (MaterialResourceManager *)*(
      uintptr_t *)(mExtractor + MaterialResourceManagerOffset);
  if (discardFrame && freeShaderBlobs && mMaterialsManager) {
    discardFrame(bgfxFrameBuilder, true);
    mMaterialsManager->forceTrim();
    freeShaderBlobs(compiledMaterialManager);
    freeShaderBlobs(compiledMaterialManager);
    return true;
  }
  return false;
}
// mce::framebuilder::BgfxFrameBuilder::endFrame
SKY_AUTO_STATIC_HOOK(mce_framebuilder_BgfxFrameBuilder_endFrame,
                     memory::HookPriority::Normal,
                     std::initializer_list<const char *>(
                         {// 1.26.0
                          "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 "
                          "8D AC 24 ? ? ? ? B8 A0 1D 00 00",
                          // 1.21.130
                          "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 "
                          "8D AC 24 ? ? ? ? B8 20 1D 00 00",
                          // 1.21.120
                          "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 "
                          "8D AC 24 ? ? ? ? B8 10 1D 00 00"}),
                     void, uintptr_t This, uintptr_t frameBuilderContext) {
  bool clear = false;
  if (brd::Options::reloadShadersAvailable && brd::Options::reloadShaders) {
    brd::Options::reloadShaders = false;
    clear = true;
  }
  if (clear && discardFrameAndClearShaderCaches(This)) {
    return;
  }
  origin(This, frameBuilderContext);
}

// RayTracingFeatureConfiguration
SKY_AUTO_STATIC_HOOK(
    RayTracingResourcesConstructor, memory::HookPriority::Normal,
    std::initializer_list<const char *>(
        {"48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 50 0F 29 74 24 ? "
         "48 "
         "8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4D 8B F1"}),
    void, bgfx::RayTracingFeatureConfiguration *_this, bool rtxOn,
    void *dlssOptions, void *screenResolution, float renderScale,
    void *onResolutionChangedCallback, void *debugModeInfo) {

  gRayTracingFeatureConfiguration = _this;

  origin(_this, rtxOn, dlssOptions, screenResolution, renderScale,
         onResolutionChangedCallback, debugModeInfo);
}

//
SKY_AUTO_STATIC_HOOK(RayTracingResourcesConstrucstor,
                     memory::HookPriority::Normal,
                     std::initializer_list<const char *>(
                         {"48 89 5C 24 ? 48 89 74 24 ? 48 89 4C 24 ? 57 48 83 "
                          "EC 20 48 8B F9 33 F6 48 89 31 48 89 71 ? C7 41"}),
                     void, void *_this) {
  origin(_this);
  if (!gDeferredParams) {
    gDeferredParams =
        (dragon::framerenderer::DeferredShadingParameters *)((int64_t)_this +
                                                             176);
  }
}
////////////////////////////////////////////////////////////////////////////////
void initMCHooks() {

  discardFrame = (PFN_mce_framebuilder_BgfxFrameBuilder_discardFrame)
      memory::resolveIdentifier(
          {// 1.21.130
           "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 "
           "41 "
           "57 48 81 EC 90 00 00 00 88 54 24",
           // 1.21.120
           "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? "
           "57 41 54 41 55 41 56 41 "
           "57 48 81 EC 90 00 00 00 48 8B 05 ? ? ? ? "
           "48 33 C4 48 89 84 24 ? ? "
           "? ? 44 0F B6 EA",
           // 1.21.111
           "4C 8B DC 49 89 5B ? 49 89 6B ? 49 89 73 ? 57 41 54 41 55 41 56 "
           "41 57 48 81 EC 90 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 "
           "24 ? ? ? ? 88 54 24"});
  if (!discardFrame) {
    printf("mce::framebuilder::BgfxFrameBuilder::discardFrame not found\n");
  }

  freeShaderBlobs =
      (PFN_dragon_materials_CompiledMaterialManager_freeShaderBlobs)
          memory::resolveIdentifier(
              {// 1.21.130
               "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 "
               "56 "
               "41 "
               "57 48 83 EC 30 4C 8B E9 48 83 C1 40",

               "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 "
               "56 "
               "41 57 48 83 EC 20 4C 8B E9 48 83 C1 40"});
  if (!freeShaderBlobs) {
    printf("dragon::materials::CompiledMaterialManager::freeShaderBlobs not "
           "found\n");
  }
}

#endif
// 59/58 1.26
#if defined(__aarch64__)
SKY_AUTO_STATIC_HOOK(HOOK1, memory::HookPriority::Normal,
                     "08 EC 40 39 09 E8 40 39", bool, int64_t a1) {
  *(bool *)(a1 + 56) = 1;
  *(bool *)(a1 + 57) = 1;
  *(bool *)(a1 + 58) = 1;
  *(bool *)(a1 + 59) = 0;
  *(bool *)(a1 + 60) = 1;
  return true;
}
#elif defined(_WIN32)

// 50/51 1.26
SKY_AUTO_STATIC_HOOK(HOOK1, memory::HookPriority::Normal,
                     "80 79 ? ? 75 ? 80 79 ? ? 74 ? B0 01 C3 32 C0 C3 CC CC CC "
                     "CC CC CC CC CC CC CC CC CC CC CC 88 51 ? C3",
                     bool, int64_t a1) {

  if (shouldForceEnableVibrantVisuals()) {
    *(bool *)(a1 + 48) = 1;
    *(bool *)(a1 + 49) = 1;
    *(bool *)(a1 + 50) = 1;
    *(bool *)(a1 + 51) = 0;
    *(bool *)(a1 + 52) = 1;
    return true;
  }
  return origin(a1);
}

#endif
