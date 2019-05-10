#ifndef SCREEN_SHAKE_EFFECT_H
#define SCREEN_SHAKE_EFFECT_H

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

#include <stdlib.h>

class ROSALILA_DLL ScreenShakeEffect
{
public:
    int magnitude;
    int time;
    int original_x;
    int original_y;
    int current_x;
    int current_y;

    void init();
    void set(int magnitude, int time, int original_x, int original_y);
    void update();
};

#endif
