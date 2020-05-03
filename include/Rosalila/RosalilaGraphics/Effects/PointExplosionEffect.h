#ifndef POINT_EXPLOSION_EFFECT
#define POINT_EXPLOSION_EFFECT

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

#include <list>
#include <cmath>
#include <stdlib.h>
#include "../Drawables/DrawablePoint.h"
#include "../../Rosalila.h"

#define PI 3.14159265

using namespace std;

class ROSALILA_DLL PointExplosion : public DrawablePoint
{
public:
    double angle;
    double velocity;
    int duration;
    int frame;
    PointExplosion(Point point, Color color, int size, double angle, double velocity, int duration);
    void update();
};

class ROSALILA_DLL PointExplosionEffect
{
public:
    list<DrawablePoint*> current_points;
    void explode(int x, int y, Color color, int intensity);
    void update();
};

#endif
