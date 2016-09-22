#ifndef LAYER_FRAME_H
#define LAYER_FRAME_H

#include "../../Rosalila/Rosalila.h"

class LayerFrame
{
public:
    Image* image;
    string type;
    int width;
    int height;
    LayerFrame(Image*image, string type, int width, int height);
    ~LayerFrame();
};

#endif
