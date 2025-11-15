#pragma once

#include "DeferredShadingParametersDebugGUI.h"
#include "RayTracingDebugGUI.h"
#include "mc/client/bgfx/bgfx.h"

extern dragon::framerenderer::DeferredShadingParameters *gDeferredParams;
void initializeImGui(bool);
void updateImGui();
void updateKeys();
void updateOptions();
