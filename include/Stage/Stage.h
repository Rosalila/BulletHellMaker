#ifndef STAGE_H
#define STAGE_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Character/Character.h"
#include <map>

class Layer
{
public:
    std::vector <Image*> textures;
    std::vector <int> textures_size_x;
    std::vector <int> textures_size_y;

    //External logic
    int frame_duration,depth_effect_x,depth_effect_y,alignment_x,alignment_y;
    int separation_x;
    //Internal logic
    int current_frame,time_elapsed;
    Layer(vector<Image*> textures,vector <int> textures_size_x,vector <int> textures_size_y,int frame_duration,int depth_effect_x,int depth_effect_y,int alignment_x,int alignment_y,int separation_x)
    {
        this->textures=textures;
        this->textures_size_x=textures_size_x;
        this->textures_size_y=textures_size_y;
        this->frame_duration=frame_duration;
        this->depth_effect_x=depth_effect_x;
        this->depth_effect_y=depth_effect_y;
        this->current_frame=0;
        this->time_elapsed=0;
        this->alignment_x=alignment_x;
        this->alignment_y=alignment_y;
        this->separation_x=separation_x;
    }

    ~Layer()
    {
        writeLogLine("Deleting layer.");
        for(;!textures.empty();)
        {
            Image*image=textures.back();
            textures.pop_back();
            delete image;
        }
    }
};

class Stage
{
private:
    std::vector<Layer*> back,front;
    std::map<std::string,Character*>*enemies;
    std::list<Character*>* active_enemies;
    std::string music_path;
    Painter* painter;
    Sonido* sonido;
    Receiver*receiver;
    int size;
    int pos_piso;
    int bound_x1,bound_y1,bound_x2,bound_y2;
    //efecto terremoto
    bool efecto_camara;
    bool moviendo_derecha;
    int movimiento;
    int borde_efecto;
    int velocity;

public:
    Stage(Painter* painter,Sonido* sonido,Receiver*receiver);
    void dibujarBack();
    void dibujarFront();
    void drawLayer(Layer*layer);
    void cargarDesdeXML(std::string archivo);
    int getBoundX1();
    int getBoundY1();
    int getBoundX2();
    int getBoundY2();
    int getVelocity();
    std::map<std::string,Character*>*getEnemies();
    void setVelocity(int velocity);
    ~Stage();
};

#endif
