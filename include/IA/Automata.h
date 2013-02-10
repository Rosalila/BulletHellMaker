#ifndef AUTOMATA_H
#define AUTOMATA_H


#include "IA/Estado.h"
#include "TinyXml/tinyxml.h"
#include <vector>
using namespace std;

class Automata
{
    std::map<std::string,Estado*> estados;
    std::string estado_actual;
    int wait;
public:
Transicion *transicion_a_recompenzar;
    Automata(char* archivo,char* archivo_default);
    Estado* getEstadoInicial();
    std::string getNextInput(std::map<std::string,std::string>*strings,
                         std::map<std::string,std::string>*strings_contrario,
                         std::map<std::string,int>*enteros,
                         std::map<std::string,int>*enteros_contrario
                         );
    void cargarDesdeXML(char* archivo,char* archivo_default);
};

#endif
