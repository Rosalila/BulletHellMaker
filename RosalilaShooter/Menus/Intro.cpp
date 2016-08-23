#include "ControllerConfig.h"

void intro(map<string,Button*> controls)
{
    RosalilaGraphics* graphics=Rosalila()->Graphics;
    RosalilaReceiver* receiver=Rosalila()->Receiver;

    Image* image=graphics->getTexture(assets_directory+"menu/rosalila.png");
    Image* image2=graphics->getTexture(assets_directory+"menu/flatshot.png");

    int current_background_transparency=0;

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
            Color(255,255,255,current_background_transparency),
            0,0,
            false,
            FlatShadow());

        current_background_transparency++;

        receiver->updateInputs();
        graphics->updateScreen();
    }

    current_background_transparency=0;

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
            Color(255,255,255,current_background_transparency),
            0,0,
            false,
            FlatShadow());

        current_background_transparency++;

        receiver->updateInputs();
        graphics->updateScreen();
    }
}
