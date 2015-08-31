#include "STG.h"

STG::STG(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,Player*player,Enemy*enemy,Stage*stage)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->player=player;
    this->enemy=enemy;
    this->stage=stage;
    painter->camera_x=0;
    painter->camera_y=0;
    iteration=0;

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

    for(TiXmlNode* sprites_node=you_win_node->FirstChild("sprite");
            sprites_node!=NULL;
            sprites_node=sprites_node->NextSibling("sprite"))
    {
        std::string path=sprites_node->ToElement()->Attribute("path");
        you_win.addImage(painter->getTexture(assets_directory+path));
    }

    stage->playMusic();

    mainLoop();
}

void STG::mainLoop()
{
    bool end_key_up_keyboard=false;
    bool end_key_up_joystick=false;
    for (;;)
    {
        if(receiver->isKeyDown(SDLK_ESCAPE))
        {
            break;
        }

        render();
        logic();
        if(player->getHP()==0
           || enemy->getHP()==0)
        {
            if(receiver->isKeyPressed(SDLK_RETURN))
                break;
            if(receiver->isKeyPressed(SDLK_z) && end_key_up_keyboard)
                break;
            if(receiver->isJoyPressed(0,0) && end_key_up_joystick)
                break;
            if(!receiver->isKeyPressed(SDLK_z))
                end_key_up_keyboard=true;
            if(!receiver->isJoyPressed(0,0))
                end_key_up_joystick=true;

        }
        receiver->updateInputs();
    }
}

void STG::logic()
{
    if(receiver->isKeyPressed(SDLK_1))
    {
        player->setType("1");
    }
    if(receiver->isKeyPressed(SDLK_2))
    {
        player->setType("2");
    }
    if(receiver->isKeyPressed(SDLK_3))
    {
        player->setType("3");
    }
    if(receiver->isKeyPressed(SDLK_4))
    {
        player->setType("4");
    }
    if(receiver->isKeyPressed(SDLK_5))
    {
        player->setType("5");
    }
    if(receiver->isKeyPressed(SDLK_6))
    {
        player->setType("6");
    }
    if(receiver->isKeyPressed(SDLK_7))
    {
        player->setType("7");
    }
    if(receiver->isKeyPressed(SDLK_8))
    {
        player->setType("8");
    }
    if(receiver->isKeyPressed(SDLK_9))
    {
        player->setType("9");
    }

    int stage_displacement = stage->getVelocity();
    if(isSlowActive())
        stage_displacement/=3;
    painter->camera_x+=stage_displacement;
    player->logic(stage_displacement);
    player->setX(player->getX()+stage_displacement);
    enemy->logic(stage_displacement,stage->getName(),iteration,username);
    //enemy->setX(enemy->getX()+stage_displacement);
    stage->logic();

    deletePatterns();
    checkCharacterOutOfBounds();
    slowExtraControl();

    if(enemy->getHP()>0)
        iteration++;
}

void STG::render()
{
    stage->dibujarBack();
    player->render();
    enemy->render();

    stage->render();
    stage->dibujarFront();

    for (std::list<Pattern*>::iterator pattern = enemy->getActivePatterns()->begin(); pattern != enemy->getActivePatterns()->end(); pattern++)
    {
        Pattern*p=(Pattern*)*pattern;
        if(!p->isHit())
        {
            for(int i=0;i<(int)p->getBullet()->getHitboxes().size();i++)
            {
                p->getBullet()->getHitboxes()[i];
                Hitbox h=p->getBullet()->getHitboxes()[i]->getPlacedHitbox(p->getX(),p->getY(),p->getBulletAngle());
                if(player->collides(h,0,0,0))
                {
                    p->hit();
                    player->hit(p->getDamage());
                }
            }
        }
    }

    for (std::list<Pattern*>::iterator pattern = player->getActivePatterns()->begin(); pattern != player->getActivePatterns()->end(); pattern++)
    {
        Pattern*p=(Pattern*)*pattern;
        if(!p->isHit())
        {
            for(int i=0;i<(int)p->getBullet()->getHitboxes().size();i++)
            {
                p->getBullet()->getHitboxes()[i];
                Hitbox h=p->getBullet()->getHitboxes()[i]->getPlacedHitbox(p->getX(),p->getY(),p->getBulletAngle());
                if(enemy->collides(h,0,0,0))
                {
                    p->hit();
                    enemy->hit(p->getDamage());
                }
            }
        }
    }

    if(enemy->getHP()==0)
        you_win.render();
    if(player->getHP()==0)
        you_loose.render();

    painter->drawText("Time: "+toString(iteration),0,65);


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
