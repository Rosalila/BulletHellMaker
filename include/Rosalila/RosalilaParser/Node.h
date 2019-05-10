#ifndef NODE_H
#define NODE_H

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
#include <map>
#include <vector>

using namespace std;

class ROSALILA_DLL Node
{
public:
    map<string,string>attributes;
    multimap<string,Node*>nodes;
    Node(map<string,string>attributes,multimap<string,Node*>nodes);
    ~Node();
    bool hasAttribute(string name);
    Node*getNodeByName(string name);
    vector<Node*> getNodesByName(string name);
};

#endif
