#pragma once

namespace bgfx {
struct RayTracingConfiguration;
}
void DrawRayTracingDebugWindow(bgfx::RayTracingConfiguration *config,
                               bool *p_open = nullptr);