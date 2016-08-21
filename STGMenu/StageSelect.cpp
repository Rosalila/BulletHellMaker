#include "StageSelect.h"

std::vector<std::string> getStageNames()
{
    std::vector<std::string> stage_names;

    string config_directory = assets_directory+"config.xml";
    TiXmlDocument config_ducment((char*)config_directory.c_str());
    config_ducment.LoadFile();

    TiXmlNode *node_stages=config_ducment.FirstChild("ConfigFile")->FirstChild("Stages");

    for(TiXmlNode* node_stage=node_stages->FirstChild("stage");
            node_stage!=NULL;
            node_stage=node_stage->NextSibling("stage"))
    {
        stage_names.push_back(std::string(node_stage->ToElement()->Attribute("name")));
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
        if(getRosalilaSound()->current_music!=assets_directory+"1.ogg")
            getRosalilaSound()->playMusic(assets_directory+"1.ogg",-1);
    }
    if(current_stage>=5 && current_stage<=9)
    {
        if(getRosalilaSound()->current_music!=assets_directory+"2.ogg")
            getRosalilaSound()->playMusic(assets_directory+"2.ogg",-1);
    }
    if(current_stage>=10 && current_stage<=14)
    {
        if(getRosalilaSound()->current_music!=assets_directory+"3.ogg")
            getRosalilaSound()->playMusic(assets_directory+"3.ogg",-1);
    }
    if(current_stage>=15 && current_stage<=19)
    {
        if(getRosalilaSound()->current_music!=assets_directory+"4.ogg")
            getRosalilaSound()->playMusic(assets_directory+"4.ogg",-1);
    }
    if(current_stage>=20 && current_stage<=24)
    {
        if(getRosalilaSound()->current_music!=assets_directory+"5.ogg")
            getRosalilaSound()->playMusic(assets_directory+"5.ogg",-1);
    }
}

std::vector<Image*> getStageImages(std::vector<std::string> stage_names)
{
    std::vector<Image*> stage_images;
    for(int i=0;i<(int)stage_names.size();i++)
    {
        Image*image=getRosalilaGraphics()->getTexture(assets_directory+std::string("stages/")+stage_names[i]+std::string("/images/preview.png"));
        stage_images.push_back(image);
    }
    return stage_images;
}

void stageSelect(map<string,Button*> controls)
{
    RosalilaGraphics* graphics=getRosalilaGraphics();
    Receiver* receiver=getReceiver();

    std::vector<std::string> stage_names = getStageNames();
    std::vector<Image*> stage_images = getStageImages(getStageNames());

    Image*background=graphics->getTexture(assets_directory+"menu/white_background.png");
    Image*left_arrow=graphics->getTexture(assets_directory+"menu/left_arrow.png");
    Image*right_arrow=graphics->getTexture(assets_directory+"menu/right_arrow.png");

    Color background_color(255,255,255,255);

    int current_stage = 0;
    int frame = 0;

    while(true)
    {
        if(receiver->isKeyPressed(SDLK_ESCAPE))
        {
            break;
        }

        if(controls["a"]->isPressed())
        {
            writeLogLine("Initializing game.");
            Stage*stage=new Stage(graphics,getRosalilaSound(),receiver);
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
            Player*player=new Player(getRosalilaSound(),graphics,receiver,"Triangle",10,controls);
            Enemy*enemy=new Enemy(getRosalilaSound(),graphics,receiver,stage_names[current_stage],player,20);
            STG*stg=new STG(getRosalilaSound(),graphics,receiver,player,enemy,stage,game_mode,controls);
            delete stg;
        }


        if(controls["6"]->isPressed())
        {
            current_stage++;
            if(current_stage>=(int)stage_images.size())
                current_stage=stage_images.size()-1;
        }

        if(controls["4"]->isPressed())
        {
            current_stage--;
            if(current_stage<0)
                current_stage=0;
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
            stage_images[current_stage]->getWidth(),stage_images[current_stage]->getHeight(),
            graphics->screen_width/2-stage_images[current_stage]->getWidth()/2,graphics->screen_height/2-stage_images[current_stage]->getHeight()/2,
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
                    graphics->screen_height/2-left_arrow->getHeight()/2,
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
                    graphics->screen_height/2-right_arrow->getHeight()/2,
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

        receiver->updateInputs();
        graphics->updateScreen();

        frame++;
    }
}
