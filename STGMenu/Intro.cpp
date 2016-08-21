#include "ControllerConfig.h"

void intro(map<string,Button*> controls)
{
    RosalilaGraphics* graphics=getRosalilaGraphics();
    Receiver* receiver=getReceiver();

    Image* image=graphics->getTexture(assets_directory+"menu/startscreen.png");
    Image* image2=graphics->getTexture(assets_directory+"menu/instructions.png");

    while(true)
    {
        if(controls["a"]->isPressed())
        {
            break;
        }
        graphics->draw2DImage
        (   image,
            image->getWidth(),image->getHeight(),
            0,0,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        receiver->updateInputs();
        graphics->updateScreen();
    }

    while(true)
    {
        if(controls["a"]->isPressed())
        {
            break;
        }
        graphics->draw2DImage
        (   image2,
            image2->getWidth(),image2->getHeight(),
            0,0,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        receiver->updateInputs();
        graphics->updateScreen();
    }
}
