#include <HamsterNet.h>

HamsterNet::HamsterNet()
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
#else
extern "C"
{
    int hamsterNet__initSession()
    { 
        std::cout << "hamsterNet__initSession is not implemented on this platform, artificially succeeding.\n";
        return 1;
    }

}
#endif

void HamsterNet::SetColor(const std::string& hamsterColor)
{
    this->hamsterColor = hamsterColor;
}

void HamsterNet::SetName(const std::string& hamsterName)
{
    this->hamsterName = hamsterName;

}

void HamsterNet::InitRace()
{

}

void HamsterNet::StopRace()
{

}

void HamsterNet::FinishRace()
{

}

