#include "../include/Spells/Bullet.h"

Bullet::Bullet(Sonido* sonido,Painter* painter,Receiver* receiver,std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,Hitbox hitbox,int damage,bool has_sound)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->sprites=sprites;
    this->sprites_on_hit=sprites_on_hit;
    this->hitbox=hitbox;
    this->damage=damage;
    this->name=name;
    this->has_sound=has_sound;
}

void Bullet::logic()
{
}

void Bullet::render()
{

}

Image* Bullet::getImage(int position)
{
    if(position>=sprites.size())
        return NULL;
    return sprites[position];
}

Image* Bullet::getOnHitImage(int position)
{
    if(position>=sprites_on_hit.size())
        return NULL;
    return sprites_on_hit[position];
}

int Bullet::spritesSize()
{
    return sprites.size();
}

int Bullet::spritesOnHitSize()
{
    return sprites_on_hit.size();
}

Hitbox Bullet::getHitbox()
{
    return hitbox;
}

int Bullet::getDamage()
{
    return damage;
}

void Bullet::playSound()
{
    if(has_sound)
    {
        sonido->reproducirSonido("bullet."+name,false);
    }
}
