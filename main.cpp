#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>

#include "RosalilaInputs/RosalilaInputs.h"
#include "RosalilaGraphics/RosalilaGraphics.h"
#include "RosalilaSound/RosalilaSound.h"
#include "STGMenu/STGMenu.h"
#include "RosalilaUtility/RosalilaUtility.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

    clearLog();
    //Creadas abierto
    Receiver* receiver=new Receiver();
    RosalilaInputs*inputa=new RosalilaInputs();
    RosalilaInputs*inputb=new RosalilaInputs();
    inputa->cargarDesdeXML(1,receiver);
    inputb->cargarDesdeXML(2,receiver);

    RosalilaGraphics*painter=new RosalilaGraphics();

    painter->video(painter);

//painter->update.start();
//painter->fps.start();


    Sound*sonido = new Sound();
    Menu* menu=new Menu(painter,receiver,sonido,(char*)"menu/main_menu.svg");
    menu->playMusic();

    menu->loopMenu();
    return 0;
}
