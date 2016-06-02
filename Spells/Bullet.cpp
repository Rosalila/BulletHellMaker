#include "Bullet.h"

Bullet::Bullet(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,vector<Hitbox*> hitboxes,vector<string>random_sounds, int randomize_sound_frequency, int arpeggio_length,int damage, int sound_channel)
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
    this->arpeggio_length=arpeggio_length;
    count_sound_plays=0;
    current_random_sound=getRandomSound();
    current_arrpegio_sound=0;
    randomizeArpeggio(arpeggio_length);
    current_channel=-1;
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
    if(random_sounds.size()<1)
    {
        return;
    }

    if(count_sound_plays%randomize_sound_frequency==0)
    {
        randomizeArpeggio(arpeggio_length);
    }

    if(current_arrpegio_sound>=current_arpeggio.size())
        current_arrpegio_sound=0;
    current_random_sound=current_arpeggio[current_arrpegio_sound];
    current_arrpegio_sound++;

    count_sound_plays++;
    if(current_random_sound!=-1)
    {
//        if(current_channel!=-1)
//            Mix_HaltChannel(current_channel);
        current_channel=sonido->playSound(random_sounds[current_random_sound],sound_channel);
    }
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

void Bullet::randomizeArpeggio(int size)
{
    current_arpeggio.clear();
    int last_random_sound=-999;
    for(int i=0;i<size;i++)
    {
        int new_random_sound=getRandomSound();
        while(last_random_sound==new_random_sound)
        {
            new_random_sound=getRandomSound();
        }
        current_arpeggio.push_back(new_random_sound);
    }
}
