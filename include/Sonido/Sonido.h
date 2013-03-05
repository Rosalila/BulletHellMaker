#ifndef SONIDO_H
#define SONIDO_H

#include <string>
#include <vector>

#include <iostream>
using namespace std;

//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "Utility/Utility.h"

#include <map>

class Sonido
{
    std::map<std::string,Mix_Chunk*> sonidos;
    std::map<std::string,int> channel;
    int current_channel;
    Mix_Music *music;
public:
    Sonido();
    void drop();
    void agregarSonido(std::string variable,std::string valor);
    void reproducirSonido(std::string variable,bool looped);
    void playMusic(std::string variable);
    void stopMusic();
};

#endif
