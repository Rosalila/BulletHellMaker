#include "StageSelect.h"

void stageSelect()
{
    RosalilaGraphics* graphics=getRosalilaGraphics();
    Receiver* receiver=getReceiver();

    Image* image=graphics->getTexture(assets_directory+"menu/startscreen.png");

    while(true)
    {
        if(receiver->isKeyPressed(SDLK_RETURN))
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
}
