#ifndef BULLET_H
#define BULLET_H

#include "../Rosalila/TinyXml/tinyxml.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
#include "../Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "../Rosalila/RosalilaUtility/Hitbox.h"

class Bullet
{
    Sound* sonido;
    RosalilaGraphics* painter;
    Receiver* receiver;
    vector<Image*>sprites;
    vector<Image*>sprites_on_hit;
    vector<Hitbox*> hitboxes;
    int damage;
    std::string name;
public:
    Bullet(){};
    Bullet(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,vector<Hitbox*> hitboxes,int damage);
    void logic();
    void render();
    vector<Hitbox*> getHitboxes();
    int getDamage();
    void playSound();
    void playHitSound();
    Image*getImage(int position);
    Image*getOnHitImage(int position);
    int spritesSize();
    int spritesOnHitSize();
};

#endif
