#ifndef BULLET_H
#define BULLET_H

#include "../Rosalila/TinyXml/tinyxml.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
#include "../Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "../Rosalila/RosalilaUtility/Hitbox.h"

class Bullet
{
public:
    RosalilaSound* sonido;
    RosalilaGraphics* painter;
    Receiver* receiver;
    vector<Image*>sprites;
    vector<Image*>sprites_on_hit;
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
    Bullet(RosalilaSound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,vector<Hitbox*> hitboxes,vector<string>random_sounds, int randomize_sound_frequency, int arpeggio_length,int damage,int sound_channel);
    void playSound();
    void playHitSound();
    Image*getImage(int position);
    Image*getOnHitImage(int position);
    int getRandomSound();
    void randomizeArpeggio(int size);
};

#endif
