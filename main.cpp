#include "App.h"
#include <memory>
//using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE prevInstance, [[maybe_unused]] PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
    #if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    // Create the app
    auto app = std::make_unique<App>();

    return 0;
}