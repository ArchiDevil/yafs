#pragma once

class timer
{
public:
    virtual double get_running_time() const = 0;        //���������� �����, ��������� �� ������ �������
    virtual double get_delta_time() const = 0;          //���������� ����� � ���������� ������ Tick()
    virtual double get_stopped_time() const = 0;        //�����, ��������� ����� ���������� � �������

    virtual void start() = 0;
    virtual void tick() = 0;
    virtual void stop() = 0;
};

#if defined (WIN32) || (_WIN32)
class windows_high_reference_timer : timer
{
public:
    windows_high_reference_timer();
    double get_running_time() const override;      //���������� �����, ��������� �� ������ �������
    double get_delta_time() const override;        //���������� ����� � ���������� ������ Tick()
    double get_stopped_time() const override;      //�����, ��������� ����� ���������� � �������

    void start() override;
    void tick() override;
    void stop() override;

private:
    long long countsPerSec = 0;     //���-�� ������� � �������
    double secPerCount = 0.0;       //���������� ������ �� ���� ������

    long long startCount = 0;       //��������� ������
    long long tickCount = 0;        //���-�� ������������ ������� � ���������� Tick()
    long long stopCount = 0;        //������, ����� ������ ��� ����������

    bool stopped = true;            //������ ����������?

};
#endif
