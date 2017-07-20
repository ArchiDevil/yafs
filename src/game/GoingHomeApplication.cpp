#include "GoingHomeApplication.h"

#include <Utilities/logger.hpp>

//TEMPORARY

int PerfCounter = 0;
int minFPS = 100000;
int maxFPS = 0;
double AllTime = 0.0f;

//END OF TEMPORARY

Log PerformanceLog("Performance.log");

GoingHomeApplication::GoingHomeApplication(int Width, int Height, LPCWSTR AppName)
    : Application(Width, Height, AppName)
{
}

GoingHomeApplication::~GoingHomeApplication()
{
}

bool GoingHomeApplication::Initialize()
{
    // ::ShowCursor(false);

    // Init files loader initialization
    settingsLoader.Initialize("settings.ini");

    // Load settings for graphics engine
    ShiftEngine::GraphicEngineSettings settings;
    settings.screenHeight       = settingsLoader.GetInteger("Height");
    settings.screenWidth        = settingsLoader.GetInteger("Width");
    settings.multisampleQuality = settingsLoader.GetInteger("MultisampleQuality");
    settings.windowed           = settingsLoader.GetBoolean("Windowed");
    settings.screenRate         = settingsLoader.GetInteger("ScreenRate");
    settings.zNear              = settingsLoader.GetFloat("zNear");
    settings.zFar               = settingsLoader.GetFloat("zFar");
    settings.anisotropyLevel    = settingsLoader.GetInteger("AnisotropyLevel");

    // Load paths settings
    ShiftEngine::PathSettings path;
    path.TexturePath   = settingsLoader.GetWString("TexturePath");
    path.ShaderPath    = settingsLoader.GetWString("ShaderPath");
    path.FontsPath     = settingsLoader.GetWString("FontsPath");

    // Graphics engine initialization
    if (!ShiftEngine::InitEngine(settings, path, GetHWND(), ShiftEngine::SceneGraphType::SGT_Plain))
        LOG_FATAL_ERROR("Unable to inititalize graphics engine");
    else
        LOG_INFO("Graphics engine has been initialized");

    if (!InputEngine::GetInstance().Initialize(GetHWND(), GetHINSTANCE()))
        LOG_FATAL_ERROR("Unable to create DIDevice");
    else
        LOG_INFO("Input system has been initialized");

    InputEngine::GetInstance().subscribe(&System);

//    guiPlatform = new MyGUI::DirectX11Platform();
//    std::string logFile = "MyGUI.log";
//#ifdef NDEBUG
//    logFile = "";
//#endif // NDEBUG
//    // MyGUI::LogManager::getInstance().setSTDOutputEnabled(false);
//
//    guiPlatform->initialise(((ShiftEngine::D3D11ContextManager*)ShiftEngine::GetContextManager())->GetDevicePtr(), logFile);
//    guiPlatform->getDataManagerPtr()->addResourceLocation("resources/ui/MyGUI_Media", false);
//    guiPlatform->getDataManagerPtr()->addResourceLocation("resources/textures", false);
//
//    guiModule = new MyGUI::Gui();
//    guiModule->initialise();
//
//    guiPlatform->getRenderManagerPtr()->setViewSize(settings.screenWidth, settings.screenHeight);

    ////////////////////////////
    /// GAME STRUCTURES INIT ///
    ////////////////////////////

    GameState * state = new GameState(&settingsLoader/*, guiModule, guiPlatform*/);
    PushState(state);

    this->SaveTechInfo();
    LOG_INFO("Successfully initialized, starting main loop");
    gameTimer.start();

    return true;
}

void GoingHomeApplication::Shutdown()
{
    ShiftEngine::ShutdownEngine();

    PerformanceLog.Message("Min FPS = " + std::to_string(minFPS) + " FPS");
    PerformanceLog.Message("Average FPS = " + std::to_string((int)(PerfCounter / AllTime)) + " FPS");
    PerformanceLog.Message("Max FPS = " + std::to_string(maxFPS) + " FPS");
    PerformanceLog.Message("Frames elapsed = " + std::to_string(PerfCounter) + " frames");
    PerformanceLog.Message("All time = " + std::to_string((int)AllTime) + " seconds");
    LOG_INFO("Working done");
}

bool GoingHomeApplication::Frame()
{
    static double elapsedTime = 0.0f;

    elapsedTime = gameTimer.get_delta_time();
    PerfCounter++;
    AllTime += elapsedTime;
    gameTimer.tick();

    //int curFPS = ShiftEngine::GetRenderer()->GetFPS();

    //if (curFPS > maxFPS)
    //    maxFPS = curFPS;
    //if (curFPS < minFPS)
    //    minFPS = curFPS;

    return stateMachine.Frame(elapsedTime);
}

void GoingHomeApplication::PushState(IAppState * state)
{
    stateMachine.PushState(state);
}

void GoingHomeApplication::Stop()
{
    gameTimer.stop();
    stateMachine.Suspend();
}

void GoingHomeApplication::Activate()
{
    gameTimer.start();
    stateMachine.Resume();
}

void GoingHomeApplication::ProcessMessage(MSG /*msg*/)
{
    static AppState prevState = AS_Running;
    AppState currState = System.GetState();

    if (prevState != currState)
    {
        switch (currState)
        {
        case AS_Running:
            this->Activate();
            break;
        case AS_Inactive:
            this->Stop();
            break;
        }
        prevState = currState;
    }
}

void GoingHomeApplication::SaveTechInfo()
{
    // Collection information about CPU
    PerformanceLog.Message("Processor name: " + utils::narrow(registryWorker.GetString(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"ProcessorNameString")));
    PerformanceLog.Message("Frequency = " + std::to_string(registryWorker.GetInteger(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"~MHz")) + " MHz");

    // Getting memory info + GPU description
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    PerformanceLog.Message("Total physical memory: " + std::to_string((int64_t)memoryStatus.ullTotalPhys / 1024 / 1024) + " MBs");
    PerformanceLog.Message("Using: " + std::to_string((int)memoryStatus.dwMemoryLoad) + "%");

    const auto& devicesDesc = ShiftEngine::GetDevicesDescription();
    for (size_t i = 0; i < devicesDesc.size(); ++i)
        PerformanceLog.Message("GPU" + std::to_string(i) + " description: " + devicesDesc[i]);
}

IAppState * GoingHomeApplication::GetTopState() const
{
    return stateMachine.GetTopState();
}
