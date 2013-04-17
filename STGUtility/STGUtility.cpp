#include "STGUtility.h"

int iterate_slowdown_flag=0;
int current_slowdown_iteration=0;
Receiver* receiver = new Receiver();

bool getIterateSlowdownFlag()
{
    return !isSlowPressed() || iterate_slowdown_flag;
}

void slowExtraControl()
{
    //slow extra control
    if(isSlowPressed())
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
    return receiver->IsKeyDownn(SDLK_x);
}
