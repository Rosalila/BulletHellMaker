#include "Menu/MenuBarra.h"

MenuBarra::MenuBarra(Painter*painter,int x, int y, int width, int height,bool visible,
          Image*fondo,int barra_x,int barra_y,Image*barra,
          Image*fondo_sel,int barra_x_sel,int barra_y_sel,Image*barra_sel,
          int maximo,int actual,int accion)
{
    this->x=x;
    this->y=y;
    this->height=height;
    this->width=width;
    this->visible=visible;

    this->fondo=fondo;
    this->barra=barra;
    this->barra_x=barra_x;
    this->barra_y=barra_y;

    this->fondo_sel=fondo_sel;
    this->barra_sel=barra_sel;
    this->barra_x_sel=barra_x_sel;
    this->barra_y_sel=barra_y_sel;

    this->maximo=maximo;
    this->actual=actual;
    this->accion=accion;
    this->seleccionado=false;
    this->painter=painter;
}

std::string MenuBarra::getTipo()
{
    return "Barra";
}

void MenuBarra::avanzar()
{
    if(actual<maximo)
        actual++;
}

void MenuBarra::retroceder()
{
    if(actual>0)
        actual--;
}

void MenuBarra::dibujar()
{
    if(!seleccionado)
    {
        painter->draw2DImage
        (   fondo,
            width,height,
            x,y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
        painter->draw2DImage
        (   barra,
            width*(actual/maximo),height,
            x+barra_x,y+barra_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
    }else
    {
        painter->draw2DImage
        (   fondo_sel,
            width,height,
            x,y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
        painter->draw2DImage
        (   barra_sel,
            width*(actual/maximo),height,
            x+barra_x_sel,y+barra_y_sel,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
    }
}

void MenuBarra::dibujar(int alineacion_x,int alineacion_y)
{
    if(!seleccionado)
    {
        painter->draw2DImage
        (   fondo,
            width,height,
            x+alineacion_x,y+alineacion_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
        painter->draw2DImage
        (   barra,
            width*((float)actual/(float)maximo),height,
            x+barra_x+alineacion_x,y+barra_y+alineacion_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
    }else
    {
        painter->draw2DImage
        (   fondo_sel,
            width,height,
            x+alineacion_x,y+alineacion_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
        painter->draw2DImage
        (   barra_sel,
            width*((float)actual/(float)maximo),height,
            x+barra_x_sel+alineacion_x,y+barra_y_sel+alineacion_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);
    }
}

bool MenuBarra::getSeleccionado()
{
    return seleccionado;
}

void MenuBarra::setSeleccionado(bool seleccionado)
{
    this->seleccionado=seleccionado;
}

int MenuBarra::getAccion()
{
    return accion;
}
