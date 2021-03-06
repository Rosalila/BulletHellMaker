#ifndef NOTIFICATION_H
#define NOTIFICATION_H


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


#include "../Image.h"

class ROSALILA_DLL Notification
{
public:
    Image* image;
    int x;
    int y;
    int target_y;
    int frame;
    int duration;
    int initial_y;
    double velocity;
    Notification(Image* image, int x, int y, int target_y, int duration);
    void update();
    bool isFinished();
};

#endif
