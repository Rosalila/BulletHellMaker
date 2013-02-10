#ifndef IA_H
#define IA_H

#include "IA/Automata.h"
#include <cstdlib>
#include <iostream>
using namespace std;

class IA
{
    Automata *automata;
public:
    Transicion *transicion_a_recompenzar;
    IA(char* archivo,char* archivo_default);
    std::string getInput(std::map<std::string,std::string>*strings,
                     std::map<std::string,std::string>*strings_contrario,
                     std::map<std::string,int>*enteros,
                     std::map<std::string,int>*enteros_contrario
                     );
    void darRecompensa();
    void quitarRecompensa();
};

#endif
