#pragma once
#include <cstdint>
#include <mutex>

// Counts app presents and Streamline presentations over the same elapsed window.
class DlssgFrameRates
{
  public:
    struct Snapshot { double app = -1.0; double presented = -1.0; };

    void Record(double nowMs, const void* swapchain, bool active, bool valid, uint32_t presented)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_started || _swapchain != swapchain || _active != active || !valid)
        {
            _started = valid;
            _swapchain = swapchain;
            _active = active;
            _startMs = nowMs;
            _appCount = _presentedCount = 0;
            _snapshot = {};
            return;
        }
        ++_appCount;
        _presentedCount += presented;
        const double seconds = (nowMs - _startMs) / 1000.0;
        if (seconds >= 1.0)
        {
            _snapshot = { _appCount / seconds, _presentedCount / seconds };
            _startMs = nowMs;
            _appCount = _presentedCount = 0;
        }
    }

    Snapshot Read()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _snapshot;
    }

  private:
    std::mutex _mutex;
    const void* _swapchain = nullptr;
    bool _started = false, _active = false;
    double _startMs = 0.0;
    uint64_t _appCount = 0, _presentedCount = 0;
    Snapshot _snapshot;
};
