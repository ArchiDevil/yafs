#pragma once

class IAppState
{
public:
    IAppState() : dead(false) {}
    virtual ~IAppState() {}

    virtual bool initState() = 0;           //when the state initializing
    virtual bool update(double dt) = 0;     //when the state should update
    virtual bool render(double dt) = 0;     //when the state should render
    virtual void onKill() = 0;              //when the state dies
    virtual void onSuspend() = 0;           //when the state changes to another
    virtual void onResume() = 0;            //when the state resumes it's work

    bool isDead() const { return dead; }
    void kill() { dead = true; }

private:
    bool dead;

};
