#include "Painter/Animation.h"

Animation::Animation(int x,int y,int animation_velocity,Painter* painter)
{
    this->painter=painter;

    this->x=x;
    this->y=y;
    this->animation_velocity=animation_velocity;
    this->animation_iteration=0;
    this->current_frame=0;
}

void Animation::addImage(Image*image)
{
    images.push_back(image);
}

void Animation::render()
{
    if(animation_iteration>=animation_velocity)
    {
        current_frame++;
        if(current_frame>=(int)images.size())
            current_frame=0;
        animation_iteration=0;
    }
    animation_iteration++;

    painter->draw2DImage
    (   images[current_frame],
        images[current_frame]->getWidth(),images[current_frame]->getHeight(),
        this->x,this->y,
        1.0,
        0.0,
        false,
        0,0,
        Color(255,255,255,255),
        false);

}
