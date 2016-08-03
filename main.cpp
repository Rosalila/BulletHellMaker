#include <stdio.h>
#include <stdlib.h>
#include <sstream>

//SDL
#include <string>

#include "Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "Rosalila/RosalilaSound/RosalilaSound.h"
#include "STGMenu/STGMenu.h"
#include "Rosalila/RosalilaUtility/RosalilaUtility.h"
#include "STGUtility/STGUtility.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
//  glutInit (&argc, argv);
//  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

    clearLog();
    //Creadas abierto
    setReceiver(new Receiver());

    RosalilaGraphics*painter=new RosalilaGraphics();

    painter->video(painter);

//painter->update.start();
//painter->fps.start();
    Image* controls_config_backgound = painter->getTexture(assets_directory+"misc/controls configuration/background.png");
    vector<string> controls_config_map_name;
    controls_config_map_name.push_back("7");
    controls_config_map_name.push_back("2");
    controls_config_map_name.push_back("4");
    controls_config_map_name.push_back("6");
    controls_config_map_name.push_back("a");
    vector<Image*> controls_config_press_images;
    controls_config_press_images.push_back(painter->getTexture(assets_directory+"misc/controls configuration/press_up.png"));
    controls_config_press_images.push_back(painter->getTexture(assets_directory+"misc/controls configuration/press_down.png"));
    controls_config_press_images.push_back(painter->getTexture(assets_directory+"misc/controls configuration/press_left.png"));
    controls_config_press_images.push_back(painter->getTexture(assets_directory+"misc/controls configuration/press_right.png"));
    controls_config_press_images.push_back(painter->getTexture(assets_directory+"misc/controls configuration/press_shoot.png"));
    Receiver* receiver = getReceiver();
    int current_button=0;
    map<string,Button*>configured_controls;

    while(true)
    {
        int key_pressed = -1;
        for(int i=SDLK_a;i<=SDLK_z;i++)
        {
            if(receiver->isKeyPressed(i))
            {
                key_pressed=i;
            }
        }
        if(key_pressed!=-1)
        {
            string current_button_map = controls_config_map_name[current_button];
            configured_controls[current_button_map]=new Button(receiver,key_pressed,current_button_map);
            current_button++;
            if(current_button>=controls_config_press_images.size())
                break;
        }
        painter->draw2DImage
        (   controls_config_backgound,
            controls_config_backgound->getWidth(),controls_config_backgound->getHeight(),
            0,0,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        painter->draw2DImage
        (   controls_config_press_images[current_button],
            controls_config_press_images[current_button]->getWidth(),
              controls_config_press_images[current_button]->getHeight(),
            painter->screen_width/2-controls_config_press_images[current_button]->getWidth()/2,
              painter->screen_height/2-controls_config_press_images[current_button]->getHeight(),
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        receiver->updateInputs();
        painter->updateScreen();
    }

    Sound*sonido = new Sound();
    string path_menu = assets_directory+"menu/main_menu.svg";
    Menu* menu=new Menu(painter,getReceiver(),sonido,(char*)path_menu.c_str());
    menu->playMusic();

    menu->loopMenu();
    return 0;
}
