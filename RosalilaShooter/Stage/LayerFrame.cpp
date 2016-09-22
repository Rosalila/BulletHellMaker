#include "LayerFrame.h"

LayerFrame::LayerFrame(Image*image, string type, int width, int height)
{
    this->image = image;
    this->type = type;
    this->width = width;
    this->height = height;
}

LayerFrame::~LayerFrame()
{
    if(image!=NULL)
    {
        delete image;
    }
}
