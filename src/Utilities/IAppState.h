#pragma once

class IAppState
{
public:
    IAppState() : Dead(false) {}
    virtual ~IAppState() {}

    virtual bool initState() = 0;           //����������, ����� ����� ����������������
    virtual bool update(double dt) = 0;     //���������� ��� ���������� ��������� �������������� ������
    virtual bool render(double dt) = 0;     //������
    virtual void onKill() = 0;              //���������� ��� �������� ������
    virtual void onSuspend() = 0;           //���������� ��� ����� ������ �� ������
    virtual void onResume() = 0;            //���������� ��� ����������� � ����� ������

    bool isDead() const { return Dead; }
    void kill() { Dead = true; }

private:
    bool Dead;

};
