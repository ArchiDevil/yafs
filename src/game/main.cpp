#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GoingHomeApplication.h" //Win32 application class

#if defined(DEBUG) || defined(_DEBUG) 
int main()
#elif defined(NDEBUG) || defined(_NDEBUG)
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PSTR /*lpCmdLine*/, int /*nShowCmd*/)
#endif
{
    srand(GetTickCount());

    IniWorker i;
    if (!i.Initialize("settings.ini"))
        return -1;

    GoingHomeApplication app(i.GetInteger("Width"), i.GetInteger("Height"), L"Going Home");

    return app.Run();
}
