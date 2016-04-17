#pragma once

class timer
{
public:
    virtual double get_running_time() const = 0;        //возвращает время, прошедшее со старта таймера
    virtual double get_delta_time() const = 0;          //возвращает время с последнего вызова Tick()
    virtual double get_stopped_time() const = 0;        //время, прошедшее между остановкой и стартом

    virtual void start() = 0;
    virtual void tick() = 0;
    virtual void stop() = 0;
};

#if defined (WIN32) || (_WIN32)
class windows_high_reference_timer : timer
{
public:
    windows_high_reference_timer();
    double get_running_time() const override;      //возвращает время, прошедшее со старта таймера
    double get_delta_time() const override;        //возвращает время с последнего вызова Tick()
    double get_stopped_time() const override;      //время, прошедшее между остановкой и стартом

    void start() override;
    void tick() override;
    void stop() override;

private:
    long long countsPerSec = 0;     //кол-во щелчков в секунду
    double secPerCount = 0.0;       //количество секунд за один щелчок

    long long startCount = 0;       //стартовый щелчок
    long long tickCount = 0;        //кол-во произошедших щелчков с последнего Tick()
    long long stopCount = 0;        //щелчок, когда таймер был остановлен

    bool stopped = true;            //таймер остановлен?

};
#endif
