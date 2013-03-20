#ifndef MENUTEST_H
#define MENUTEST_H

#include "Stage/Stage.h"
#include "Painter/Painter.h"
#include "Menu/MenuContenedor.h"
#include "Menu/MenuCharSelect.h"
#include "RosalilaSound/RosalilaSound.h"
#include "STG.h"

class Menu
{
private:
    //Engines
    Sound* sonido;
    Painter* painter;
    Receiver* receiver;
    std::vector<Elemento*> elementos;
    MenuContenedor*selectables_container;
    TiXmlDocument abrirXml(char* archivo);
    void dibujarMenu();
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
    Menu(Painter* painter,Receiver* receiver,Sound* sonido,char* archivo);
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
    void playMusic();
};

#endif
