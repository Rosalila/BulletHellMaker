#ifndef MENUTEST_H
#define MENUTEST_H

#include "../Stage/Stage.h"
#include "RosalilaGraphics/RosalilaGraphics.h"
#include "RosalilaMenus/MenuContenedor.h"
#include "RosalilaMenus/MenuCharSelect.h"
#include "RosalilaSound/RosalilaSound.h"
#include "../STG.h"

class Menu
{
private:
    //Engines
    Sound* sonido;
    RosalilaGraphics* painter;
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
    RosalilaInputs *inputa,*inputb;
    Stage* stage;

    //Misc
    Image* vs_screen;

    Menu(){}
    Menu(RosalilaGraphics* painter,Receiver* receiver,Sound* sonido,char* archivo);
    void cargarDesdeXml(char*archivo,vector<std::string> chars,vector<std::string> stages);
    void cargarConfig();
    void loopMenu();
    std::string getStage();
    bool getExitSignal();
    bool getSaveRosalilaInputssSignal();
    void escribirRosalilaInputssXML(RosalilaInputs* ia,RosalilaInputs* ib);
    std::string getRosalilaInputsPressed();
    void llenarRosalilaInputssBotones();
    int toKeyCode(std::string str);
    void printVsScreen(vector<Image*>pa_previews,vector<Image*>pb_previews);
    void playMusic();
};

#endif
