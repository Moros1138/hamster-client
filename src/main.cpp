#include "olcPixelGameEngine.h"

#include <chrono>
#include <emscripten.h>

EM_JS(int, InitSession, (), {
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

EM_JS(int, SetRacerName, (const char* str), {
    
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

EM_JS(int, StartRace, (), {
    
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

EM_JS(int, FinishRace, (const char* raceMap, const char* raceColor, int raceTime), {
    
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

class Test : public olc::PixelGameEngine
{
public:
    Test()
    {
        sAppName = "Hamster API Testbed";
    }

public:

    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserDestroy() override
    {
        return true;
    }

    enum State : uint8_t {
        Init = 0,
        RacerName,
        BeginRace,
        EndRace
    };

    State state = State::Init;
    std::chrono::time_point<std::chrono::system_clock> tp1, tp2;
    
    bool OnUserUpdate(float fElapsedTime) override
    {
        if(GetKey(olc::R).bPressed)
        {
            state = State::Init;
        }

        if(GetKey(olc::SPACE).bReleased)
        {
            switch(state)
            {
                case State::Init:
                    InitSession();
                    state = State::RacerName;
                    break;
                case State::RacerName:
                    SetRacerName("Jonny");
                    state = State::BeginRace;
                    break;
                case State::BeginRace:
                    StartRace();
                    tp1 = std::chrono::system_clock::now();
            		tp2 = std::chrono::system_clock::now();
                    state = State::EndRace;
                    break;
                case State::EndRace:
                    tp2 = std::chrono::system_clock::now();
                    std::chrono::duration<double, std::milli> duration = tp2 - tp1;
                    int raceTime = static_cast<int>(duration.count());
                    std::cout << "RACETIME: " << raceTime << "\n";
                    FinishRace("map-123", "red", raceTime);
                    break;
            }
        }
        auto start = std::chrono::high_resolution_clock::now();
        return true;
    }
};

int main()
{
    Test game;
    if(game.Construct(320, 180, 4, 4))
        game.Start();

    return 0;
}
