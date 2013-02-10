#ifndef INPUT_H
#define INPUT_H

#include "Input/Boton.h"
#include <IA/IA.h>

class Input
{
protected:
    std::vector<std::string> buffer_inputs;
    int jugador;

public:
Receiver* receiver;
bool tecla_arriba;
IA *ia;
bool inteligencia_artificial;
vector<Boton> botones;
vector<Boton> cruz;
    Input(){}
    void cargarDesdeXML(int jugador,Receiver* receiver);
    void cargarIAXML(int jugador,char* archivo,char* archivo_default);
    TiXmlDocument* getXML(TiXmlDocument *doc);
    void actualizarBuffer();
    void actualizarBuffer(std::map<std::string,std::string>*strings,
                          std::map<std::string,std::string>*strings_contrario,
                          std::map<std::string,int>*enteros,
                          std::map<std::string,int>*enteros_contrario
                          );
    std::vector<std::string> getBufferInputs();
    void endEventProcess();
    void startEventProcess();
};

#endif
