#include "SDL/SDL_image.h"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"

class Animation
{
    Painter*painter;

    std::vector <Image*> images;
    int x,y;
    int animation_velocity;
    int animation_iteration;
    int current_frame;
public:
    Animation(){}
    Animation(int x,int y,int animation_velocity,Painter* painter);
    void addImage(Image*image);
    void render();
};
