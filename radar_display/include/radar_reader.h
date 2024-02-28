#pragma once

#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include "radar_event.h"

class TRadarReader
{
public:
    TRadarReader();
    ~TRadarReader();
    bool ReadEvent(TRadarEvent &ev);
    void ReaderLoop();

private:
    std::atomic<bool> Running;
    std::thread ReaderThread;
    std::mutex ReaderMutex;
    std::queue<TRadarEvent> Events;
};
