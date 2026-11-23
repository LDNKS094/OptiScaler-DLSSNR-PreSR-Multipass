#pragma once
#include <atomic>
#include <cstdint>

// Callbacks run synchronously under the D3D11 feature lock. Registration replays existing handles.
using OptiScalerLifecycleCallbackV2 = void (*)(uint32_t event, uint64_t handle);
enum : uint32_t
{
    OptiScalerLifecycleCreatedV1 = 1,
    OptiScalerLifecycleEvaluatedV1 = 2,
    OptiScalerLifecycleReleasedV1 = 3
};

namespace LifecycleAdapterV1
{
inline std::atomic<bool> frameGenerationPaused { false };
inline bool FrameGenerationPaused() { return frameGenerationPaused.load(std::memory_order_acquire); }
}
