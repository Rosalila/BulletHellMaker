#ifndef MENUTEXTO_H
#define MENUTEXTO_H

#include "Menu/Elemento.h"

class MenuTexto:Elemento
{
public:
    std::string texto;
    MenuTexto(Painter*painter,int x, int y, int width, int height,bool visible,std::string texto);
    virtual std::string getTipo();
    virtual void dibujar();
    virtual void dibujar(int alineacion_x,int alineacion_y);
};

#endif
