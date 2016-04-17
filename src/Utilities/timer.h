#pragma once

#if defined (WIN32) || (_WIN32)
#include <Windows.h>
#endif

class ITimer
{
public:
    virtual double GetRunningTime() const = 0;		//���������� �����, ��������� �� ������ �������
    virtual double GetDeltaTime() const = 0;		//���������� ����� � ���������� ������ Tick()
    virtual double GetStoppedTime() const = 0;		//�����, ��������� ����� ���������� � �������

    virtual void Start() = 0;
    virtual void Tick() = 0;
    virtual void Stop() = 0;
};

#if defined (WIN32) || (_WIN32)
class cTimer : ITimer
{
public:
    cTimer();
    virtual double GetRunningTime() const;      //���������� �����, ��������� �� ������ �������
    virtual double GetDeltaTime() const;        //���������� ����� � ���������� ������ Tick()
    virtual double GetStoppedTime() const;      //�����, ��������� ����� ���������� � �������

    virtual void Start();
    virtual void Tick();
    virtual void Stop();

private:
    long long countsPerSec; //���-�� ������� � �������
    double secPerCount;     //���������� ������ �� ���� ������

    long long startCount;   //��������� ������
    long long tickCount;    //���-�� ������������ ������� � ���������� Tick()
    long long stopCount;    //������, ����� ������ ��� ����������

    bool stopped;           //������ ����������?

};
#endif
