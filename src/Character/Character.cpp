#include "../include/Character/Character.h"

Character::Character(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory)
{
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

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;

    loadFromXML(directory);
}

void Character::loadFromXML(std::string directory)
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
    this->animation_velocity=atoi(attributes->Attribute("animation_velocity"));
    this->max_hp=atoi(attributes->Attribute("hp"));
    this->x=atoi(attributes->Attribute("initial_x"));
    this->y=atoi(attributes->Attribute("initial_y"));
    this->hp=this->max_hp;

    TiXmlElement *hitbox_node=main_file->FirstChild("Hitbox")->ToElement();
    int hitbox_x=atoi(hitbox_node->Attribute("x"));
    int hitbox_y=atoi(hitbox_node->Attribute("y"));
    int hitbox_width=atoi(hitbox_node->Attribute("width"));
    int hitbox_height=atoi(hitbox_node->Attribute("height"));
    int hitbox_angle=atoi(hitbox_node->Attribute("angle"));
    this->hitbox.setValues(hitbox_x,hitbox_y,hitbox_width,hitbox_height,hitbox_angle);

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
        int damage=atoi(bullet_node->ToElement()->Attribute("damage"));
        vector<Image*>sprites_temp;
        for(TiXmlNode* sprite_node=bullet_node->FirstChild("Sprite");
                sprite_node!=NULL;
                sprite_node=sprite_node->NextSibling("Sprite"))
        {
            sprites_temp.push_back(painter->getTexture(directory+"sprites/"+sprite_node->ToElement()->Attribute("path")));
        }

        TiXmlNode* hitbox_node=bullet_node->FirstChild("Hitbox");
        int x=atoi(hitbox_node->ToElement()->Attribute("x"));
        int y=atoi(hitbox_node->ToElement()->Attribute("y"));
        int width=atoi(hitbox_node->ToElement()->Attribute("width"));
        int height=atoi(hitbox_node->ToElement()->Attribute("height"));
        int angle=atoi(hitbox_node->ToElement()->Attribute("angle"));
        bullets[node_name]=new Bullet(sonido,painter,receiver,sprites_temp,Hitbox(x,y,width,height,angle),damage);
    }

    //Loading file
    std::string pattern_path=directory+"patterns.xml";
    TiXmlDocument doc_pattern_t(pattern_path.c_str());
    doc_pattern_t.LoadFile();
    TiXmlDocument *doc_pattern;
    doc_pattern=&doc_pattern_t;
    TiXmlNode *patterns_file=doc_pattern->FirstChild("PatternsFile");

    //Loading patterns
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

            TiXmlNode* hitbox_node=pattern_node->FirstChild("Hitbox");
            Hitbox* pattern_hitbox;
            if(hitbox_node)
            {
                int hitbox_x=atoi(hitbox_node->ToElement()->Attribute("x"));
                int hitbox_y=atoi(hitbox_node->ToElement()->Attribute("y"));
                int hitbox_width=atoi(hitbox_node->ToElement()->Attribute("width"));
                int hitbox_height=atoi(hitbox_node->ToElement()->Attribute("height"));
                float hitbox_angle=atoi(hitbox_node->ToElement()->Attribute("angle"));
                pattern_hitbox=new Hitbox(hitbox_x,hitbox_y,hitbox_width,hitbox_height,hitbox_angle);
            }else
            {
                pattern_hitbox=new Hitbox(-40,-40,80,80,0);
            }

            patterns.push_back(new Pattern(sonido,painter,receiver,velocity,max_velocity,acceleration,a_frequency,angle,angle_change,stop_ac_at,ac_frequency,animation_velocity,bullet,offset_x,offset_y,startup,cooldown,duration));
        }
        type[type_name]=patterns;
    }
    iteration=0;
}

void Character::logic(int stage_velocity)
{
    animationControl();
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

void Character::spellControl(int stage_velocity)
{
    std::vector<Pattern*> patterns=type[current_type];
    for(int i=0;i<patterns.size();i++)
    {
        if(shooting && this->hp!=0)
        {
            patterns[i]->updateStateShouting();
            if(patterns[i]->isReady())
            {
                active_patterns->push_back(new Pattern(patterns[i],this->x,this->y));
            }
        }else
        {
            patterns[i]->updateStateNotShouting();
        }
    }

    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
        ((Pattern*)*pattern)->logic(stage_velocity);
}

void Character::parrentRender()
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

    if(receiver->IsKeyDownn(SDLK_h))
    {
        painter->drawRectangle(this->getHitbox().getX(),
                               this->getHitbox().getY(),
                               hitbox.getWidth(),hitbox.getHeight(),
                               hitbox.getAngle(),100,0,0,100,true);
    }

    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
        ((Pattern*)*pattern)->render();
}

void Character::render()
{
    parrentRender();
}

int Character::getX()
{
    return this->x;
}

int Character::getY()
{
    return this->y;
}

int Character::getHP()
{
    return this->hp;
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

void Character::setType(std::string type)
{
    this->current_type=type;
}

bool Character::collides(Hitbox hitbox,int hitbox_x,int hitbox_y,float hitbox_angle)
{
    return this->hitbox.getPlacedHitbox(Point(this->x,this->y),0).collides(hitbox);
}

void Character::hit(int damage)
{
    this->hp-=damage;
    if(hp<0)
        hp=0;
}

Hitbox Character::getHitbox()
{
    return hitbox.getPlacedHitbox(Point(this->x,this->y),0);
}
