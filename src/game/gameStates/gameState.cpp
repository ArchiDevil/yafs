#include "GameState.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"
#include "../AI/AISmallSpirit.h"
#include "../Spells/SpellsDatabase.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/inputConverter.h>

using namespace GoingHome;
using namespace ShiftEngine;
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

    CreateGame();

    SceneGraph * pScene = GetSceneGraph();
    Player* playerPtr = GetGamePtr()->GetPlayerPtr();
    SpellsDatabase* database = GetGamePtr()->GetSpellsDatabase();
    EntityManager* entityMgrPtr = GetGamePtr()->GetEntityMgr();

    //pGame->gameHud.reset(new GameHUD(guiModule));
    //LOG_INFO("HUD has been created");

    CameraSceneNode * pCamera = pScene->AddCameraSceneNode(CameraViewType::Orthographic);
    pCamera->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
    pCamera->SetScreenWidth(1024.0f / 600.0f * 6.0f);
    pCamera->SetScreenHeight(600.0f / 600.0f * 6.0f);

    pScene->SetAmbientColor(vec3f(0.1f, 0.1f, 0.15f));

    // just for example, let's create some enemies
    Enemy* testEnemy = entityMgrPtr->CreateEnemy({ 1.0f, 1.0f }, 5.0f, 100, 0.2f, Enemy::EnemyType::SmallSpirit).get();

    playerPtr->SetSpellController(database->GetSpellByName("projectile").CreateSpellController(playerPtr),      Player::CS_MainSlot);
    playerPtr->SetSpellController(database->GetSpellByName("multiprojectile").CreateSpellController(playerPtr), Player::CS_AdditionalSlot);
    playerPtr->SetSpellController(database->GetSpellByName("timed-mine").CreateSpellController(playerPtr),      Player::CS_MineSlot);
    playerPtr->SetSpellController(database->GetSpellByName("shield").CreateSpellController(playerPtr),          Player::CS_ShieldSlot);

    LOG_INFO("End of game state initializing");

    return true;
}

bool GameState::update(double dt)
{
    SceneGraph * pScene = GetSceneGraph();
    Game* pGame = GetGamePtr();

    pGame->GetPhysicsMgr()->Update(dt);
    pGame->GetEntityMgr()->UpdateAllEntities(dt);

    auto playerPosition = pGame->GetPlayerPtr()->GetPosition();
    pScene->GetActiveCamera()->SetLocalPosition({ playerPosition.x, playerPosition.y, 0.0f });

    ProcessInput(dt);
    // pGame->gameHud->Update(dt);

    pGame->GetEntityMgr()->UpdateAllEntities(dt);

    return true;
}

bool GameState::render(double dt)
{
    SceneGraph * pScene = GetSceneGraph();
    IContextManager * pCtxMgr = GetContextManager();

#if defined (DEBUG) || (_DEBUG)
    const int infoSize = 7;
    std::ostringstream di[infoSize];

    di[0] << "Test"; // << "FPS: " << pRenderer->GetFPS();
    di[1] << "Test"; // << "Shader changes: " << pRenderer->GetShaderChanges();
    di[2] << "Test"; // << "Matrix bindings: " << pRenderer->GetMatricesBindings();
    di[3] << "Test"; // << "Uniform bindings: " << pRenderer->GetUniformsBindings();
    di[4] << "Test"; // << "Texture bindings: " << pRenderer->GetTextureBindings();
    di[5] << "Test"; // << "Draw calls: " << pRenderer->GetDrawCalls();
    di[6] << "Player position: " << GetGamePtr()->GetPlayerPtr()->GetPosition().x << " " << GetGamePtr()->GetPlayerPtr()->GetPosition().y;
#else
    const int infoSize = 1;
    std::ostringstream di[infoSize];
    //di[0] << "FPS: " << pRenderer->GetFPS();
#endif

    ////////////
    // RENDER //
    ////////////

    static std::vector<TextSceneNodePtr> textNodes;

    FontManager * pFntMgr = pCtxMgr->GetFontManager();

    if (textNodes.empty())
    {
        for (int i = 0; i < std::size(di); ++i)
        {
            MathLib::vec2f position = {
                5.0f,
                5.0f + i * 16.0f
            };
            textNodes.emplace_back(pFntMgr->CreateTextSceneNode(di[i].str(), position, L"2"));
        }
    }

    for (int i = 0; i < std::size(di); ++i)
        textNodes[i]->SetText(di[i].str());

    std::ostringstream experienceCount;
    experienceCount << "Experience: " << GetGamePtr()->GetPlayerPtr()->GetExperienceCount();

    static TextSceneNodePtr experienceCountNode;
    if (!experienceCountNode)
    {
        MathLib::vec2f position = {
            pCtxMgr->GetEngineSettings().screenWidth - 160.0f,
            pCtxMgr->GetEngineSettings().screenHeight - 40.0f
        };

        experienceCountNode = pFntMgr->CreateTextSceneNode(experienceCount.str(), position, L"2");
    }

    experienceCountNode->SetText(experienceCount.str());

    pScene->DrawAll(dt);

    //guiPlatform->getRenderManagerPtr()->drawOneFrame();

    return true; //return false if something wrong
}

void GameState::onKill()
{
    TerminateGame();
}

void GameState::onSuspend()
{
}

void GameState::onResume()
{
}

void GameState::ProcessInput(double dt)
{
    dt;

    InputEngine & inputEngine = InputEngine::GetInstance();
    auto settings = GetContextManager()->GetEngineSettings();

    inputEngine.GetKeys();
    auto mouseInfo = inputEngine.GetMouseInfo();

    if (inputEngine.IsKeyDown(DIK_ESCAPE))
        kill();

    if (inputEngine.IsKeyUp(DIK_V))
        switchWireframe();

    Player* player = GetGamePtr()->GetPlayerPtr();

    const float x = (float)mouseInfo.clientX - settings.screenWidth / 2;
    const float y = settings.screenHeight / 2 - (float)mouseInfo.clientY;

    player->SetTargetDirection({ x, y });

    // TODO: we need some handler of this shit
    if (inputEngine.IsMouseDown(LButton))
        player->StartSpellInSlot(LiveEntity::CS_MainSlot);

    if (inputEngine.IsMouseUp(LButton))
        player->StopSpellInSlot(LiveEntity::CS_MainSlot);

    if (inputEngine.IsMouseDown(RButton))
        player->StartSpellInSlot(LiveEntity::CS_AdditionalSlot);

    if (inputEngine.IsMouseUp(RButton))
        player->StopSpellInSlot(LiveEntity::CS_AdditionalSlot);

    if (inputEngine.IsKeyDown(DIK_E))
        player->StartSpellInSlot(LiveEntity::CS_MineSlot);

    if (inputEngine.IsKeyUp(DIK_E))
        player->StopSpellInSlot(LiveEntity::CS_MineSlot);

    if (inputEngine.IsKeyDown(DIK_LSHIFT))
        player->StartSpellInSlot(LiveEntity::CS_ShieldSlot);

    if (inputEngine.IsKeyUp(DIK_LSHIFT))
        player->StopSpellInSlot(LiveEntity::CS_ShieldSlot);

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
    player->SetMoveVector({ xVelocity, yVelocity });

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
    IContextManager * pCtxMgr = GetContextManager();

    static bool Wflag = false;
    Wflag = !Wflag;
    pCtxMgr->SetWireframeState(Wflag);
#endif
}

MathLib::Ray GameState::getUnprojectedRay(const MathLib::vec2i & clientMouseCoords) const
{
    SceneGraph * pScene = GetSceneGraph();
    IContextManager * pCtxMgr = GetContextManager();

    GraphicEngineSettings settings = pCtxMgr->GetEngineSettings();
    vec2<unsigned int> sizes = vec2<unsigned int>(settings.screenWidth, settings.screenHeight);

    // do the raycasting
    mat4f projMatrix = pScene->GetActiveCamera()->GetProjectionMatrix();
    mat4f viewMatrix = pScene->GetActiveCamera()->GetViewMatrix();
    vec3f resultNear = getUnprojectedVector(vec3f((float)clientMouseCoords.x, (float)clientMouseCoords.y, 0.0f), projMatrix, viewMatrix, sizes);
    vec3f resultFar = getUnprojectedVector(vec3f((float)clientMouseCoords.x, (float)clientMouseCoords.y, 1.0f), projMatrix, viewMatrix, sizes);
    Ray unprojectedRay = Ray(resultNear, normalize(resultFar - resultNear));

    return unprojectedRay;
}
