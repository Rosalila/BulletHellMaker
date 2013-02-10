#include "Sonido/Sonido.h"

Sonido::Sonido()
{
    music=NULL;

    writeLogLine("Initializing SLD sound engine.");
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        writeLogLine("Failed. :(");
        return;
    }
    writeLogLine("Success!");
}

void Sonido::drop()
{
    //TODO for each chunk
    //Mix_FreeChunk( chunk );
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}
void Sonido::agregarSonido(std::string variable,std::string valor)
{
    if(sonidos.find(variable)==sonidos.end())
        sonidos[variable]=Mix_LoadWAV(valor.c_str());
}
void Sonido::reproducirSonido(std::string variable,bool looped)
{
    if(sonidos.find(variable)==sonidos.end())
        return;

    if(sonidos[variable]!=NULL)
    {
        if(looped)
            Mix_PlayChannel( -1, sonidos[variable], -1 );
        else
            Mix_PlayChannel( -1, sonidos[variable], 0 );
    }
}

void Sonido::playMusic(std::string path)
{
    stopMusic();
    writeLogLine("Playing music: "+path);
    music = Mix_LoadMUS(path.c_str());
    Mix_PlayMusic(music,-1);
}

void Sonido::stopMusic()
{
    if(music!=NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music=NULL;
    }
}
