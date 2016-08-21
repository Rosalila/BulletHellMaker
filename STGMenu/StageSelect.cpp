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

std::vector<Image*> getStageImages(std::vector<std::string> stage_names)
{
    std::vector<Image*> stage_images;
    for(int i=0;i<stage_names.size();i++)
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
            if(current_stage>=stage_images.size())
                current_stage=stage_images.size()-1;
        }

        if(controls["4"]->isPressed())
        {
            current_stage--;
            if(current_stage<0)
                current_stage=0;
        }

        graphics->draw2DImage
        (   background,
            graphics->screen_width,graphics->screen_height,
            0,0,
            1.0,
            0.0,
            false,
            0,0,
            Color(0,0,0,255),
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

            if(current_stage<stage_images.size()-1)
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
