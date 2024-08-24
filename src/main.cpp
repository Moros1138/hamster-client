#include "olcPixelGameEngine.h"

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
                    break;
                case State::RacerName:
                    state = State::BeginRace;
                    break;
                case State::BeginRace:
                    break;
                case State::EndRace:
                    break;
            }
        }
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
