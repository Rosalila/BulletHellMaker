#include "STG.h"

STG::STG(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,Player*player,Enemy*enemy,Stage*stage,string game_mode,map<string,Button*>controls)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->player=player;
    this->enemy=enemy;
    this->stage=stage;
    this->game_mode=game_mode;
    this->controls=controls;

    painter->camera_y=0;
    iteration=0;
    boss_fury_level=0;

    parry_count=0;
    charge_destroy_count=0;
    parry_dash_count=0;

    image_training_text=NULL;
    image_training_text_final=NULL;
    current_training_transparency=0;
    current_training_final_transparency=0;

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

    if(game_mode=="Stage select" || game_mode=="charge training" || game_mode=="parry training" || game_mode=="parry dash training")
    {
        stageSelectModeInit();
    }

    if(game_mode=="charge training")
    {
        image_training_bar=painter->getTexture(assets_directory+"misc/training/bar.png");
        image_training_bar_fill=painter->getTexture(assets_directory+"misc/training/bar_fill.png");
        charge_destroy_count_objective=300;
    }
    if(game_mode=="parry training")
    {
        image_training_box=painter->getTexture(assets_directory+"misc/training/box.png");
        image_training_x=painter->getTexture(assets_directory+"misc/training/x.png");
        parry_count_objective=3;
    }
    if(game_mode=="parry dash training")
    {
        image_training_bar=painter->getTexture(assets_directory+"misc/training/bar.png");
        image_training_bar_fill=painter->getTexture(assets_directory+"misc/training/bar_fill.png");
        parry_dash_count_objective=15;
    }

    if(stage->getName()=="Training1"
       ||stage->getName()=="Training2"
       ||stage->getName()=="Training3"
       ||stage->getName()=="Training4"
       ||stage->getName()=="Training5"
       )
    {
        image_training_text=painter->getTexture(assets_directory+"misc/training/"+stage->getName()+".png");
        image_training_text_final=painter->getTexture(assets_directory+"misc/training/"+stage->getName()+"_final.png");
    }


    setGameOver(false);
    mainLoop();
}

void STG::stageSelectModeInit()
{
    //stage->playMusic();
    painter->camera_x=0;
    current_training_transparency=0;
}

void STG::mainLoop()
{
    bool end_key_up_keyboard=false;
    string music_path_temp = assets_directory+"stages/"+stage->getName()+"/music.ogg";
    Mix_Music *music=Mix_LoadMUS(music_path_temp.c_str());

    for (;;)
    {
        if(receiver->isKeyPressed(SDLK_ESCAPE))
        {
            player->exit();
            sonido->playSound(std::string("Menu.back"),1,0);
            break;
        }

//        if(receiver->isKeyPressed(SDLK_s))
//        {
//            string random_number = toString((rand()*999999)%1000);
//            painter->screenshot(0,0, 1366, 768, "screenshot"+ random_number +".bmp");
//        }

        if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(music,0);
        }

        render();

        logic();

//        receiver->isKeyDown(SDLK_z)
        receiver->updateInputs();
        if(getGameOver())
        {
            if(receiver->isKeyPressed(SDLK_RETURN)
               || (controls["a"]->isDown() && end_key_up_keyboard)
               )
            {
                sonido->playSound(std::string("Menu.select"),1,0);
                break;
            }
            if(!controls["a"]->isDown())
                end_key_up_keyboard=true;

        }
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
            for(int i=0;i<(int)p->getBullet()->hitboxes.size();i++)
            {
                Hitbox h=p->getBullet()->hitboxes[i]->getPlacedHitbox(p->getX(),p->getY(),p->getBulletAngle());
                if(player->isParrying() || player->isInvulnerable())
                {
                    if(!p->isHit() && p->collides_opponent && (player->collidesParry(h,0,0,0)||player->collides(h,0,0,0)))
                    {
                        p->hit(player->sound_channel_base+1,false);
                        if(player->isInvulnerable())
                        {
                            parry_dash_count++;
                            if(game_mode=="parry dash training" && parry_dash_count==parry_dash_count_objective)
                            {
                                win();
                            }
                        }
                        if(game_mode=="parry training" || game_mode=="parry dash training")
                        {
                            player->parry(true);
                        }else
                        {
                            player->parry(false);
                        }
                        if(player->isParrying())
                        {
                            parry_count++;
                            if(game_mode=="parry training" && parry_count==parry_count_objective)
                            {
                                win();
                            }
                        }
                        if(p->x>player->getX())
                        {
                            p->angle=135;
                        }else
                        {
                            p->angle=-135;
                        }
                    }
                }else if(p->collides_opponent && player->collides(h,0,0,0))
                {
                    p->hit(enemy->sound_channel_base+1,false);
                    player->hit(p->getDamage());
                    parry_count = 0;
                    painter->shakeScreen(30,10);
                    if(this->sonido->soundExists(player->getName()+".hit"))
                        this->sonido->playSound(player->getName()+".hit",3,0);
                    if(player->getHP()==0)
                    {
                        lose();
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
            for(int i=0;i<(int)p->getBullet()->hitboxes.size();i++)
            {
                Hitbox h=p->getBullet()->hitboxes[i]->getPlacedHitbox(p->getX(),p->getY(),p->getBulletAngle());
                if(p->collides_opponent && enemy->collides(h,0,0,0))
                {
                    p->hit(player->sound_channel_base+1,false);
                    enemy->hit(p->getDamage()+damage_level);
                    enemy->shakeScreen(p->getDamage()+damage_level*3,p->getDamage()+damage_level*2);
                    if(this->sonido->soundExists(enemy->getName()+".hit"))
                        this->sonido->playSound(enemy->getName()+".hit",1,0);
                    if(enemy->getHP()==0)
                    {
                        win();
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
                        vector<Hitbox*>enemy_hitboxes=enemy_pattern->getBullet()->hitboxes;
                        for(int i=0;i<(int)enemy_hitboxes.size();i++)
                        {
                            Hitbox enemy_hitbox=enemy_hitboxes[i]->getPlacedHitbox(enemy_pattern->getX(),enemy_pattern->getY(),enemy_pattern->getBulletAngle());
                            vector<Hitbox*>player_hitboxes=player_pattern->getBullet()->hitboxes;
                            for(int j=0;j<(int)player_hitboxes.size();j++)
                            {
                                Hitbox player_hitbox=player_hitboxes[j]->getPlacedHitbox(player_pattern->getX(),player_pattern->getY(),player_pattern->getBulletAngle());
                                if(!enemy_pattern->isHit()&&!player_pattern->isHit()&&enemy_hitbox.collides(player_hitbox))
                                {
                                    charge_destroy_count++;
                                    if(game_mode=="charge training" && charge_destroy_count==charge_destroy_count_objective)
                                    {
                                        win();
                                    }
                                    enemy_pattern->hit(enemy->sound_channel_base+1,false);
                                    player_pattern->hit(player->sound_channel_base+1,false);
                                }
                            }

                        }
                    }
                }
            }
        }
    }

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

    if(!getGameOver())
        iteration++;
}

void STG::render()
{
    stage->dibujarBack();
    player->bottomRender();
    enemy->bottomRender();
    player->topRender();
    enemy->topRender();

    stage->dibujarFront();

    if(enemy->getHP()==0)
        you_win.render();
    if(player->getHP()==0)
        you_loose.render();

//    painter->drawText("Time: "+toString(iteration),25,70);
//    painter->drawText(enemy->getName(),25,110);
//    painter->drawText("Damage level: "+toString(damage_level),25,170);

    int tutorial_text_spacing_y=10;
    int tutorial_control_spacing_y=100;
    current_training_transparency+=2;
    if(current_training_transparency>255)
        current_training_transparency=255;
    if(game_mode=="charge training")
    {
        painter->draw2DImage
        (   image_training_bar_fill,
            image_training_bar_fill->getWidth()*(charge_destroy_count/charge_destroy_count_objective),image_training_bar_fill->getHeight(),
            painter->screen_width/2-image_training_bar_fill->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
        painter->draw2DImage
        (   image_training_bar,
            image_training_bar->getWidth(),image_training_bar->getHeight(),
            painter->screen_width/2-image_training_bar->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
    }
    if(game_mode=="parry training")
    {
        for(int i=0;i<parry_count_objective;i++)
        {
            painter->draw2DImage
            (   image_training_box,
                image_training_box->getWidth(),image_training_box->getHeight(),
                painter->screen_width/2-(image_training_box->getWidth()/2)*3+i*(image_training_box->getWidth()+10),tutorial_control_spacing_y,
                1.0,
                0.0,
                false,
                0,0,
                Color(255,255,255,current_training_transparency),
                0,0,
                false,
                FlatShadow());
            if(i<parry_count)
            {
                painter->draw2DImage
                (   image_training_x,
                    image_training_x->getWidth(),image_training_x->getHeight(),
                    painter->screen_width/2-(image_training_x->getWidth()/2)*3+i*(image_training_x->getWidth()+10),tutorial_control_spacing_y,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,current_training_transparency),
                    0,0,
                    false,
                    FlatShadow());
            }
        }
    }
    if(game_mode=="parry dash training")
    {
        painter->draw2DImage
        (   image_training_bar_fill,
            image_training_bar_fill->getWidth()*(parry_dash_count/parry_dash_count_objective),image_training_bar_fill->getHeight(),
            painter->screen_width/2-image_training_bar_fill->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
        painter->draw2DImage
        (   image_training_bar,
            image_training_bar->getWidth(),image_training_bar->getHeight(),
            painter->screen_width/2-image_training_bar->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
    }
    if(image_training_text)
    {
        painter->draw2DImage
        (   image_training_text,
            image_training_text->getWidth(),image_training_text->getHeight(),
            painter->screen_width/2-image_training_text->getWidth()/2,tutorial_text_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
    }
    if(getGameOver())
    {
        current_training_final_transparency+=2;
        if(current_training_final_transparency>255)
            current_training_final_transparency=255;
    }
    if(image_training_text_final)
    {
        painter->draw2DImage
        (   image_training_text_final,
            image_training_text_final->getWidth(),image_training_text_final->getHeight(),
            painter->screen_width/2-image_training_text_final->getWidth()/2,600,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_final_transparency),
            0,0,
            false,
            FlatShadow());
    }

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

void STG::win()
{
    enemy->setHP(0);
    painter->shakeScreen(50,20);
    sonido->playSound("you win",2,0);
    enemy->deleteActivePatterns();
    setGameOver(true);
}

void STG::lose()
{
    sonido->playSound("you lose",4,0);
    setGameOver(true);
}
