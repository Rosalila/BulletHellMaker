#ifndef LEADERBOARD_H
#define LEADERBOARD_H

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

#include <vector>
#include "LeaderboardEntry.h"

class ROSALILA_DLL Leaderboard
{
public:
    vector<LeaderboardEntry*>top_entries;
    vector<LeaderboardEntry*>near_entries;
    vector<LeaderboardEntry*>friends_entries;
    LeaderboardEntry* leaderboard_self_entry;
    Leaderboard();
};

#endif
