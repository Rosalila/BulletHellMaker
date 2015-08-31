#include <pthread.h>

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


    Sound*sonido = new Sound();
    string path_menu = assets_directory+"menu/main_menu.svg";
    Menu* menu=new Menu(painter,getReceiver(),sonido,(char*)path_menu.c_str());
    menu->playMusic();

    menu->loopMenu();
    return 0;
}
