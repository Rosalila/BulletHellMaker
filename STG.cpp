#include "STG.h"

STG::STG(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,Player*player,Enemy*enemy,Stage*stage,string game_mode)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->player=player;
    this->enemy=enemy;
    this->stage=stage;
    this->game_mode=game_mode;

    painter->camera_y=0;
    iteration=0;
    boss_fury_level=0;

    //XML Initializations
    string config_directory = assets_directory+"config.xml";
    TiXmlDocument doc_t( (char*)config_directory.c_str() );
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *config_file=doc->FirstChild("ConfigFile");

    TiXmlNode *user_node=config_file->FirstChild("User");
    username=user_node->ToElement()->Attribute("name");

    TiXmlNode *you_loose_node=config_file->FirstChild("YouLose");

    int you_loose_x=atoi(you_loose_node->ToElement()->Attribute("x"));
    int you_loose_y=atoi(you_loose_node->ToElement()->Attribute("y"));
    int you_loose_animation_velocity=atoi(you_loose_node->ToElement()->Attribute("animation_velocity"));
    you_loose=Animation(you_loose_x,you_loose_y,you_loose_animation_velocity,painter);

    if(you_loose_node->ToElement()->Attribute("sound"))
    {
        sonido->addSound("you lose",assets_directory+you_loose_node->ToElement()->Attribute("sound"));
    }

    for(TiXmlNode* sprites_node=you_loose_node->FirstChild("sprite");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("sprite"))
    {
        std::string path=sprites_node->ToElement()->Attribute("path");
        you_loose.addImage(painter->getTexture(assets_directory+path));
    }


    TiXmlNode *you_win_node=config_file->FirstChild("YouWin");

    int you_win_x=atoi(you_win_node->ToElement()->Attribute("x"));
    int you_win_y=atoi(you_win_node->ToElement()->Attribute("y"));
    int you_win_animation_velocity=atoi(you_win_node->ToElement()->Attribute("animation_velocity"));
    you_win=Animation(you_win_x,you_win_y,you_win_animation_velocity,painter);

    if(you_win_node->ToElement()->Attribute("sound"))
    {
        sonido->addSound("you win",assets_directory+you_win_node->ToElement()->Attribute("sound"));
    }

    for(TiXmlNode* sprites_node=you_win_node->FirstChild("sprite");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("sprite"))
    {
        std::string path=sprites_node->ToElement()->Attribute("path");
        you_win.addImage(painter->getTexture(assets_directory+path));
    }

    if(game_mode=="Stage select")
    {
        stageSelectModeInit();
    }

    mainLoop();
}

void STG::stageSelectModeInit()
{
    stage->playMusic();
    painter->camera_x=0;
}

void STG::mainLoop()
{
    bool end_key_up_keyboard=false;
    bool end_key_up_joystick=false;
    string music_path_temp = assets_directory+"stages/"+stage->getName()+"/music.ogg";
    Mix_Music *music=Mix_LoadMUS(music_path_temp.c_str());
    for (;;)
    {
        if(receiver->isKeyDown(SDLK_ESCAPE))
        {
            exit(0);
            break;
        }

        if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(music,0);
        }

        render();
        logic();
        if(player->getHP()==0
           || enemy->getHP()==0)
        {
            if(receiver->isKeyPressed(SDLK_RETURN))
                break;
            if(receiver->isKeyPressed(SDLK_z) && end_key_up_keyboard)
                break;
            if(receiver->isJoyDown(0,0) && end_key_up_joystick)
                break;
            if(!receiver->isKeyPressed(SDLK_z))
                end_key_up_keyboard=true;
            if(!receiver->isJoyPressed(0,0))
                end_key_up_joystick=true;

        }
        receiver->updateInputs();
    }
}

void STG::logic()
{
    float distance_x=enemy->x - player->x;
    float distance_y=enemy->y - player->y;
    float distance=sqrt(distance_x*distance_x+distance_y*distance_y);
    float damage_level=6-distance/250.0;

    for (std::list<Pattern*>::iterator pattern = enemy->getActivePatterns()->begin(); pattern != enemy->getActivePatterns()->end(); pattern++)
    {
        Pattern*p=(Pattern*)*pattern;
        if(!p->isHit())
        {
            for(int i=0;i<(int)p->getBullet()->getHitboxes().size();i++)
            {
                Hitbox h=p->getBullet()->getHitboxes()[i]->getPlacedHitbox(p->getX(),p->getY(),p->getBulletAngle());
                if(p->collides_opponent && player->collides(h,0,0,0))
                {
                    p->hit();
                    if(player->isParrying())
                    {

                        if(receiver->isKeyDown(SDL_SCANCODE_RIGHT)
                           || receiver->isJoyDown(-6,0))
                        {
                            player->setX(player->getX()+150);
                        }
                        if(receiver->isKeyDown(SDL_SCANCODE_LEFT)
                           || receiver->isJoyDown(-4,0))
                        {
                            player->setX(player->getX()-150);
                        }
                        if(receiver->isKeyDown(SDL_SCANCODE_UP)
                           || receiver->isJoyDown(-2,0))
                        {
                            player->setY(player->getY()-150);
                        }
                        if(receiver->isKeyDown(SDL_SCANCODE_DOWN)
                           || receiver->isJoyDown(-8,0))
                        {
                            player->setY(player->getY()+150);
                        }

                        p->velocity=10;
                        if(p->x>player->getX())
                        {
                            p->angle=135;
                        }else
                        {
                            p->angle=-135;
                        }
                    }else
                    {
                        player->hit(p->getDamage());
                        painter->shakeScreen(30,10);
                        if(this->sonido->soundExists(player->getName()+".hit"))
                            this->sonido->playSound(player->getName()+".hit");
                        if(player->getHP()==0)
                        {
                            sonido->playSound("you lose");
                        }
                    }
                }
            }
        }
    }

    for (std::list<Pattern*>::iterator pattern = player->getActivePatterns()->begin(); pattern != player->getActivePatterns()->end(); pattern++)
    {
        Pattern*p=(Pattern*)*pattern;
        if(!p->isHit())
        {
            for(int i=0;i<(int)p->getBullet()->getHitboxes().size();i++)
            {
                Hitbox h=p->getBullet()->getHitboxes()[i]->getPlacedHitbox(p->getX(),p->getY(),p->getBulletAngle());
                if(p->collides_opponent && enemy->collides(h,0,0,0))
                {
                    p->hit();
                    enemy->hit(p->getDamage()+damage_level);
                    enemy->shakeScreen(p->getDamage()+damage_level*3,p->getDamage()+damage_level*2);
                    if(this->sonido->soundExists(enemy->getName()+".hit"))
                        this->sonido->playSound(enemy->getName()+".hit");
                    if(enemy->getHP()==0)
                    {
                        painter->shakeScreen(50,20);
                        sonido->playSound("you win");
                        enemy->deleteActivePatterns();
                    }
                }
            }
        }
    }

    //BulletxBullet Collision
    for (std::list<Pattern*>::iterator enemy_pattern_iterator = enemy->getActivePatterns()->begin(); enemy_pattern_iterator != enemy->getActivePatterns()->end(); enemy_pattern_iterator++)
    {
        Pattern*enemy_pattern=(Pattern*)*enemy_pattern_iterator;
        if(!enemy_pattern->isHit())
        {
            for (std::list<Pattern*>::iterator player_pattern_iterator = player->getActivePatterns()->begin(); player_pattern_iterator != player->getActivePatterns()->end(); player_pattern_iterator++)
            {
                Pattern*player_pattern=(Pattern*)*player_pattern_iterator;
                if(player_pattern->collides_bullets||enemy_pattern->collides_bullets)
                {
                    if(!player_pattern->isHit())
                    {
                        vector<Hitbox*>enemy_hitboxes=enemy_pattern->getBullet()->getHitboxes();
                        for(int i=0;i<(int)enemy_hitboxes.size();i++)
                        {
                            Hitbox enemy_hitbox=enemy_hitboxes[i]->getPlacedHitbox(enemy_pattern->getX(),enemy_pattern->getY(),enemy_pattern->getBulletAngle());
                            vector<Hitbox*>player_hitboxes=player_pattern->getBullet()->getHitboxes();
                            for(int j=0;j<(int)player_hitboxes.size();j++)
                            {
                                Hitbox player_hitbox=player_hitboxes[j]->getPlacedHitbox(player_pattern->getX(),player_pattern->getY(),player_pattern->getBulletAngle());
                                if(enemy_hitbox.collides(player_hitbox))
                                {
                                    enemy_pattern->hit();
                                    player_pattern->hit();
                                }
                            }

                        }
                    }
                }
            }
        }
    }

//    if(receiver->isKeyPressed(SDLK_t))
//    {
//        //boss_fury_level+=1;
//        //player->deleteActivePatterns();
//        //enemy->deleteActivePatterns();
//    }
//    if(receiver->isKeyPressed(SDLK_1))
//    {
//        player->setType("1");
//    }

    int stage_displacement = stage->getVelocity();
    if(isSlowActive())
        stage_displacement/=3;
    painter->camera_x+=stage_displacement;
    player->logic(stage_displacement);
    player->setX(player->getX()+stage_displacement);
    enemy->logic(stage_displacement,stage->getName(),iteration,username);
    for(int i=0;i<boss_fury_level;i++)
    {
        iteration++;
        enemy->logic(0,stage->getName(),iteration,username);
    }
    //enemy->setX(enemy->getX()+stage_displacement);
    stage->logic();

    deletePatterns();
    checkCharacterOutOfBounds();
    slowExtraControl();

    if(enemy->getHP()>0)
        iteration++;
}

void STG::render()
{
    stage->dibujarBack();
    player->bottomRender();
    enemy->bottomRender();
    player->topRender();
    enemy->topRender();

    stage->render();
    stage->dibujarFront();

    if(enemy->getHP()==0)
        you_win.render();
    if(player->getHP()==0)
        you_loose.render();

//    painter->drawText("Time: "+toString(iteration),25,70);
//    painter->drawText(enemy->getName(),25,110);
//    painter->drawText("Damage level: "+toString(damage_level),25,170);


    painter->updateScreen();
}

bool STG::isOutOfBounds(int pos_x,int pos_y)
{
    int bullet_bound_addition_x = (stage->getBoundX2()-stage->getBoundX1())/2;
    int bullet_bound_addition_y = (stage->getBoundY2()-stage->getBoundY1())/2;
    if(pos_x<stage->getBoundX1()+painter->camera_x-bullet_bound_addition_x
       ||pos_x>stage->getBoundX2()+painter->camera_x+bullet_bound_addition_x
       ||pos_y<stage->getBoundY1()-bullet_bound_addition_y
       ||pos_y>stage->getBoundY2()+bullet_bound_addition_y
       )
    {
        return true;
    }
    return false;
}

void STG::deletePatterns()
{
    std::list<Pattern*>* active_patterns=player->getActivePatterns();
    std::list<Pattern*>::iterator i = active_patterns->begin();
    while (i != active_patterns->end())
    {
        Pattern*p=(Pattern*)*i;
        if (isOutOfBounds(p->getX(),p->getY()) || p->destroyFlag())
        {
            active_patterns->erase(i++);
            delete p;
        }
        else
        {
            ++i;
        }
    }


    active_patterns=enemy->getActivePatterns();
    i = active_patterns->begin();
    while (i != active_patterns->end())
    {
        Pattern*p=(Pattern*)*i;
        if (isOutOfBounds(p->getX(),p->getY()) || p->destroyFlag())
        {
            active_patterns->erase(i++);
            delete p;
        }
        else
        {
            ++i;
        }
    }
}

void STG::checkCharacterOutOfBounds()
{
    if(player->getX()<stage->getBoundX1()+painter->camera_x)
        player->setX(stage->getBoundX1()+painter->camera_x);
    if(player->getX()>stage->getBoundX2()+painter->camera_x)
        player->setX(stage->getBoundX2()+painter->camera_x);
    if(player->getY()<stage->getBoundY1())
        player->setY(stage->getBoundY1());
    if(player->getY()>stage->getBoundY2())
        player->setY(stage->getBoundY2());
}

bool STG::playerWon()
{
    return enemy->getHP()==0;
}

bool STG::enemyWon()
{
    return player->getHP()==0;
}
