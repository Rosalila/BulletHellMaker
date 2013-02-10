#ifndef ELEMENTO_H
#define ELEMENTO_H

#include "Painter/Painter.h"

class Elemento
{
protected:
    Painter*painter;
public:
    int x;
    int y;
    int displacement_x;
    int displacement_y;
    int stop_displacement_x_at;
    int stop_displacement_y_at;
    int current_displacement_x;
    int current_displacement_y;
    int width;
    int height;
    bool visible;
    Elemento();
    Elemento(Painter*painter,int x, int y,int displacement_x,int displacement_y,int stop_displacement_x_at,int stop_displacement_y_at,int current_displacement_x,int current_displacement_y, int width, int height,bool visible);
    virtual void dibujar()=0;
    virtual void dibujar(int alineacion_x,int alineacion_y)=0;
    virtual std::string getTipo()=0;
};
#endif
