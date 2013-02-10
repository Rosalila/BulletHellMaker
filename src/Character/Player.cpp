#include "../include/Character/Player.h"

Player::Player(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory)
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
    this->current_type="1";




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

    for(TiXmlNode* pattern_type=patterns_file->FirstChild("Type");
            pattern_type!=NULL;
            pattern_type=pattern_type->NextSibling("Type"))
    {
        std::string type_name=pattern_type->ToElement()->Attribute("name");
        std::vector<Pattern*> patterns;
        for(TiXmlNode* pattern_node=pattern_type->FirstChild("Pattern");
                pattern_node!=NULL;
                pattern_node=pattern_node->NextSibling("Pattern"))
        {
            int velocity=atoi(pattern_node->ToElement()->Attribute("velocity"));
            int max_velocity=9999999;
            if(pattern_node->ToElement()->Attribute("max_velocity")!=NULL)
                max_velocity=atoi(pattern_node->ToElement()->Attribute("max_velocity"));
            int acceleration=0;
            if(pattern_node->ToElement()->Attribute("acceleration")!=NULL)
                acceleration=atoi(pattern_node->ToElement()->Attribute("acceleration"));
            int a_frequency=1;
            if(pattern_node->ToElement()->Attribute("a_frequency")!=NULL)
                a_frequency=atoi(pattern_node->ToElement()->Attribute("a_frequency"));
            int angle=atoi(pattern_node->ToElement()->Attribute("angle"));
            int angle_change=0;
            if(pattern_node->ToElement()->Attribute("angle_change")!=NULL)
                angle_change=atoi(pattern_node->ToElement()->Attribute("angle_change"));
            int stop_ac_at=-1;
            if(pattern_node->ToElement()->Attribute("stop_ac_at")!=NULL)
                stop_ac_at=atoi(pattern_node->ToElement()->Attribute("stop_ac_at"));
            int ac_frequency=1;
            if(pattern_node->ToElement()->Attribute("ac_frequency")!=NULL)
                ac_frequency=atoi(pattern_node->ToElement()->Attribute("ac_frequency"));
            int offset_x=atoi(pattern_node->ToElement()->Attribute("offset_x"));
            int offset_y=atoi(pattern_node->ToElement()->Attribute("offset_y"));
            int startup=atoi(pattern_node->ToElement()->Attribute("startup"));
            int cooldown=atoi(pattern_node->ToElement()->Attribute("cooldown"));
            int animation_velocity=atoi(pattern_node->ToElement()->Attribute("animation_velocity"));
            int duration=-1;
            if(pattern_node->ToElement()->Attribute("duration"))
                duration=atoi(pattern_node->ToElement()->Attribute("duration"));
            std::string bullet_name=pattern_node->ToElement()->Attribute("bullet");
            Bullet*bullet=bullets[bullet_name];

            patterns.push_back(new Pattern(sonido,painter,receiver,velocity,max_velocity,acceleration,a_frequency,angle,angle_change,stop_ac_at,ac_frequency,animation_velocity,bullet,offset_x,offset_y,startup,cooldown,duration,new Hitbox(0,0,40,10,0)));
        }
        type[type_name]=patterns;
    }

    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;
}

void Player::inputControl()
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
