#include "../include/STG.h"

STG::STG(Sonido* sonido,Painter* painter,Receiver* receiver,Player*player,Enemy*enemy,Stage*stage)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->player=player;
    this->enemy=enemy;
    this->stage=stage;
    mainLoop();
}

void STG::mainLoop()
{
    for (;;)
    {
        if(receiver->IsKeyDownn(SDLK_ESCAPE))
        {
            exit(0);
        }
        logic();
        render();
    }
}

void STG::logic()
{
    if(receiver->IsKeyPressed(SDLK_q))
    {
        stage->setVelocity(0);
    }
    if(receiver->IsKeyPressed(SDLK_w))
    {
        stage->setVelocity(5);
    }
    if(receiver->IsKeyPressed(SDLK_e))
    {
        stage->setVelocity(10);
    }
    if(receiver->IsKeyPressed(SDLK_r))
    {
        stage->setVelocity(50);
    }
    if(receiver->IsKeyPressed(SDLK_r))
    {
        stage->setVelocity(100);
    }
    if(receiver->IsKeyPressed(SDLK_t))
    {
        stage->setVelocity(200);
    }
    if(receiver->IsKeyPressed(SDLK_y))
    {
        stage->setVelocity(500);
    }



    if(receiver->IsKeyPressed(SDLK_1))
    {
        player->setType("1");
    }
    if(receiver->IsKeyPressed(SDLK_2))
    {
        player->setType("2");
    }
    if(receiver->IsKeyPressed(SDLK_3))
    {
        player->setType("3");
    }
    if(receiver->IsKeyPressed(SDLK_4))
    {
        player->setType("4");
    }
    if(receiver->IsKeyPressed(SDLK_5))
    {
        player->setType("5");
    }
    if(receiver->IsKeyPressed(SDLK_6))
    {
        player->setType("6");
    }
    if(receiver->IsKeyPressed(SDLK_7))
    {
        player->setType("7");
    }
    if(receiver->IsKeyPressed(SDLK_8))
    {
        player->setType("8");
    }
    if(receiver->IsKeyPressed(SDLK_9))
    {
        player->setType("9");
    }

    painter->camera_x+=stage->getVelocity();
    player->logic(stage->getVelocity());
    player->setX(player->getX()+stage->getVelocity());
    enemy->logic(stage->getVelocity());
    enemy->setX(enemy->getX()+stage->getVelocity());
    stage->logic();

    deletePatterns(stage->getBoundX1(),stage->getBoundY1(),stage->getBoundX2(),stage->getBoundY2());
    checkCharacterOutOfBounds();
}

void STG::render()
{
    stage->dibujarBack();
    player->render();
    enemy->render();

    stage->render();
    stage->dibujarFront();

    receiver->updateInputs();
    painter->updateScreen();
}

bool STG::isOutOfBounds(int pos_x,int pos_y)
{
    if(pos_x<stage->getBoundX1()+painter->camera_x
       ||pos_x>stage->getBoundX2()+painter->camera_x
       ||pos_y<stage->getBoundY1()
       ||pos_y>stage->getBoundY2())
    {
        return true;
    }
    return false;
}

void STG::deletePatterns(int stage_bound_x1,int stage_bound_y1,int stage_bound_x2,int stage_bound_y2)
{
    std::list<Pattern*>* active_patterns=player->getActivePatterns();
    std::list<Pattern*>::iterator i = active_patterns->begin();
    while (i != active_patterns->end())
    {
        Pattern*p=(Pattern*)*i;
        if (isOutOfBounds(p->getX(),p->getY()) || p->destroyFlag())
        {
            active_patterns->erase(i++);
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
