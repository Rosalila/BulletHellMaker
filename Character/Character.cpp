#include "Character.h"

Character::Character(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name)
{
    //Setting up the other variables
    this->name=name;
    this->directory="";
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->active_patterns=new std::list<Pattern*>;
    this->x=500;
    this->y=500;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="1";
    this->visible=true;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;

    this->life_bar_x=0;
    this->life_bar_y=0;

    this->iteration=0;

    loadFromXML();
}

void Character::loadFromXML()
{
    loadMainXML();

    loadBulletsXML();

    loadPatternsXML();
}

void Character::loadMainXML()
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
    this->velocity=5;
    if(attributes->Attribute("velocity")!=NULL)
    {
        this->velocity=atoi(attributes->Attribute("velocity"));
    }

    this->animation_velocity=5;
    if(attributes->Attribute("animation_velocity")!=NULL)
    {
        this->animation_velocity=atoi(attributes->Attribute("animation_velocity"));
    }

    this->max_hp=100;
    this->hp=100;
    if(attributes->Attribute("hp")!=NULL)
    {
        this->max_hp=atoi(attributes->Attribute("hp"));
    }
    this->hp=this->max_hp;

    this->x=100;
    if(attributes->Attribute("initial_x")!=NULL)
    {
        this->x=atoi(attributes->Attribute("initial_x"));
    }

    this->y=500;
    if(attributes->Attribute("initial_y")!=NULL)
    {
        this->y=atoi(attributes->Attribute("initial_y"));
    }

    this->life_bar_x=0;
    this->life_bar_y=0;
    this->life_bar_rect_offset_x=0;
    this->life_bar_rect_offset_y=0;
    this->life_bar_rect_height=0;
    this->life_bar_rect_width=0;
    this->color.red=0;
    this->color.green=0;
    this->color.blue=0;
    this->color.alpha=255;

    if(main_file->FirstChild("LifeBar")!=NULL)
    {
        TiXmlElement *life_bar=main_file->FirstChild("LifeBar")->ToElement();
        if(life_bar->Attribute("x")!=NULL)
            this->life_bar_x=atoi(life_bar->Attribute("x"));
        if(life_bar->Attribute("y")!=NULL)
            this->life_bar_y=atoi(life_bar->Attribute("y"));
        if(life_bar->Attribute("color_r")!=NULL)
            this->color.red=atoi(life_bar->Attribute("color_r"));
        if(life_bar->Attribute("color_g")!=NULL)
            this->color.green=atoi(life_bar->Attribute("color_g"));
        if(life_bar->Attribute("color_b")!=NULL)
            this->color.blue=atoi(life_bar->Attribute("color_b"));
        if(life_bar->Attribute("color_a")!=NULL)
            this->color.alpha=atoi(life_bar->Attribute("color_a"));
        if(life_bar->Attribute("rect_offset_x")!=NULL)
            this->life_bar_rect_offset_x=atoi(life_bar->Attribute("rect_offset_x"));
        if(life_bar->Attribute("rect_offset_y")!=NULL)
            this->life_bar_rect_offset_y=atoi(life_bar->Attribute("rect_offset_y"));
        if(life_bar->Attribute("rect_height")!=NULL)
            this->life_bar_rect_height=atoi(life_bar->Attribute("rect_height"));
        if(life_bar->Attribute("rect_width")!=NULL)
            this->life_bar_rect_width=atoi(life_bar->Attribute("rect_width"));
    }

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
        if(sprites_node->ToElement()->Attribute("sound")!=NULL)
        {
            std::string sprites_sound=sprites_node->ToElement()->Attribute("sound");
            this->sonido->addSound(name+"."+sprites_orientation,directory+"sounds/"+sprites_sound);
        }
        for(TiXmlNode* sprite_node=sprites_node->FirstChild("Sprite");
                sprite_node!=NULL;
                sprite_node=sprite_node->NextSibling("Sprite"))
        {
            sprites_vector.push_back(painter->getTexture(directory+"sprites/"+sprite_node->ToElement()->Attribute("path")));
        }
        sprites[sprites_orientation]=sprites_vector;
    }
}

void Character::loadBulletsXML()
{
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
        if(bullet_node->ToElement()->Attribute("sound")!=NULL)
        {
            std::string sound=directory+"sounds/"+bullet_node->ToElement()->Attribute("sound");
            sonido->addSound("bullet."+node_name,sound);
        }
        if(bullet_node->ToElement()->Attribute("sound_hit")!=NULL)
        {
            std::string sound_hit=directory+"sounds/"+bullet_node->ToElement()->Attribute("sound_hit");
            sonido->addSound("bullet_hit."+node_name,sound_hit);
        }

        int damage=0;
        if(bullet_node->ToElement()->Attribute("damage")!=NULL)
        {
            damage=atoi(bullet_node->ToElement()->Attribute("damage"));
        }

        vector<Image*>sprites_temp;
        for(TiXmlNode* sprite_node=bullet_node->FirstChild("Sprite");
                sprite_node!=NULL;
                sprite_node=sprite_node->NextSibling("Sprite"))
        {
            sprites_temp.push_back(painter->getTexture(directory+"sprites/"+sprite_node->ToElement()->Attribute("path")));
        }

        vector<Hitbox*>hitboxes_temp;
        for(TiXmlNode* hitbox_node=bullet_node->FirstChild("Hitbox");
                hitbox_node!=NULL;
                hitbox_node=hitbox_node->NextSibling("Hitbox"))
        {
            int x=0;
            if(hitbox_node->ToElement()->Attribute("x")!=NULL)
            {
                x=atoi(hitbox_node->ToElement()->Attribute("x"));
            }
            int y=0;
            if(hitbox_node->ToElement()->Attribute("y")!=NULL)
            {
                y=atoi(hitbox_node->ToElement()->Attribute("y"));
            }

            int width=atoi(hitbox_node->ToElement()->Attribute("width"));
            int height=atoi(hitbox_node->ToElement()->Attribute("height"));

            int angle=0;
            if(hitbox_node->ToElement()->Attribute("angle")!=NULL)
            {
                angle=atoi(hitbox_node->ToElement()->Attribute("angle"));
            }

            hitboxes_temp.push_back(new Hitbox(x,y,width,height,angle));
        }

        TiXmlNode* onhit_node=bullet_node->FirstChild("OnHit");
        vector<Image*>sprites_onhit_temp;
        if(onhit_node!=NULL)
        {
            for(TiXmlNode* sprite_node=onhit_node->FirstChild("Sprite");
                    sprite_node!=NULL;
                    sprite_node=sprite_node->NextSibling("Sprite"))
            {
                sprites_onhit_temp.push_back(painter->getTexture(directory+"sprites/"+sprite_node->ToElement()->Attribute("path")));
            }
        }

        bullets[node_name]=new Bullet(sonido,painter,receiver,node_name,sprites_temp,sprites_onhit_temp,hitboxes_temp,damage);
    }
}

void Character::loadPatternsXML()
{
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
            std::string bullet_name=pattern_node->ToElement()->Attribute("bullet");
            Bullet*bullet=bullets[bullet_name];

            int velocity=0;
            if(pattern_node->ToElement()->Attribute("velocity")!=NULL)
                velocity=atoi(pattern_node->ToElement()->Attribute("velocity"));

            int max_velocity=9999999;
            if(pattern_node->ToElement()->Attribute("max_velocity")!=NULL)
                max_velocity=atoi(pattern_node->ToElement()->Attribute("max_velocity"));

            int acceleration=0;
            if(pattern_node->ToElement()->Attribute("acceleration")!=NULL)
                acceleration=atoi(pattern_node->ToElement()->Attribute("acceleration"));

            int a_frequency=0;
            if(pattern_node->ToElement()->Attribute("a_frequency")!=NULL)
                a_frequency=atoi(pattern_node->ToElement()->Attribute("a_frequency"));

            int angle=0;
            if(pattern_node->ToElement()->Attribute("angle")!=NULL)
                angle=atoi(pattern_node->ToElement()->Attribute("angle"));

            int angle_change=0;
            if(pattern_node->ToElement()->Attribute("angle_change")!=NULL)
                angle_change=atoi(pattern_node->ToElement()->Attribute("angle_change"));

            int stop_ac_at=-1;
            if(pattern_node->ToElement()->Attribute("stop_ac_at")!=NULL)
                stop_ac_at=atoi(pattern_node->ToElement()->Attribute("stop_ac_at"));

            int ac_frequency=0;
            if(pattern_node->ToElement()->Attribute("ac_frequency")!=NULL)
                ac_frequency=atoi(pattern_node->ToElement()->Attribute("ac_frequency"));

            int offset_x=0;
            if(pattern_node->ToElement()->Attribute("offset_x")!=NULL)
                offset_x=atoi(pattern_node->ToElement()->Attribute("offset_x"));

            int offset_y=0;
            if(pattern_node->ToElement()->Attribute("offset_y")!=NULL)
                offset_y=atoi(pattern_node->ToElement()->Attribute("offset_y"));

            int startup=0;
            if(pattern_node->ToElement()->Attribute("startup")!=NULL)
                startup=atoi(pattern_node->ToElement()->Attribute("startup"));

            int cooldown=0;
            if(pattern_node->ToElement()->Attribute("cooldown")!=NULL)
                cooldown=atoi(pattern_node->ToElement()->Attribute("cooldown"));

            int animation_velocity=0;
            if(pattern_node->ToElement()->Attribute("animation_velocity")!=NULL)
                animation_velocity=atoi(pattern_node->ToElement()->Attribute("animation_velocity"));

            int duration=-1;
            if(pattern_node->ToElement()->Attribute("duration"))
                duration=atoi(pattern_node->ToElement()->Attribute("duration"));

            int random_angle=0;
            if(pattern_node->ToElement()->Attribute("random_angle"))
                random_angle=atoi(pattern_node->ToElement()->Attribute("random_angle"));

            bool aim_player=false;
            if(pattern_node->ToElement()->Attribute("aim_player"))
                aim_player=strcmp(pattern_node->ToElement()->Attribute("aim_player"),"yes")==0;

            bool freeze=false;
            if(pattern_node->ToElement()->Attribute("freeze"))
                freeze=strcmp(pattern_node->ToElement()->Attribute("freeze"),"yes")==0;

            bool homing = false;
            if (pattern_node->ToElement()->Attribute("homing"))
                 homing = strcmp(pattern_node->ToElement()->Attribute("homing"), "yes") == 0;

            //Modifiers
            std::map<int, vector<Modifier*>* >*pattern_modifiers=new std::map<int, vector<Modifier*>* >();

            if(pattern_node->FirstChild("Modifier")!=NULL)
            {
                for(TiXmlNode* pattern_modifier_node=pattern_node->FirstChild("Modifier");
                        pattern_modifier_node!=NULL;
                        pattern_modifier_node=pattern_modifier_node->NextSibling("Modifier"))
                {
                    vector<Modifier*>* temp_modifiers=new vector<Modifier*>();

                    int at=atoi(pattern_modifier_node->ToElement()->Attribute("at"));

                    if(pattern_modifier_node->ToElement()->Attribute("bullet")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("bullet");
                        temp_modifiers->push_back(new Modifier("bullet",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("velocity")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("velocity");
                        temp_modifiers->push_back(new Modifier("velocity",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("max_velocity")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("max_velocity");
                        temp_modifiers->push_back(new Modifier("max_velocity",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("acceleration")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("acceleration");
                        temp_modifiers->push_back(new Modifier("acceleration",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("a_frequency")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("a_frequency");
                        temp_modifiers->push_back(new Modifier("a_frequency",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("angle")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("angle");
                        temp_modifiers->push_back(new Modifier("angle",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("angle_change")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("angle_change");
                        temp_modifiers->push_back(new Modifier("angle_change",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("stop_ac_at")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("stop_ac_at");
                        temp_modifiers->push_back(new Modifier("stop_ac_at",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("ac_frequency")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("ac_frequency");
                        temp_modifiers->push_back(new Modifier("ac_frequency",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("animation_velocity")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("animation_velocity");
                        temp_modifiers->push_back(new Modifier("animation_velocity",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("offset_x")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("offset_x");
                        temp_modifiers->push_back(new Modifier("offset_x",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("offset_y")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("offset_y");
                        temp_modifiers->push_back(new Modifier("offset_y",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("startup")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("startup");
                        temp_modifiers->push_back(new Modifier("startup",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("cooldown")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("cooldown");
                        temp_modifiers->push_back(new Modifier("cooldown",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("duration")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("duration");
                        temp_modifiers->push_back(new Modifier("duration",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("random_angle")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("random_angle");
                        temp_modifiers->push_back(new Modifier("random_angle",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("aim_player")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("aim_player");
                        temp_modifiers->push_back(new Modifier("aim_player",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("freeze")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("freeze");
                        temp_modifiers->push_back(new Modifier("freeze",value));
                    }

                    if(pattern_modifier_node->ToElement()->Attribute("homing")!=NULL)
                    {
                        std::string value=pattern_modifier_node->ToElement()->Attribute("homing");
                        temp_modifiers->push_back(new Modifier("homing",value));
                    }

                    (*pattern_modifiers)[at]=temp_modifiers;
                }
            }

            //Pattern ready, now push
            patterns.push_back(new Pattern(sonido,painter,receiver,velocity,max_velocity,acceleration,a_frequency,angle,angle_change,stop_ac_at,ac_frequency,animation_velocity,bullet,offset_x,offset_y,
                                           startup,cooldown,duration,random_angle,aim_player,freeze, homing,pattern_modifiers,&bullets));
        }
        type[type_name]=patterns;
    }
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
        if(current_sprite>=(int)sprites[orientation].size())
        {
            current_sprite=0;
            if(orientation=="destroyed")
                visible=false;
        }
        animation_iteration=0;
    }
    if(getIterateSlowdownFlag())
        animation_iteration++;
}

void Character::spellControl(int stage_velocity)
{
    std::vector<Pattern*> patterns=type[current_type];
    for(int i=0;i<(int)patterns.size();i++)
    {
        if(shooting && this->hp!=0)
        {
            patterns[i]->updateStateShouting();
            if(patterns[i]->isReady())
            {
                patterns[i]->getBullet()->playSound();
                this->addActivePattern(patterns[i]);
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
    if(!visible)
        return;
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

int Character::getIteration()
{
    return iteration;
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
    return this->hitbox.getPlacedHitbox(Point(this->x,this->y),0).collides(hitbox,0,0,0);
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


void Character::addActivePattern(Pattern* pattern)
{
    Pattern* pattern_temp=new Pattern(pattern,this->x,this->y);
    float angle=pattern_temp->getAngle();
    angle+=pattern_temp->getRandomAngle();
    pattern_temp->setAngle(angle);

    active_patterns->push_back(pattern_temp);
}
