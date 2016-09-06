#include "GameState.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/inputConverter.h>

using namespace MathLib;

Enemy* testEnemy = nullptr;

GameState::GameState(IniWorker * iw/*, MyGUI::Gui * guiModule, MyGUI::DirectX11Platform * guiPlatform*/)
    : iniLoader(iw)
    //, guiModule(guiModule)
    //, guiPlatform(guiPlatform)
{}

GameState::~GameState()
{}

bool GameState::initState()
{
    // to receive events for GUI
    subscribe(&InputEngine::GetInstance());

    GoingHome::CreateGame();

    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();

    //pGame->gameHud.reset(new GameHUD(guiModule));
    //LOG_INFO("HUD has been created");

    ShiftEngine::CameraSceneNode * pCamera = pScene->AddCameraSceneNode(ShiftEngine::CameraViewType::Orthographic);
    pCamera->SetLocalPosition({0.0f, 0.0f, 0.0f});
    pCamera->SetScreenWidth(1024.0f / 600.0f * 6.0f);
    pCamera->SetScreenHeight(600.0f / 600.0f * 6.0f);

    pScene->SetAmbientColor(vec3f(0.1f, 0.1f, 0.15f));

    // just for example, let's create some enemies
    testEnemy = GoingHome::GetGamePtr()->GetEntityMgr()->CreateEnemy({1.0f, 1.0f}, 2.0f, 100).get();
    testEnemy->MoveTo({0.0f, 0.0f});

    LOG_INFO("End of game state initializing");

    return true;
}

bool GameState::update(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();

    // for example
    static double totalTime = 0.0;
    totalTime += dt;

    GoingHome::GetGamePtr()->GetBackgroundMgr()->Update(dt);
    GoingHome::GetGamePtr()->GetEntityMgr()->UpdateAllEntities(dt);

    auto playerPosition = GoingHome::GetGamePtr()->GetPlayerPtr()->GetPosition();
    pScene->GetActiveCamera()->SetLocalPosition({playerPosition.x, playerPosition.y, 0.0f});
    
    ProcessInput(dt);
    // pGame->gameHud->Update(dt);

    GoingHome::GetGamePtr()->GetEntityMgr()->UpdateAllEntities(dt);

    return true;
}

bool GameState::render(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    ShiftEngine::Renderer * pRenderer = ShiftEngine::GetRenderer();

#if defined (DEBUG) || (_DEBUG)
    const int infoSize = 7;
    std::ostringstream di[infoSize];

    di[0] << "FPS: " << pRenderer->GetFPS();
    di[1] << "Shader changes: " << pRenderer->GetShaderChanges();
    di[2] << "Matrix bindings: " << pRenderer->GetMatricesBindings();
    di[3] << "Uniform bindings: " << pRenderer->GetUniformsBindings();
    di[4] << "Texture bindings: " << pRenderer->GetTextureBindings();
    di[5] << "Draw calls: " << pRenderer->GetDrawCalls();
    di[6] << "Player position: " << GoingHome::GetGamePtr()->GetPlayerPtr()->GetPosition().x << " " << GoingHome::GetGamePtr()->GetPlayerPtr()->GetPosition().y;
#else
    const int infoSize = 1;
    std::ostringstream di[infoSize];
    di[0] << "FPS: " << pRenderer->GetFPS();
#endif

    ////////////
    // RENDER //
    ////////////

    ShiftEngine::FontManager * pFntMgr = pCtxMgr->GetFontManager();
    pFntMgr->SetFont(L"2");

    pCtxMgr->SetBlendingState(ShiftEngine::BlendingState::AlphaEnabled);

    pCtxMgr->BeginScene(); //no more needed here, cause clear frame should be called from renderer

    pScene->DrawAll(dt);

    for (int i = 0; i < infoSize; i++)
        pFntMgr->DrawTextTL(di[i].str(), 5.0f, 5.0f + i * 16.0f);

    //guiPlatform->getRenderManagerPtr()->drawOneFrame();

    pCtxMgr->EndScene();

    return true; //return false if something wrong
}

void GameState::onKill()
{
    GoingHome::TerminateGame();
}

void GameState::onSuspend()
{}

void GameState::onResume()
{}

void GameState::ProcessInput(double dt)
{
    dt;

    InputEngine & inputEngine = InputEngine::GetInstance();
    auto settings = ShiftEngine::GetContextManager()->GetEngineSettings();

    inputEngine.GetKeys();
    auto mouseInfo = inputEngine.GetMouseInfo();

    if (inputEngine.IsKeyDown(DIK_ESCAPE))
        kill();

    if (inputEngine.IsKeyUp(DIK_V))
        switchWireframe();

    Player* player = GoingHome::GetGamePtr()->GetPlayerPtr();

    float x = (float)mouseInfo.clientX - settings.screenWidth / 2;
    float y = settings.screenHeight / 2 - (float)mouseInfo.clientY;

    static bool lm_click = false;
    if (inputEngine.IsMouseDown(LButton) && !lm_click)
    {
        lm_click = true;
        player->Shoot({x, y});
    }
    if (inputEngine.IsMouseUp(LButton))
        lm_click = false;

    static bool rm_click = false;
    if (inputEngine.IsMouseDown(RButton) && !rm_click)
    {
        rm_click = true;
        player->ShootAlternative({x, y});
    }
    if (inputEngine.IsMouseUp(RButton))
        rm_click = false;


    float xVelocity = 0.0f, yVelocity = 0.0f;
    if (inputEngine.IsKeyDown(DIK_W))
    {
        yVelocity = 1.0f;
    }
    else if (inputEngine.IsKeyDown(DIK_S))
    {
        yVelocity = -1.0f;
    }

    if (inputEngine.IsKeyDown(DIK_A))
    {
        xVelocity = -1.0f;
    }
    else if (inputEngine.IsKeyDown(DIK_D))
    {
        xVelocity = 1.0f;
    }
    player->SetMoveVelocity({xVelocity, yVelocity});

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
}

bool GameState::handleEvent(const InputEvent& /*event*/)
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

MathLib::Ray GameState::getUnprojectedRay(const MathLib::vec2i & clientMouseCoords) const
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();

    ShiftEngine::GraphicEngineSettings settings = pCtxMgr->GetEngineSettings();
    vec2<unsigned int> sizes = vec2<unsigned int>(settings.screenWidth, settings.screenHeight);

    // do the raycasting
    mat4f projMatrix = pScene->GetActiveCamera()->GetProjectionMatrix();
    mat4f viewMatrix = pScene->GetActiveCamera()->GetViewMatrix();
    vec3f resultNear = getUnprojectedVector(vec3f((float)clientMouseCoords.x, (float)clientMouseCoords.y, 0.0f), projMatrix, viewMatrix, sizes);
    vec3f resultFar = getUnprojectedVector(vec3f((float)clientMouseCoords.x, (float)clientMouseCoords.y, 1.0f), projMatrix, viewMatrix, sizes);
    Ray unprojectedRay = Ray(resultNear, normalize(resultFar - resultNear));

    return unprojectedRay;
}
