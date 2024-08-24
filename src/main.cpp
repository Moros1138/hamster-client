#include "olcPixelGameEngine.h"
#include "HamsterNet.h"

class Test : public olc::PixelGameEngine
{
public:
    Test()
    {
        sAppName = "Hamster API Testbed";
    }

public:

    HamsterNet hamsterNet;
    std::vector<LeaderboardEntry> map1;

    enum State : uint8_t {
        Init = 0,
        RacerName,
        RacerColor,
        BeginRace,
        EndRace
    };

    State state = State::Init;
    
    std::string text = "";

    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserDestroy() override
    {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        if(GetKey(olc::A).bPressed)
        {
            map1 = hamsterNet.GetLeaderboard("map1");
            for(auto &entry : map1)
            {
                std::cout << entry.name << " " << entry.color << " " << entry.time << "\n";
            }
        }

        if(GetKey(olc::R).bPressed)
        {
            state = State::Init;
        }

        if(GetKey(olc::SPACE).bReleased)
        {
            switch(state)
            {
                case State::Init:
                    if(hamsterNet.InitSession())
                    {
                        text = "Session started.\n";
                        state = State::RacerName;
                    }
                    else
                    {
                        text = "Session failed to start.\n";
                    }
                    break;
                case State::RacerName:
                    if(hamsterNet.SetName("Jonny"))
                    {
                        text = "Set name to: Jonny\n";
                        state = State::RacerColor;
                    }
                    else
                    {
                        text = "Failed to set name.\n";
                    }
                    break;
                case State::RacerColor:
                    hamsterNet.SetColor("red");
                    text = "Set color to: red\n";
                    state = State::BeginRace;
                    break;
                case State::BeginRace:
                    if(hamsterNet.StartRace("map-123"))
                    {
                        text = "Started a race on map-123\n";
                        state = State::EndRace;
                    }
                    else
                    {
                        text = "Failed to start race on map-123.\n";
                    }
                    break;
                case State::EndRace:
                    if(hamsterNet.FinishRace())
                    {
                        text = "Finished race successfully.\n";
                    }
                    else
                    {
                        text = "Failed to finish race on map-123.\n";
                    }
                    break;
            }
        }
        
        DrawString(5, 5, text);
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
