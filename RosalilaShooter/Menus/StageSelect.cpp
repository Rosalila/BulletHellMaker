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
        Rosalila()->Utility->setRandomSeed(random_seed);
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
    Node* root_node = Rosalila()->Parser->getNodes(assets_directory+"config.xml");

    vector<Node*> stage_nodes = root_node->getNodeByName("Stages")->getNodesByName("stage");

    for(int i=0;i<(int)stage_nodes.size();i++)
    {
        stage_names.push_back(stage_nodes[i]->attributes["name"]);
    }

    return stage_names;
}

Color getBackgroundColor(int current_stage)
{
    if(current_stage>=0 && current_stage<=4)
    {
        return Color(33,150,243,255);
    }
    if(current_stage>=5 && current_stage<=9)
    {
        return Color(139,195,74,255);
    }
    if(current_stage>=10 && current_stage<=14)
    {
        return Color(103,58,183,255);
    }
    if(current_stage>=15 && current_stage<=19)
    {
        return Color(255,152,0,255);
    }
    if(current_stage>=20 && current_stage<=24)
    {
        return Color(244,67,54,255);
    }

    return Color();
}

void updateMusic(int current_stage)
{
    if(current_stage>=0 && current_stage<=4)
    {
        if(Rosalila()->Sound->current_music!=assets_directory+"1.ogg")
            Rosalila()->Sound->playMusic(assets_directory+"1.ogg",-1);
    }
    if(current_stage>=5 && current_stage<=9)
    {
        if(Rosalila()->Sound->current_music!=assets_directory+"2.ogg")
            Rosalila()->Sound->playMusic(assets_directory+"2.ogg",-1);
    }
    if(current_stage>=10 && current_stage<=14)
    {
        if(Rosalila()->Sound->current_music!=assets_directory+"3.ogg")
            Rosalila()->Sound->playMusic(assets_directory+"3.ogg",-1);
    }
    if(current_stage>=15 && current_stage<=19)
    {
        if(Rosalila()->Sound->current_music!=assets_directory+"4.ogg")
            Rosalila()->Sound->playMusic(assets_directory+"4.ogg",-1);
    }
    if(current_stage>=20 && current_stage<=24)
    {
        if(Rosalila()->Sound->current_music!=assets_directory+"5.ogg")
            Rosalila()->Sound->playMusic(assets_directory+"5.ogg",-1);
    }
}

std::vector<Image*> getStageImages(std::vector<std::string> stage_names)
{
    std::vector<Image*> stage_images;
    for(int i=0;i<(int)stage_names.size();i++)
    {
        Image*image=Rosalila()->Graphics->getTexture(assets_directory+std::string("stages/")+stage_names[i]+std::string("/images/preview.png"));
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

void stageSelect(map<string,Button*> controls)
{
    RosalilaGraphics* graphics=Rosalila()->Graphics;
    std::vector<std::string> stage_names = getStageNames();
    std::vector<Image*> stage_images = getStageImages(getStageNames());

    Image*background=graphics->getTexture(assets_directory+"menu/white_background.png");
    Image*left_arrow=graphics->getTexture(assets_directory+"menu/left_arrow.png");
    Image*right_arrow=graphics->getTexture(assets_directory+"menu/right_arrow.png");
    Image*line=graphics->getTexture(assets_directory+"menu/line.png");

    Color background_color(255,255,255,255);

    int current_stage = Rosalila()->ApiIntegrator->getStat("current stage");
    int frame = 0;
    int entry_navigator = 0;
    LeaderboardEntry* selected_entry = NULL;

    bool select_button_was_up = false;

    Rosalila()->Receiver->updateInputs();

    double menu_displacement_velocity = 20;
    double entry_height = 50;

    double top_menu_y = -6*entry_height/2 - 200;
    double middle_menu_y = 0;
    double bottom_menu_y = Rosalila()->Graphics->screen_height;

    double target_top_menu_y = Rosalila()->Graphics->screen_height/2 - 6*entry_height/2;
    double target_middle_menu_y = 0;
    double target_bottom_menu_y = Rosalila()->Graphics->screen_height;

    while(true)
    {
        if(Rosalila()->Receiver->isKeyPressed(SDLK_ESCAPE))
        {
            break;
        }

        if(!controls["a"]->isDown())
        {
            select_button_was_up = true;
        }

        if(controls["6"]->isPressed())
        {
            current_stage++;
            entry_navigator=0;
            if(current_stage>=(int)stage_images.size())
                current_stage=stage_images.size()-1;
            Rosalila()->ApiIntegrator->setStat("current stage",current_stage);
        }

        if(controls["4"]->isPressed())
        {
            current_stage--;
            entry_navigator=0;
            if(current_stage<0)
                current_stage=0;
            Rosalila()->ApiIntegrator->setStat("current stage",current_stage);
        }

        Leaderboard* current_leaderboard = Rosalila()->ApiIntegrator->getLeaderboard(stage_names[current_stage]);

        if(controls["2"]->isPressed())
        {
            entry_navigator++;
            if(entry_navigator > (int)current_leaderboard->friends_entries.size())
            {
                entry_navigator = current_leaderboard->friends_entries.size();
            }
        }
        if(controls["8"]->isPressed())
        {
            entry_navigator--;
            if(entry_navigator<-6)
            {
               entry_navigator=-6;
            }
        }

        target_bottom_menu_y = Rosalila()->Graphics->screen_height/2 - current_leaderboard->friends_entries.size()*entry_height/2;

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
        bottom_menu_y = std::max(bottom_menu_y,(double)-Rosalila()->Graphics->screen_height);

        top_menu_y = std::min(top_menu_y,(double)Rosalila()->Graphics->screen_height);
        middle_menu_y = std::min(middle_menu_y,(double)Rosalila()->Graphics->screen_height);
        bottom_menu_y = std::min(bottom_menu_y,(double)Rosalila()->Graphics->screen_height);


        selected_entry = getSelectedEntry(current_leaderboard, entry_navigator);

        bool error_found = false;

        if(controls["a"]->isDown() && select_button_was_up)
        {
            Rosalila()->Utility->writeLogLine("Initializing game.");
            Rosalila()->Utility->setRandomSeed(time(NULL));
            vector<string>replay_input;
            vector<string>intro_input;

            string game_mode="Stage select";

            if(stage_names[current_stage]=="Training1")
            {
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training1");
                Rosalila()->Graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training2")
            {
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training2");
                Rosalila()->Graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training3")
            {
                game_mode="charge training";
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training3");
                Rosalila()->Graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training4")
            {
                game_mode="parry training";
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training4");
                Rosalila()->Graphics->grayscale_effect.set(0,1);
            }
            if(stage_names[current_stage]=="Training5")
            {
                game_mode="parry dash training";
                intro_input = getReplayInput(assets_directory+"misc/training/intros/Training5");
                Rosalila()->Graphics->grayscale_effect.set(0,1);
            }

            Rosalila()->Graphics->transparency_effect.set(0,1);
            Rosalila()->Graphics->transparency_effect.set(1,0.03);

            if(selected_entry!=NULL)
            {
                game_mode="replay";

                Rosalila()->ApiIntegrator->downloadEntryAttachment(selected_entry);

                Rosalila()->Graphics->notification_handler.notifications.push_back(
                    new Notification(getLoadingImage(), Rosalila()->Graphics->screen_width/2-getLoadingImage()->getWidth()/2,
                                        Rosalila()->Graphics->screen_height,
                                        Rosalila()->Graphics->screen_height-getLoadingImage()->getHeight(),
                                        999999));

                while(selected_entry->attachment_state!="loaded"
                      && Rosalila()->ApiIntegrator->getState()=="loading")
                {

                    Rosalila()->ApiIntegrator->updateCallbacks();
                    SDL_Delay(17);
                    graphics->updateScreen();
                }

                Rosalila()->Graphics->notification_handler.interruptCurrentNotification();

                if(Rosalila()->ApiIntegrator->getState()!="error")
                {
                    char* replay_data = selected_entry->attachment;

                    int replay_size = selected_entry->attachment_size;

                    replay_input = getReplayInput(replay_data, replay_size);

                }else
                {
                    Rosalila()->Graphics->notification_handler.notifications.push_back(
                        new Notification(getErrorImage(), Rosalila()->Graphics->screen_width/2-getErrorImage()->getWidth()/2,
                                            Rosalila()->Graphics->screen_height,
                                            Rosalila()->Graphics->screen_height-getErrorImage()->getHeight(),
                                            getNotificationDuration()));
                    error_found=true;
                    cout<<"Error"<<endl;
                    Rosalila()->Graphics->grayscale_effect.set(1,1);
                }
            }

            if(!error_found)
            {
                int current_player_best_score = -1;
                if(current_leaderboard->leaderboard_self_entry!=NULL)
                    current_player_best_score = current_leaderboard->leaderboard_self_entry->score;

                Stage*stage=new Stage();
                stage->loadFromXML(stage_names[current_stage]);
                Player*player=new Player("Triangle",10,controls,intro_input,replay_input);
                Enemy*enemy=new Enemy(stage_names[current_stage],player,20);
                STG*stg=new STG(player,enemy,stage,game_mode,controls,current_player_best_score);
                delete stg;
                delete player;
                delete enemy;
                delete stage;
                select_button_was_up=false;
                Rosalila()->Graphics->grayscale_effect.set(1,1);
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

        graphics->draw2DImage
        (   background,
            graphics->screen_width,graphics->screen_height,
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
        for(int i=0;i<current_leaderboard->top_entries.size();i++)
        {
            int align_x = 400;
            int align_y = top_menu_y;
            int separation = 50;
            LeaderboardEntry* current_entry = current_leaderboard->top_entries[i];
            if(selected_entry==current_entry)
            {
                graphics->draw2DImage
                (   line,
                    line->getWidth(),line->getHeight(),
                    graphics->screen_width/2-line->getWidth()/2,
                    top_menu_y+i*separation,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());

                graphics->draw2DImage
                (   line,
                    line->getWidth(),line->getHeight(),
                    graphics->screen_width/2-line->getWidth()/2,
                    top_menu_y+i*separation+line_separation,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());
            }
            string entry_text = Rosalila()->Utility->toString(current_entry->rank)+"." + current_entry->name + " " +Rosalila()->Utility->toString(current_entry->score);

            graphics->drawText(entry_text, 0, align_y+i*separation, true, false);
        }

        for(int i=0;i<current_leaderboard->near_entries.size();i++)
        {
            int align_x = 400;
            int align_y = 200+top_menu_y;
            int separation = 50;
            LeaderboardEntry* current_entry = current_leaderboard->near_entries[i];
            if(selected_entry==current_entry)
            {
                graphics->draw2DImage
                (   line,
                    line->getWidth(),line->getHeight(),
                    graphics->screen_width/2-line->getWidth()/2,
                    align_y+i*separation,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());

                graphics->draw2DImage
                (   line,
                    line->getWidth(),line->getHeight(),
                    graphics->screen_width/2-line->getWidth()/2,
                    align_y+i*separation+line_separation,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());
            }
            string entry_text = Rosalila()->Utility->toString(current_entry->rank)+"." + current_entry->name + " " +Rosalila()->Utility->toString(current_entry->score);

            graphics->drawText(entry_text, 0, align_y+i*separation, true, false);
        }

        //Middle menu
        graphics->draw2DImage
        (   stage_images[current_stage],
            stage_images[current_stage]->getWidth(),
                stage_images[current_stage]->getHeight(),
            graphics->screen_width/2-stage_images[current_stage]->getWidth()/2,
                graphics->screen_height/2-stage_images[current_stage]->getHeight()/2+middle_menu_y,
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
                graphics->draw2DImage
                (   left_arrow,
                    left_arrow->getWidth(),left_arrow->getHeight(),
                    graphics->screen_width/2-stage_images[current_stage]->getWidth()/2-left_arrow->getWidth(),
                    graphics->screen_height/2-left_arrow->getHeight()/2+middle_menu_y,
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
                graphics->draw2DImage
                (   right_arrow,
                    right_arrow->getWidth(),right_arrow->getHeight(),
                    graphics->screen_width/2+stage_images[current_stage]->getWidth()/2,
                    graphics->screen_height/2-right_arrow->getHeight()/2+middle_menu_y,
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

        //Bottom menu
        for(int i=0;i<current_leaderboard->friends_entries.size();i++)
        {
            int align_x = 400;
            int align_y = bottom_menu_y;
            int separation = 50;
            LeaderboardEntry* current_entry = current_leaderboard->friends_entries[i];
            if(selected_entry==current_entry)
            {
                align_x=450;
                graphics->draw2DImage
                (   line,
                    line->getWidth(),line->getHeight(),
                    graphics->screen_width/2-line->getWidth()/2,
                    bottom_menu_y+i*separation,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());

                graphics->draw2DImage
                (   line,
                    line->getWidth(),line->getHeight(),
                    graphics->screen_width/2-line->getWidth()/2,
                    bottom_menu_y+i*separation+line_separation,
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    0,0,
                    false,
                    FlatShadow());
            }

            string entry_text = Rosalila()->Utility->toString(current_entry->rank)+"." + current_entry->name + " " +Rosalila()->Utility->toString(current_entry->score);

            graphics->drawText(entry_text, 0, align_y+i*separation, true, false);
        }

        Rosalila()->Receiver->updateInputs();
        graphics->updateScreen();
        Rosalila()->ApiIntegrator->updateCallbacks();

        frame++;
    }
}
