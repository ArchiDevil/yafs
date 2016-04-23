#pragma once

#include "../game.h"
#include "../Entities/Player.h"

#include <GraphicsEngine/ShiftEngine.h>

//#include <MyGUI.h>
//#include <MyGUI_DirectX11Platform.h>

#include <MathLib/math.h>

#include <System/InputEngine.h>

#include <Utilities/IAppState.h>
#include <Utilities/ut.h>
#include <Utilities/iniloader.h>
#include <Utilities/IManager.h>
#include <Utilities/observer.h>

#include <memory>

class GameState final
    : public IAppState
    , public IManager
    , public observer<InputEvent>
{
public:
    GameState(IniWorker * iw/*, MyGUI::Gui * guiModule, MyGUI::DirectX11Platform * guiPlatform*/);
    ~GameState();

    // Inherited via appState
    bool initState() override;
    bool update(double dt) override;
    bool render(double dt) override;
    void onKill() override;
    void onSuspend() override;
    void onResume() override;

    // Inherited via observer<InputEvent>
    bool handleEvent(const InputEvent & event) override;

private:
    void ProcessInput(double dt);

    void switchWireframe();
    MathLib::Ray getUnprojectedRay(const MathLib::vec2i & clientMouseCoords) const;
    void playerAction(const MathLib::vec2i & clientMouseCoords);

    IniWorker *                     iniLoader = nullptr;

    std::shared_ptr<Player>           player = nullptr;
    //MyGUI::Gui *                    guiModule = nullptr;
    //MyGUI::DirectX11Platform *      guiPlatform = nullptr;

};
