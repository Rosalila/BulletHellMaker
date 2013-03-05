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

    current_channel=0;
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
    writeLogLine("Loading "+valor);
    if(sonidos.find(variable)==sonidos.end())
    {
        sonidos[variable]=Mix_LoadWAV(valor.c_str());
        channel[variable]=current_channel;
        current_channel++;
    }
}
void Sonido::reproducirSonido(std::string variable,bool looped)
{
    writeLogLine("Playing"+variable);
    if(sonidos.find(variable)==sonidos.end())
        return;

    if(sonidos[variable]!=NULL && !Mix_Playing(channel[variable]))
    {
        if(looped)
            Mix_PlayChannel( channel[variable], sonidos[variable], -1 );
        else
            Mix_PlayChannel( channel[variable], sonidos[variable], 0 );
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
