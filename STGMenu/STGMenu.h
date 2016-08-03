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
    map<string,vector<string> >arcade_paths;

    int frame;
    int skip_frame;

    void iniciarJuego(std::string character_name,std::string stage_name,string game_mode);
public:
    std::string music_path;
    RosalilaInputs *inputa,*inputb;
    map<string,Button*>controls;
    Stage* stage;

    //Misc
    Image* loading_screen;
    Image* white_background;
    int white_image_current_r;
    int white_image_current_g;
    int white_image_current_b;
    int white_image_target_r;
    int white_image_target_g;
    int white_image_target_b;

    bool color_changing_background;

    Menu(){}
    Menu(RosalilaGraphics* painter,Receiver* receiver,Sound* sonido,char* archivo,map<string,Button*>controls);
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
    void backgroundTargetUpdate(int current_selection);
};

#endif
