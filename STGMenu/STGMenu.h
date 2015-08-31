#ifndef MENUTEST_H
#define MENUTEST_H

#include "../Stage/Stage.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaMenus/MenuContenedor.h"
#include "../Rosalila/RosalilaMenus/MenuCharSelect.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
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

    void iniciarJuego(std::string character_name,std::string stage_name);
public:
    std::string music_path;
    RosalilaInputs *inputa,*inputb;
    Stage* stage;

    //Misc
    Image* loading_screen;

    Menu(){}
    Menu(RosalilaGraphics* painter,Receiver* receiver,Sound* sonido,char* archivo);
    void cargarDesdeXml(char*archivo,vector<std::string> chars,vector<std::string> stages);
    void loopMenu();
    std::string getStage();
    bool getExitSignal();
    bool getSaveRosalilaInputssSignal();
    void escribirRosalilaInputssXML(RosalilaInputs* ia,RosalilaInputs* ib);
    std::string getRosalilaInputsPressed();
    void llenarRosalilaInputssBotones();
    int toKeyCode(std::string str);
    void printLoadingScreen();
    void playMusic();
};

#endif
