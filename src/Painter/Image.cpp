#include "Painter/Painter.h"

Image::Image()
{
    texture=NULL;
    int width=-1;
    int height=-1;
}

int Image::getWidth()
{
    return width;
}
int Image::getHeight()
{
    return height;
}

GLuint Image::getTexture()
{
    return texture;
}

void Image::setTexture(GLuint texture)
{
    this->texture=texture;
}

void Image::setWidth(int width)
{
    this->width=width;
}
void Image::setHeight(int height)
{
    this->height=height;
}

Image::~Image()
{
    glDeleteTextures( 1, &texture );
}
