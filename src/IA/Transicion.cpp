#include "IA/Transicion.h"

Transicion::Transicion(std::string input,std::string destino,bool recompensable)
{
    this->input=input;
    this->destino=destino;
    this->recompensable=recompensable;
    this->recompensa=0;
    this->wait=0;
}

void Transicion::agregarCondicion(Condicion2 condicion)
{
    condiciones.push_back(condicion);
}

void Transicion::setRecompensa(int recompensa)
{
    this->recompensa=recompensa;
}

void Transicion::setWait(int wait)
{
    this->wait=wait;
}

std::string Transicion::getInput()
{
    return input;
}

std::string Transicion::getDestino()
{
    return destino;
}

int Transicion::getWait()
{
    return wait;
}

int Transicion::getRecompensa()
{
    return recompensa;
}

bool Transicion::cumpleCondiciones(std::map<std::string,std::string>*strings,
                                   std::map<std::string,std::string>*strings_contrario,
                                   std::map<std::string,int>*enteros,
                                   std::map<std::string,int>*enteros_contrario
                                   )
{
    for(int i=0;i<(int)condiciones.size();i++)
    {
        if(condiciones[i].tipo=="cadena")
        {
            if(!cumpleCondicion(condiciones[i],strings,strings_contrario))
                return false;
        }else
        {
            if(!cumpleCondicion(condiciones[i],enteros,enteros_contrario))
                return false;
        }
    }
    return true;
}

bool Transicion::cumpleCondicion(Condicion2 condicion,std::map<std::string,std::string>*strings,std::map<std::string,std::string>*strings_contrario)
{
    if(condicion.personaje_contrario)
        return condicion.comparar(strings_contrario->operator[](condicion.variable));
    return condicion.comparar(strings->operator[](condicion.variable));
}

bool Transicion::cumpleCondicion(Condicion2 condicion,std::map<std::string,int>*enteros,std::map<std::string,int>*enteros_contrario)
{
    if(condicion.personaje_contrario)
        return condicion.comparar(enteros_contrario->operator[](condicion.variable));
    return condicion.comparar(enteros->operator[](condicion.variable));
}


///////////////////////////////////////////////7


Condicion2::Condicion2(std::string variable,std::string comparacion,int entero,bool personaje_contrario)
{
    tipo="entero";
    this->variable=variable;
    this->comparacion=comparacion;
    this->entero=entero;
    this->personaje_contrario=personaje_contrario;
}

Condicion2::Condicion2(std::string variable,std::string comparacion,std::string cadena,bool personaje_contrario)
{
    tipo="cadena";
    this->variable=variable;
    this->comparacion=comparacion;
    this->cadena=cadena;
    this->personaje_contrario=personaje_contrario;
}

bool Condicion2::comparar(int c)
{
    if(comparacion=="=")
        return c==entero;
    if(comparacion=="!=")
        return c!=entero;
    if(comparacion=="<")
        return c<entero;
    if(comparacion==">")
        return c>entero;
    if(comparacion=="<=")
        return c<=entero;
    if(comparacion==">=")
        return c>=entero;
    return false;
}

bool Condicion2::comparar(std::string s)
{
    if(comparacion=="=")
        return s==cadena;
    if(comparacion=="!=")
        return s!=cadena;
    return false;
}
