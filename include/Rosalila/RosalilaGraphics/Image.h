#ifndef ROSALILAIMAGE_H
#define ROSALILAIMAGE_H





//Choose your build platform by defining it:
#ifdef __linux__
#define LINUX
#endif

#ifdef __APPLE__
#define OSX
#endif
#ifdef __MACH__
#define OSX
#endif

#ifdef _WIN32
#define WINDOWS
#endif
#ifdef _WIN64
#define WINDOWS
#endif

#ifdef ROSALILA_STATICLIB
#  define ROSALILA_DLL
#elif defined(WINDOWS)
#  ifdef ROSALILA_DYNAMICLIB
#    define ROSALILA_DLL  __declspec(dllexport)
#  else
#    define ROSALILA_DLL  __declspec(dllimport)
#  endif
#else
#  define ROSALILA_DLL
#endif





#ifdef LINUX
#include <SDL2/SDL_image.h>
#endif

#ifdef OSX
#include <SDL2_image/SDL_image.h>
#endif

#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include "Color.h"

class ROSALILA_DLL Image
{
public:
    GLuint texture;
    int original_width;
    int original_height;

    int width;
    int height;
    float scale;
    float rotation;
    bool horizontal_flip;
    bool blend_effect;
    Color color_filter;

    Image();
    ~Image();
    int getWidth();
    int getHeight();
    GLuint getTexture();
    void setTexture(GLuint texture);
    void setWidth(int width);
    void setHeight(int height);
};

#endif
