#include <iostream>
#include <cstdio>
#include <radar_reader.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

#include "radar_reader.h"

TRadarReader::TRadarReader()
    : Running(true)
{
    ReaderThread = std::thread(&TRadarReader::ReaderLoop, this);
}

TRadarReader::~TRadarReader() {
    Running = false;
    ReaderThread.join();
}

void TRadarReader::ReaderLoop() {
    TRadarEvent ev;
    std::string line;
    char buf[1024];

    struct pollfd cinfd = {0, POLLIN, 0};

    while (Running) {
        int pres = poll(&cinfd, 1, 100);
        if (pres <= 0 || !(cinfd.revents & POLLIN)) {
            continue;
        }

        memset(buf, 0, 1024);
        int nread = read(0, buf, 1024);
        if (nread <= 0) {
            continue;
        }

        int nargs = sscanf(buf, "%u %u", &ev.Angle, &ev.Distance);
        if (nargs == 2) {
            std::scoped_lock lock(ReaderMutex);
            ev.Distance *= 10; // Convert from cm to mm
            Events.push(ev);
        }
    }
}

bool TRadarReader::ReadEvent(TRadarEvent &ev) {
    std::scoped_lock lock(ReaderMutex);
    if (Events.size() > 0) {
        ev = Events.front();
        Events.pop();
        return true;
    }
    return false;
}
