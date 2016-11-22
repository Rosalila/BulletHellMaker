#include "StageSelect.h"

vector<string> getReplayInput(char* replay_data, int replay_size)
{
    vector<string> replay_input;
    string random_seed_str;

    int replay_iterator=0;

    for(;replay_iterator<replay_size
            && replay_data[replay_iterator]!='\n';
            replay_iterator++)
    {
        random_seed_str+=replay_data[replay_iterator];
    }
    replay_iterator++;
    if(random_seed_str!="")
    {
        int random_seed = atoi(random_seed_str.c_str());
        rosalila()->utility->setRandomSeed(random_seed);
    }

    for(;replay_iterator<replay_size;replay_iterator++)
    {
        string line="";
        for(;replay_iterator<replay_size
                && replay_data[replay_iterator]!='\n';
                replay_iterator++)
        {
            line+=replay_data[replay_iterator];
        }
        replay_input.push_back(line);
    }

    return replay_input;
}

vector<string> getReplayInput(string file_name)
{
    ifstream in(file_name.c_str());
    in.seekg(0,ios::end);
    int replay_size = in.tellg();
    in.seekg(0);
    char* replay_data=new char[replay_size];
    in.read(replay_data,replay_size);
    return getReplayInput(replay_data, replay_size);
}

std::vector<std::string> getStageNames()
{
    std::vector<std::string> stage_names;
    Node* root_node = rosalila()->parser->getNodes(assets_directory+"config.xml");

    vector<Node*> stage_nodes = root_node->getNodeByName("Stages")->getNodesByName("stage");

    for(int i=0;i<(int)stage_nodes.size();i++)
    {
        stage_names.push_back(stage_nodes[i]->attributes["name"]);
    }

    return stage_names;
}

Color getBackgroundColor(int current_stage)
{
    int training_begin = 0;
    int training_end = 4;
    Color training_color(33,150,255,255);

    int stage_a = training_end + 1;
    int stage_b = stage_a + 1;
    int stage_c = stage_b + 1;
    int stage_d = stage_c + 1;
    int stage_e = stage_d + 1;
    int stage_f = stage_e + 1;

    Color stage_a_color(255,0,0,255);
    Color stage_b_color(0,255,0,255);
    Color stage_c_color(0,0,255,255);
    Color stage_d_color(255,255,0,255);
    Color stage_e_color(255,0,255,255);
    Color stage_f_color(0,255,255,255);

    int stage_a_stages = stage_f + 1;
    int stage_b_stages = stage_a_stages + 5;
    int stage_c_stages = stage_b_stages + 5;
    int stage_d_stages = stage_c_stages + 5;
    int stage_e_stages = stage_d_stages + 5;
    int stage_f_stages = stage_e_stages + 5;

    if(current_stage>=training_begin && current_stage<=training_end)
        return training_color;

    if(current_stage==stage_a
       || (current_stage >= stage_a_stages && current_stage < stage_b_stages))
        return stage_a_color;

    if(current_stage==stage_b
       || (current_stage >= stage_b_stages && current_stage < stage_c_stages))
        return stage_b_color;

    if(current_stage==stage_c
       || (current_stage >= stage_c_stages && current_stage < stage_d_stages))
        return stage_c_color;

    if(current_stage==stage_d
       || (current_stage >= stage_d_stages && current_stage < stage_e_stages))
        return stage_d_color;

    if(current_stage==stage_e
       || (current_stage >= stage_e_stages && current_stage < stage_f_stages))
        return stage_e_color;

    if(current_stage==stage_f
       || (current_stage >= stage_f_stages))
        return stage_f_color;

    return Color();
}

void updateMusic(int current_stage)
{
    if(current_stage>=0 && current_stage<=4)
    {
        if(rosalila()->sound->current_music!=assets_directory+"1.ogg")
            rosalila()->sound->playMusic(assets_directory+"1.ogg",-1);
    }
    if(current_stage>=5 && current_stage<=9)
    {
        if(rosalila()->sound->current_music!=assets_directory+"2.ogg")
            rosalila()->sound->playMusic(assets_directory+"2.ogg",-1);
    }
    if(current_stage>=10 && current_stage<=14)
    {
        if(rosalila()->sound->current_music!=assets_directory+"3.ogg")
            rosalila()->sound->playMusic(assets_directory+"3.ogg",-1);
    }
    if(current_stage>=15 && current_stage<=19)
    {
        if(rosalila()->sound->current_music!=assets_directory+"4.ogg")
            rosalila()->sound->playMusic(assets_directory+"4.ogg",-1);
    }
    if(current_stage>=20 && current_stage<=24)
    {
        if(rosalila()->sound->current_music!=assets_directory+"5.ogg")
            rosalila()->sound->playMusic(assets_directory+"5.ogg",-1);
    }
}

std::vector<Image*> getStageImages(std::vector<std::string> stage_names)
{
    std::vector<Image*> stage_images;
    for(int i=0;i<(int)stage_names.size();i++)
    {
        Image*image=rosalila()->graphics->getTexture(assets_directory+std::string("stages/")+stage_names[i]+std::string("/images/preview.png"));
        stage_images.push_back(image);
    }
    return stage_images;
}

LeaderboardEntry* getSelectedEntry(Leaderboard* current_leaderboard, int entry_navigator)
{
    if(entry_navigator==-6 && current_leaderboard->top_entries.size()>=1)
    {
        return current_leaderboard->top_entries[0];
    }
    if(entry_navigator==-5 && current_leaderboard->top_entries.size()>=2)
    {
        return current_leaderboard->top_entries[1];
    }
    if(entry_navigator==-4 && current_leaderboard->top_entries.size()>=3)
    {
        return current_leaderboard->top_entries[2];
    }

    if(entry_navigator==-3 && current_leaderboard->near_entries.size()>=1)
    {
        return current_leaderboard->near_entries[0];
    }
    if(entry_navigator==-2 && current_leaderboard->near_entries.size()>=2)
    {
        return current_leaderboard->near_entries[1];
    }
    if(entry_navigator==-1 && current_leaderboard->near_entries.size()>=3)
    {
        return current_leaderboard->near_entries[2];
    }

    if(entry_navigator>0 && entry_navigator<=current_leaderboard->near_entries.size()+1)
    {
        return current_leaderboard->friends_entries[entry_navigator-1];
    }

    return NULL;
}

void stageSelect()
{
    std::vector<std::string> stage_names = getStageNames();
    std::vector<Image*> stage_images = getStageImages(getStageNames());

    Image*background = rosalila()->graphics->getTexture(assets_directory+"menu/white_background.png");
    Image*left_arrow = rosalila()->graphics->getTexture(assets_directory+"menu/left_arrow.png");
    Image*right_arrow = rosalila()->graphics->getTexture(assets_directory+"menu/right_arrow.png");
    Image*up_arrow = rosalila()->graphics->getTexture(assets_directory+"menu/up_arrow.png");
    Image*down_arrow = rosalila()->graphics->getTexture(assets_directory+"menu/down_arrow.png");
    Image*line = rosalila()->graphics->getTexture(assets_directory+"menu/line.png");

    double line_width = 0;

    Color background_color(255,255,255,255);

    int current_stage = rosalila()->api_integrator->getStat("current stage");
    int frame = 0;
    int entry_navigator = 0;
    LeaderboardEntry* selected_entry = NULL;

    bool select_button_was_up = false;

    rosalila()->receiver->updateInputs();

    double menu_displacement_velocity = 20;
    double entry_height = 50;

    double top_menu_y = -6*entry_height/2 - 200;
    double middle_menu_y = 0;
    double bottom_menu_y = rosalila()->graphics->screen_height;

    double target_top_menu_y = rosalila()->graphics->screen_height/2 - 6*entry_height/2;
    double target_middle_menu_y = 0;
    double target_bottom_menu_y = rosalila()->graphics->screen_height;

    while(true)
    {
        if(rosalila()->receiver->isPressed("b"))
        {
            break;
        }

        if(!rosalila()->receiver->isDown("a"))
        {
            select_button_was_up = true;
        }

        if(rosalila()->receiver->isPressed("6"))
        {
            current_stage++;
            entry_navigator=0;
            if(current_stage>=(int)stage_images.size())
                current_stage=stage_images.size()-1;
            rosalila()->api_integrator->setStat("current stage",current_stage);
        }

        if(rosalila()->receiver->isPressed("4"))
        {
            current_stage--;
            entry_navigator=0;
            if(current_stage<0)
                current_stage=0;
            rosalila()->api_integrator->setStat("current stage",current_stage);
        }

        Leaderboard* current_leaderboard = rosalila()->api_integrator->getLeaderboard(stage_names[current_stage]);

        if(current_leaderboard)
        {
            if(rosalila()->receiver->isPressed("2"))
            {
                entry_navigator++;
                line_width=0;
                if(entry_navigator > (int)current_leaderboard->friends_entries.size())
                {
                    entry_navigator = current_leaderboard->friends_entries.size();
                }
            }
            if(rosalila()->receiver->isPressed("8"))
            {
                entry_navigator--;
                line_width=0;
                if(entry_navigator<-6)
                {
                   entry_navigator=-6;
                }
            }

            target_bottom_menu_y = rosalila()->graphics->screen_height/2 - current_leaderboard->friends_entries.size()*entry_height/2;

            if(entry_navigator<0)
            {
                if(top_menu_y<target_top_menu_y)
                {
                    top_menu_y+=menu_displacement_velocity;
                    if(top_menu_y>target_top_menu_y)
                        top_menu_y = target_top_menu_y;
                }
                if(top_menu_y>target_top_menu_y)
                {
                    top_menu_y-=menu_displacement_velocity;
                    if(top_menu_y<target_top_menu_y)
                        top_menu_y = target_top_menu_y;
                }
                middle_menu_y += menu_displacement_velocity;
                bottom_menu_y += menu_displacement_velocity;
            }
            if(entry_navigator==0)
            {
                if(middle_menu_y<target_middle_menu_y)
                {
                    middle_menu_y+=menu_displacement_velocity;
                    if(middle_menu_y>target_middle_menu_y)
                        middle_menu_y = target_middle_menu_y;
                }
                if(middle_menu_y>target_middle_menu_y)
                {
                    middle_menu_y-=menu_displacement_velocity;
                    if(middle_menu_y<target_middle_menu_y)
                        middle_menu_y = target_middle_menu_y;
                }
                top_menu_y -= menu_displacement_velocity;
                bottom_menu_y += menu_displacement_velocity;
            }
            if(entry_navigator>0)
            {
                if(bottom_menu_y<target_bottom_menu_y)
                {
                    bottom_menu_y+=menu_displacement_velocity;
                    if(bottom_menu_y>target_bottom_menu_y)
                        bottom_menu_y = target_bottom_menu_y;
                }
                if(bottom_menu_y>target_bottom_menu_y)
                {
                    bottom_menu_y-=menu_displacement_velocity;
                    if(bottom_menu_y<target_bottom_menu_y)
                        bottom_menu_y = target_bottom_menu_y;
                }
                top_menu_y -= menu_displacement_velocity;
                middle_menu_y -= menu_displacement_velocity;
            }

            top_menu_y = std::max(top_menu_y,(double)-7*entry_height);
            middle_menu_y = std::max(middle_menu_y,(double)-stage_images[current_stage]->getHeight());
            bottom_menu_y = std::max(bottom_menu_y,(double)-rosalila()->graphics->screen_height);

            top_menu_y = std::min(top_menu_y,(double)rosalila()->graphics->screen_height);
            middle_menu_y = std::min(middle_menu_y,(double)rosalila()->graphics->screen_height);
            bottom_menu_y = std::min(bottom_menu_y,(double)rosalila()->graphics->screen_height);


            selected_entry = getSelectedEntry(current_leaderboard, entry_navigator);
        }

        bool error_found = false;

        if(rosalila()->receiver->isDown("a") && select_button_was_up)
        {
            rosalila()->utility->writeLogLine("Initializing game.");
            rosalila()->utility->setRandomSeed(time(NULL));
            vector<string>replay_input;
            vector<string>intro_input;

            string game_mode="Stage select";

            if(stage_names[current_stage]=="Training1")
            {
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training1");
                rosalila()->graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training2")
            {
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training2");
                rosalila()->graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training3")
            {
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training3");
                rosalila()->graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training4")
            {
                game_mode="charge training";
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training4");
                rosalila()->graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training5")
            {
                game_mode="parry training";
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training5");
                rosalila()->graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training6")
            {
                game_mode="parry dash training";
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training6");
                rosalila()->graphics->grayscale_effect.set(0,1);
            }

            rosalila()->graphics->transparency_effect.set(0,1);
            rosalila()->graphics->transparency_effect.set(1,0.03);

            rosalila()->utility->setRandomSeed(time(NULL));

            if(selected_entry!=NULL)
            {
                game_mode="replay";

                rosalila()->api_integrator->downloadEntryAttachment(selected_entry);

                rosalila()->graphics->notification_handler.notifications.push_back(
                    new Notification(getLoadingImage(), rosalila()->graphics->screen_width/2-getLoadingImage()->getWidth()/2,
                                        rosalila()->graphics->screen_height,
                                        rosalila()->graphics->screen_height-getLoadingImage()->getHeight(),
                                        999999));

                while(selected_entry->attachment_state!="loaded"
                      && rosalila()->api_integrator->getState()=="loading")
                {
                    SDL_Delay(17);
                    rosalila()->update();
                }

                rosalila()->graphics->notification_handler.interruptCurrentNotification();

                if(rosalila()->api_integrator->getState()!="error")
                {
                    char* replay_data = selected_entry->attachment;

                    int replay_size = selected_entry->attachment_size;

                    replay_input = getReplayInput(replay_data, replay_size);

                }else
                {
                    rosalila()->graphics->notification_handler.notifications.push_back(
                        new Notification(getErrorImage(), rosalila()->graphics->screen_width/2-getErrorImage()->getWidth()/2,
                                            rosalila()->graphics->screen_height,
                                            rosalila()->graphics->screen_height-getErrorImage()->getHeight(),
                                            getNotificationDuration()));
                    error_found=true;
                    rosalila()->graphics->grayscale_effect.set(1,1);
                }
            }

            if(!error_found)
            {
                int current_player_best_score = -1;
                if(current_leaderboard && current_leaderboard->leaderboard_self_entry!=NULL)
                    current_player_best_score = current_leaderboard->leaderboard_self_entry->score;

                Stage*stage=new Stage();
                stage->loadFromXML(stage_names[current_stage]);
                Player*player=new Player("Triangle",10,intro_input,replay_input);
                Enemy*enemy=new Enemy(stage_names[current_stage],player,20);
                rosalila()->api_integrator->setCurrentControllerActionSet("InGameControls");
                STG*stg=new STG(player,enemy,stage,game_mode,current_player_best_score);
                rosalila()->api_integrator->setCurrentControllerActionSet("MenuControls");

//                for(int i=0;i<1000;i++)
//                {
//                    Stage*x=new Stage();
//                    stage->loadFromXML(stage_names[current_stage]);
//                    delete x;
//                }

                delete stg;
                delete player;
                delete enemy;
                delete stage;
                select_button_was_up=false;
                rosalila()->graphics->grayscale_effect.set(1,1);
            }
        }

        Color target_color = getBackgroundColor(current_stage);

        if(background_color.red<target_color.red)
            background_color.red++;
        if(background_color.red>target_color.red)
            background_color.red--;

        if(background_color.green<target_color.green)
            background_color.green++;
        if(background_color.green>target_color.green)
            background_color.green--;

        if(background_color.blue<target_color.blue)
            background_color.blue++;
        if(background_color.blue>target_color.blue)
            background_color.blue--;

        updateMusic(current_stage);

        rosalila()->graphics->draw2DImage
        (   background,
            rosalila()->graphics->screen_width,rosalila()->graphics->screen_height,
            0,0,
            1.0,
            0.0,
            false,
            0,0,
            background_color,
            0,0,
            false,
            FlatShadow());

        int line_separation = 35;

        //Top menu
        int separation = 50;
        int line_y = -100;
        if(entry_navigator<0)
            line_y = top_menu_y+(entry_navigator+7)*separation;
        if(entry_navigator<-3)
            line_y -= 50;
        if(entry_navigator>=1)
            line_y = bottom_menu_y+(entry_navigator-1)*separation;

        line_width+=15;
        if(line_width>line->getWidth())
            line_width=line->getWidth();

        rosalila()->graphics->draw2DImage
        (   line,
            line_width,line->getHeight(),
            rosalila()->graphics->screen_width/2-line_width/2,
            line_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        rosalila()->graphics->draw2DImage
        (   line,
            line_width,line->getHeight(),
            rosalila()->graphics->screen_width/2-line_width/2,
            line_y+line_separation,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        if(current_leaderboard)
        {
            for(int i=0;i<current_leaderboard->top_entries.size();i++)
            {
                LeaderboardEntry* current_entry = current_leaderboard->top_entries[i];
                string entry_text = rosalila()->utility->toString(current_entry->rank)+"." + current_entry->name + " " +rosalila()->utility->toString(current_entry->score);

                rosalila()->graphics->drawText(entry_text, 0, top_menu_y+i*separation, true, false);
            }

            for(int i=0;i<current_leaderboard->near_entries.size();i++)
            {
                int align_y = 200+top_menu_y;
                LeaderboardEntry* current_entry = current_leaderboard->near_entries[i];
                string entry_text = rosalila()->utility->toString(current_entry->rank)+"." + current_entry->name + " " +rosalila()->utility->toString(current_entry->score);

                rosalila()->graphics->drawText(entry_text, 0, align_y+i*separation, true, false);
            }
        }

        //Middle menu
        rosalila()->graphics->draw2DImage
        (   stage_images[current_stage],
            stage_images[current_stage]->getWidth(),
                stage_images[current_stage]->getHeight(),
            rosalila()->graphics->screen_width/2-stage_images[current_stage]->getWidth()/2,
                rosalila()->graphics->screen_height/2-stage_images[current_stage]->getHeight()/2+middle_menu_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());


        if(frame%60>=0 && frame%60<30)
        {
            if(current_stage>0)
            {
                rosalila()->graphics->draw2DImage
                (   left_arrow,
                    left_arrow->getWidth(),left_arrow->getHeight(),
                    rosalila()->graphics->screen_width/2-stage_images[current_stage]->getWidth()/2-left_arrow->getWidth(),
                    rosalila()->graphics->screen_height/2-left_arrow->getHeight()/2+middle_menu_y,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());
            }

            if(current_stage<(int)stage_images.size()-1)
            {
                rosalila()->graphics->draw2DImage
                (   right_arrow,
                    right_arrow->getWidth(),right_arrow->getHeight(),
                    rosalila()->graphics->screen_width/2+stage_images[current_stage]->getWidth()/2,
                    rosalila()->graphics->screen_height/2-right_arrow->getHeight()/2+middle_menu_y,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());
            }

            if(entry_navigator>-6)
            {
                rosalila()->graphics->draw2DImage
                (   up_arrow,
                    up_arrow->getWidth(),up_arrow->getHeight(),
                    rosalila()->graphics->screen_width/2-up_arrow->getWidth()/2,
                    0,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());
            }

            if(entry_navigator<=0 || entry_navigator<current_leaderboard->friends_entries.size())
            {
                rosalila()->graphics->draw2DImage
                (   down_arrow,
                    down_arrow->getWidth(),down_arrow->getHeight(),
                    rosalila()->graphics->screen_width/2-down_arrow->getWidth()/2,
                    rosalila()->graphics->screen_height-down_arrow->getHeight(),
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());
            }
        }

        if(current_leaderboard)
        {
            //Bottom menu
            for(int i=0;i<current_leaderboard->friends_entries.size();i++)
            {
                LeaderboardEntry* current_entry = current_leaderboard->friends_entries[i];
                string entry_text = rosalila()->utility->toString(current_entry->rank)+"." + current_entry->name + " " +rosalila()->utility->toString(current_entry->score);
                rosalila()->graphics->drawText(entry_text, 0, bottom_menu_y+i*separation, true, false);
            }
        }

        rosalila()->graphics->drawText(stage_names[current_stage], 0, 0, true, false);


        rosalila()->update();

        frame++;
    }
}
