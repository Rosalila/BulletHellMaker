#ifndef GRAFICO_H
#define GRAFICO_H

#include <iostream>
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "TinyXml/tinyxml.h"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>


#include "Painter/Image.h"
#include "Painter/Timer.h"
#include "Utility/Utility.h"

#include <iostream>
#include <fstream>
using namespace std;

class Color
{
public:
    int red;
    int green;
    int blue;
    int alpha;

    Color()
    {
        this->red=0;
        this->green=0;
        this->blue=0;
        this->alpha=0;
    }

    Color(int red,int green,int blue,int alpha)
    {
        this->red=red;
        this->green=green;
        this->blue=blue;
        this->alpha=alpha;
    }

    int getRed()
    {
        return red;
    }

    int getGreen()
    {
        return green;
    }

    int getBlue()
    {
        return blue;
    }

    int getAlpha()
    {
        return alpha;
    }
};

class Painter
{
    public:
    SDL_Surface* screen;
    TTF_Font* font;
    SDL_Color textColor;
    int screen_width;
    int screen_height;
    int screen_bpp;
    int camera_x,camera_y;
    SDL_Joystick *joystick_1;
    SDL_Joystick *joystick_2;

    //Set fps
    int frames_per_seccond;
    int frame;
    Timer *fps;
    Timer *update;

    Painter();
    ~Painter();
    Image* getTexture(std::string filename);
    void draw2DImage(
	             Image* texture,
				 int size_x,int size_y,
				 int position_x,int position_y,
				 float scale,
				 float rotation,
				 bool flipHorizontally,
				 int depth_effect_x,
				 int depth_effect_y,
				 Color color_effects,
				 bool camera_align);
    void drawText(std::string text,int position_x,int position_y);
    void drawRectangle(int x,int y,int width,int height,float rotation,int red,int green,int blue,int alpha,bool camera_align);
    void updateScreen();
    void frameCap();
};
#endif
