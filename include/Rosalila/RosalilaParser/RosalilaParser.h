#ifndef ROSALILA_PARSER_H
#define ROSALILA_PARSER_H

#include "Node.h"
#include "../Rosalila.h"
#include "../RosalilaUtility/RosalilaUtility.h"
#include "json.hpp"

class ROSALILA_DLL RosalilaParser
{
public:
    void init();
    Node* getNodes(string file_name);
};

#endif
