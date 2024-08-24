#include <HamsterNet.h>

#ifdef __EMSCRIPTEN__

EM_JS(int, hamsterNet__initSession, (), {
    return Asyncify.handleSleep(function(wakeUp) {
        fetch('/session', { method: 'POST', credentials: 'same-origin' })
            .then((response) =>
            {
                return response.ok
                ? response.json().then((data) => JSON.stringify(data, null, 2))
                : Promise.reject(new Error("Unexpected response"));
            })
            .then((message) =>
            {
                console.log(message);
                wakeUp(1);
            })
            .catch((err) =>
            {
                console.error(err.message);
                wakeUp(0);
            })    
    });
});

EM_JS(int, hamsterNet__setRacerName, (const char* str), {
    
    let racerName = UTF8ToString(str);
    
    return Asyncify.handleSleep(function(wakeUp) {
        fetch('/name', {
            method: 'POST',
            credentials: 'same-origin',
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify({ userName: racerName })
        }).then((response) =>
        {
            return response.ok
            ? response.json().then((data) => JSON.stringify(data, null, 2))
            : Promise.reject(new Error("Unexpected response"));
        })
        .then((message) =>
        {
            wakeUp(1);
        })
        .catch((err) =>
        {
            console.error(err.message);
            wakeUp(0);
        })    
    });
});

EM_JS(int, hamsterNet__startRace, (), {
    
    return Asyncify.handleSleep(function(wakeUp) {
        fetch('/race', {
            method: 'POST',
            credentials: 'same-origin',
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify({})
        }).then((response) =>
        {
            return response.ok
            ? response.json()
            : Promise.reject(new Error("Unexpected response"));
        })
        .then((message) =>
        {
            Module.hamsterRaceId = message.raceId;
            wakeUp(1);
        })
        .catch((err) =>
        {
            console.error(err.message);
            wakeUp(0);
        })    
    });
});
EM_JS(int, hamsterNet__finishRace, (const char* raceMap, const char* raceColor, int raceTime), {
    
    if(Module.hamsterRaceId === undefined)
    {
        console.error("Trying to finish a race that never start!");
        return 0;
    }
    
    const raceData = {
        raceId: Module.hamsterRaceId,
        raceTime: raceTime,
        raceMap: UTF8ToString(raceMap),
        raceColor: UTF8ToString(raceColor),
    };
    
    return Asyncify.handleSleep(function(wakeUp) {
        fetch('/race', {
            method: 'PATCH',
            credentials: 'same-origin',
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify(raceData)
        }).then((response) =>
        {
            return response.ok
            ? response.json().then((data) => JSON.stringify(data, null, 2))
            : Promise.reject(new Error("Unexpected response"));
        })
        .then((message) =>
        {
            wakeUp(1);
        })
        .catch((err) =>
        {
            console.error(err.message);
            wakeUp(0);
        })    
    });
});

#else
extern "C"
{
    int hamsterNet__initSession()
    { 
        std::cout << "hamsterNet__initSession is not implemented on this platform, artificially succeeding.\n";
        return 1;
    }
    
    int hamsterNet__setRacerName(const char* str)
    {
        std::cout << "hamsterNet__setRacerName is not implemented on this platform, artificially succeeding.\n";
        return 1;
    }
    
    int hamsterNet__startRace()
    {
        std::cout << "hamsterNet__startRace is not implemented on this platform, artificially succeeding.\n";
        return 1;
    }

    int hamsterNet__finishRace(const char* raceMap, const char* raceColor, int raceTime)
    {
        std::cout << "hamsterNet__finishRace is not implemented on this platform, artificially succeeding.\n";
        return 1;
    }
}
#endif

void HamsterNet::SetColor(const std::string& hamsterColor)
HamsterNet::HamsterNet()
{ }
{
    this->hamsterColor = hamsterColor;
}

void HamsterNet::SetName(const std::string& hamsterName)
{
    this->hamsterName = hamsterName;

}

void HamsterNet::InitRace()
bool HamsterNet::SetName(const std::string& name)
{
    m_name = name;
    return (hamsterNet__setRacerName(m_name.c_str()) == 1);
}

bool HamsterNet::StartRace(const std::string& map)
{
    m_map = map;
    m_tp1 = std::chrono::system_clock::now();
    m_tp2 = std::chrono::system_clock::now();
    return (hamsterNet__startRace() == 1);
}

bool HamsterNet::FinishRace()
{
    m_tp2 = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> duration = m_tp2 - m_tp1;
    m_time = static_cast<int>(duration.count());

    return (hamsterNet__finishRace(m_map.c_str(), m_color.c_str(), m_time) == 1);
}
