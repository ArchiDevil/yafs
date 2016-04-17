#include "GameState.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/inputConverter.h>

// #include "../Entities/GameObjectsManager.h"

//TEMPORARY
const float minR = 15.0f;
const float maxR = 60.0f;
float phi = 0.0f;
float theta = -35.0f;
float r = 20.0f;
size_t mousePath = 0;
//END OF TEMPORARY

using namespace MathLib;

GameState::GameState(IniWorker * iw/*, MyGUI::Gui * guiModule, MyGUI::DirectX11Platform * guiPlatform*/)
    : iniLoader(iw)
    //, guiModule(guiModule)
    //, guiPlatform(guiPlatform)
{
}

GameState::~GameState()
{
}

bool GameState::initState()
{
    // to receive events for GUI
    subscribe(&InputEngine::GetInstance());

    LostIsland::CreateGame();

    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    Game * pGame = LostIsland::GetGamePtr();

    ::utils::filesystem::create_dir(L"saves/worlds/");
    ::utils::filesystem::create_dir(L"saves/players/");
    ::utils::filesystem::create_dir(L"saves/worlds/tempWorld/");

    //pGame->gameHud.reset(new GameHUD(guiModule));
    //LOG_INFO("HUD has been created");

    ShiftEngine::CameraSceneNode * pCamera = pScene->AddCameraSceneNode();
    pCamera->SetPosition({ 0.0f, 0.0f, 0.0f });
    pCamera->RotateByQuaternion(quaternionFromVecAngle(Vector3F(1.0f, 0.0f, 0.0f), degrad(-60.0f)));

    pScene->SetAmbientColor(Vector3F(0.1f, 0.1f, 0.15f));

    LOG_INFO("End of game state initializing");

    return true;
}

bool GameState::update(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    Game * pGame = LostIsland::GetGamePtr();

    ProcessInput(dt);
    // pGame->gameHud->Update(dt);

    // pCamera->SetSphericalCoords(playerPosition, phi, theta, r);

    return true;
}

bool GameState::render(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    ShiftEngine::Renderer * pRenderer = ShiftEngine::GetRenderer();
    Game * pGame = LostIsland::GetGamePtr();

#if defined (DEBUG) || (_DEBUG)
    const int infoSize = 6;
    std::ostringstream di[infoSize];

    di[0] << "FPS: " << pRenderer->GetFPS();
    di[1] << "Shader changes: " << pRenderer->GetShaderChanges();
    di[2] << "Matrix bindings: " << pRenderer->GetMatricesBindings();
    di[3] << "Uniform bindings: " << pRenderer->GetUniformsBindings();
    di[4] << "Texture bindings: " << pRenderer->GetTextureBindings();
    di[5] << "Draw calls: " << pRenderer->GetDrawCalls();
#else
    const int infoSize = 2;
    std::ostringstream di[infoSize];
    di[0] << "FPS: " << pRenderer->GetFPS();
    di[1] << "Time of day: " << pGame->environmentMgr->GetTime().GetHours() << ":"
        << pGame->environmentMgr->GetTime().GetMinutes();
#endif

    ////////////
    // RENDER //
    ////////////

    ShiftEngine::FontManager * pFntMgr = pCtxMgr->GetFontManager();
    pFntMgr->SetFont(L"1");

    pCtxMgr->SetBlendingState(ShiftEngine::BlendingState::AlphaEnabled);

    pCtxMgr->BeginScene(); //no more needed here, cause clear frame should be called from renderer

    pScene->DrawAll(dt);

    for (int i = 0; i < infoSize; i++)
        pFntMgr->DrawTextTL(di[i].str(), 0.0f, i * 32.0f);

    //guiPlatform->getRenderManagerPtr()->drawOneFrame();

    pCtxMgr->EndScene();

    return true; //return false if something wrong
}

void GameState::onKill()
{
}

void GameState::onSuspend()
{
}

void GameState::onResume()
{
}

void GameState::ProcessInput(double dt)
{
    InputEngine & inputEngine = InputEngine::GetInstance();
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    Game * pGame = LostIsland::GetGamePtr();
    inputEngine.GetKeys();
    auto mouseInfo = inputEngine.GetMouseInfo();

    if (inputEngine.IsKeyDown(DIK_ESCAPE))
        kill();

    if (inputEngine.IsKeyUp(DIK_V))
        switchWireframe();

    //MyGUI::InputManager& inputManager = MyGUI::InputManager::getInstance();
    //bool guiInjected = inputManager.injectMouseMove(mouseInfo.clientX, mouseInfo.clientY, 0);

    //if (inputEngine.IsMouseDown(LButton))
    //    guiInjected |= inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);

    //if (inputEngine.IsMouseUp(LButton))
    //    guiInjected |= inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);

    //if (inputEngine.IsMouseDown(RButton))
    //    guiInjected |= inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);

    //if (inputEngine.IsMouseUp(RButton))
    //    guiInjected |= inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);

    //if (guiInjected)
    //    return;

    //if (inputEngine.IsMouseMoved() && inputEngine.IsMouseDown(RButton))
    //{
    //    theta -= (float)mouseInfo.deltaY * (float)dt * 10.0f;
    //    phi += (float)mouseInfo.deltaX * (float)dt * 10.0f;
    //    if (theta <= -35.0f)
    //        theta = -35.0f;
    //    if (theta >= -5.0f)
    //        theta = -5.0f;
    //}

    r -= (float)mouseInfo.deltaZ * (float)dt;
    r = clamp(r, minR, maxR);
}

bool GameState::handleEvent(const InputEvent & event)
{
    //MyGUI::InputManager& inputManager = MyGUI::InputManager::getInstance();

    //switch (event.type)
    //{
    //    // there will be always DirectInput keys in first two handlers
    //case InputEventType::KeyDown:
    //    inputManager.injectKeyPress((MyGUI::KeyCode::Enum)event.key);
    //    break;
    //case InputEventType::KeyUp:
    //    inputManager.injectKeyRelease((MyGUI::KeyCode::Enum)event.key);
    //    break;

    //    // there will be windows keys
    //case InputEventType::SystemKey:
    //    inputManager.injectKeyPress((MyGUI::KeyCode::Enum)InputConverter::VirtualKeyToScanCode(event.key), event.key);
    //    break;
    //}

    return true;
}

void GameState::switchWireframe()
{
#if defined DEBUG || defined _DEBUG
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();

    static bool Wflag = false;
    Wflag = !Wflag;
    if (Wflag)
        pCtxMgr->SetRasterizerState(ShiftEngine::RasterizerState::Wireframe);
    else
        pCtxMgr->SetRasterizerState(ShiftEngine::RasterizerState::Normal);
#endif
}

MathLib::Ray GameState::getUnprojectedRay(const MathLib::Vector2I & clientMouseCoords) const
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();

    ShiftEngine::GraphicEngineSettings settings = pCtxMgr->GetEngineSettings();
    vec2<unsigned int> sizes = vec2<unsigned int>(settings.screenWidth, settings.screenHeight);

    // do the raycasting
    mat4f projMatrix = pScene->GetActiveCamera()->GetProjectionMatrix();
    mat4f viewMatrix = pScene->GetActiveCamera()->GetViewMatrix();
    Vector3F resultNear = getUnprojectedVector(Vector3F((float)clientMouseCoords.x, (float)clientMouseCoords.y, 0.0f), projMatrix, viewMatrix, sizes);
    Vector3F resultFar = getUnprojectedVector(Vector3F((float)clientMouseCoords.x, (float)clientMouseCoords.y, 1.0f), projMatrix, viewMatrix, sizes);
    Ray unprojectedRay = Ray(resultNear, normalize(resultFar - resultNear));

    return unprojectedRay;
}
