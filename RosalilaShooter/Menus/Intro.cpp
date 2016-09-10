#include "ControllerConfig.h"

void intro(map<string,Button*> controls)
{
    RosalilaGraphics* graphics=Rosalila()->Graphics;
    RosalilaReceiver* receiver=Rosalila()->Receiver;

    Image* image=graphics->getTexture(assets_directory+"menu/rosalila.png");
    Image* image2=graphics->getTexture(assets_directory+"menu/flatshot.png");

    int current_background_transparency=0;

    Rosalila()->Graphics->notification_handler.notifications.push_back(
        new Notification(getLoadingImage(), Rosalila()->Graphics->screen_width/2-getLoadingImage()->getWidth()/2,
                            Rosalila()->Graphics->screen_height,
                            Rosalila()->Graphics->screen_height-getLoadingImage()->getHeight(),
                            200));

    while(true)
    {
        if(controls["a"]->isPressed() && Rosalila()->ApiIntegrator->getState()!="loading")
        {
            break;
        }

        if(Rosalila()->Receiver->isKeyPressed(SDLK_ESCAPE))
        {
            exit(0);
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

        if(Rosalila()->ApiIntegrator->getState()=="loading")
        {
            graphics->draw2DImage
            (   getLoadingImage(),
                getLoadingImage()->getWidth(),getLoadingImage()->getHeight(),
                0,0,
                1.0,
                0.0,
                false,
                0,0,
                Color(255,255,255,current_background_transparency),
                0,0,
                false,
                FlatShadow());
        }

        current_background_transparency++;

        receiver->updateInputs();
        graphics->updateScreen();
        Rosalila()->ApiIntegrator->updateCallbacks();
    }
Rosalila()->ApiIntegrator->getData("a");
    current_background_transparency=0;

/*
vector<LeaderboardEntry*>l = Rosalila()->ApiIntegrator->getLeaderboard("liditi");
for(int i=0;i<l.size();i++)
{
    cout<<l[i]->name<<endl;
    cout<<l[i]->rank<<endl;
    cout<<l[i]->score<<endl;
}*/


    while(true)
    {
        if(controls["a"]->isPressed())
        {
            break;
        }

        if(Rosalila()->Receiver->isKeyPressed(SDLK_ESCAPE))
        {
            exit(0);
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
        Rosalila()->ApiIntegrator->updateCallbacks();
    }
}
