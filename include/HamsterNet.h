#pragma once
#ifndef HAMSTER_NET_H
#define HAMSTER_NET_H

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

#include <string>
#include <cstdint>

class HamsterNet
{
public:
    HamsterNet();

    void SetColor(const std::string& hamsterColor);
    void SetName(const std::string& hamsterName);

    void InitRace();
    void StopRace();
    void FinishRace();

private:
    std::string hamsterColor;
    std::string hamsterName;
    std::string raceMapName;
    uint64_t raceTime;

    std::string raceId;
};





#endif
