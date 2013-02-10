#include "Menu/MenuBoton.h"

MenuBoton::MenuBoton(Painter*painter,int x, int y, int width, int height,bool visible,
          Image* imagen,int alineacion_texto_x,int alineacion_texto_y, std::string texto,
          Image* imagen_sel,int alineacion_texto_x_sel,int alineacion_texto_y_sel, std::string texto_sel,
          std::string accion,char* load_menu
          )
{
    this->x=x;
    this->y=y;
    this->height=height;
    this->width=width;
    this->visible=visible;
    this->imagen=imagen;
    this->texto=texto;
    this->alineacion_texto_x=alineacion_texto_x;
    this->alineacion_texto_y=alineacion_texto_y;
    this->imagen_sel=imagen_sel;
    this->texto_sel=texto_sel;
    this->alineacion_texto_x_sel=alineacion_texto_x_sel;
    this->alineacion_texto_y_sel=alineacion_texto_y_sel;
    this->seleccionado=false;
    this->accion=accion;
    this->load_menu=load_menu;
    this->painter=painter;
    this->input_config="not set";
}

std::string MenuBoton::getAccion()
{
    return accion;
}

std::string MenuBoton::getTipo()
{
    return "Boton";
}

void MenuBoton::dibujar()
{
    if(!seleccionado)
    {
        painter->draw2DImage
        (   imagen,
            width,height,
            x,y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);

        std::string temp=texto;
        if(accion.substr(0,6)=="Player")
        {
            temp+=input_config;
        }
        painter->drawText(temp,x+alineacion_texto_x,y+alineacion_texto_y);
    }else
    {
        painter->draw2DImage
        (   imagen_sel,
            width,height,
            x,y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);

        std::string temp=texto_sel;
        if(accion.substr(0,6)=="Player")
        {
            temp+=input_config;
        }
        painter->drawText(temp,x+alineacion_texto_x_sel,y+alineacion_texto_y_sel);
    }
}

void MenuBoton::dibujar(int alineacion_x,int alineacion_y)
{
    if(!seleccionado)
    {
        painter->draw2DImage
        (   imagen,
            width,height,
            x+alineacion_x,y+alineacion_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);

        std::string temp=texto;
        if(accion.substr(0,6)=="Player")
        {
            temp+=input_config;
        }
        painter->drawText(temp,x+alineacion_texto_x+alineacion_x,y+alineacion_texto_y+alineacion_y);
    }else
    {
        painter->draw2DImage
        (   imagen_sel,
            width,height,
            x+alineacion_x,y+alineacion_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            false);

        std::string temp=texto_sel;
        if(accion.substr(0,6)=="Player")
        {
            temp+=input_config;
        }
        painter->drawText(temp,x+alineacion_texto_x_sel+alineacion_x,y+alineacion_texto_y_sel+alineacion_y);
    }
}

bool MenuBoton::estaSeleccionado()
{
    return seleccionado;
}

void MenuBoton::setSeleccionado(bool seleccionado)
{
    this->seleccionado=seleccionado;
}
