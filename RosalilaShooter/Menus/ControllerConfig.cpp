#include "ControllerConfig.h"

map<string,Button*> getExistingConfiguration(string controls_file_retrived)
{
    map<string,Button*>controls;

    int i=0;
    while(i<(int)controls_file_retrived.length())
    {
        char maped = controls_file_retrived[i];
        i++;
        char type = controls_file_retrived[i];
        i++;
        string number_key = "";
        while(controls_file_retrived[i]!=',' && i<(int)controls_file_retrived.length())
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
    string controls_file_retrived = rosalila()->api_integrator->getData("controls");
    if(controls_file_retrived!="" && !reconfigure)
    {
        return getExistingConfiguration(controls_file_retrived);
    }

    Image* controls_config_backgound = rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/background.png");
    Image* player_image = rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/player.png");
    Image* bullet_image = rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/bullet.png");
    vector<Image*> icon_images;
    icon_images.push_back(rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/icons/up_arrow.png"));
    icon_images.push_back(rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/icons/down_arrow.png"));
    icon_images.push_back(rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/icons/left_arrow.png"));
    icon_images.push_back(rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/icons/right_arrow.png"));
    icon_images.push_back(rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/icons/player.png"));
    icon_images.push_back(rosalila()->graphics->getTexture(assets_directory+"misc/controls configuration/icons/back.png"));
    int player_image_initial_x = rosalila()->graphics->screen_width/2-player_image->getWidth()/2;
    int player_image_initial_y = rosalila()->graphics->screen_height/2-player_image->getHeight()/2;
    int player_image_x = player_image_initial_x;
    int player_image_y = player_image_initial_y;
    int bullet_initial_x = rosalila()->graphics->screen_width/2-bullet_image->getWidth()/2 + 25;
    int bullet_image_x = bullet_initial_x;
    int bullet_image_y = rosalila()->graphics->screen_height/2-bullet_image->getHeight()/2;
    vector<string> controls_config_map_name;
    controls_config_map_name.push_back("8");
    controls_config_map_name.push_back("2");
    controls_config_map_name.push_back("4");
    controls_config_map_name.push_back("6");
    controls_config_map_name.push_back("a");
    controls_config_map_name.push_back("b");

    map<string,Button*>controls;
    int current_button=0;

    int frame=0;
    while(true)
    {
        int key_pressed = -1;
        int joy_pressed = -1;

        if(controls_config_map_name[current_button]!="b" && rosalila()->receiver->isKeyPressed(SDLK_ESCAPE))
        {
            rosalila()->sound->playSound("Menu.down",0,0,0,false);
            if(current_button<=0)
                exit(0);
            current_button--;
            controls.erase(controls.find(controls_config_map_name[current_button]));
            player_image_x = player_image_initial_x;
            player_image_y = player_image_initial_y;

            continue;
        }
        for(int i=0/*SDLK_a*/;i<=255/*SDLK_z*/;i++)
        {
            if(rosalila()->receiver->isKeyPressed(i))
            {
                key_pressed=i;
                player_image_x = player_image_initial_x;
                player_image_y = player_image_initial_y;
            }
        }

        for(int i=0;i<100;i++)
        {
            if(rosalila()->receiver->isJoyPressed(i,0))
            {
                joy_pressed=i;
                player_image_x = player_image_initial_x;
                player_image_y = player_image_initial_y;
            }
        }
        if(rosalila()->receiver->isJoyPressed(-2,0))
        {
            joy_pressed=-2;
            player_image_x = player_image_initial_x;
            player_image_y = player_image_initial_y;
        }
        if(rosalila()->receiver->isJoyPressed(-8,0))
        {
            joy_pressed=-8;
            player_image_x = player_image_initial_x;
            player_image_y = player_image_initial_y;

        }
        if(rosalila()->receiver->isJoyPressed(-4,0))
        {
            joy_pressed=-4;
            player_image_x = player_image_initial_x;
            player_image_y = player_image_initial_y;

        }
        if(rosalila()->receiver->isJoyPressed(-6,0))
        {
            joy_pressed=-6;
            player_image_x = player_image_initial_x;
            player_image_y = player_image_initial_y;

        }

        if(key_pressed!=-1)
        {
            bool key_exists = false;
            map<string,Button*>::iterator control_iterator=controls.begin();
            for(int i=0;i<current_button;i++)
            {
                if((*control_iterator).second->key==key_pressed)
                {
                    rosalila()->sound->playSound("Menu.invalid",0,0,0,0);
                    key_exists = true;
                    rosalila()->graphics->notification_handler.notifications.push_back(
                        new Notification(getErrorImage(), rosalila()->graphics->screen_width/2-getErrorImage()->getWidth()/2,
                                            rosalila()->graphics->screen_height,
                                            rosalila()->graphics->screen_height-getErrorImage()->getHeight(),
                                            getNotificationDuration()));
                    break;
                }
                control_iterator++;
            }
            if(!key_exists)
            {
                rosalila()->sound->playSound("Menu.up",0,0,0,0);
                string current_button_map = controls_config_map_name[current_button];
                controls[current_button_map]=new Button(key_pressed,current_button_map);
                current_button++;
                if(current_button>=(int)controls_config_map_name.size())
                    break;
            }
        }
        if(joy_pressed!=-1)
        {
            bool key_exists = false;
            map<string,Button*>::iterator control_iterator=controls.begin();
            for(int i=0;i<current_button;i++)
            {
                if((*control_iterator).second->joystick_button==joy_pressed)
                {
                    key_exists = true;
                    rosalila()->graphics->notification_handler.notifications.push_back(
                        new Notification(getErrorImage(), rosalila()->graphics->screen_width/2-getErrorImage()->getWidth()/2,
                                            rosalila()->graphics->screen_height,
                                            rosalila()->graphics->screen_height-getErrorImage()->getHeight(),
                                            getNotificationDuration()));
                    break;
                }
                control_iterator++;
            }
            if(!key_exists)
            {
                rosalila()->sound->playSound("Menu.up",0,0,0,0);
                string current_button_map = controls_config_map_name[current_button];
                controls[current_button_map]=new Button(joy_pressed,0,current_button_map);
                current_button++;
                if(current_button>=(int)controls_config_map_name.size())
                    break;
            }
        }

        rosalila()->graphics->draw2DImage
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

        int animation_max_distance = 150;
        int center_separation_x = 0;
        int center_separation_y = 0;

        if(current_button==0)
        {
            player_image_y -= 3;
            center_separation_y = -50;
            if(player_image_y<player_image_initial_y-animation_max_distance)
                player_image_y = player_image_initial_y;
        }
        if(current_button==1)
        {
            player_image_y += 3;
            center_separation_y = 50;
            if(player_image_y>player_image_initial_y+animation_max_distance)
                player_image_y = player_image_initial_y;
        }
        if(current_button==2)
        {
            player_image_x -= 3;
            center_separation_x = -50;
            if(player_image_x<player_image_initial_x-animation_max_distance)
                player_image_x = player_image_initial_x;
        }
        if(current_button==3)
        {
            player_image_x += 3;
            center_separation_x = 50;
            if(player_image_x>player_image_initial_x+animation_max_distance)
                player_image_x = player_image_initial_x;
        }

        if(current_button<4)
        rosalila()->graphics->draw2DImage
        (   player_image,
            player_image->getWidth(),player_image->getHeight(),
            player_image_x+center_separation_x,player_image_y+center_separation_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        if(current_button==4)
        {
            bullet_image_x+=3;
            if(bullet_image_x>bullet_initial_x + animation_max_distance)
                bullet_image_x = bullet_initial_x;
            rosalila()->graphics->draw2DImage
            (   bullet_image,
                bullet_image->getWidth(),bullet_image->getHeight(),
                bullet_image_x,bullet_image_y,
                1.0,
                0.0,
                false,
                0,0,
                Color(255,255,255,255),
                0,0,
                false,
                FlatShadow());
        }

        rosalila()->graphics->draw2DImage
        (   icon_images[current_button],
            icon_images[current_button]->getWidth(),icon_images[current_button]->getHeight(),
            rosalila()->graphics->screen_width/2-icon_images[current_button]->getWidth()/2,
                rosalila()->graphics->screen_height/2-icon_images[current_button]->getHeight()/2,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false,
            FlatShadow());

        rosalila()->update();
        frame++;
    }

    rosalila()->sound->playSound("Menu.confirm",0,0,0,0);

    string controls_file = "";

    for(map<string,Button*>::iterator i=controls.begin();i!=controls.end();i++)
    {
        controls_file+=(*i).first;
        if((*i).second->uses_joystick)
        {
            controls_file+="J";
            controls_file+=rosalila()->utility->toString((*i).second->joystick_button)+",";
        }else
        {
            controls_file+="K";
            controls_file+=rosalila()->utility->toString((*i).second->key)+",";
        }
    }
    rosalila()->api_integrator->storeData("controls",controls_file);

    return controls;
}
