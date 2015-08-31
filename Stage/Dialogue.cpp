#include "Dialogue.h"

Dialogue::Dialogue(RosalilaGraphics* painter,Sound* sonido,Receiver*receiver,std::string text,Image*image)
{
    this->painter=painter;
    this->sonido=sonido;
    this->receiver=receiver;
    this->text=text;
    this->iterator=0;
    this->duration=30;
    this->image=image;
}

std::string Dialogue::getText()
{
    return text;
}

void Dialogue::render(int x,int y)
{
    painter->drawText(text,x,y);

    painter->draw2DImage
    (   image,
        image->getWidth(),image->getHeight(),
        x,y,
        1.0,
        0.0,
        false,
        0,0,
        Color(255,255,255,255),
        0,0,
        false);
}

void Dialogue::logic()
{
    iterator++;
}

bool Dialogue::destroyFlag()
{
    return iterator>duration;
}
