#pragma once

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <Utilities/ut.h>
#include <Utilities/observer.h>
#include <Utilities/singleton.h>

#include "Win32/System.h"

enum class InputEventType
{
    MouseUp,
    MouseDown,
    KeyDown,
    KeyUp,
    SystemKey,
};

struct InputEvent
{
    InputEvent(InputEventType type, long key)
        : type(type)
        , key(key)
    {
    }

    InputEventType type;
    long key;
};

//���������� ������� ��� �����
enum MouseKeys
{
    LButton,
    RButton
};

//���������� ���������� � ����, ���������� � ������ �����
struct MouseInfo
{
    MouseInfo()
        : deltaX(0)
        , deltaY(0)
        , deltaZ(0)
        , absoluteX(0)
        , absoluteY(0)
        , clientX(0)
        , clientY(0)
    {}

    MouseInfo(long _deltaX, long _deltaY, long _deltaZ, unsigned long _absoluteX, unsigned long _absoluteY, long _clientX, long _clientY)
        : deltaX(_deltaX)
        , deltaY(_deltaY)
        , deltaZ(_deltaZ)
        , absoluteX(_absoluteX)
        , absoluteY(_absoluteY)
        , clientX(_clientX)
        , clientY(_clientY)
    {}

    long deltaX, deltaY, deltaZ;
    unsigned long absoluteX, absoluteY;
    long clientX, clientY;

    bool operator == (const MouseInfo & ref) const
    {
        return (this->absoluteX == ref.absoluteX) && (this->absoluteY == ref.absoluteY);
    };

    bool operator != (const MouseInfo & ref) const
    {
        return !((this->absoluteX == ref.absoluteX) && (this->absoluteY == ref.absoluteY));
    };
};

class InputEngine
    : public singleton < InputEngine >
    , public notifier < InputEvent >
    , public observer < SystemKeyMessage >
{
public:
    InputEngine();
    ~InputEngine();

    bool Initialize(HWND hWnd, HINSTANCE hInstance);
    void GetKeys();

    bool IsMouseDown(MouseKeys key) const;
    bool IsMouseUp(MouseKeys key) const;
    bool IsMouseMoved() const;

    bool IsKeyDown(unsigned char Key) const;
    bool IsKeyUp(unsigned char Key) const;
    MouseInfo GetMouseInfo() const;

    bool handleEvent(const SystemKeyMessage & keyEvent) override;

private:
    IDirectInput8 * di; //DirectInput ���������
    IDirectInputDevice8 * keyboard;
    IDirectInputDevice8 * mouse;

    char curKeyBuffer[256]; //����� ��� ���������� ��� DI
    char preKeyBuffer[256];

    MouseInfo curMouseBuffer1;
    MouseInfo preMouseBuffer1;

    DIMOUSESTATE curMouseBuffer;
    DIMOUSESTATE preMouseBuffer;

    HWND hWnd;

};
