#ifndef MENULISTA_H
#define MENULISTA_H

#include <vector>
#include "Menu/Elemento.h"

class MenuLista:Elemento
{
private:
    Image*flecha_izquierda;
    Image*flecha_derecha;
    int flecha_izq_x_sel,flecha_izq_y;
    int flecha_der_x_sel,flecha_der_y;
    Image*flecha_izquierda_sel;
    Image*flecha_derecha_sel;
    int flecha_izq_x,flecha_izq_y_sel;
    int flecha_der_x,flecha_der_y_sel;
    bool seleccionado;
    int accion;
    int actual;//!!!!!!!
    std::vector<Elemento*>elementos;//!!!!!!!
public:
    MenuLista(Painter*painter,int x, int y, int width, int height,bool visible,
              int flecha_izq_x,int flecha_izq_y,Image*flecha_izquierda,int flecha_der_x,int flecha_der_y,Image*flecha_derecha,
              int flecha_izq_x_sel,int flecha_izq_y_sel,Image*flecha_izquierda_sel,int flecha_der_x_sel,int flecha_der_y_sel,Image*flecha_derecha_sel,
              std::vector<Elemento*>elementos,int accion
              );
    virtual std::string getTipo();
    void avanzar();
    void retroceder();
    virtual void dibujar();
    virtual void dibujar(int alineacion_x,int alineacion_y);
    bool estaSeleccionado();
    void setSeleccionado(bool seleccionado);
    int getAccion();
    Elemento*getElementoActual();
};

#endif
