#ifndef MENUBOTON_H
#define MENUBOTON_H

#include "Menu/Elemento.h"

class MenuBoton:Elemento
{
private:
    bool seleccionado;
    std::string accion;
    Image* imagen;
    std::string texto;
    int alineacion_texto_x,alineacion_texto_y;
    Image* imagen_sel;
    std::string texto_sel;
    int alineacion_texto_x_sel,alineacion_texto_y_sel;
    //otra
public:
    char* load_menu;
    std::string input_config;
    MenuBoton(Painter*painter,int x, int y, int width, int height,bool visible,
              Image* imagen,int alineacion_texto_x,int alineacion_texto_y, std::string texto,
              Image* imagen_sel,int alineacion_texto_x_sel,int alineacion_texto_y_sel, std::string texto_sel,
              std::string accion,char* load_menu
              );
    std::string getAccion();
    virtual std::string getTipo();
    virtual void dibujar();
    virtual void dibujar(int alineacion_x,int alineacion_y);
    bool estaSeleccionado();
    void setSeleccionado(bool seleccionado);
};

#endif
