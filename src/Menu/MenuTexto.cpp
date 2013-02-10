#include "Menu/MenuTexto.h"

MenuTexto::MenuTexto(Painter*painter,int x, int y, int width, int height,bool visible,std::string texto)
{
    this->x=x;
    this->y=y;
    this->height=height;
    this->width=width;
    this->visible=visible;
    this->texto=texto;
    this->painter=painter;
}

std::string MenuTexto::getTipo()
{
    return "Texto";
}

void MenuTexto::dibujar()
{
    painter->drawText(texto,x,y);
}

void MenuTexto::dibujar(int alineacion_x,int alineacion_y)
{
    painter->drawText(texto,x+alineacion_x,y+alineacion_y);
}
