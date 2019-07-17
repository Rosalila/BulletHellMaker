#ifndef GRAFICO_H
#define GRAFICO_H

#ifdef WINDOWS
#include <windows.h>
#endif

#include <GL/glew.h>

#include <iostream>
#include <list>

#include "../Rosalila.h"

#ifdef LINUX
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#ifdef WINDOWS
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#ifdef OSX
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif

#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include "Color.h"
#include "FlatShadow.h"
#include "Image.h"
#include "Timer.h"
#include "../RosalilaUtility/RosalilaUtility.h"
#include "Drawables/DrawableRectangle.h"
#include "Drawables/DrawableTriangle.h"
#include "Drawables/DrawablePoint.h"
#include "Effects/GrayscaleEffect.h"
#include "Effects/TransparencyEffect.h"
#include "Effects/ScreenShakeEffect.h"
#include "Effects/PointExplosionEffect.h"
#include "Notifications/NotificationHandler.h"

#include <iostream>
#include <fstream>

using namespace std;

class FlatShadow;
class NotificationHandler;
class PointExplosionEffect;

class ROSALILA_DLL RosalilaGraphics
{
    public:
    SDL_Window* window;
    TTF_Font* font;
    SDL_Color textColor;
    int screen_width;
    int screen_height;
    bool fullscreen;
    int screen_bpp;
    SDL_Joystick *joystick_1;
    SDL_Joystick *joystick_2;

    //Set fps
    int frames_per_seccond;
    int frame;
    int current_fps;
    int last_tick;

    Image* notification_background;
    int notification_background_x;
    int notification_background_y;
    NotificationHandler notification_handler;

    TransparencyEffect transparency_effect;
    ScreenShakeEffect screen_shake_effect;
    GrayscaleEffect grayscale_effect;
    PointExplosionEffect* point_explosion_effect;

    RosalilaGraphics();
    ~RosalilaGraphics();
    void init();
    Image* getImage(std::string filename);
    void drawImage(Image* texture, int x, int y);
    void draw2DImageBatch(
	             Image* texture,
				 int size_x,int size_y,
				 vector<int> position_x,vector<int>position_y,
				 float scale,
				 vector<float>rotation,
				 bool flipHorizontally,
				 Color color_effects);
    void drawText(std::string text, int position_x, int position_y, bool center_x, bool center_y);
    void drawText(TTF_Font* font, std::string text, int position_x, int position_y, bool center_x, bool center_y);
    void drawRectangle(int x,int y,int width,int height,float rotation,int red,int green,int blue,int alpha);
    void drawRectangles(vector<DrawableRectangle*>rectangles);
    void drawTriangles(vector<DrawableTriangle*>triangles);
    void drawPoints(list<DrawablePoint*>points);
    void updateScreen();
    void frameCap();
    void clearScreen(Color color);
    void screenshot(int x, int y, int w, int h, string filename);
};
#endif
