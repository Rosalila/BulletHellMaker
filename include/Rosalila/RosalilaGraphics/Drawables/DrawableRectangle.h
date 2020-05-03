#ifndef DRAWABLE_RECTANGLE_H
#define DRAWABLE_RECTANGLE_H

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

#include "../Color.h"

class ROSALILA_DLL DrawableRectangle
{
public:
    int x;
    int y;
    int width;
    int height;
    int angle;
    Color color;
    DrawableRectangle(int x, int y, int width, int height, int angle, Color color);
};

#endif
