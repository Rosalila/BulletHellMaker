#include "../include/Spells/Bullet.h"

Bullet::Bullet(Sonido* sonido,Painter* painter,Receiver* receiver,vector<Image*>sprites,Hitbox hitbox,int damage)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->sprites=sprites;
    this->hitbox=hitbox;
    this->damage=damage;
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

Hitbox Bullet::getHitbox()
{
    return hitbox;
}

int Bullet::getDamage()
{
    return damage;
}
