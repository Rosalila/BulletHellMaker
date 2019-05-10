#ifndef LEADERBOARD_ENTRY_H
#define LEADERBOARD_ENTRY_H

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

#include <iostream>
using namespace std;

class ROSALILA_DLL LeaderboardEntry
{
public:
    string name;
    int rank;
    int score;
    int details;
    char* attachment;
    int attachment_size;
    string attachment_state;

    LeaderboardEntry(string name, int rank, int score, int details);
};

#endif
