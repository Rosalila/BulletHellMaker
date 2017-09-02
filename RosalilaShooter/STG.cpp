#include "STG.h"

STG::STG(Player*player,Enemy*enemy,Stage*stage,string game_mode, int current_player_best_score)
{
    this->player=player;
    this->enemy=enemy;
    this->stage=stage;
    this->game_mode=game_mode;
    this->current_player_best_score = current_player_best_score;

    this->api_state = "";

    this->score = -1;

    this->game_over_timeout = 128;

    this->image_upload_error = rosalila()->graphics->getTexture(assets_directory+"misc/upload_error.png");

    this->image_training_box = NULL;
    this->image_training_x = NULL;
    this->image_training_bar = NULL;
    this->image_training_bar_fill = NULL;

    this->player_is_immortal = false;

    rosalila()->graphics->camera_y=0;
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
        rosalila()->sound->addSound("you lose",assets_directory+you_loose_node->ToElement()->Attribute("sound"));
    }

    for(TiXmlNode* sprites_node=you_loose_node->FirstChild("sprite");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("sprite"))
    {
        std::string path=sprites_node->ToElement()->Attribute("path");
        you_loose.addImage(rosalila()->graphics->getTexture(assets_directory+path));
    }


    TiXmlNode *you_win_node=config_file->FirstChild("YouWin");

    int you_win_x=atoi(you_win_node->ToElement()->Attribute("x"));
    int you_win_y=atoi(you_win_node->ToElement()->Attribute("y"));
    int you_win_animation_velocity=atoi(you_win_node->ToElement()->Attribute("animation_velocity"));
    you_win=Animation(you_win_x,you_win_y,you_win_animation_velocity);

    if(you_win_node->ToElement()->Attribute("sound"))
    {
        rosalila()->sound->addSound("you win",assets_directory+you_win_node->ToElement()->Attribute("sound"));
    }

    for(TiXmlNode* sprites_node=you_win_node->FirstChild("sprite");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("sprite"))
    {
        std::string path=sprites_node->ToElement()->Attribute("path");
        you_win.addImage(rosalila()->graphics->getTexture(assets_directory+path));
    }

    if(game_mode=="Stage select" || game_mode=="charge training" || game_mode=="parry training" || game_mode=="parry dash training" || "replay")
    {
        stageSelectModeInit();
    }

    if(game_mode=="charge training")
    {
        image_training_bar=rosalila()->graphics->getTexture(assets_directory+"misc/training/bar.png");
        image_training_bar_fill=rosalila()->graphics->getTexture(assets_directory+"misc/training/bar_fill.png");
        charge_destroy_count_objective=300;
        player_is_immortal = true;
    }
    if(game_mode=="parry training")
    {
        image_training_box=rosalila()->graphics->getTexture(assets_directory+"misc/training/box.png");
        image_training_x=rosalila()->graphics->getTexture(assets_directory+"misc/training/x.png");
        parry_count_objective=3;
        player_is_immortal = true;
    }
    if(game_mode=="parry dash training")
    {
        image_training_bar=rosalila()->graphics->getTexture(assets_directory+"misc/training/bar.png");
        image_training_bar_fill=rosalila()->graphics->getTexture(assets_directory+"misc/training/bar_fill.png");
        parry_dash_count_objective=15;
        player_is_immortal = true;
    }

    setGameOver(false);
    setIsFirstWin(false);
    mainLoop();
}

STG::~STG()
{
    if(image_training_box)
        delete image_training_box;

    if(image_training_x)
        delete image_training_x;

    if(image_training_bar)
        delete image_training_bar;

    if(image_training_bar_fill)
        delete image_training_bar_fill;
}

void STG::stageSelectModeInit()
{
    //stage->playMusic();
    rosalila()->graphics->camera_x=0;
    current_training_transparency=0;
}

void STG::mainLoop()
{
    bool end_key_up_keyboard=false;

    initial_ticks=SDL_GetTicks();

    for (;;)
    {
        if(rosalila()->receiver->isPressed("b") && api_state == "")
        {
//            int replay_size=0;
//            string seed_str = rosalila()->utility->toString(rosalila()->utility->random_seed);
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
            rosalila()->sound->playSound(std::string("Menu.back"), 1, 0, 0, false);
            break;
        }

//        if(rosalila()->receiver->isKeyPressed(SDLK_k))
//        {
////            string random_number = toString((rand()*999999)%1000);
//            rosalila()->graphics->screenshot(0,0, 1366, 768, stage->name +".bmp");
//        }

        render();

        logic();

//        rosalila()->receiver->isKeyDown(SDLK_z)

        rosalila()->update();

        if(getGameOver() && api_state == "")
        {
            game_over_timeout--;
            if(game_over_timeout<0)
                game_over_timeout=0;
            if(game_over_timeout==0)
            {
                if(rosalila()->receiver->isKeyPressed(SDLK_RETURN)
                   || (rosalila()->receiver->isDown("a") && end_key_up_keyboard)
                   )
                {
                    break;
                }
                if(!rosalila()->receiver->isDown("a"))
                    end_key_up_keyboard=true;
            }
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
                        p->hit(player->sound_channel_base+1,true);
                        rosalila()->graphics->point_explosion_effect->explode(p->x,p->y,Color(255,255,255,200),15);
                        if(player->isInvulnerable())
                        {
                            if(!player->isOnIntro())
                            {

                                int counter = rosalila()->api_integrator->getStat("TotalParryDashDestroys") + 1;
                                rosalila()->api_integrator->setStat("TotalParryDashDestroys",counter);

                                if(counter >= 15)
                                {
                                    rosalila()->api_integrator->unlockAchievement("ParryDash1");
                                }
                                if(counter >= 50)
                                {
                                    rosalila()->api_integrator->unlockAchievement("ParryDash2");
                                }
                                if(counter >= 200)
                                {
                                    rosalila()->api_integrator->unlockAchievement("ParryDash3");
                                }
                                if(counter >= 500)
                                {
                                    rosalila()->api_integrator->unlockAchievement("ParryDash4");
                                }

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
                                if(player->invulnerable_frames_left==15)
                                {
                                    int counter = rosalila()->api_integrator->getStat("TotalParries") + 1;
                                    rosalila()->api_integrator->setStat("TotalParries",counter);

                                    if(counter >= 3)
                                    {
                                        rosalila()->api_integrator->unlockAchievement("Parry1");
                                    }
                                    if(counter >= 20)
                                    {
                                        rosalila()->api_integrator->unlockAchievement("Parry2");
                                    }
                                    if(counter >= 100)
                                    {
                                        rosalila()->api_integrator->unlockAchievement("Parry3");
                                    }
                                    if(counter >= 250)
                                    {
                                        rosalila()->api_integrator->unlockAchievement("Parry4");
                                    }
                                }
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
                    rosalila()->graphics->point_explosion_effect->explode(p->x,p->y,Color(255,255,255,200),p->bullet->damage);
                    p->hit(enemy->sound_channel_base+1,false);
                    if(!player_is_immortal)
                        player->hit(p->bullet->damage);
                    parry_count = 0;
                    rosalila()->graphics->screen_shake_effect.set(30,15,rosalila()->graphics->camera_x,rosalila()->graphics->camera_y);
                    if(rosalila()->sound->soundExists(player->name+".hit"))
                        rosalila()->sound->playSound(player->name+".hit", 3, 0, player->x, true);
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
                    rosalila()->graphics->point_explosion_effect->explode(p->x,p->y,Color(255,255,255,200),p->bullet->damage+damage_level*5);
                    p->hit(player->sound_channel_base+1,false);
                    enemy->hit(p->bullet->damage+damage_level);
                    enemy->shakeScreen(p->bullet->damage+damage_level*3,p->bullet->damage+damage_level*2);
                    if(rosalila()->sound->soundExists(enemy->name+".hit"))
                        rosalila()->sound->playSound(enemy->name+".hit", 1, 0, enemy->x, true);
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
                                        int counter = rosalila()->api_integrator->getStat("TotalChargeBulletDestroys") + 1;
                                        rosalila()->api_integrator->setStat("TotalChargeBulletDestroys",counter);

                                        if(counter >= 300)
                                        {
                                            rosalila()->api_integrator->unlockAchievement("ChargeDestroy1");
                                        }
                                        if(counter >= 1000)
                                        {
                                            rosalila()->api_integrator->unlockAchievement("ChargeDestroy2");
                                        }
                                        if(counter >= 5000)
                                        {
                                            rosalila()->api_integrator->unlockAchievement("ChargeDestroy3");
                                        }
                                        if(counter >= 10000)
                                        {
                                            rosalila()->api_integrator->unlockAchievement("ChargeDestroy4");
                                        }

                                        charge_destroy_count++;
                                        if(game_mode=="charge training" && charge_destroy_count==charge_destroy_count_objective)
                                        {
                                            win();
                                        }
                                    }
                                    enemy_pattern->hit(enemy->sound_channel_base+1,false);
                                    player_pattern->hit(player->sound_channel_base+1,false);
                                    rosalila()->graphics->point_explosion_effect->explode(enemy_pattern->x,enemy_pattern->y,Color(255,255,255,200),15);
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
    rosalila()->graphics->camera_x+=stage_displacement;
    player->logic(stage_displacement);
    player->x=player->x+stage_displacement;
    enemy->logic(stage_displacement,stage->name);
    //enemy->setX(enemy->x+stage_displacement);
    stage->logic();

    deletePatterns();
    checkCharacterOutOfBounds();
    slowExtraControl();

    if(api_state == "uploading score")
    {
        if(rosalila()->api_integrator->getState()=="finished")
        {
            uploadReplay();
            rosalila()->utility->writeLogLine("uploading replay");
        }

        if(rosalila()->api_integrator->getState()=="error")
        {
            uploadErrorLoop();
			uploadScore();
        }
    }
    if(api_state == "uploading replay")
    {
        if(rosalila()->api_integrator->getState()=="finished")
        {
            findLeaderboard();
            rosalila()->utility->writeLogLine("finding leaderboard");
        }

        if(rosalila()->api_integrator->getState()=="error")
        {
            uploadErrorLoop();
            uploadReplay();
        }
    }

    if(api_state == "finding leaderboard")
    {
        if(rosalila()->api_integrator->getState()=="finished")
        {
            rosalila()->graphics->notification_handler.interruptCurrentNotification();
            rosalila()->graphics->notification_handler.notifications.push_back(
                new Notification(getSuccessImage(), rosalila()->graphics->screen_width/2-getSuccessImage()->getWidth()/2,
                                    rosalila()->graphics->screen_height,
                                    rosalila()->graphics->screen_height-getSuccessImage()->getHeight(),
                                    getNotificationDuration()));

            api_state="";
            rosalila()->utility->writeLogLine("finished");
        }

        if(rosalila()->api_integrator->getState()=="error")
        {
            uploadErrorLoop();
            findLeaderboard();
        }
    }

    if(!getGameOver())
        frame++;
}

void STG::render()
{
    stage->dibujarBack();
    if(player->hp>0)
        player->bottomRender();
    if(enemy->hp>0)
        enemy->bottomRender();
    if(player->hp>0)
        player->topRender();
    if(enemy->hp>0)
        enemy->topRender();

    stage->dibujarFront();

    if(getGameOver() && score!=-1 && game_mode!="replay")
    {
        if(current_player_best_score==-1)
        {
            rosalila()->graphics->drawText(rosalila()->utility->toString(score), 0, 0, true, true);
        }
        else
        {
            int difference = score - current_player_best_score;
            string score_text = rosalila()->utility->toString(score);
            score_text += " (";
            if(difference>0)
                score_text += "+";
            score_text += rosalila()->utility->toString(difference);
            score_text += ")";
            rosalila()->graphics->drawText(score_text,0, 0, true, true);
        }
    }

    if(enemy->hp==0)
        you_win.render();
    if(player->hp==0)
        you_loose.render();

//    rosalila()->graphics->drawText("Time: "+toString(iteration),25,70);
//    rosalila()->graphics->drawText(enemy->name,25,110);
//    rosalila()->graphics->drawText("Damage level: "+toString(damage_level),25,170);

    int tutorial_control_spacing_y=100;
    if(!player->isOnIntro())
        current_training_transparency+=2;
    if(current_training_transparency>255)
        current_training_transparency=255;
    if(game_mode=="charge training")
    {
        rosalila()->graphics->draw2DImage
        (   image_training_bar_fill,
            image_training_bar_fill->getWidth()*(charge_destroy_count/charge_destroy_count_objective),image_training_bar_fill->getHeight(),
            rosalila()->graphics->screen_width/2-image_training_bar_fill->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
        rosalila()->graphics->draw2DImage
        (   image_training_bar,
            image_training_bar->getWidth(),image_training_bar->getHeight(),
            rosalila()->graphics->screen_width/2-image_training_bar->getWidth()/2,tutorial_control_spacing_y,
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
            rosalila()->graphics->draw2DImage
            (   image_training_box,
                image_training_box->getWidth(),image_training_box->getHeight(),
                rosalila()->graphics->screen_width/2-(image_training_box->getWidth()/2)*3+i*(image_training_box->getWidth()+10),tutorial_control_spacing_y,
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
                rosalila()->graphics->draw2DImage
                (   image_training_x,
                    image_training_x->getWidth(),image_training_x->getHeight(),
                    rosalila()->graphics->screen_width/2-(image_training_x->getWidth()/2)*3+i*(image_training_x->getWidth()+10),tutorial_control_spacing_y,
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
        rosalila()->graphics->draw2DImage
        (   image_training_bar_fill,
            image_training_bar_fill->getWidth()*(parry_dash_count/parry_dash_count_objective),image_training_bar_fill->getHeight(),
            rosalila()->graphics->screen_width/2-image_training_bar_fill->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
        rosalila()->graphics->draw2DImage
        (   image_training_bar,
            image_training_bar->getWidth(),image_training_bar->getHeight(),
            rosalila()->graphics->screen_width/2-image_training_bar->getWidth()/2,tutorial_control_spacing_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());
    }

//    rosalila()->graphics->drawText("TotalParries:" +
//                               rosalila()->utility->toString(rosalila()->api_integrator->getStat("TotalParries")),
//                               0, 0, false, false);
//    rosalila()->graphics->drawText("TotalCharges:" +
//                               rosalila()->utility->toString(rosalila()->api_integrator->getStat("TotalCharges")),
//                               0, 50, false, false);
//
//    rosalila()->graphics->drawText("TotalParryDashDestroys:" +
//                               rosalila()->utility->toString(rosalila()->api_integrator->getStat("TotalParryDashDestroys")),
//                               0, 100, false, false);
//
//    rosalila()->graphics->drawText("TotalChargeBulletDestroys:" +
//                               rosalila()->utility->toString(rosalila()->api_integrator->getStat("TotalChargeBulletDestroys")),
//                               0, 150, false, false);


    //rosalila()->graphics->updateScreen();
}

bool STG::isOutOfBounds(int pos_x,int pos_y)
{
    int bullet_bound_addition_x = (stage->bound_x2-stage->bound_x1)/2;
    int bullet_bound_addition_y = (stage->bound_y2-stage->bound_y1)/2;
    if(pos_x<stage->bound_x1+rosalila()->graphics->camera_x-bullet_bound_addition_x
       ||pos_x>stage->bound_x2+rosalila()->graphics->camera_x+bullet_bound_addition_x
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
    if(player->x<stage->bound_x1+rosalila()->graphics->camera_x)
        player->x=stage->bound_x1+rosalila()->graphics->camera_x;
    if(player->x>stage->bound_x2+rosalila()->graphics->camera_x)
        player->x=stage->bound_x2+rosalila()->graphics->camera_x;
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
    int old_clears = rosalila()->api_integrator->getStat(stage->name+"Clears");
    rosalila()->api_integrator->setStat(stage->name+"Clears",old_clears+1);

    if(player->hp == player->max_hp)
    {
        int old_perfects = rosalila()->api_integrator->getStat(stage->name+"Perfects");
        rosalila()->api_integrator->setStat(stage->name+"Perfects",old_perfects+1);
    }

    setPlayerWon(true);
    setGameOver(true);
    setIsFirstWin(old_clears==0);

    rosalila()->api_integrator->unlockAchievement("B");
    double milliseconds = SDL_GetTicks()-initial_ticks;
    double hp_penalty = (1.0 + ((double)player->max_hp-(double)player->hp)/100.0);
    score = milliseconds * hp_penalty;
    enemy->hp=0;
    rosalila()->graphics->screen_shake_effect.set(50,20,rosalila()->graphics->camera_x,rosalila()->graphics->camera_y);
    rosalila()->sound->playSound("you win", 2, 0, 0, false);
    enemy->deleteActivePatterns();

    if(game_mode!="replay" && (score<current_player_best_score || current_player_best_score==-1))
    {
		if(rosalila()->api_integrator->isUsingApi())
			uploadScore();
    }
}

void STG::lose()
{
    setPlayerWon(false);
    setGameOver(true);
    rosalila()->sound->fadeMusicVolume(0, 2);
    rosalila()->sound->playSound("you lose", 4, 0, 0, false);
}

void STG::uploadScore()
{
    api_state = "uploading score";
    rosalila()->utility->writeLogLine("uploading score");

    rosalila()->api_integrator->setScore(stage->name, score);

    rosalila()->graphics->notification_handler.notifications.push_back(
        new Notification(getLoadingImage(), rosalila()->graphics->screen_width/2-getLoadingImage()->getWidth()/2,
                            rosalila()->graphics->screen_height,
                            rosalila()->graphics->screen_height-getLoadingImage()->getHeight(),
                            999999));
}

void STG::uploadReplay()
{
    int replay_size=0;
    string seed_str = rosalila()->utility->toString(rosalila()->utility->random_seed);
    replay_size+=seed_str.size()+1;
    for(int i=0;i<(int)player->replay_storage.size();i++)
    {
        replay_size+=player->replay_storage[i].size()+1;
    }
    replay_size+=1;

    char*replay_data = new char[replay_size];

    strcpy(replay_data,"");
    strcat(replay_data,seed_str.c_str());
    strcat(replay_data,"\n");

    for(int i=0;i<(int)player->replay_storage.size();i++)
    {
        strcat(replay_data,player->replay_storage[i].c_str());
        strcat(replay_data,"\n");
    }
    strcat(replay_data,"\0");

    rosalila()->api_integrator->storeLeaderboardAttachment(stage->name,replay_data,replay_size);

    api_state = "uploading replay";
}

void STG::uploadErrorLoop()
{
    rosalila()->graphics->notification_handler.interruptCurrentNotification();
    rosalila()->graphics->notification_handler.notifications.push_back(
        new Notification(getErrorImage(), rosalila()->graphics->screen_width/2-getErrorImage()->getWidth()/2,
                            rosalila()->graphics->screen_height,
                            rosalila()->graphics->screen_height-getErrorImage()->getHeight(),
                            getNotificationDuration()));

    api_state = "error";

    while(true)
    {
        if(rosalila()->receiver->isPressed("a"))
        {
            break;
        }
        rosalila()->graphics->draw2DImage
        (   image_upload_error,
            image_upload_error->getWidth(),image_upload_error->getHeight(),
            0,0,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,current_training_transparency),
            0,0,
            false,
            FlatShadow());

        rosalila()->update();
    }
}

void STG::findLeaderboard()
{
    rosalila()->api_integrator->findLeaderboard(stage->name);
    api_state = "finding leaderboard";
}
