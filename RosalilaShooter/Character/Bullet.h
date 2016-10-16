#ifndef BULLET_H
#define BULLET_H

#include "../../Rosalila/Rosalila.h"

class Bullet
{
public:
    vector<Image*>sprites;
    vector<Image*>sprites_on_hit;
    double width;
    double height;
    Color color;
    vector<Hitbox*> hitboxes;
    int damage;
    std::string name;
    int sound_channel;
    vector<string>random_sounds;
    int count_sound_plays;
    int current_random_sound;
    int randomize_sound_frequency;
    vector<int> current_arpeggio;
    int current_arrpegio_sound;
    int arpeggio_length;
    int current_channel;

    Bullet(){};
    Bullet(std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,double width, double height, Color color,vector<Hitbox*> hitboxes,vector<string>random_sounds, int randomize_sound_frequency, int arpeggio_length,int damage,int sound_channel);
    ~Bullet();
    int getRandomSound();
    void randomizeArpeggio(int size);
    void playSound(int panning, bool uses_camera);
    void playHitSound(int panning, bool uses_camera);
    Image*getImage(int position);
    Image*getOnHitImage(int position);
};

#endif
