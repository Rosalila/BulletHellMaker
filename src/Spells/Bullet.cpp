#include "../include/Spells/Bullet.h"

Bullet::Bullet(Sonido* sonido,Painter* painter,Receiver* receiver,vector<Image*>sprites)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->sprites=sprites;
}

void Bullet::logic()
{
}

void Bullet::render()
{

}

Image* Bullet::getImage(int position)
{
    return sprites[position];
}

int Bullet::spritesSize()
{
    return sprites.size();
}
