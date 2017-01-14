#include "Bullet.h"

Bullet::Bullet(std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,double width, double height, Color color,vector<Hitbox*> hitboxes,vector<string>random_sounds, int randomize_sound_frequency, int arpeggio_length,int damage, int sound_channel)
{
    this->sprites = sprites;
    this->sprites_on_hit = sprites_on_hit;
    this->width = width;
    this->height = height;
    this->color = color;
    this->hitboxes = hitboxes;
    this->damage = damage;
    this->name = name;
    this->sound_channel = sound_channel;
    this->random_sounds = random_sounds;
    this->randomize_sound_frequency=randomize_sound_frequency;
    this->arpeggio_length = arpeggio_length;
    count_sound_plays=0;
    current_random_sound = getRandomSound();
    current_arrpegio_sound = 0;
    randomizeArpeggio(arpeggio_length);
    current_channel = -1;
}

Bullet::~Bullet()
{
    for(int i=0;i<(int)sprites.size();i++)
    {
        delete sprites[i];
    }

    for(int i=0;i<(int)sprites_on_hit.size();i++)
    {
        delete sprites_on_hit[i];
    }

    for(int i=0;i<(int)hitboxes.size();i++)
    {
        delete hitboxes[i];
    }
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

void Bullet::playSound(int panning, bool uses_camera)
{
    if(random_sounds.size()<1)
    {
        return;
    }

    if(count_sound_plays%randomize_sound_frequency==0)
    {
        randomizeArpeggio(arpeggio_length);
    }

    if(current_arrpegio_sound>=(int)current_arpeggio.size())
        current_arrpegio_sound=0;
    current_random_sound=current_arpeggio[current_arrpegio_sound];
    current_arrpegio_sound++;

    count_sound_plays++;
    if(current_random_sound!=-1)
    {
        current_channel = rosalila()->sound->playSound(random_sounds[current_random_sound], sound_channel, 0, panning, uses_camera);
    }
}

void Bullet::playHitSound(int panning, bool uses_camera)
{
    if(rosalila()->sound->soundExists("bullet_hit."+name))
    {
        rosalila()->sound->playSound("bullet_hit."+name, sound_channel+5 ,0 ,panning ,uses_camera);
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
