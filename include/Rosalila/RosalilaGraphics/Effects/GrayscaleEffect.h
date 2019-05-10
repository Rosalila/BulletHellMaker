#ifndef GRAYSCALE_EFFECT_H
#define GRAYSCALE_EFFECT_H

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


class ROSALILA_DLL GrayscaleEffect
{
public:
    double current_percentage;
    double target_percentage;
    double change_speed;

    void init();
    void set(double target_percentage, double change_speed);
    void update();
};

#endif
