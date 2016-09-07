#include "STG.h"

STG::STG(Player*player,Enemy*enemy,Stage*stage,string game_mode,map<string,Button*>controls, int current_player_best_score)
{
    this->player=player;
    this->enemy=enemy;
    this->stage=stage;
    this->game_mode=game_mode;
    this->controls=controls;
    this->current_player_best_score = current_player_best_score;

    Rosalila()->Graphics->camera_y=0;
    frame=0;

    parry_count=0;
    charge_destroy_count=0;
    parry_dash_count=0;

    current_training_transparency=0;

    //XML Initializations
    string config_directory = assets_directory+"config.xml";
    TiXmlDocument doc_t( (char*)config_directory.c_str() );
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *config_file=doc->FirstChild("ConfigFile");

    TiXmlNode *you_loose_node=config_file->FirstChild("YouLose");

    int you_loose_x=atoi(you_loose_node->ToElement()->Attribute("x"));
    int you_loose_y=atoi(you_loose_node->ToElement()->Attribute("y"));
    int you_loose_animation_velocity=atoi(you_loose_node->ToElement()->Attribute("animation_velocity"));
    you_loose=Animation(you_loose_x,you_loose_y,you_loose_animation_velocity);

    if(you_loose_node->ToElement()->Attribute("sound"))
    {
        Rosalila()->Sound->addSound("you lose",assets_directory+you_loose_node->ToElement()->Attribute("sound"));
    }

    for(TiXmlNode* sprites_node=you_loose_node->FirstChild("sprite");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("sprite"))
    {
        std::string path=sprites_node->ToElement()->Attribute("path");
        you_loose.addImage(Rosalila()->Graphics->getTexture(assets_directory+path));
    }


    TiXmlNode *you_win_node=config_file->FirstChild("YouWin");

    int you_win_x=atoi(you_win_node->ToElement()->Attribute("x"));
    int you_win_y=atoi(you_win_node->ToElement()->Attribute("y"));
    int you_win_animation_velocity=atoi(you_win_node->ToElement()->Attribute("animation_velocity"));
    you_win=Animation(you_win_x,you_win_y,you_win_animation_velocity);

    if(you_win_node->ToElement()->Attribute("sound"))
    {
        Rosalila()->Sound->addSound("you win",assets_directory+you_win_node->ToElement()->Attribute("sound"));
    }

    for(TiXmlNode* sprites_node=you_win_node->FirstChild("sprite");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("sprite"))
    {
        std::string path=sprites_node->ToElement()->Attribute("path");
        you_win.addImage(Rosalila()->Graphics->getTexture(assets_directory+path));
    }

    if(game_mode=="Stage select" || game_mode=="charge training" || game_mode=="parry training" || game_mode=="parry dash training" || "replay")
    {
        stageSelectModeInit();
    }

    if(game_mode=="charge training")
    {
        image_training_bar=Rosalila()->Graphics->getTexture(assets_directory+"misc/training/bar.png");
        image_training_bar_fill=Rosalila()->Graphics->getTexture(assets_directory+"misc/training/bar_fill.png");
        charge_destroy_count_objective=300;
    }
    if(game_mode=="parry training")
    {
        image_training_box=Rosalila()->Graphics->getTexture(assets_directory+"misc/training/box.png");
        image_training_x=Rosalila()->Graphics->getTexture(assets_directory+"misc/training/x.png");
        parry_count_objective=3;
    }
    if(game_mode=="parry dash training")
    {
        image_training_bar=Rosalila()->Graphics->getTexture(assets_directory+"misc/training/bar.png");
        image_training_bar_fill=Rosalila()->Graphics->getTexture(assets_directory+"misc/training/bar_fill.png");
        parry_dash_count_objective=15;
    }

    setGameOver(false);
    mainLoop();
}

void STG::stageSelectModeInit()
{
    //stage->playMusic();
    Rosalila()->Graphics->camera_x=0;
    current_training_transparency=0;
}

void STG::mainLoop()
{
    bool end_key_up_keyboard=false;

    initial_ticks=SDL_GetTicks();

    for (;;)
    {
        if(Rosalila()->Receiver->isKeyPressed(SDLK_ESCAPE))
        {
//            int replay_size=0;
//            string seed_str = Rosalila()->Utility->toString(Rosalila()->Utility->random_seed);
//            replay_size+=seed_str.size()+1;
//            for(int i=0;i<player->replay_storage.size();i++)
//            {
//                replay_size+=player->replay_storage[i].size()+1;
//            }
//            replay_size+=1;
//
//            char*replay_data = new char[replay_size];
//
//            strcpy(replay_data,"");
//            strcat(replay_data,seed_str.c_str());
//            strcat(replay_data,"\n");
//
//            for(int i=0;i<player->replay_storage.size();i++)
//            {
//                strcat(replay_data,player->replay_storage[i].c_str());
//                strcat(replay_data,"\n");
//            }
//            strcat(replay_data,"\0");
//
//            ofstream out(stage->name.c_str());
//            out.write(replay_data,replay_size);
//            out.close();


            player->exit();
            Rosalila()->Sound->playSound(std::string("Menu.back"),1,0);
            break;
        }

//        if(Rosalila()->Receiver->isKeyPressed(SDLK_s))
//        {
//            string random_number = toString((rand()*999999)%1000);
//            Rosalila()->Graphics->screenshot(0,0, 1366, 768, "screenshot"+ random_number +".bmp");
//        }

        render();

        logic();

//        Rosalila()->Receiver->isKeyDown(SDLK_z)
        Rosalila()->Receiver->updateInputs();
        if(getGameOver())
        {
            if(Rosalila()->Receiver->isKeyPressed(SDLK_RETURN)
               || (controls["a"]->isDown() && end_key_up_keyboard)
               )
            {
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

    for (std::list<Pattern*>::iterator pattern = enemy->active_patterns->begin(); pattern != enemy->active_patterns->end(); pattern++)
    {
        Pattern*p=(Pattern*)*pattern;
        if(!p->is_hit)
        {
            for(int i=0;i<(int)p->bullet->hitboxes.size();i++)
            {
                Hitbox h=p->bullet->hitboxes[i]->getPlacedHitbox(p->x,p->y,p->getBulletAngle());
                if(player->isParrying() || player->isInvulnerable())
                {
                    if(!p->is_hit && p->collides_opponent && (player->collidesParry(h,0,0,0)||player->collides(h,0,0,0)))
                    {
                        p->hit(player->sound_channel_base+1,false);
                        if(player->isInvulnerable())
                        {
                            if(!player->isOnIntro())
                            {
                                parry_dash_count++;
                                if(game_mode=="parry dash training" && parry_dash_count==parry_dash_count_objective)
                                {
                                    win();
                                }
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
                            if(!player->isOnIntro())
                            {
                                parry_count++;
                                if(game_mode=="parry training" && parry_count==parry_count_objective)
                                {
                                    win();
                                }
                            }
                        }
                        if(p->x>player->x)
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
                    player->hit(p->bullet->damage);
                    parry_count = 0;
                    Rosalila()->Graphics->shakeScreen(30,10);
                    if(Rosalila()->Sound->soundExists(player->name+".hit"))
                        Rosalila()->Sound->playSound(player->name+".hit",3,0);
                    if(player->hp==0)
                    {
                        lose();
                    }
                }
            }
        }
    }

    for (std::list<Pattern*>::iterator pattern = player->active_patterns->begin(); pattern != player->active_patterns->end(); pattern++)
    {
        Pattern*p=(Pattern*)*pattern;
        if(!p->is_hit)
        {
            for(int i=0;i<(int)p->bullet->hitboxes.size();i++)
            {
                Hitbox h=p->bullet->hitboxes[i]->getPlacedHitbox(p->x,p->y,p->getBulletAngle());
                if(p->collides_opponent && enemy->collides(h,0,0,0))
                {
                    p->hit(player->sound_channel_base+1,false);
                    enemy->hit(p->bullet->damage+damage_level);
                    enemy->shakeScreen(p->bullet->damage+damage_level*3,p->bullet->damage+damage_level*2);
                    if(Rosalila()->Sound->soundExists(enemy->name+".hit"))
                        Rosalila()->Sound->playSound(enemy->name+".hit",1,0);
                    if(enemy->hp==0)
                    {
                        win();
                    }
                }
            }
        }
    }

    //BulletxBullet Collision
    for (std::list<Pattern*>::iterator enemy_pattern_iterator = enemy->active_patterns->begin(); enemy_pattern_iterator != enemy->active_patterns->end(); enemy_pattern_iterator++)
    {
        Pattern*enemy_pattern=(Pattern*)*enemy_pattern_iterator;
        if(!enemy_pattern->is_hit)
        {
            for (std::list<Pattern*>::iterator player_pattern_iterator = player->active_patterns->begin(); player_pattern_iterator != player->active_patterns->end(); player_pattern_iterator++)
            {
                Pattern*player_pattern=(Pattern*)*player_pattern_iterator;
                if(player_pattern->collides_bullets||enemy_pattern->collides_bullets)
                {
                    if(!player_pattern->is_hit)
                    {
                        vector<Hitbox*>enemy_hitboxes=enemy_pattern->bullet->hitboxes;
                        for(int i=0;i<(int)enemy_hitboxes.size();i++)
                        {
                            Hitbox enemy_hitbox=enemy_hitboxes[i]->getPlacedHitbox(enemy_pattern->x,enemy_pattern->y,enemy_pattern->getBulletAngle());
                            vector<Hitbox*>player_hitboxes=player_pattern->bullet->hitboxes;
                            for(int j=0;j<(int)player_hitboxes.size();j++)
                            {
                                Hitbox player_hitbox=player_hitboxes[j]->getPlacedHitbox(player_pattern->x,player_pattern->y,player_pattern->getBulletAngle());
                                if(!enemy_pattern->is_hit&&!player_pattern->is_hit&&enemy_hitbox.collides(player_hitbox))
                                {
                                    if(!player->isOnIntro())
                                    {
                                        charge_destroy_count++;
                                        if(game_mode=="charge training" && charge_destroy_count==charge_destroy_count_objective)
                                        {
                                            win();
                                        }
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

    int stage_displacement = stage->velocity;
    if(isSlowActive())
        stage_displacement/=3;
    Rosalila()->Graphics->camera_x+=stage_displacement;
    player->logic(stage_displacement);
    player->x=player->x+stage_displacement;
    enemy->logic(stage_displacement,stage->name);
    //enemy->setX(enemy->x+stage_displacement);
    stage->logic();

    deletePatterns();
    checkCharacterOutOfBounds();
    slowExtraControl();

    if(!getGameOver())
        frame++;
}

void STG::render()
{
    stage->dibujarBack();
    player->bottomRender();
    enemy->bottomRender();
    player->topRender();
    enemy->topRender();

    stage->dibujarFront();

    if(enemy->hp==0)
        you_win.render();
    if(player->hp==0)
        you_loose.render();

//    Rosalila()->Graphics->drawText("Time: "+toString(iteration),25,70);
//    Rosalila()->Graphics->drawText(enemy->name,25,110);
//    Rosalila()->Graphics->drawText("Damage level: "+toString(damage_level),25,170);

    int tutorial_text_spacing_y=10;
    int tutorial_control_spacing_y=100;
    if(!player->isOnIntro())
        current_training_transparency+=2;
    if(current_training_transparency>255)
        current_training_transparency=255;
    if(game_mode=="charge training")
    {
        Rosalila()->Graphics->draw2DImage
        (   image_training_bar_fill,
            image_training_bar_fill->getWidth()*(charge_destroy_count/charge_destroy_count_objective),image_training_bar_fill->getHeight(),
            Rosalila()->Graphics->screen_width/2-image_training_bar_fill->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
        Rosalila()->Graphics->draw2DImage
        (   image_training_bar,
            image_training_bar->getWidth(),image_training_bar->getHeight(),
            Rosalila()->Graphics->screen_width/2-image_training_bar->getWidth()/2,tutorial_control_spacing_y,
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
            Rosalila()->Graphics->draw2DImage
            (   image_training_box,
                image_training_box->getWidth(),image_training_box->getHeight(),
                Rosalila()->Graphics->screen_width/2-(image_training_box->getWidth()/2)*3+i*(image_training_box->getWidth()+10),tutorial_control_spacing_y,
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
                Rosalila()->Graphics->draw2DImage
                (   image_training_x,
                    image_training_x->getWidth(),image_training_x->getHeight(),
                    Rosalila()->Graphics->screen_width/2-(image_training_x->getWidth()/2)*3+i*(image_training_x->getWidth()+10),tutorial_control_spacing_y,
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
        Rosalila()->Graphics->draw2DImage
        (   image_training_bar_fill,
            image_training_bar_fill->getWidth()*(parry_dash_count/parry_dash_count_objective),image_training_bar_fill->getHeight(),
            Rosalila()->Graphics->screen_width/2-image_training_bar_fill->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
        Rosalila()->Graphics->draw2DImage
        (   image_training_bar,
            image_training_bar->getWidth(),image_training_bar->getHeight(),
            Rosalila()->Graphics->screen_width/2-image_training_bar->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
    }

    Rosalila()->Graphics->updateScreen();
}

bool STG::isOutOfBounds(int pos_x,int pos_y)
{
    int bullet_bound_addition_x = (stage->bound_x2-stage->bound_x1)/2;
    int bullet_bound_addition_y = (stage->bound_y2-stage->bound_y1)/2;
    if(pos_x<stage->bound_x1+Rosalila()->Graphics->camera_x-bullet_bound_addition_x
       ||pos_x>stage->bound_x2+Rosalila()->Graphics->camera_x+bullet_bound_addition_x
       ||pos_y<stage->bound_y1-bullet_bound_addition_y
       ||pos_y>stage->bound_y2+bullet_bound_addition_y
       )
    {
        return true;
    }
    return false;
}

void STG::deletePatterns()
{
    std::list<Pattern*>* active_patterns=player->active_patterns;
    std::list<Pattern*>::iterator i = active_patterns->begin();
    while (i != active_patterns->end())
    {
        Pattern*p=(Pattern*)*i;
        if (isOutOfBounds(p->x,p->y) || p->destroyFlag())
        {
            active_patterns->erase(i++);
            delete p;
        }
        else
        {
            ++i;
        }
    }

    active_patterns=enemy->active_patterns;
    i = active_patterns->begin();
    while (i != active_patterns->end())
    {
        Pattern*p=(Pattern*)*i;
        if (isOutOfBounds(p->x,p->y) || p->destroyFlag())
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
    if(player->x<stage->bound_x1+Rosalila()->Graphics->camera_x)
        player->x=stage->bound_x1+Rosalila()->Graphics->camera_x;
    if(player->x>stage->bound_x2+Rosalila()->Graphics->camera_x)
        player->x=stage->bound_x2+Rosalila()->Graphics->camera_x;
    if(player->y<stage->bound_y1)
        player->y=stage->bound_y1;
    if(player->y>stage->bound_y2)
        player->y=stage->bound_y2;
}

bool STG::playerWon()
{
    return enemy->hp==0;
}

bool STG::enemyWon()
{
    return player->hp==0;
}

void STG::win()
{
    Rosalila()->ApiIntegrator->unlockAchievement("B");
    double milliseconds = SDL_GetTicks()-initial_ticks;
    double hp_penalty = (1.0 + ((double)player->max_hp-(double)player->hp)/100.0);
    double score = milliseconds * hp_penalty;
    enemy->hp=0;
    Rosalila()->Graphics->shakeScreen(50,20);
    Rosalila()->Sound->playSound("you win",2,0);
    enemy->deleteActivePatterns();
    setGameOver(true);

    if(game_mode!="replay" && (score<current_player_best_score || current_player_best_score==-1))
    {
        int replay_size=0;
        string seed_str = Rosalila()->Utility->toString(Rosalila()->Utility->random_seed);
        replay_size+=seed_str.size()+1;
        for(int i=0;i<player->replay_storage.size();i++)
        {
            replay_size+=player->replay_storage[i].size()+1;
        }
        replay_size+=1;

        char*replay_data = new char[replay_size];

        strcpy(replay_data,"");
        strcat(replay_data,seed_str.c_str());
        strcat(replay_data,"\n");

        for(int i=0;i<player->replay_storage.size();i++)
        {
            strcat(replay_data,player->replay_storage[i].c_str());
            strcat(replay_data,"\n");
        }
        strcat(replay_data,"\0");

        Rosalila()->ApiIntegrator->setScore(stage->name, score,replay_data,replay_size);

        while(Rosalila()->ApiIntegrator->getState()!="loading")
        {
            Rosalila()->ApiIntegrator->updateCallbacks();
            SDL_Delay(17);
        }
    }
}

void STG::lose()
{
    Rosalila()->Sound->playSound("you lose",4,0);
    setGameOver(true);
}
