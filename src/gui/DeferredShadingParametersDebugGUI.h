#pragma once

namespace dragon {
namespace framerenderer {
struct DeferredShadingParameters;
}
} // namespace dragon

void DrawDeferredShadingParametersDebugWindow(
    dragon::framerenderer::DeferredShadingParameters *params,
    bool *p_open = nullptr);