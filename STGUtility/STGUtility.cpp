#include "STGUtility.h"

int iterate_slowdown_flag=0;
int current_slowdown_iteration=0;
bool slow_enabled=false;
Receiver* receiver = new Receiver();

bool getIterateSlowdownFlag()
{
    return !isSlowActive() || iterate_slowdown_flag;
}

void slowExtraControl()
{
    //slow extra control
    if(isSlowActive())
    {
        iterate_slowdown_flag=false;
        current_slowdown_iteration++;
        if(current_slowdown_iteration>=3)
        {
            current_slowdown_iteration=0;
            iterate_slowdown_flag=true;
        }
    }
}

void setReceiver(Receiver*receiver_param)
{
    receiver=receiver;
}

Receiver* getReceiver()
{
    return receiver;
}

bool isSlowPressed()
{
    return receiver->isKeyDown(SDLK_x) || receiver->isJoyDown(5,0);
}

void disableSlow()
{
    slow_enabled=false;
}
void enableSlow()
{
    slow_enabled=true;
}
bool isSlowEnabled()
{
    return slow_enabled;
}
bool isSlowActive()
{
    return isSlowEnabled() && isSlowPressed();
}

double getSlowdown()
{
    int slowdown = 1.0;
    if(isSlowPressed() && isSlowEnabled())
        slowdown = 3.0;
    return slowdown;
}
