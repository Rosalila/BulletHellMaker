#include "Menu/Elemento.h"

Elemento::Elemento(Painter*painter,int x, int y, int displacement_x,int displacement_y,int stop_displacement_x_at,int stop_displacement_y_at,int current_displacement_x,int current_displacement_y, int width, int height,bool visible)
{
    this->current_displacement_x=0;
    this->current_displacement_y=0;
    this->x=x;
    this->y=y;
    this->displacement_x=displacement_x;
    this->displacement_y=displacement_y;
    this->stop_displacement_x_at=stop_displacement_x_at;
    this->stop_displacement_y_at=stop_displacement_y_at;
    this->height=height;
    this->width=width;
    this->visible=visible;
    this->painter=painter;
}

Elemento::Elemento()
{
    this->current_displacement_x=0;
    this->current_displacement_y=0;
    this->x=0;
    this->y=0;
    this->displacement_x=0;
    this->displacement_y=0;
    this->stop_displacement_x_at=stop_displacement_x_at;
    this->stop_displacement_y_at=stop_displacement_y_at;
    this->height=0;
    this->width=0;
    this->visible=false;
    this->painter=NULL;
}
