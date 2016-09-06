#include "StageSelect.h"

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

void stageSelect(map<string,Button*> controls)
{
    RosalilaGraphics* graphics=Rosalila()->Graphics;
    std::vector<std::string> stage_names = getStageNames();
    std::vector<Image*> stage_images = getStageImages(getStageNames());

    Image*background=graphics->getTexture(assets_directory+"menu/white_background.png");
    Image*left_arrow=graphics->getTexture(assets_directory+"menu/left_arrow.png");
    Image*right_arrow=graphics->getTexture(assets_directory+"menu/right_arrow.png");

    Color background_color(255,255,255,255);

    int current_stage = Rosalila()->ApiIntegrator->getStat("current stage");
    int frame = 0;
    int entry_selected = -1;

    bool select_button_was_up = false;

    Rosalila()->Receiver->updateInputs();

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
            entry_selected=-1;
            if(current_stage>=(int)stage_images.size())
                current_stage=stage_images.size()-1;
            Rosalila()->ApiIntegrator->setStat("current stage",current_stage);
        }

        if(controls["4"]->isPressed())
        {
            current_stage--;
            entry_selected=-1;
            if(current_stage<0)
                current_stage=0;
            Rosalila()->ApiIntegrator->setStat("current stage",current_stage);
        }

        Leaderboard* current_leaderboard = Rosalila()->ApiIntegrator->getLeaderboard(stage_names[current_stage]);

        if(controls["2"]->isPressed())
        {
            entry_selected++;
            if(entry_selected >= current_leaderboard->entries.size())
            {
                entry_selected = current_leaderboard->entries.size()-1;
            }
        }
        if(controls["8"]->isPressed())
        {
            entry_selected--;
            if(entry_selected<-1)
            {
               entry_selected=-1;
            }
        }

        bool error_found = false;

        if(controls["a"]->isDown() && select_button_was_up)
        {
            Rosalila()->Utility->writeLogLine("Initializing game.");
            Stage*stage=new Stage();
            stage->loadFromXML(stage_names[current_stage]);
            string game_mode="Stage select";
            if(stage_names[current_stage]=="Training3")
            {
                game_mode="charge training";
            }
            if(stage_names[current_stage]=="Training4")
            {
                game_mode="parry training";
            }
            if(stage_names[current_stage]=="Training5")
            {
                game_mode="parry dash training";
            }

            Rosalila()->Utility->setRandomSeed(rand());
            vector<string>replay_input;
            if(entry_selected!=-1)
            {
                game_mode="replay";
                LeaderboardEntry* le = Rosalila()->ApiIntegrator->getLeaderboard(stage_names[current_stage])->entries[entry_selected];

                Rosalila()->ApiIntegrator->downloadEntryAttachment(
                        Rosalila()->ApiIntegrator->getLeaderboard(stage_names[current_stage])->entries[entry_selected]);

                while(Rosalila()->ApiIntegrator->getLeaderboard(stage_names[current_stage])->entries[entry_selected]->attachment_state!="loaded"
                      && Rosalila()->ApiIntegrator->getState()=="loading")
                {
                    Rosalila()->ApiIntegrator->updateCallbacks();
                    SDL_Delay(17);
                }
                if(Rosalila()->ApiIntegrator->getState()!="error")
                {
                    char* replay_data = Rosalila()->ApiIntegrator->getLeaderboard(stage_names[current_stage])->entries[entry_selected]->attachment;

                    int replay_size = Rosalila()->ApiIntegrator->getLeaderboard(stage_names[current_stage])->entries[entry_selected]->attachment_size;

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
                }else
                {
                    error_found=true;
                    cout<<"Error"<<endl;
                }
            }

            if(!error_found)
            {
                int current_player_best_score = -1;
                if(current_leaderboard->leaderboard_self_entry!=NULL)
                    current_player_best_score = current_leaderboard->leaderboard_self_entry->score;

                Player*player=new Player("Triangle",10,controls,replay_input);
                Enemy*enemy=new Enemy(stage_names[current_stage],player,20);
                STG*stg=new STG(player,enemy,stage,game_mode,controls,current_player_best_score);
                delete stg;
                select_button_was_up=false;
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

        graphics->draw2DImage
        (   stage_images[current_stage],
            stage_images[current_stage]->getWidth(),
                stage_images[current_stage]->getHeight(),
            graphics->screen_width/2-stage_images[current_stage]->getWidth()/2,
                graphics->screen_height/2-stage_images[current_stage]->getHeight()/2-300,
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
                    graphics->screen_height/2-left_arrow->getHeight()/2-300,
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
                    graphics->screen_height/2-right_arrow->getHeight()/2-300,
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

        for(int i=0;i<current_leaderboard->entries.size();i++)
        {
            int selecituu=400;
            if(entry_selected==i)
            {
                selecituu=450;
                graphics->drawText("-->",
                                   450,
                                   200+i*50);
            }
            graphics->drawText(Rosalila()->Utility->toString(current_leaderboard->entries[i]->rank)+".",
                               selecituu+70,
                               200+i*50);
            graphics->drawText(current_leaderboard->entries[i]->name,
                               selecituu+100,
                               200+i*50);
            graphics->drawText(Rosalila()->Utility->toString(current_leaderboard->entries[i]->score),
                               selecituu+100+200,
                               200+i*50);
        }

        Rosalila()->Receiver->updateInputs();
        graphics->updateScreen();
        Rosalila()->ApiIntegrator->updateCallbacks();

        frame++;
    }
}
