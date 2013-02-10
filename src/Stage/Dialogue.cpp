#include "Stage/Dialogue.h"

Dialogue::Dialogue(Painter* painter,Sonido* sonido,Receiver*receiver,std::string text)
{
    this->painter=painter;
    this->sonido=sonido;
    this->receiver=receiver;
    this->text=text;
    this->iterator=0;
    this->duration=30;
}

std::string Dialogue::getText()
{
    return text;
}

void Dialogue::render()
{
    painter->drawText(text,100,100);
}

void Dialogue::logic()
{
    iterator++;
}

bool Dialogue::destroyFlag()
{
    return iterator>duration;
}
