#include "ControllerConfig.h"

void intro()
{
    RosalilaGraphics* graphics=Rosalila()->Graphics;
    RosalilaReceiver* receiver=Rosalila()->Receiver;

    Image* image=graphics->getTexture(assets_directory+"menu/rosalila.png");
    Image* image2=graphics->getTexture(assets_directory+"menu/flatshot.png");

    int current_fade_in=0;

    Rosalila()->Graphics->notification_handler.notifications.push_back(
        new Notification(getLoadingImage(), Rosalila()->Graphics->screen_width/2-getLoadingImage()->getWidth()/2,
                            Rosalila()->Graphics->screen_height,
                            Rosalila()->Graphics->screen_height-getLoadingImage()->getHeight(),
                            999999));

    while(true)
    {
        if(Rosalila()->Receiver->isPressed("a") && Rosalila()->ApiIntegrator->getState()!="loading")
        {
            break;
        }

        if(Rosalila()->Receiver->isPressed("b"))
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
            Color(current_fade_in,current_fade_in,current_fade_in,255),
            0,0,
            false,
            FlatShadow());

        if(Rosalila()->ApiIntegrator->getState()!="loading")
        {
            Rosalila()->Graphics->notification_handler.interruptCurrentNotification();
        }

//        vector<DrawableTriangle*>triangles;
//
//        triangles.push_back(new DrawableTriangle(100, 200,Point(10,15), Point(100,150), Point(10,150), 0, Color (255,255,0,255)));
//        triangles.push_back(new DrawableTriangle(200, 200,Point(10,15), Point(100,150), Point(10,150), 30, Color (255,0,255,255)));
//
//        Rosalila()->Graphics->drawTriangles(triangles,2, 1,false);

        current_fade_in+=2;
        if(current_fade_in>255)
            current_fade_in=255;

        Rosalila()->update();
    }

    int current_background_transparency=0;

    while(true)
    {
        if(Rosalila()->Receiver->isPressed("a"))
        {
            break;
        }

        if(Rosalila()->Receiver->isPressed("b"))
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

        current_background_transparency+=2;
        if(current_background_transparency>255)
            current_background_transparency=255;

        Rosalila()->update();
    }
}
