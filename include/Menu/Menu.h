#ifndef MENUTEST_H
#define MENUTEST_H

#include "Stage/Stage.h"
#include "Painter/Painter.h"
#include "Menu/MenuContenedor.h"
#include "Menu/MenuCharSelect.h"
#include "Sonido/Sonido.h"
#include "STG.h"

class Menu
{
private:
    //Engines
    Sonido* sonido;
    Painter* painter;
    Receiver* receiver;
    std::vector<Elemento*> elementos;
    MenuContenedor*contenedor_actual;
    TiXmlDocument abrirXml(char* archivo);
    void dibujarMenu();
//    std::vector<int> pos_pa;
//    std::vector<int> pos_pb;
    int pos_stage,pos_ia_lvl;
    bool exit_signal,save_inputs_signal;
    MenuCharSelect* char_select;

    //config vars
    int ai_level,time,rounds;

    void iniciarJuego(std::string character_name,std::string stage_name);
public:
    std::string music_path;
    Input *inputa,*inputb;
    Stage* stage;

    //Misc
    Image* vs_screen;

    Menu(){}
    Menu(Painter* painter,Receiver* receiver,Sonido* sonido,char* archivo);
    void cargarDesdeXml(char*archivo,vector<std::string> chars,vector<std::string> stages);
    void cargarConfig();
    void loopMenu();
    std::string getStage();
    bool getExitSignal();
    bool getSaveInputsSignal();
    void escribirInputsXML(Input* ia,Input* ib);
    std::string getInputPressed();
    void llenarInputsBotones();
    int toKeyCode(std::string str);
    void printVsScreen(vector<Image*>pa_previews,vector<Image*>pb_previews);
};

#endif
