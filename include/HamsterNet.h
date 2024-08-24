#pragma once
#ifndef HAMSTER_NET_H
#define HAMSTER_NET_H

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

#include <chrono>
#include <iostream>
#include <string>

class HamsterNet
{
public:
    HamsterNet();
    
    bool InitSession();
    void SetColor(const std::string& color);
    bool SetName(const std::string& name);

    bool StartRace(const std::string& map);
    bool FinishRace();

private:
    std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;

    std::string m_color;
    std::string m_name;
    std::string m_map;
    int m_time;

    std::string raceId;
};

#endif // HAMSTER_NET_H
