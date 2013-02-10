#ifndef MENUBARRA_H
#define MENUBARRA_H

#include "Menu/Elemento.h"

class MenuBarra:Elemento
{
private:
    Image*fondo;
    Image*barra;
    int barra_x,barra_y;
    Image*fondo_sel;
    Image*barra_sel;
    int barra_x_sel,barra_y_sel;
    bool seleccionado;
    int accion;
public:
    int actual;
    int maximo;
    MenuBarra(Painter*painter,int x, int y, int width, int height,bool visible,
              Image*fondo,int barra_x,int barra_y,Image*barra,
              Image*fondo_sel,int barra_x_sel,int barra_y_sel,Image*barra_sel,
              int maximo,int actual,int accion);
    virtual std::string getTipo();
    virtual void dibujar();
    virtual void dibujar(int alineacion_x,int alineacion_y);
    void avanzar();
    void retroceder();
    bool getSeleccionado();
    void setSeleccionado(bool seleccionado);
    int getAccion();
};

#endif
