#include "../include/Character/Character.h"

Character::Character(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory)
{
    //Loading file
    std::string main_path=directory+"main.xml";
    TiXmlDocument doc_t(main_path.c_str());
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;
    TiXmlNode *main_file=doc->FirstChild("MainFile");

    //Loading attributes
    TiXmlElement *attributes=main_file->FirstChild("Attributes")->ToElement();
    this->velocity=atoi(attributes->Attribute("velocity"));

    //Loading sprites
    for(TiXmlNode* sprites_node=main_file->FirstChild("Sprites");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("Sprites"))
    {
        std::vector<Image*>sprites_vector;
        std::string sprites_orientation=sprites_node->ToElement()->Attribute("orientation");
        for(TiXmlNode* sprite_node=sprites_node->FirstChild("Sprite");
                sprite_node!=NULL;
                sprite_node=sprite_node->NextSibling("Sprite"))
        {
            sprites_vector.push_back(painter->getTexture(directory+"sprites/"+sprite_node->ToElement()->Attribute("path")));
        }
        sprites[sprites_orientation]=sprites_vector;
    }

    //Setting up the other variables
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->active_patterns=new std::list<Pattern*>;
    this->x=500;
    this->y=500;
    this->shooting=true;
    this->orientation="idle";




    //Loading bullets
    std::string bullets_path=directory+"bullets.xml";
    TiXmlDocument doc_bullets_t(bullets_path.c_str());
    doc_bullets_t.LoadFile();
    TiXmlDocument *doc_bullets;
    doc_bullets=&doc_bullets_t;
    TiXmlNode *bullet_file=doc_bullets->FirstChild("BulletsFile");

    //Loading bullets
    for(TiXmlNode* bullet_node=bullet_file->FirstChild("Bullet");
            bullet_node!=NULL;
            bullet_node=bullet_node->NextSibling("Bullet"))
    {
        std::string node_name=bullet_node->ToElement()->Attribute("name");
        vector<Image*>sprites_temp;
        for(TiXmlNode* sprite_node=bullet_node->FirstChild("Sprite");
                sprite_node!=NULL;
                sprite_node=sprite_node->NextSibling("Sprite"))
        {
            sprites_temp.push_back(painter->getTexture(directory+"sprites/"+sprite_node->ToElement()->Attribute("path")));
        }
        bullets[node_name]=new Bullet(sonido,painter,receiver,sprites_temp);
    }

    //Loading file
    std::string pattern_path=directory+"patterns.xml";
    TiXmlDocument doc_pattern_t(pattern_path.c_str());
    doc_pattern_t.LoadFile();
    TiXmlDocument *doc_pattern;
    doc_pattern=&doc_pattern_t;
    TiXmlNode *patterns_file=doc_pattern->FirstChild("PatternsFile");

    //Loading patterns
    int offset_x=0;
    int offset_y=0;
    for(TiXmlNode* pattern_node=patterns_file->FirstChild("Pattern");
            pattern_node!=NULL;
            pattern_node=pattern_node->NextSibling("Pattern"))
    {
        int velocity=atoi(pattern_node->ToElement()->Attribute("velocity"));
        int angle=atoi(pattern_node->ToElement()->Attribute("angle"));
        int offset_x=atoi(pattern_node->ToElement()->Attribute("offset_x"));
        int offset_y=atoi(pattern_node->ToElement()->Attribute("offset_y"));
        int startup=atoi(pattern_node->ToElement()->Attribute("startup"));
        int cooldown=atoi(pattern_node->ToElement()->Attribute("cooldown"));
        int animation_velocity=atoi(pattern_node->ToElement()->Attribute("animation_velocity"));
        std::string bullet_name=pattern_node->ToElement()->Attribute("bullet");
        Bullet*bullet=bullets[bullet_name];

        patterns.push_back(new Pattern(sonido,painter,receiver,velocity,angle,animation_velocity,bullet,offset_x,offset_y,startup,cooldown));
    }

    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;
}

void Character::logic(int stage_velocity)
{
    animationControl();
    inputControl();
    spellControl(stage_velocity);
}

void Character::animationControl()
{
    if(animation_iteration>=animation_velocity)
    {
        current_sprite++;
        if(current_sprite>=sprites[orientation].size())
        {
            current_sprite=0;
        }
        animation_iteration=0;
    }
    animation_iteration++;
}

void Character::inputControl()
{
    if(receiver->IsKeyDownn(SDLK_DOWN))
        orientation="down";
    else if(receiver->IsKeyDownn(SDLK_UP))
        orientation="up";
    else if(receiver->IsKeyDownn(SDLK_LEFT))
        orientation="left";
    else if(receiver->IsKeyDownn(SDLK_RIGHT))
        orientation="right";
    else
        orientation="idle";

    if(receiver->IsKeyDownn(SDLK_DOWN))
        this->y+=velocity;
    if(receiver->IsKeyDownn(SDLK_UP))
        this->y-=velocity;
    if(receiver->IsKeyDownn(SDLK_LEFT))
        this->x-=velocity;
    if(receiver->IsKeyDownn(SDLK_RIGHT))
        this->x+=velocity;

    if(receiver->IsKeyDownn(SDLK_z))
        this->shooting=true;
    else
        this->shooting=false;
}

void Character::spellControl(int stage_velocity)
{
    if(shooting)
    {
        for(int i=0;i<patterns.size();i++)
        {
            patterns[i]->updateState();
            if(patterns[i]->isReady())
                active_patterns->push_back(new Pattern(patterns[i],this->x,this->y));
        }
    }

    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
        ((Pattern*)*pattern)->logic(stage_velocity);
}

void Character::render()
{
    painter->draw2DImage
    (   sprites[orientation][current_sprite],
        sprites[orientation][current_sprite]->getWidth(),sprites[orientation][current_sprite]->getHeight(),
        this->x-sprites[orientation][current_sprite]->getWidth()/2,this->y-sprites[orientation][current_sprite]->getHeight()/2,
        1.0,
        0.0,
        false,
        0,0,
        Color(255,255,255,255),
        true);

    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
        ((Pattern*)*pattern)->render();
}

int Character::getX()
{
    return this->x;
}

int Character::getY()
{
    return this->y;
}

void Character::setX(int x)
{
    this->x=x;
}

void Character::setY(int y)
{
    this->y=y;
}

std::list<Pattern*>* Character::getActivePatterns()
{
    return active_patterns;
}
