#include "GameState.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/inputConverter.h>

// #include "../Entities/GameObjectsManager.h"

ShiftEngine::SpriteSceneNode * spriteNode = nullptr;

using namespace MathLib;

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
    pCamera->SetScreenWidth(1024.0f / 600.0f * 4.0f);
    pCamera->SetScreenHeight(600.0f / 600.0f * 4.0f);

    pScene->SetAmbientColor(vec3f(0.1f, 0.1f, 0.15f));

    spriteNode = pScene->AddSpriteNode(L"sprite.png");
    spriteNode->SetLocalPosition({0.0f, 0.0f, -1.0f});
    // temporary here, due to some issues with matrices
    spriteNode->SetLocalScale(1.0f);

    LOG_INFO("End of game state initializing");

    return true;
}

bool GameState::update(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();

    // for example
    static double totalTime = 0.0;
    totalTime += dt;
    // pScene->GetActiveCamera()->SetLocalPosition({(float)totalTime, 0.0f, 0.0f});
    
    // doesn't work somehow :(
    // pScene->GetActiveCamera()->RotateByQuaternion(MathLib::quaternionFromVecAngle<float>({0.0f, 1.0f, 0.0f}, totalTime / 1000.0f));
    spriteNode->SetLocalPosition({std::cosf((float)totalTime), std::sinf((float)totalTime), 1.0f});

    ProcessInput(dt);
    // pGame->gameHud->Update(dt);

    GoingHome::GetGamePtr()->entityMgr->UpdateAllEntities(dt);

    return true;
}

bool GameState::render(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    ShiftEngine::Renderer * pRenderer = ShiftEngine::GetRenderer();

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
{}

void GameState::onSuspend()
{}

void GameState::onResume()
{}

void GameState::ProcessInput(double dt)
{
    InputEngine & inputEngine = InputEngine::GetInstance();
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    inputEngine.GetKeys();
    auto mouseInfo = inputEngine.GetMouseInfo();

    if (inputEngine.IsKeyDown(DIK_ESCAPE))
        kill();

    if (inputEngine.IsKeyUp(DIK_V))
        switchWireframe();

    if (inputEngine.IsControllerConnected())
    {
        if (inputEngine.IsControllerKeyDown(XINPUT_GAMEPAD_A))
        {
            inputEngine.VibrateController(65535, 0);
        }
        else if (inputEngine.IsControllerKeyUp(XINPUT_GAMEPAD_B))
        {
            inputEngine.VibrateController(0, 65535);
        }
        else if (inputEngine.IsControllerKeyDown(XINPUT_GAMEPAD_X))
        {
            inputEngine.VibrateController(65535, 65535);
        }
        else
        {
            inputEngine.VibrateController(0, 0);
        }
    }

    static bool click = false;
    if (inputEngine.IsMouseDown(LButton) && !click)
    {
        click = true;
        MouseInfo & mouseInfo = InputEngine::GetInstance().GetMouseInfo();

        auto settings = ShiftEngine::GetContextManager()->GetEngineSettings();
        float x = (float)mouseInfo.absoluteX - settings.screenWidth / 2;
        float y = (float)mouseInfo.absoluteY + settings.screenHeight / 2;

        GoingHome::GetGamePtr()->player.lock()->Shoot(
            MathLib::vec2f(x, y));
    }
    if (inputEngine.IsMouseUp(LButton))
        click = false;

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
