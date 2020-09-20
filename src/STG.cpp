#include "STG.h"

STG::STG(Player *player, Enemy *enemy, Stage *stage, string game_mode, int current_player_best_score)
{
  this->player = player;
  this->enemy = enemy;
  this->stage = stage;
  this->game_mode = game_mode;
  this->current_player_best_score = current_player_best_score;

  this->you_win = rosalila()->graphics->getImage(std::string(assets_directory) + "misc/you_win.png");
  this->you_lose = rosalila()->graphics->getImage(std::string(assets_directory) + "misc/you_lose.png");

  this->api_state = "";

  setScore(0);

  this->game_over_timeout = 128;

  this->player_is_immortal = false;

  this->frame = 0;

  setGameOver(false);
  setIsFirstWin(false);
  mainLoop();
}

STG::~STG()
{
  if(you_win)
    delete you_win;
  if(you_lose)
    delete you_lose;
}

void STG::mainLoop()
{
  bool end_key_up_keyboard = false;

  initial_ticks = SDL_GetTicks();

  for (;;)
  {
    if (rosalila()->receiver->isPressed(0, "back") && api_state == "")
    {
      rosalila()->sound->playSound(std::string("Menu.back"), 1, 0, 0);
      break;
    }
    render();

    if (!logic())
    {
      rosalila()->sound->playSound(std::string("Menu.back"), 1, 0, 0);
      break;
    }

    rosalila()->update();

    if (getGameOver() && api_state == "")
    {
      game_over_timeout--;
      if (game_over_timeout < 0)
        game_over_timeout = 0;
      if (game_over_timeout == 0)
      {
        if (rosalila()->receiver->isKeyPressed(SDLK_RETURN) || (this->player->isDownWrapper("a") && end_key_up_keyboard))
        {
          break;
        }
        if (!rosalila()->receiver->isDown(0, "a"))
          end_key_up_keyboard = true;
      }
    }
  }
}

bool STG::logic()
{
  float distance_x = enemy->x - player->x;
  float distance_y = enemy->y - player->y;
  float distance = sqrt(distance_x * distance_x + distance_y * distance_y);
  float damage_level = 6 - distance / 250.0;

  for (std::list<Pattern *>::iterator pattern_iterator = enemy->active_patterns->begin(); pattern_iterator != enemy->active_patterns->end(); pattern_iterator++)
  {
    Pattern *pattern = (Pattern *)*pattern_iterator;
    if (pattern->is_hit)
      continue;
    for (int i = 0; i < (int)pattern->bullet->hitboxes.size(); i++)
    {
      Hitbox h = pattern->bullet->hitboxes[i]->getPlacedHitbox(pattern->x, pattern->y, pattern->getBulletAngle());
      if (pattern->collides_opponent && player->collides(h, 0, 0, 0))
      {
        //rosalila()->graphics->point_explosion_effect->explode(p->x, p->y, Color(255, 255, 255, 200), p->bullet->damage);
        pattern->hit(enemy->sound_channel_base + 1, false);
        if (!player_is_immortal)
        {
          player->hit(pattern->bullet->damage);
          if(pattern->bullet->damage)
            rosalila()->graphics->screen_shake_effect.set(20, 10, 0, 0);
        }
        //rosalila()->graphics->screen_shake_effect.set(30,15,rosalila()->graphics->camera_x,rosalila()->graphics->camera_y);
        if (rosalila()->sound->soundExists(player->name + ".hit"))
          rosalila()->sound->playSound(player->name + ".hit", 3, 0, player->x);
      }
    }
  }
  
  if(!getGameOver())
  {
    if (player->hp == 0)
    {
      lose();
    }
  }

  for (std::list<Pattern *>::iterator pattern_iterator = player->active_patterns->begin(); pattern_iterator != player->active_patterns->end(); pattern_iterator++)
  {
    Pattern *pattern = (Pattern *)*pattern_iterator;
    if (!pattern->is_hit)
    {
      for (int i = 0; i < (int)pattern->bullet->hitboxes.size(); i++)
      {
        Hitbox h = pattern->bullet->hitboxes[i]->getPlacedHitbox(pattern->x, pattern->y, pattern->getBulletAngle());
        if (pattern->collides_opponent && enemy->collides(h, 0, 0, 0))
        {
          pattern->hit(player->sound_channel_base + 1, false);
          enemy->hit(pattern->bullet->damage);
          player->current_slow += damage_level;
          //!!-!!
          enemy->addActivePattern(enemy->type["reward"][0], pattern->x, pattern->y);
          setScore(getScore()+2);
          enemy->shakeScreen(pattern->bullet->damage + damage_level * 3, pattern->bullet->damage + damage_level * 2);
          if (rosalila()->sound->soundExists(enemy->name + ".hit"))
            rosalila()->sound->playSound(enemy->name + ".hit", 1, 0, enemy->x);
          if (enemy->hp == 0)
            win();
        }
      }
    }
  }

  player->logic(0);
  enemy->logic(0, stage->name);
  stage->logic();

  deletePatterns();
  slowExtraControl();

  if (api_state == "uploading score")
  {
    if (rosalila()->api_integrator->getState() == "finished")
    {
      uploadReplay();
      rosalila()->utility->writeLogLine("uploading replay");
    }

    if (rosalila()->api_integrator->getState() == "error")
    {
      bool retry = uploadErrorLoop();
      if (retry)
        uploadScore();
      else
        return false;
    }
  }
  if (api_state == "uploading replay")
  {
    if (rosalila()->api_integrator->getState() == "finished")
    {
      findLeaderboard();
      rosalila()->utility->writeLogLine("finding leaderboard");
    }

    if (rosalila()->api_integrator->getState() == "error")
    {
      bool retry = uploadErrorLoop();
      if (retry)
        uploadReplay();
      else
        return false;
    }
  }

  if (api_state == "finding leaderboard")
  {
    if (rosalila()->api_integrator->getState() == "finished")
    {
      rosalila()->graphics->notification_handler.interruptCurrentNotification();
      rosalila()->graphics->notification_handler.notifications.push_back(
          new Notification(getSuccessImage(), rosalila()->graphics->screen_width / 2 - getSuccessImage()->getWidth() / 2,
                           rosalila()->graphics->screen_height,
                           rosalila()->graphics->screen_height - getSuccessImage()->getHeight(),
                           getNotificationDuration()));

      api_state = "";
      rosalila()->utility->writeLogLine("finished");
    }

    if (rosalila()->api_integrator->getState() == "error")
    {
      bool retry = uploadErrorLoop();
      if (retry)
        findLeaderboard();
      else
        return false;
    }
  }

  if (!getGameOver())
    frame++;
  return true;
}

void STG::render()
{
  stage->dibujarBack();
  if (player->hp > 0)
    player->bottomRender();
  //if (enemy->hp > 0)
    enemy->bottomRender();
  if (player->hp > 0)
    player->topRender();
  if (enemy->hp > 0)
    enemy->topRender();

  stage->dibujarFront();

  rosalila()->graphics->drawText(400,-20,rosalila()->utility->toString(getScore()));

  if (getGameOver())// && score != -1 && game_mode != "replay")
  {
    if(enemy->hp==0)
      rosalila()->graphics->drawImage(you_win, 0,0);
    if(player->hp==0)
        rosalila()->graphics->drawImage(you_lose, 0,0);
  }

  int tutorial_control_spacing_y = 100;
}

bool STG::isOutOfBounds(int pos_x, int pos_y)
{
  int bullet_bound_addition_x = (stage->bound_x2 - stage->bound_x1) / 2;
  int bullet_bound_addition_y = (stage->bound_y2 - stage->bound_y1) / 2;
  if(pos_x < stage->bound_x1 - 500)
    return true;
  if(pos_x > stage->bound_x2 + 500)
    return true;
  if(pos_y < stage->bound_y1 - 500)
    return true;
  if(pos_y > stage->bound_y2 + 500)
    return true;
  /*
    if(pos_x<stage->bound_x1+rosalila()->graphics->camera_x-bullet_bound_addition_x
       ||pos_x>stage->bound_x2+rosalila()->graphics->camera_x+bullet_bound_addition_x
       ||pos_y<stage->bound_y1-bullet_bound_addition_y
       ||pos_y>stage->bound_y2+bullet_bound_addition_y
       )
    {
        return true;
    }
    */
  return false;
}

void STG::deletePatterns()
{
  std::list<Pattern *> *active_patterns = player->active_patterns;
  std::list<Pattern *>::iterator i = active_patterns->begin();
  while (i != active_patterns->end())
  {
    Pattern *p = (Pattern *)*i;
    if (isOutOfBounds(p->x, p->y) || p->destroyFlag())
    {
      active_patterns->erase(i++);
      delete p;
    }
    else
    {
      ++i;
    }
  }

  active_patterns = enemy->active_patterns;
  i = active_patterns->begin();
  while (i != active_patterns->end())
  {
    Pattern *p = (Pattern *)*i;
    if (isOutOfBounds(p->x, p->y) || p->destroyFlag())
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

bool STG::playerWon()
{
  return enemy->hp == 0;
}

bool STG::enemyWon()
{
  return player->hp == 0;
}

void STG::win()
{
  int old_clears = rosalila()->api_integrator->getStat(stage->name + "Clears");
  if (game_mode != "replay")
    rosalila()->api_integrator->setStat(stage->name + "Clears", old_clears + 1);

  if (player->hp == player->max_hp)
  {
    int old_perfects = rosalila()->api_integrator->getStat(stage->name + "Perfects");
    if (game_mode != "replay")
      rosalila()->api_integrator->setStat(stage->name + "Perfects", old_perfects + 1);
  }

  setPlayerWon(true);
  setGameOver(true);
  enemy->onDefeated();
  setIsFirstWin(old_clears == 0);

  if (game_mode != "replay")
    rosalila()->api_integrator->unlockAchievement("B");
  double milliseconds = SDL_GetTicks() - initial_ticks;
  double hp_penalty = (1.0 + ((double)player->max_hp - (double)player->hp) / 100.0);
  setScore(getScore() + milliseconds * hp_penalty);
  enemy->hp = 0;
  //rosalila()->graphics->screen_shake_effect.set(50,20,rosalila()->graphics->camera_x,rosalila()->graphics->camera_y);
  rosalila()->sound->playSound("you win", 2, 0, 0);
  enemy->deleteActivePatterns();

  if (game_mode != "replay" && (getScore() < current_player_best_score || current_player_best_score == -1))
  {
    if (rosalila()->api_integrator->isUsingApi())
      uploadScore();
  }
}

void STG::lose()
{
  rosalila()->utility->writeLogLine("Player lost");
  setPlayerWon(false);
  setGameOver(true);
  rosalila()->sound->fadeMusicVolume(0, 2);
  if(rosalila()->sound->soundExists("you lose"))
    rosalila()->sound->playSound("you lose", 4, 0, 0);
}

void STG::uploadScore()
{
  api_state = "uploading score";
  rosalila()->utility->writeLogLine("uploading score");

  if (game_mode != "replay")
    rosalila()->api_integrator->setScore(stage->name, getScore());

  rosalila()->graphics->notification_handler.notifications.push_back(
      new Notification(getLoadingImage(), rosalila()->graphics->screen_width / 2 - getLoadingImage()->getWidth() / 2,
                       rosalila()->graphics->screen_height,
                       rosalila()->graphics->screen_height - getLoadingImage()->getHeight(),
                       999999));
}

void STG::uploadReplay()
{
  int replay_size = 0;
  string seed_str = rosalila()->utility->toString(rosalila()->utility->random_seed);
  replay_size += seed_str.size() + 1;
  for (int i = 0; i < (int)player->replay_storage.size(); i++)
  {
    replay_size += player->replay_storage[i].size() + 1;
  }
  replay_size += 1;

  char *replay_data = new char[replay_size];

  strcpy(replay_data, "");
  strcat(replay_data, seed_str.c_str());
  strcat(replay_data, "\n");

  for (int i = 0; i < (int)player->replay_storage.size(); i++)
  {
    strcat(replay_data, player->replay_storage[i].c_str());
    strcat(replay_data, "\n");
  }
  strcat(replay_data, "\0");

  rosalila()->api_integrator->storeLeaderboardAttachment(stage->name, replay_data, replay_size);

  api_state = "uploading replay";
}

bool STG::uploadErrorLoop()
{
  rosalila()->graphics->notification_handler.interruptCurrentNotification();
  rosalila()->graphics->notification_handler.notifications.push_back(
      new Notification(getErrorImage(), rosalila()->graphics->screen_width / 2 - getErrorImage()->getWidth() / 2,
                       rosalila()->graphics->screen_height,
                       rosalila()->graphics->screen_height - getErrorImage()->getHeight(),
                       getNotificationDuration()));

  api_state = "error";

  while (true)
  {
    if (rosalila()->receiver->isPressed(0, "a"))
    {
      return true;
    }

    if (rosalila()->receiver->isPressed(0, "back"))
    {
      return false;
    }
    rosalila()->update();
  }
}

void STG::findLeaderboard()
{
  rosalila()->api_integrator->findLeaderboard(stage->name);
  api_state = "finding leaderboard";
}
