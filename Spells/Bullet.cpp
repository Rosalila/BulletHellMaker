#include "Bullet.h"

Bullet::Bullet(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,vector<Hitbox*> hitboxes,vector<string>random_sounds, int randomize_sound_frequency,int damage, int sound_channel)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->sprites=sprites;
    this->sprites_on_hit=sprites_on_hit;
    this->hitboxes=hitboxes;
    this->damage=damage;
    this->name=name;
    this->sound_channel=sound_channel;
    this->random_sounds=random_sounds;
    this->randomize_sound_frequency=randomize_sound_frequency;
    count_sound_plays=0;
    current_random_sound=getRandomSound();
}

void Bullet::logic()
{
}

void Bullet::render()
{

}

Image* Bullet::getImage(int position)
{
    if(position>=(int)sprites.size())
        return NULL;
    return sprites[position];
}

Image* Bullet::getOnHitImage(int position)
{
    if(position>=(int)sprites_on_hit.size())
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

vector<Hitbox*> Bullet::getHitboxes()
{
    return hitboxes;
}

int Bullet::getDamage()
{
    return damage;
}

void Bullet::playSound()
{
    if(random_sounds.size()>1 && count_sound_plays%randomize_sound_frequency==0)
    {
        int new_sound=getRandomSound();
        while(current_random_sound==new_sound)
        {
            new_sound=getRandomSound();
        }
        current_random_sound=new_sound;
    }
    count_sound_plays++;
    if(current_random_sound!=-1)
    {
        sonido->playSound(random_sounds[current_random_sound],sound_channel);

//        current_random_sound++;
//        if(current_random_sound>=random_sounds.size())
//            current_random_sound=0;
    }
//    if(sonido->soundExists("bullet."+name))
//    {
//        sonido->playSound("bullet."+name,sound_channel);
//    }
}

void Bullet::playHitSound()
{
    if(sonido->soundExists("bullet_hit."+name))
    {
        sonido->playSound("bullet_hit."+name,sound_channel+5);
    }
}

int Bullet::getRandomSound()
{
    if(random_sounds.size()==0)
        return -1;
    return rand()%random_sounds.size();
}
