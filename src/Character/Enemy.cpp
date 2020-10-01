#include "Character/Enemy.h"
#include "Utility/Utility.h"

double getPlayerDistanceX(Player* player, Pattern* pattern)
{
  return player->x - pattern->x;
}

double getPlayerDistanceY(Player* player, Pattern* pattern)
{
  return player->y - pattern->y;
}

Enemy::Enemy(std::string name, Player *player, int sound_channel_base, bool is_mod)
{
  //Setting up the other variables
  this->name = name;
  this->is_mod = is_mod;
  if (!is_mod)
    this->directory = "stages/" + name + "/Enemy/";
  else
    this->directory = "mods/stages/" + name + "/Enemy/";
  this->player = player;
  this->active_patterns = new std::list<Pattern *>;
  this->shooting = true;
  this->current_state = "start";
  this->current_type = "";
  this->visible = true;

  this->life_bar = NULL;

  this->bullet_cancel_count = 0;

  //Enemy variables
  this->angle = 180;
  this->velocity = 0;
  this->angle_change = 0;

  //Sprites animation
  this->animation_velocity = 4;
  this->animation_iteration = 0;
  this->current_sprite = 0;

  this->iteration = 0;

  //Color effect
  color_filter_red = 255;
  color_filter_green = 255;
  color_filter_blue = 255;
  color_filter_alpha = 255;

  //Shake
  current_screen_shake_x = 0;
  current_screen_shake_y = 0;
  shake_time = 0;
  shake_magnitude = 0;

  this->sound_channel_base = sound_channel_base;

  loadFromXML();

  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "character.json");

  Node *sounds_node = root_node->getNodeByName("sounds");
  if (sounds_node)
  {
    if (sounds_node->hasAttribute("bullet_cancel"))
      rosalila()->sound->addSound(this->name + ".bullet_cancel", std::string(assets_directory) + directory + sounds_node->attributes["bullet_cancel"]);
  }

  loadModifiersFromXML();
}

Enemy::~Enemy()
{
  for (map<int, vector<Modifier *>>::iterator i = modifiers.begin(); i != modifiers.end(); i++)
  {
    for (int j = 0; (int)j < (int)(*i).second.size(); j++)
    {
      delete (*i).second[j];
    }
  }
}

void Enemy::modifiersControl()
{
  bool flag_iterator_change = false;

  vector<Modifier *> current_modifiers = this->modifiers[iteration];
  for (auto modifier : current_modifiers)
  {
    if (modifier->variable == "velocity")
    {
      this->velocity = atoi(modifier->value.c_str());
    }
    if (modifier->variable == "angle")
    {
      this->angle = atoi(modifier->value.c_str());
    }
    if (modifier->variable == "angle_change")
    {
      this->angle_change = atoi(modifier->value.c_str());
    }
    if (modifier->variable == "iterator")
    {
      this->iteration = atoi(modifier->value.c_str());
      flag_iterator_change = true;
    }
    if (modifier->variable == "pattern_type")
    {
      //Reset cooldowns
      for (int i = 0; i < (int)type[current_type].size(); i++)
      {
        type[current_type][i]->state_manager = 0;
        type[current_type][i]->iteration = 0;
        type[current_type][i]->state = "startup";
      }

      //Clean screen
      for(std::list<Pattern *>::iterator i = this->active_patterns->begin(); i != this->active_patterns->end(); i++)
            (*i)->hit(this->sound_channel_base + 1, false);
      
      this->current_type = modifier->value;
    }
  }

  for (auto life_at_modifier_vectors : this->life_at_modifiers)
  {
    if (life_at_modifier_vectors.first < this->last_frame_hp &&
        life_at_modifier_vectors.first >= this->hp)
    {
      for (auto life_at_modifier : life_at_modifier_vectors.second)
      {
        if (life_at_modifier->variable == "velocity")
        {
          this->velocity = atoi(life_at_modifier->value.c_str());
        }
        if (life_at_modifier->variable == "angle")
        {
          this->angle = atoi(life_at_modifier->value.c_str());
        }
        if (life_at_modifier->variable == "angle_change")
        {
          this->angle_change = atoi(life_at_modifier->value.c_str());
        }
        if (life_at_modifier->variable == "iterator")
        {
          this->iteration = atoi(life_at_modifier->value.c_str());
          flag_iterator_change = true;
        }
        if (life_at_modifier->variable == "pattern_type")
        {
          this->onBulletCancel();
          this->current_type = life_at_modifier->value;
        }
      }
    }
  }
  last_frame_hp = hp;

  if (!flag_iterator_change && getIterateSlowdownFlag())
    iteration++;
}

void Enemy::logic(int stage_velocity, string stage_name)
{
  animationControl();

  for (auto pattern : type[current_type])
  {
    if(pattern->aim_player_on_begin && 
        (
          (pattern->state=="startup" && pattern->state_manager + pattern->repeat_startup_offset == pattern->startup)
          || (pattern->state=="cooldown" && pattern->state_manager + pattern->repeat_startup_offset == pattern->cooldown)
        )
      )
    {
      pattern->player_x_on_begin = player->x;
      pattern->player_y_on_begin = player->y;
    }
    if (shooting && this->hp != 0)
    {
      pattern->updateStateShouting();
      if (pattern->isReady())
      {
        pattern->bullet->playSound((int)(pattern->x + this->x), true);
        this->addActivePattern(pattern, this->x, this->y);
      }
    }
    else
    {
      pattern->updateStateNotShouting();
    }
  }
  
  if (color_filter_red < 255)
    color_filter_red++;
  if (color_filter_green < 255)
    color_filter_green++;
  if (color_filter_blue < 255)
    color_filter_blue++;
  if (color_filter_alpha < 255)
    color_filter_alpha++;

  player->additional_velocity_x = 0;
  player->additional_velocity_y = 0;
  player->additional_hp_change = 0;
  player->velocity_override = 0;

  for (std::list<Pattern *>::iterator iterator = active_patterns->begin(); iterator != active_patterns->end(); iterator++)
  {
    Pattern *active_pattern = (Pattern *)*iterator;
    active_pattern->logic(stage_velocity);

    if (active_pattern->homing)
    {
      active_pattern->angle = (float)(-atan2(getPlayerDistanceY(player, active_pattern), getPlayerDistanceX(player, active_pattern)) * 180 / PI);
      active_pattern->angle += (float)(active_pattern->homing_angle);
    }
    else if (active_pattern->getAimPlayer())
    {
      active_pattern->angle = (int)(active_pattern->angle - atan2(getPlayerDistanceY(player, active_pattern), getPlayerDistanceX(player, active_pattern)) * 180 / PI);
    }

    player->additional_velocity_x = active_pattern->additional_player_velocity_x;
    player->additional_velocity_y = active_pattern->additional_player_velocity_y;
    player->additional_hp_change = active_pattern->additional_player_hp_change;
    player->velocity_override = active_pattern->player_velocity_override;

    player->hp += (int)active_pattern->additional_player_hp_change;
    if(active_pattern->additional_player_hp_change < 0 && player->hp <= 0)
      player->hp = 1;
  }

  modifiersControl();

  if (getGameOver())
  {
    if (current_state != "destroyed")
    {
      current_state = "destroyed";
      if (rosalila()->sound->soundExists(name + ".destroyed"))
        rosalila()->sound->playSound(name + ".destroyed", 1, 0, (int)this->x);

      for (int i = 0; i < (int)hitboxes.size(); i++)
        this->hitboxes[i]->setValues(0, 0, 0, 0, 0);
    }
  }

  this->angle += this->angle_change / getSlowdown();

  this->x += cos(angle * PI / 180) * velocity / getSlowdown() + stage_velocity;
  this->y -= sin(angle * PI / 180) * velocity / getSlowdown();

  getIterateSlowdownFlag();

  //current_color_effect_a = /*255-*/(255*hp)/max_hp;
  this->frame++;
}

void Enemy::bottomRender()
{
  if (this->hp > 0)
    Character::bottomRender();

  for(auto animation_control : animation_controls)
  {
    if(animation_control->frames_until_startup > 0)
    {
      animation_control->frames_until_startup--;
      continue;
    }
    Image *image = this->animation_images[animation_control->name][animation_control->current_frame];
    rosalila()->graphics->drawImage(image,
      animation_control->x - image->getWidth() / 2 + current_screen_shake_x,
      animation_control->y - image->getHeight() / 2 + current_screen_shake_y);
    animation_control->current_frame++;
    if(animation_control->current_frame >= this->animation_images[animation_control->name].size())
    {
      animation_control->delete_flag = true;
      animation_control->current_frame = 0;
    }
  }

  std::list<AnimationControl *>::iterator i = animation_controls.begin();
  while (i != animation_controls.end())
  {
    AnimationControl *a_c = (AnimationControl *)*i;
    if (a_c->delete_flag)
    {
      animation_controls.erase(i++);
      delete a_c;
    }
    else
    {
      ++i;
    }
  }
}

void Enemy::topRender()
{
  Character::topRender();
}

void Enemy::loadModifiersFromXML()
{
  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "modifiers.json");

  vector<Node *> modifier_nodes = root_node->getNodesByName("modifiers");

  for (int i = 0; i < (int)modifier_nodes.size(); i++)
  {
    vector<Modifier *> temp_modifiers;

    if (modifier_nodes[i]->hasAttribute("velocity"))
    {
      std::string value = modifier_nodes[i]->attributes["velocity"];
      temp_modifiers.push_back(new Modifier("velocity", value));
    }

    if (modifier_nodes[i]->hasAttribute("angle"))
    {
      std::string value = modifier_nodes[i]->attributes["angle"];
      temp_modifiers.push_back(new Modifier("angle", value));
    }

    if (modifier_nodes[i]->hasAttribute("pattern_type"))
    {
      std::string value = modifier_nodes[i]->attributes["pattern_type"];
      temp_modifiers.push_back(new Modifier("pattern_type", value));
    }

    if (modifier_nodes[i]->hasAttribute("angle_change"))
    {
      std::string value = modifier_nodes[i]->attributes["angle_change"];
      temp_modifiers.push_back(new Modifier("angle_change", value));
    }

    if (modifier_nodes[i]->hasAttribute("iterator"))
    {
      std::string value = modifier_nodes[i]->attributes["iterator"];
      temp_modifiers.push_back(new Modifier("iterator", value));
    }

    if (modifier_nodes[i]->hasAttribute("at"))
    {
      int at = atoi(modifier_nodes[i]->attributes["at"].c_str());

      this->modifiers[at] = temp_modifiers;

      if (modifier_nodes[i]->hasAttribute("repeat"))
      {
        int repeats = atoi(modifier_nodes[i]->attributes["repeat"].c_str());
        int frequency = 1;
        if (modifier_nodes[i]->hasAttribute("repeat_frequency"))
          frequency = atoi(modifier_nodes[i]->attributes["repeat_frequency"].c_str());

        for (int i = 0; i < repeats; i++)
        {
          this->modifiers[at + frequency * (i + 1)] = temp_modifiers;
        }
      }
    }
    else if (modifier_nodes[i]->hasAttribute("life_at"))
    {
      int life_at = atoi(modifier_nodes[i]->attributes["life_at"].c_str());

      this->life_at_modifiers[life_at] = temp_modifiers;

      if (modifier_nodes[i]->hasAttribute("repeat"))
      {
        int repeats = atoi(modifier_nodes[i]->attributes["repeat"].c_str());
        int frequency = 1;
        if (modifier_nodes[i]->hasAttribute("repeat_frequency"))
          frequency = atoi(modifier_nodes[i]->attributes["repeat_frequency"].c_str());

        for (int i = 0; i < repeats; i++)
        {
          this->life_at_modifiers[life_at + frequency * (i + 1)] = temp_modifiers;
        }
      }
    }
  }

  delete root_node;
}

void Enemy::addActivePattern(Pattern *pattern, int new_pattern_x, int new_pattern_y)
{
  Pattern *new_pattern = new Pattern(pattern, new_pattern_x, new_pattern_y);
  float angle = new_pattern->angle;
  angle += new_pattern->getRandomAngle();

  new_pattern->angle = angle;
  new_pattern->homing_angle = angle;

  if (new_pattern->getAimPlayer())
  {
    double distance_x = player->x - new_pattern->x;
    double distance_y = player->y - new_pattern->y;
    new_pattern->angle = new_pattern->angle - atan2(distance_y, distance_x) * 180 / PI;
  }

  if(pattern->aim_player_on_begin)
  {
    double distance_x = pattern->player_x_on_begin - new_pattern->x;
    double distance_y = pattern->player_y_on_begin - new_pattern->y;
    new_pattern->angle = new_pattern->angle - atan2(distance_y, distance_x) * 180 / PI;
  }
  
  active_patterns->push_back(new_pattern);
}

void Enemy::onBulletCancel()
{
  if (rosalila()->sound->soundExists(name + ".bullet_cancel"))
    rosalila()->sound->playSound(name + ".bullet_cancel", 1, 0, this->x);
  
  for (int i = 0; i < (int)type[current_type].size(); i++)
  {
    type[current_type][i]->state_manager = 0;
    type[current_type][i]->iteration = 0;
    type[current_type][i]->state = "startup";
  }

  animation_controls.push_back(new AnimationControl("explosion", 5, this->x+200, this->y+150, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x-150, this->y-300, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x+120, this->y, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x+120, this->y, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x+50, this->y-210, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x-60, this->y-30, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x-190, this->y+200, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x+100, this->y-80, 0));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x-200, this->y+200, 30));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x-200, this->y+200, 30));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x-100, this->y+200, 30));
  animation_controls.push_back(new AnimationControl("explosion", 5, this->x, this->y, 20));
  
  this->cancelAllBullets();
}

void Enemy::cancelAllBullets()
{
  setScore(getScore()+this->active_patterns->size()+5);
  this->bullet_cancel_count += this->active_patterns->size();
  for(std::list<Pattern *>::iterator i = this->active_patterns->begin(); i != this->active_patterns->end(); i++)
  {
    if((*i)->bullet->name != "cancel_reward" && (*i)->bullet->name != "proximity_reward")
    {
      this->addActivePattern(this->type["cancel_reward"][0], (*i)->x, (*i)->y);
      (*i)->hit(this->sound_channel_base + 1, false);
    }
  }
}

void Enemy::onDefeated()
{
  if (current_state != "destroyed" && rosalila()->sound->soundExists(name + ".destroyed"))
    rosalila()->sound->playSound(name + ".destroyed", 1, 0, this->x);
  
  rosalila()->graphics->screen_shake_effect.set(40, 100, 0, 0);
  
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x+200, this->y+150, 0));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x-150, this->y-300, 0));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x+120, this->y, 0));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x+120, this->y, 10));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x+50, this->y-210, 10));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x-60, this->y-30, 15));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x-190, this->y+200, 20));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x+100, this->y-80, 20));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x-200, this->y+200, 20));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x-200, this->y+200, 30));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x-100, this->y+200, 40));
  animation_controls.push_back(new AnimationControl("explosion", 10, this->x, this->y, 50));
  animation_controls.push_back(new AnimationControl("explosion_big", 20, this->x, this->y, 30));
  animation_controls.push_back(new AnimationControl("explosion_big", 20, this->x+50, this->y+20, 20));
  animation_controls.push_back(new AnimationControl("explosion_big", 20, this->x+50, this->y+20, 34));
  animation_controls.push_back(new AnimationControl("explosion_big", 20, this->x-20, this->y-15, 50));
  animation_controls.push_back(new AnimationControl("explosion_big", 20, this->x-5, this->y+30, 50));
  animation_controls.push_back(new AnimationControl("explosion_big", 40, this->x-5, this->y, 60));
}