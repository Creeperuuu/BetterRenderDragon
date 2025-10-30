#include "Options.h"
#include "imgui.h"
#include <atomic>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <shlobj.h>
#include <string>
#include <vector>
#include <windows.h>


using nlohmann::json;

namespace brd {
// --- Option defs ---
namespace Options {
Option<bool> showImGui = true;
Option<bool> performanceEnabled = true;
Option<bool> windowSettingsEnabled = true;
bool vanilla2DeferredAvailable = true;
bool newVideoSettingsAvailable = false;
Option<bool> vanilla2DeferredEnabled = true;
Option<bool> deferredRenderingEnabled = false;
Option<bool> forceEnableDeferredTechnicalPreview = false;
Option<bool> disableRendererContextD3D12RTX = false;
Option<bool> materialBinLoaderEnabled = true;
Option<bool> redirectShaders = true;
bool reloadShadersAvailable = true;
std::atomic_bool reloadShaders = false;
Option<bool> customUniformsEnabled = false;
Option<bool> forceEnableVibrantVisuals = true;
Option<int> uiKey = ImGuiKey_F6;
Option<int> reloadShadersKey = ImGuiKey_None;

std::string optionsDir;
std::string optionsFile;
} // namespace Options

std::string wstringToString(const std::wstring &wstr) {
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(),
                                        (int)wstr.size(), NULL, 0, NULL, NULL);
  std::string strTo(size_needed, 0);
  WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0],
                      size_needed, NULL, NULL);
  return strTo;
}

static void DebugTrace(const char *fmt, ...) {
  char buf[1024];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  OutputDebugStringA(buf);
}

std::string getMinecraftModsPath() {
  char appDataPath[MAX_PATH];
  if (FAILED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
    printf("Failed to get APPDATA path.\n");
    return "";
  }

  std::string path = std::string(appDataPath) + "\\Minecraft Bedrock\\mods";
  return path;
}

// ----------------- 选项处理函数 -----------------
static std::vector<IOption *> options;
bool Options::init() {
  options.clear();
  options.push_back(&showImGui);
  options.push_back(&performanceEnabled);
  options.push_back(&windowSettingsEnabled);
  options.push_back(&vanilla2DeferredEnabled);
  options.push_back(&deferredRenderingEnabled);
  options.push_back(&forceEnableDeferredTechnicalPreview);
  options.push_back(&disableRendererContextD3D12RTX);
  options.push_back(&materialBinLoaderEnabled);
  options.push_back(&redirectShaders);
  options.push_back(&customUniformsEnabled);
  options.push_back(&uiKey);
  options.push_back(&reloadShadersKey);
  options.push_back(&forceEnableVibrantVisuals);

  if (optionsDir.empty()) {
    std::string localStatePath = getMinecraftModsPath();
    printf("getMinecraftModsPath: %s\n", localStatePath.c_str());
    if (localStatePath.empty()) {
      return false;
    }
    optionsDir = localStatePath + "\\BetterRenderDragon";
    optionsFile = optionsDir + "\\BetterRenderDragon.json";
  }

  if (!std::filesystem::exists(optionsDir)) {
    if (!std::filesystem::create_directories(optionsDir)) {
      printf("Failed to create options directory: %s\n", optionsDir.c_str());
      return false;
    }
  }
  if (!std::filesystem::is_directory(optionsDir)) {
    printf("optionsDir not a directory: %s\n", optionsDir.c_str());
    return false;
  }
  return true;
}

bool Options::load() {
  if (!std::filesystem::exists(optionsFile)) {
    printf("Options file does not exist: %s\n", optionsFile.c_str());
    return save();
  }
  if (!std::filesystem::is_regular_file(optionsFile)) {
    printf("Options file is not a regular file: %s\n", optionsFile.c_str());
    return false;
  }

  json data;
  try {
    std::ifstream ifs(optionsFile, std::ifstream::binary);
    if (!ifs) {
      printf("Cannot open options file: %s\n", optionsFile.c_str());
      return save();
    }
    ifs >> data;
  } catch (json::parse_error &e) {
    printf("Failed to parse json: %s\n", e.what());
    return false;
  } catch (...) {
    printf("Failed to read options file: %s (unknown error)\n",
           optionsFile.c_str());
    return false;
  }

  if (data.contains("showImGui"))
    showImGui = data["showImGui"];
  if (data.contains("performanceEnabled"))
    performanceEnabled = data["performanceEnabled"];
  if (data.contains("vanilla2DeferredEnabled"))
    vanilla2DeferredEnabled = data["vanilla2DeferredEnabled"];
  if (data.contains("deferredRenderingEnabled"))
    deferredRenderingEnabled = data["deferredRenderingEnabled"];
  if (data.contains("forceEnableDeferredTechnicalPreview"))
    forceEnableDeferredTechnicalPreview =
        data["forceEnableDeferredTechnicalPreview"];
  if (data.contains("disableRendererContextD3D12RTX"))
    disableRendererContextD3D12RTX = data["disableRendererContextD3D12RTX"];
  if (data.contains("materialBinLoaderEnabled"))
    materialBinLoaderEnabled = data["materialBinLoaderEnabled"];
  if (data.contains("redirectShaders"))
    redirectShaders = data["redirectShaders"];
  if (data.contains("forceEnableVibrantVisuals"))
    forceEnableVibrantVisuals = data["forceEnableVibrantVisuals"];

  return true;
}

bool Options::save() {
  json data;
  data["showImGui"] = showImGui.get();
  data["performanceEnabled"] = performanceEnabled.get();
  data["vanilla2DeferredEnabled"] = vanilla2DeferredEnabled.get();
  data["deferredRenderingEnabled"] = deferredRenderingEnabled.get();
  data["forceEnableDeferredTechnicalPreview"] =
      forceEnableDeferredTechnicalPreview.get();
  data["disableRendererContextD3D12RTX"] = disableRendererContextD3D12RTX.get();
  data["materialBinLoaderEnabled"] = materialBinLoaderEnabled.get();
  data["redirectShaders"] = redirectShaders.get();
  data["forceEnableVibrantVisuals"] = forceEnableVibrantVisuals.get();

  try {
    std::ofstream ofs(optionsFile,
                      std::ofstream::binary | std::ofstream::trunc);
    if (!ofs) {
      printf("Failed to open options file for write: %s\n",
             optionsFile.c_str());
      return false;
    }
    ofs << std::setw(4) << data << '\n';
  } catch (...) {
    printf("Failed to write options file: %s (unknown error)\n",
           optionsFile.c_str());
    return false;
  }
  return true;
}

void Options::record() {
  for (auto opt : options)
    opt->record();
}
bool Options::isDirty() {
  for (auto opt : options) {
    if (opt->isChanged())
      return true;
  }
  return false;
}
} // namespace brd