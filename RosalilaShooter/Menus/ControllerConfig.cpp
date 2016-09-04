#include "ControllerConfig.h"

map<string,Button*> getExistingConfiguration(string controls_file_retrived)
{
    map<string,Button*>controls;

    int i=0;
    while(i<controls_file_retrived.length())
    {
        char maped = controls_file_retrived[i];
        i++;
        char type = controls_file_retrived[i];
        i++;
        string number_key = "";
        while(controls_file_retrived[i]!=',' && i<controls_file_retrived.length())
        {
            number_key+=controls_file_retrived[i];
            i++;
        }
        i++;

        if(type=='K')
        {
            string maped_str = "";
            maped_str+=maped;
            int key_num = atoi(number_key.c_str());
            controls[maped_str]=new Button(key_num,maped_str);
        }
        if(type=='J')
        {
            string maped_str = "";
            maped_str+=maped;
            int joy_button = atoi(number_key.c_str());
            controls[maped_str]=new Button(joy_button,0,maped_str);
        }
    }
    return controls;
}

map<string,Button*> ControllerConfig(bool reconfigure)
{
    string controls_file_retrived = Rosalila()->ApiIntegrator->getData("controls");
    if(controls_file_retrived!="" && !reconfigure)
    {
        return getExistingConfiguration(controls_file_retrived);
    }

    RosalilaGraphics* graphics=Rosalila()->Graphics;
    Image* controls_config_backgound = graphics->getTexture(assets_directory+"misc/controls configuration/background.png");
    vector<string> controls_config_map_name;
    controls_config_map_name.push_back("8");
    controls_config_map_name.push_back("2");
    controls_config_map_name.push_back("4");
    controls_config_map_name.push_back("6");
    controls_config_map_name.push_back("a");
    vector<Image*> controls_config_press_images;
    controls_config_press_images.push_back(graphics->getTexture(assets_directory+"misc/controls configuration/press_up.png"));
    controls_config_press_images.push_back(graphics->getTexture(assets_directory+"misc/controls configuration/press_down.png"));
    controls_config_press_images.push_back(graphics->getTexture(assets_directory+"misc/controls configuration/press_left.png"));
    controls_config_press_images.push_back(graphics->getTexture(assets_directory+"misc/controls configuration/press_right.png"));
    controls_config_press_images.push_back(graphics->getTexture(assets_directory+"misc/controls configuration/press_shoot.png"));
    RosalilaReceiver* receiver = Rosalila()->Receiver;

    map<string,Button*>controls;
    int current_button=0;

    int frame=0;
    while(true)
    {
        int key_pressed = -1;
        int joy_pressed = -1;
        if(receiver->isKeyPressed(SDLK_ESCAPE))
        {
            current_button--;
            if(current_button<0)
                exit(0);
            continue;
        }
        for(int i=0/*SDLK_a*/;i<=255/*SDLK_z*/;i++)
        {
            if(receiver->isKeyPressed(i))
            {
                key_pressed=i;
            }
        }
        for(int i=0;i<20;i++)
        {
            if(receiver->isJoyPressed(i,0))
            {
                joy_pressed=i;
            }
        }
        if(receiver->isJoyPressed(-2,0))
            joy_pressed=-2;
        if(receiver->isJoyPressed(-8,0))
            joy_pressed=-8;
        if(receiver->isJoyPressed(-4,0))
            joy_pressed=-4;
        if(receiver->isJoyPressed(-6,0))
            joy_pressed=-6;

        if(key_pressed!=-1)
        {
            string current_button_map = controls_config_map_name[current_button];
            controls[current_button_map]=new Button(key_pressed,current_button_map);
            current_button++;
            if(current_button>=(int)controls_config_press_images.size())
                break;
        }
        if(joy_pressed!=-1)
        {
            string current_button_map = controls_config_map_name[current_button];
            controls[current_button_map]=new Button(joy_pressed,0,current_button_map);
            current_button++;
            if(current_button>=(int)controls_config_press_images.size())
                break;
        }

        graphics->draw2DImage
        (   controls_config_backgound,
            controls_config_backgound->getWidth(),controls_config_backgound->getHeight(),
            0,0,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        if(frame%60>=0 && frame%60<30)
        graphics->draw2DImage
        (   controls_config_press_images[current_button],
            controls_config_press_images[current_button]->getWidth(),
              controls_config_press_images[current_button]->getHeight(),
            graphics->screen_width/2-controls_config_press_images[current_button]->getWidth()/2,
              graphics->screen_height/2-controls_config_press_images[current_button]->getHeight(),
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        receiver->updateInputs();
        graphics->updateScreen();
        frame++;
    }

    string controls_file = "";

    for(map<string,Button*>::iterator i=controls.begin();i!=controls.end();i++)
    {
        controls_file+=(*i).first;
        if((*i).second->uses_joystick)
        {
            controls_file+="J";
            controls_file+=Rosalila()->Utility->toString((*i).second->joystick_button)+"\n";
        }else
        {
            controls_file+="K";
            controls_file+=Rosalila()->Utility->toString((*i).second->key)+",";
        }
    }
    Rosalila()->ApiIntegrator->storeData("controls",controls_file);

    return controls;
}
