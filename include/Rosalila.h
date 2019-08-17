#ifndef ROSALILA_H
#define ROSALILA_H

#include <iostream>
using namespace std;

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

#ifdef WINDOWS
#include <windows.h>
#endif

#include "RosalilaGraphics/RosalilaGraphics.h"
#include "RosalilaSound/RosalilaSound.h"
#include "RosalilaInputs/RosalilaInputs.h"
#include "RosalilaUtility/RosalilaUtility.h"
#include "RosalilaParser/RosalilaParser.h"
#include "RosalilaNet/RosalilaNet.h"
#include "RosalilaApiIntegrator/RosalilaApiIntegrator.h"

class RosalilaGraphics;
class RosalilaSound;
class RosalilaReceiver;
class RosalilaUtility;
class RosalilaParser;
//class RosalilaNet;
class RosalilaApiIntegrator;

//extern std::string assets_directory;

#define assets_directory "assets/"

extern std::string CONFIG_FILE_PATH;

class ROSALILA_DLL RosalilaContainer
{
public:
    RosalilaGraphics* graphics;
    RosalilaSound* sound;
    RosalilaReceiver* receiver;
    RosalilaApiIntegrator* api_integrator;
    RosalilaUtility* utility;
    RosalilaParser* parser;
    //RosalilaNet* net;
    RosalilaContainer();
    void init(std::string config_file_path);
    void update();
};

extern ROSALILA_DLL RosalilaContainer* rosalila();

#endif // SYSTEM_H_INCLUDED
