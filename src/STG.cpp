#include "../include/STG.h"

STG::STG(Sonido* sonido,Painter* painter,Receiver* receiver,Character*character,Stage*stage)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->character=character;
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

    painter->camera_x+=stage->getVelocity();
    character->logic(stage->getVelocity());
    character->setX(character->getX()+stage->getVelocity());

//    enemy->animationControl();
//    enemy->spellControl(stage->getVelocity());
//    enemy->setX(enemy->getX()+stage->getVelocity());
//
//
//    enemy->setY(enemy->getY()-6);
//    if(enemy->getY()<0)
//        enemy->setY(800);

    deletePatterns(stage->getBoundX1(),stage->getBoundY1(),stage->getBoundX2(),stage->getBoundY2());
    checkCharacterOutOfBounds();
}

void STG::render()
{
    stage->dibujarBack();
    character->render();

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
    std::list<Pattern*>* active_patterns=character->getActivePatterns();
    std::list<Pattern*>::iterator i = active_patterns->begin();
    while (i != active_patterns->end())
    {
        Pattern*p=(Pattern*)*i;
        if (isOutOfBounds(p->getX(),p->getY()))
        {
            active_patterns->erase(i++);
        }
        else
        {
            ++i;
        }
    }


//    std::list<Pattern*>* active_enemy_patterns=enemy->getActivePatterns();
//    i = active_enemy_patterns->begin();
//    while (i != active_enemy_patterns->end())
//    {
//        Pattern*p=(Pattern*)*i;
//        if (isOutOfBounds(p->getX(),p->getY()))
//        {
//            active_enemy_patterns->erase(i++);
//        }
//        else
//        {
//            ++i;
//        }
//    }
}

void STG::checkCharacterOutOfBounds()
{
    if(character->getX()<stage->getBoundX1()+painter->camera_x)
        character->setX(stage->getBoundX1()+painter->camera_x);
    if(character->getX()>stage->getBoundX2()+painter->camera_x)
        character->setX(stage->getBoundX2()+painter->camera_x);
    if(character->getY()<stage->getBoundY1())
        character->setY(stage->getBoundY1());
    if(character->getY()>stage->getBoundY2())
        character->setY(stage->getBoundY2());
}
