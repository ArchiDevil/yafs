#include "System.h"

cSystem::cSystem()
    : State(AS_Running)
    , WindowHandler(0)
    , WindowInstance(0)
{
    System = this;
}

cSystem::~cSystem()
{
}

HWND cSystem::GetHWND() const
{
    return WindowHandler;
}

HINSTANCE cSystem::GetHINSTANCE() const
{
    return WindowInstance;
}

LRESULT CALLBACK cSystem::MessageHandler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_ACTIVATE:
        if (wParam == WA_ACTIVE)
            State = AS_Running;
        if (wParam == WA_CLICKACTIVE)
            State = AS_Running;
        if (wParam == WA_INACTIVE)
            State = AS_Inactive;
        break;
    case WM_CHAR:
        notifyAll(SystemKeyMessage(wParam));
        break;
    default:
        break;
    }
    return ::DefWindowProc(windowHandle, msg, wParam, lParam); //��� ���� ������ �� ������������������ �� �������
}

bool cSystem::InitializeWindow(int Width, int Height, LPCWSTR AppName)
{
    this->WindowInstance = GetModuleHandle(0);

    WNDCLASS wc;
    wc.cbClsExtra = 0;                                                      //�� ���������
    wc.cbWndExtra = 0;                                                      //�� ���������
    wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));  //����������� ��� ����� ������
    wc.hCursor = ::LoadCursor(0, IDC_ARROW);                                //������
    wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);                              //������
    wc.hInstance = this->WindowInstance;                                    //���� �����������, �������� � �������
    wc.lpfnWndProc = WndProc;                                               //��������� ���������
    wc.lpszClassName = AppName;                                             //��� ������, ����� �������
    wc.lpszMenuName = 0;                                                    //�������, ��� �� ���������
    wc.style = CS_HREDRAW | CS_VREDRAW;	                                    //����� ���� - �������������� ��� ��������� ������� �� ����������� � ���������

    if (!::RegisterClass(&wc))
        return false;

    int originPosX = 0;
    int originPosY = 0;

    DWORD windowStyle = WS_OVERLAPPEDWINDOW &~(WS_MAXIMIZEBOX)&~(WS_THICKFRAME); //non-resizeable

    RECT r = { originPosX, originPosY, originPosX + Width, originPosY + Height };
    AdjustWindowRect(&r, windowStyle, false);

    int actualWidth = r.right - r.left;
    int actualHeight = r.bottom - r.top;

    this->WindowHandler = ::CreateWindow(
        AppName,                //��� ������ ���� ������� �� ���� ��������) � ����� ���-�� ���, � ��� ��������
        AppName,                //��������� ����
        windowStyle,            //����� ������������ ����
        originPosX,             //���������� �� X
        originPosY,             //���������� �� Y
        actualWidth,            //������
        actualHeight,           //������
        0,                      //���������� ��������, �������� ���, ������������� 0
        0,                      //���������� ����, ��� ���� ���
        this->WindowInstance,   //���������� ���� �������
        0);                     //��������� �� ������������ ������������� ������

    if (this->WindowHandler == 0)
        return false;

    ::ShowWindow(this->WindowHandler, SW_SHOW); //������� ��������� ����
    ::UpdateWindow(this->WindowHandler);        //� �������

    return true;
}

AppState cSystem::GetState() const
{
    return State;
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return System->MessageHandler(windowHandle, msg, wParam, lParam);
}
