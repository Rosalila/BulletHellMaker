#ifndef STAGE_H
#define STAGE_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Character/Character.h"
#include "Stage/Layer.h"
#include "Stage/Dialogue.h"
#include <map>

class Stage
{
private:
    std::vector<Layer*> back,front;
    std::map<std::string,Character*>*enemies;
    std::list<Character*>* active_enemies;
    std::map<int,Dialogue*>dialogues;
    std::list<Dialogue*>active_dialogues;
    std::string music_path;
    Painter* painter;
    Sonido* sonido;
    Receiver*receiver;
    int size;
    int pos_piso;
    int bound_x1,bound_y1,bound_x2,bound_y2;
    int velocity;
    int iterator;

public:
    Stage(Painter* painter,Sonido* sonido,Receiver*receiver);
    void dibujarBack();
    void dibujarFront();
    void drawLayer(Layer*layer);
    void cargarDesdeXML(std::string archivo);
    void loadDialogues(std::string file);
    int getBoundX1();
    int getBoundY1();
    int getBoundX2();
    int getBoundY2();
    int getVelocity();
    void logic();
    void render();
    std::map<std::string,Character*>*getEnemies();
    void setVelocity(int velocity);
    ~Stage();
};

#endif
