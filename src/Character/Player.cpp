#include "Character/Player.h"

Player::Player(std::string name, int sound_channel_base, vector<string> intro_input, vector<string> replay_input, string game_mode)
{
  this->stage = NULL;
  this->game_mode = game_mode;
  //Setting up the other variables
  this->name = name;
  this->directory = "chars/" + name + "/";
  this->active_patterns = new std::list<Pattern *>;
  this->shooting = true;
  this->current_state = "start";
  this->current_type = "primary";
  this->visible = true;
  this->charge_ready = false;
  this->frame = 0;
  this->charging_sound_channel = -1;
  this->slow_bar = NULL;
  this->shield_image = NULL;
  this->charge_image = NULL;
  this->parrying_image = NULL;
  this->parryed_image = NULL;

  this->life_bar = NULL;

  //Sprites animation
  this->animation_velocity = 4;
  this->animation_iteration = 0;
  this->current_sprite = 0;
  this->replay_input = replay_input;
  this->intro_input = intro_input;

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

  this->iteration = 0;

  this->slow_in_cooldown = false;

  this->sound_channel_base = sound_channel_base;

  loadPlayerFromXML();

  current_shield = max_shield;

  //Input control
  this->pressed_1_last_frame = this->pressed_2_last_frame = this->pressed_3_last_frame = this->pressed_4_last_frame =
  this->pressed_6_last_frame = this->pressed_7_last_frame = this->pressed_8_last_frame = this->pressed_9_last_frame = false;

  this->pressed_1_last_frameX = this->pressed_2_last_frameX = this->pressed_3_last_frameX = this->pressed_4_last_frameX =
  this->pressed_6_last_frameX = this->pressed_7_last_frameX = this->pressed_8_last_frameX = this->pressed_9_last_frameX = 0;

  //Dash
  this->dash_last_tap_frame = 0;
  this->dash_extra_velocity_x = this->dash_extra_velocity_y = 0;

  //Parry
  this->current_parry_frame = parry_duration + 1;

  //Acceleration
  this->invulnerable_frames_left = 0;

  parries_left = 3;
  //parry_sprites.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/parry/1.png"));
  //parry_sprites.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/parry/2.png"));
  //parry_sprites.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/parry/3.png"));
  //Charge
  //rosalila()->sound->addSound("charge ready", std::string(assets_directory) + directory + "sounds/charge_ready.ogg");
  //rosalila()->sound->addSound("charging", std::string(assets_directory) + directory + "sounds/charging.ogg");
  //charging_sound_channel = rosalila()->sound->playSound("charging", 21, -1, this->x);

  // buffs and debuffs
  this->additional_velocity_x = 0;
  this->additional_velocity_y = 0;
  this->additional_hp_change = 0;
  this->velocity_override = 0;
}

Player::~Player()
{
  if (this->slow_bar)
    delete this->slow_bar;
  if (this->shield_image)
    delete this->shield_image;
  if (this->charge_image)
    delete this->charge_image;
  if (this->parrying_image)
    delete this->parrying_image;
  if (this->parryed_image)
    delete this->parryed_image;

  for (int i = 0; i < (int)parry_hitboxes.size(); i++)
  {
    delete parry_hitboxes[i];
  }

  for (int i = 0; i < (int)parry_sprites.size(); i++)
  {
    delete parry_sprites[i];
  }
}

void Player::loadPlayerFromXML()
{
  loadFromXML();

  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "character.json");

  this->current_slow = 0;
  this->max_slow = -1;

  if (root_node->hasAttribute("slow"))
  {
    this->current_slow = atoi(root_node->attributes["slow"].c_str());
    this->max_slow = atoi(root_node->attributes["slow"].c_str());
  }

  this->slow_decrement = 3;
  if (root_node->hasAttribute("slow_decrement"))
  {
    this->slow_decrement = atoi(root_node->attributes["slow_decrement"].c_str());
  }

  this->slow_increment = 1;
  if (root_node->hasAttribute("slow_increment"))
  {
    this->slow_increment = atoi(root_node->attributes["slow_increment"].c_str());
  }

  this->slow_cooldown_increment = 2;
  if (root_node->hasAttribute("slow_cooldown_increment"))
  {
    this->slow_cooldown_increment = atoi(root_node->attributes["slow_cooldown_increment"].c_str());
  }

  this->slow_bar_x = 0;
  this->slow_bar_y = 0;
  this->slow_bar_rect_offset_x = 0;
  this->slow_bar_rect_offset_y = 0;
  this->slow_bar_rect_height = 0;
  this->slow_bar_rect_width = 0;
  this->slow_bar_color.red = 0;
  this->slow_bar_color.green = 0;
  this->slow_bar_color.blue = 0;
  this->slow_bar_color.alpha = 255;
  this->slow_bar_cooldown_color.red = 0;
  this->slow_bar_cooldown_color.green = 0;
  this->slow_bar_cooldown_color.blue = 0;
  this->slow_bar_cooldown_color.alpha = 128;

  Node *slow_bar_node = root_node->getNodeByName("SlowBar");
  if (slow_bar_node)
  {
    if (slow_bar_node->hasAttribute("x"))
      this->slow_bar_x = atoi(slow_bar_node->attributes["x"].c_str());
    if (slow_bar_node->hasAttribute("y"))
      this->slow_bar_y = atoi(slow_bar_node->attributes["y"].c_str());
    if (slow_bar_node->hasAttribute("color_r"))
      this->slow_bar_color.red = atoi(slow_bar_node->attributes["color_r"].c_str());
    if (slow_bar_node->hasAttribute("color_g"))
      this->slow_bar_color.green = atoi(slow_bar_node->attributes["color_g"].c_str());
    if (slow_bar_node->hasAttribute("color_b"))
      this->slow_bar_color.blue = atoi(slow_bar_node->attributes["color_b"].c_str());
    if (slow_bar_node->hasAttribute("color_a"))
      this->slow_bar_color.alpha = atoi(slow_bar_node->attributes["color_a"].c_str());

    if (slow_bar_node->hasAttribute("cooldown_color_r"))
      this->slow_bar_cooldown_color.red = atoi(slow_bar_node->attributes["cooldown_color_r"].c_str());
    if (slow_bar_node->hasAttribute("cooldown_color_g"))
      this->slow_bar_cooldown_color.green = atoi(slow_bar_node->attributes["cooldown_color_g"].c_str());
    if (slow_bar_node->hasAttribute("cooldown_color_b"))
      this->slow_bar_cooldown_color.blue = atoi(slow_bar_node->attributes["cooldown_color_b"].c_str());
    if (slow_bar_node->hasAttribute("cooldown_color_a"))
      this->slow_bar_cooldown_color.alpha = atoi(slow_bar_node->attributes["cooldown_color_a"].c_str());

    if (slow_bar_node->hasAttribute("rect_offset_x"))
      this->slow_bar_rect_offset_x = atoi(slow_bar_node->attributes["rect_offset_x"].c_str());
    if (slow_bar_node->hasAttribute("rect_offset_y"))
      this->slow_bar_rect_offset_y = atoi(slow_bar_node->attributes["rect_offset_y"].c_str());
    if (slow_bar_node->hasAttribute("rect_height"))
      this->slow_bar_rect_height = atoi(slow_bar_node->attributes["rect_height"].c_str());
    if (slow_bar_node->hasAttribute("rect_width"))
      this->slow_bar_rect_width = atoi(slow_bar_node->attributes["rect_width"].c_str());
  }

  delete root_node;
}

void Player::inputControl()
{
  bool up_pressed, down_pressed, left_pressed, right_pressed;
  up_pressed = down_pressed = left_pressed = right_pressed = false;
  string current_input_replay_store;

  if (isDownWrapper("2"))
  {
    down_pressed = true;
    current_input_replay_store += "2";
  }
  if (isDownWrapper("8"))
  {
    up_pressed = true;
    current_input_replay_store += "8";
  }
  if (isDownWrapper("4"))
  {
    left_pressed = true;
    current_input_replay_store += "4";
  }
  if (isDownWrapper("6"))
  {
    right_pressed = true;
    current_input_replay_store += "6";
  }

  int velocity_boost = invulnerable_frames_left;

  double delta_y = 0;
  double delta_x = 0;

  double current_velocity = velocity;
  if(velocity_override != 0)
  {
    current_velocity = velocity_override;
  }

  // Double tap
  /*
  if (up_pressed && !down_pressed && !left_pressed && !right_pressed) //8
  {
    if(this->frame - this->pressed_8_last_frameX < 10)
    {
      this->dash_extra_velocity_y = -20;
      this->pressed_8_last_frameX = 0;
    }else
    {
      if(this->dash_extra_velocity_y == 0)
      {
        this->pressed_8_last_frame = true;
      }
    }
    delta_y = -(current_velocity + velocity_boost) / getSlowdown();
  }else
  {
    if(this->pressed_8_last_frame)
    {
      this->pressed_1_last_frameX = 0;
      this->pressed_2_last_frameX = 0;
      this->pressed_3_last_frameX = 0;
      this->pressed_4_last_frameX = 0;
      this->pressed_6_last_frameX = 0;
      this->pressed_7_last_frameX = 0;
      this->pressed_8_last_frameX = this->frame;
      this->pressed_9_last_frameX = 0;
    }
    this->pressed_8_last_frame = false;
  }
  
  if (!up_pressed && down_pressed && !left_pressed && !right_pressed) //2
  {
    if(this->frame - this->pressed_2_last_frameX < 10)
    {
      this->dash_extra_velocity_y = 20;
      this->pressed_2_last_frameX = 0;
    }else
    {
      if(this->dash_extra_velocity_y == 0)
      {
        this->pressed_2_last_frame = true;
      }
    }
    delta_y = (current_velocity + velocity_boost) / getSlowdown();
  }else
  {
    if(this->pressed_2_last_frame)
    {
      this->pressed_1_last_frameX = 0;
      this->pressed_2_last_frameX = this->frame;
      this->pressed_3_last_frameX = 0;
      this->pressed_4_last_frameX = 0;
      this->pressed_6_last_frameX = 0;
      this->pressed_7_last_frameX = 0;
      this->pressed_8_last_frameX = 0;
      this->pressed_9_last_frameX = 0;
    }
    this->pressed_2_last_frame = false;
  }

  if (!up_pressed && !down_pressed && left_pressed && !right_pressed) //4
  {
    if(this->frame - this->pressed_4_last_frameX < 10)
    {
      this->dash_extra_velocity_x = -20;
      this->pressed_4_last_frameX = 0;
    }else
    {
      if(this->dash_extra_velocity_y == 0)
      {
        this->pressed_4_last_frame = true;
      }
    }
    delta_x = -(current_velocity + velocity_boost) / getSlowdown();
  }else
  {
    if(this->pressed_4_last_frame)
    {
      this->pressed_1_last_frameX = 0;
      this->pressed_2_last_frameX = 0;
      this->pressed_3_last_frameX = 0;
      this->pressed_4_last_frameX = this->frame;
      this->pressed_6_last_frameX = 0;
      this->pressed_7_last_frameX = 0;
      this->pressed_8_last_frameX = 0;
      this->pressed_9_last_frameX = 0;
    }
    this->pressed_4_last_frame = false;
  }


  if (!up_pressed && !down_pressed && !left_pressed && right_pressed) //6
  {
    if(this->frame - this->pressed_6_last_frameX < 10)
    {
      this->dash_extra_velocity_x = 20;
      this->pressed_6_last_frameX = 0;
    }else
    {
      if(this->dash_extra_velocity_y == 0)
      {
        this->pressed_6_last_frame = true;
      }
    }
    delta_x = (current_velocity + velocity_boost) / getSlowdown();
  }else
  {
    if(this->pressed_6_last_frame)
    {
      this->pressed_1_last_frameX = 0;
      this->pressed_2_last_frameX = 0;
      this->pressed_3_last_frameX = 0;
      this->pressed_4_last_frameX = 0;
      this->pressed_6_last_frameX = this->frame;
      this->pressed_7_last_frameX = 0;
      this->pressed_8_last_frameX = 0;
      this->pressed_9_last_frameX = 0;
    }
    this->pressed_6_last_frame = false;
  }
  */

  if (up_pressed && !down_pressed && !left_pressed && !right_pressed) //8
  {
    delta_y = -(current_velocity + velocity_boost) / getSlowdown();
  }
  if (!up_pressed && down_pressed && !left_pressed && !right_pressed) //2
  {
    delta_y = (current_velocity + velocity_boost) / getSlowdown();
  }

  if (!up_pressed && !down_pressed && left_pressed && !right_pressed) //4
  {
    delta_x = -(current_velocity + velocity_boost) / getSlowdown();
  }

  if (!up_pressed && !down_pressed && !left_pressed && right_pressed) //6
  {
    delta_x = (current_velocity + velocity_boost) / getSlowdown();
  }
 
  if (!up_pressed && down_pressed && left_pressed && !right_pressed) //1
  {
    this->pressed_1_last_frame = true;
    delta_x = cos(225 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
    delta_y = -sin(225 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
  }
  if (!up_pressed && down_pressed && !left_pressed && right_pressed) //3
  {
    this->pressed_3_last_frame = true;
    delta_x = cos(315 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
    delta_y = -sin(315 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
  }
  if (up_pressed && !down_pressed && left_pressed && !right_pressed) //7
  {
    this->pressed_7_last_frame = true;
    delta_x = cos(135 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
    delta_y = -sin(135 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
  }
  if (up_pressed && !down_pressed && !left_pressed && right_pressed) //9
  {
    this->pressed_9_last_frame = true;
    delta_x = cos(45 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
    delta_y = -sin(45 * PI / 180) * (current_velocity + velocity_boost) / getSlowdown();
  }

  if(dash_extra_velocity_x == 0 && dash_extra_velocity_y == 0
      && delta_x > 0
      && this->current_state != "right"
      && this->hasState("right"))
    this->setState("right");
  else if(dash_extra_velocity_x == 0 && dash_extra_velocity_y == 0
      && delta_x < 0
      && this->current_state != "left"
      && this->hasState("left"))
    this->setState("left");
  else if(dash_extra_velocity_x == 0 && dash_extra_velocity_y == 0
      && delta_x == 0 && delta_y > 0
      && this->current_state != "down"
      && this->hasState("down"))
    this->setState("down");
  else if(dash_extra_velocity_x == 0 && dash_extra_velocity_y == 0
      && delta_x == 0 && delta_y < 0
      && this->current_state != "up"
      && this->hasState("up"))
    this->setState("up");

  bool dash_pressed = rosalila()->receiver->isPressed(0,"c");

  if(dash_pressed && isDownWrapper("6")
      && this->hasState("dash right"))
  {
    dash_extra_velocity_x = 20;
    this->setState("dash right");
  }
  if(dash_pressed && isDownWrapper("4")
      && this->hasState("dash left"))
  {
    dash_extra_velocity_x = -20;
    this->setState("dash left");
  }
  if(dash_pressed && isDownWrapper("2")
      && this->hasState("dash down"))
  {
    dash_extra_velocity_y = 20;
    this->setState("dash down");
  }
  if(dash_pressed && isDownWrapper("8")
      && this->hasState("dash up"))
  {
    dash_extra_velocity_y = -20;
    this->setState("dash up");
  }

  if( delta_x == 0 && (this->current_state == "right" || this->current_state == "left") && this->hasState("after left/right"))
  {
    this->setState("after left/right");
  }
  if( delta_y == 0 && (this->current_state == "up" || this->current_state == "down") && this->hasState("after up/down"))
  {
    this->setState("after up/down");
  }
  
  if( dash_extra_velocity_x == 0 && (this->current_state == "dash right" || this->current_state == "dash left") && this->hasState("after left/right"))
  {
    this->setState("after left/right");
  }
  else if( dash_extra_velocity_y == 0 && (this->current_state == "dash up" || this->current_state == "dash down") && this->hasState("after up/down"))
  {
    this->setState("after up/down");
  }

  int last_x = this->x;
  int last_y = this->y;
  bool player_was_in_bounds = this->stage->playerIsInBounds();

  this->x += delta_x + additional_velocity_x + this->dash_extra_velocity_x;
  this->y += delta_y + additional_velocity_y + this->dash_extra_velocity_y;

  if(player_was_in_bounds && !this->stage->playerIsInBounds()
      && (this->dash_extra_velocity_x == 0 && this->dash_extra_velocity_y == 0) )
  {
    this->x = last_x;
    this->y = last_y;
  }

  if(this->dash_extra_velocity_x < 0)
    this->dash_extra_velocity_x++;
  if(this->dash_extra_velocity_x > 0)
    this->dash_extra_velocity_x--;
  if(this->dash_extra_velocity_y < 0)
    this->dash_extra_velocity_y++;
  if(this->dash_extra_velocity_y > 0)
    this->dash_extra_velocity_y--;

  if (isDownWrapper("a"))
  {
    current_input_replay_store += "a";
    this->shooting = true;
    this->current_type = "primary";
    this->velocity = 6;

    /*
        if(!this->shooting && !isParrying() && parries_left>0)
        {
            current_parry_frame=0;
        }
        if(max_charge!=0 && current_charge==max_charge)
        {
            if(!isOnIntro())
            {
                int counter = rosalila()->api_integrator->getStat("TotalCharges") + 1;
                if(game_mode!="replay")rosalila()->api_integrator->setStat("TotalCharges",counter);

                if(counter >= 3)
                {
                    if(game_mode!="replay")rosalila()->api_integrator->unlockAchievement("Charge1");
                }
                if(counter >= 20)
                {
                    if(game_mode!="replay")rosalila()->api_integrator->unlockAchievement("Charge2");
                }
                if(counter >= 100)
                {
                    if(game_mode!="replay")rosalila()->api_integrator->unlockAchievement("Charge3");
                }
                if(counter >= 200)
                {
                    if(game_mode!="replay")rosalila()->api_integrator->unlockAchievement("Charge4");
                }
            }

            std::vector<Pattern*> patterns=type["bomb"];
            patterns[0]->bullet->playSound(patterns[0]->x + this->x, true);
            this->addActivePattern(patterns[0]);
        }
        current_charge=0;
        */
  }else if (isDownWrapper("b"))
  {
    current_input_replay_store += "b";
    this->shooting = true;
    this->current_type = "secondary";
    this->velocity = 6;
  }
  /*
  else if (isDownWrapper("c"))
  {
    current_input_replay_store += "c";
    this->shooting = true;
    this->current_type = "bomb";
    this->velocity = 6;
  }
  */
  else
  {
    this->velocity = 6;
    this->shooting = false;
  }

  if (!getGameOver())
    replay_storage.push_back(current_input_replay_store);
}

void Player::logic(int stage_velocity)
{
  current_parry_frame++;

  invulnerable_frames_left -= 1;
  if (invulnerable_frames_left < 0)
    invulnerable_frames_left = 0;

  if (!shooting)
  {
    invulnerable_frames_left = 0;
  }

  if (invulnerable_frames_left > 0)
  {
    enableSlow();
  }
  else
  {
    disableSlow();
  }

  animationControl();
  if (this->hp != 0)
  {
    inputControl();
  }
  else
  {
    if (current_state != "destroyed" && rosalila()->sound->soundExists(name + ".destroyed"))
      rosalila()->sound->playSound(name + ".destroyed", 1, 0, this->x);
    this->setState("destroyed");
    //this->hitbox.setValues(0,0,0,0,0);
  }

  // Die if out of bounds
  if(!getGameOver()
    && !this->stage->playerIsInBounds()
    && this->current_state != "dash up"
    && this->current_state != "dash down"
    && this->current_state != "dash left"
    && this->current_state != "dash right")
  {
    this->hit(9999);
  }

  //Enable or disable slow
  if (isSlowPressed() && !slow_in_cooldown)
  {
    enableSlow();
    current_slow -= slow_decrement;
  }
  else
  {
    disableSlow();
    if (slow_in_cooldown)
      current_slow += slow_cooldown_increment;
    else
      current_slow += slow_increment;
  }

  //Check max and min slow
  if (current_slow <= 0)
  {
    current_slow = 0;
  }
  if (current_slow > max_slow)
  {
    current_slow = max_slow;
  }

  //Slow cooldown
  if (slow_in_cooldown && current_slow >= max_slow)
  {
    slow_in_cooldown = false;
  }
  if (!slow_in_cooldown && current_slow <= 0)
  {
    slow_in_cooldown = true;
  }

  spellControl(stage_velocity);

  iteration++;

  //current_color_effect_a = (255*hp)/max_hp;

  current_shield -= shield_fade;
  if (current_shield < 0)
    current_shield = 0;

  current_charge += charge_velocity;
  if (current_charge >= max_charge)
  {
    current_charge = max_charge;
    if (!charge_ready)
    {
      //rosalila()->sound->playSound("charge ready", -1, 0, this->x);
    }
    charge_ready = true;
  }
  else
  {
    charge_ready = false;
  }

/*
  if (!charge_ready && !shooting && !getGameOver())
    Mix_Volume(charging_sound_channel, 128);
  else
    Mix_Volume(charging_sound_channel, 0);
  */
  for(std::map < /*current state*/ std::string, /*trigger validation*/ std::map< std::string, std::string > >::iterator i = state_triggers.begin();
        i != state_triggers.end();
        i++)
  {
    if((*i).second.count("hp") != 0)
    {
      if(atoi((*i).second["hp"].c_str()) == this->hp)
      {
        this->setState((*i).first);
      }
    }
  }
  this->frame++;
}

void Player::bottomRender()
{
  Character::bottomRender();

  if (current_shield > 0)
  {
    if (shield_image)
    {
      shield_image->color_filter.alpha = 255.0 * getShieldPercentage();
      rosalila()->graphics->drawImage(shield_image,
                                        this->x - shield_image->getWidth() / 2 + current_screen_shake_x,
                                        this->y - shield_image->getHeight() / 2 + current_screen_shake_y);
    }
  }
/*
  if (current_charge > 0)
  {
    int transparency_divider = 1;
    if (getGameOver())
      transparency_divider = 8;

    int charge_transparency_effect = 255;
    if (charge_ready)
      charge_transparency_effect = (frame * 10) % 255;

    if (charge_image)
      rosalila()->graphics->draw2DImage(charge_image,
                                        charge_image->getWidth(), charge_image->getHeight() * ((double)current_charge / (double)max_charge),
                                        this->x + charge_sprite_x,
                                        this->y + charge_sprite_y - charge_image->getHeight() * ((double)current_charge / (double)max_charge) / 2,
                                        1.0,
                                        0.0,
                                        false,
                                        Color(255, 255, 255, charge_transparency_effect / transparency_divider));
  }
*/
}

void Player::topRender()
{
  Character::topRender();

  if (isParrying() && parries_left >= 1)
  {
    Image *image = parry_sprites[parries_left - 1];
    rosalila()->graphics->drawImage(image,
                                      this->x + parrying_x,
                                      this->y + parrying_y);
  }

  if (parrying_image != NULL && false)
    if (isParrying())
      rosalila()->graphics->drawImage(parrying_image,
                                        this->x + parrying_x,
                                        this->y + parrying_y);

  if (parryed_image != NULL)
    if (invulnerable_frames_left > 0)
      rosalila()->graphics->drawImage(parryed_image,
                                        this->x + parryed_x,
                                        this->y + parryed_y);

  if (rosalila()->receiver->isKeyDown(SDLK_h))
    for (int i = 0; i < (int)parry_hitboxes.size(); i++)
    {
      rosalila()->graphics->drawRectangle(parry_hitboxes[i]->getX() + x,
                                          parry_hitboxes[i]->getY() + y,
                                          parry_hitboxes[i]->getWidth(), parry_hitboxes[i]->getHeight(),
                                          parry_hitboxes[i]->getAngle(), 100, 0, 0, 100);
    }
}

void Player::hit(int damage)
{
  if (current_shield == 0.0)
  {
    Character::hit(damage);
  }
  else
  {
    double prorated_damage = ((double)damage) * proration * getShieldPercentage();
    Character::hit(prorated_damage);
  }
  current_shield = max_shield;
}

double Player::getShieldPercentage()
{
  if (current_shield == 0 || max_shield == 0)
    return 1.0;
  return (double)current_shield / (double)max_shield;
}

void Player::loadFromXML()
{
  Character::loadFromXML();

  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "character.json");

  this->max_shield = 0;
  this->shield_fade = 0;
  this->proration = 0;
  this->shield_image = NULL;

  Node *shield_node = root_node->getNodeByName("shield");

  if (shield_node)
  {
    if (shield_node->hasAttribute("max_shield"))
    {
      this->max_shield = atoi(shield_node->attributes["max_shield"].c_str());
    }

    if (shield_node->hasAttribute("shield_fade"))
    {
      this->shield_fade = atoi(shield_node->attributes["shield_fade"].c_str());
    }

    if (shield_node->hasAttribute("shield_fade"))
    {
      this->proration = ((double)atoi(shield_node->attributes["shield_fade"].c_str())) / 100.0;
    }

    if (shield_node->hasAttribute("sprite"))
    {
      this->shield_image = rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + shield_node->attributes["sprite"]);
    }
  }

  charge_image = NULL;
  current_charge = 0;
  max_charge = 0;
  charge_velocity = 0;
  charge_sprite_x = 0;
  charge_sprite_y = 0;

  Node *charge_node = root_node->getNodeByName("Charge");

  if (charge_node)
  {
    if (charge_node->hasAttribute("max_charge"))
    {
      this->max_charge = atoi(charge_node->attributes["max_charge"].c_str());
    }

    if (charge_node->hasAttribute("charge_velocity"))
    {
      this->charge_velocity = atoi(charge_node->attributes["charge_velocity"].c_str());
    }

    if (charge_node->hasAttribute("x"))
    {
      this->charge_sprite_x = atoi(charge_node->attributes["x"].c_str());
    }

    if (charge_node->hasAttribute("y"))
    {
      this->charge_sprite_y = atoi(charge_node->attributes["y"].c_str());
    }

    if (charge_node->hasAttribute("sprite"))
    {
      this->charge_image = rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + charge_node->attributes["sprite"]);
    }
  }

  this->parry_duration = 0;

  this->parrying_image = NULL;
  this->parrying_x = 0;
  this->parrying_y = 0;
  this->parryed_image = NULL;
  this->parryed_x = 0;
  this->parryed_y = 0;

  Node *parry_node = root_node->getNodeByName("parry");

  if (parry_node)
  {
    if (parry_node->hasAttribute("duration"))
    {
      this->parry_duration = atoi(parry_node->attributes["duration"].c_str());
    }

    if (parry_node->hasAttribute("sound"))
    {
      std::string sprites_sound = parry_node->attributes["sound"];
      rosalila()->sound->addSound(name + ".parry", std::string(assets_directory) + directory + sprites_sound);
    }

    Node *parrying_node = parry_node->getNodeByName("Parrying");

    if (parrying_node)
    {
      if (parrying_node->hasAttribute("sprite"))
      {
        this->parrying_image = rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + parrying_node->attributes["sprite"]);
      }
      if (parrying_node->hasAttribute("x"))
      {
        this->parrying_x = atoi(parrying_node->attributes["x"].c_str());
      }
      if (parrying_node->hasAttribute("y"))
      {
        this->parrying_y = atoi(parrying_node->attributes["y"].c_str());
      }
    }

    Node *parryed_node = parry_node->getNodeByName("parryed");

    if (parryed_node)
    {
      if (parryed_node->hasAttribute("sprite"))
      {
        this->parryed_image = rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + parryed_node->attributes["sprite"]);
      }
      if (parryed_node->hasAttribute("x"))
      {
        this->parryed_x = atoi(parryed_node->attributes["x"].c_str());
      }
      if (parryed_node->hasAttribute("y"))
      {
        this->parryed_y = atoi(parryed_node->attributes["y"].c_str());
      }
    }

    vector<Node *> hitboxes_nodes = parry_node->getNodesByName("hitboxes");

    for (int i = 0; i < (int)hitboxes_nodes.size(); i++)
    {
      int hitbox_x = atoi(hitboxes_nodes[i]->attributes["x"].c_str());
      int hitbox_y = atoi(hitboxes_nodes[i]->attributes["y"].c_str());
      int hitbox_width = atoi(hitboxes_nodes[i]->attributes["width"].c_str());
      int hitbox_height = atoi(hitboxes_nodes[i]->attributes["height"].c_str());
      int hitbox_angle = atoi(hitboxes_nodes[i]->attributes["angle"].c_str());
      Hitbox *hitbox = new Hitbox(hitbox_x, hitbox_y, hitbox_width, hitbox_height, hitbox_angle);
      this->parry_hitboxes.push_back(hitbox);
    }
  }

  delete root_node;
}

bool Player::isParrying()
{
  return current_parry_frame < parry_duration;
}

bool Player::isInvulnerable()
{
  return invulnerable_frames_left > 0;
}

void Player::parry(bool infinite_parries)
{
  if (invulnerable_frames_left == 0)
  {
    invulnerable_frames_left = 15;
    if (!infinite_parries)
    {
      parries_left -= 1;
    }
  }
  if (rosalila()->sound->soundExists(this->name + ".parry"))
  {
    rosalila()->sound->playSound(this->name + ".parry", sound_channel_base + 1, 0, this->x);
  }
}

bool Player::collidesParry(Hitbox hitbox, int hitbox_x, int hitbox_y, float hitbox_angle)
{
  if (!visible)
    return false;
  for (int i = 0; i < (int)parry_hitboxes.size(); i++)
    if (parry_hitboxes[i]->getPlacedHitbox(this->x, this->y).collides(hitbox))
      return true;
  return false;
}

void Player::exit()
{
  //Mix_Volume(charging_sound_channel, 0);
}

bool Player::isDownWrapper(string button_map)
{
  if (replay_input.size() > 0)
  {
    if (frame >= (int)replay_input.size())
      return false;
    for (int i = 0; i < (int)replay_input[frame].size(); i++)
      if (replay_input[frame][i] == button_map[0])
        return true;
    return false;
  }
  else if (intro_input.size() > 0)
  {
    if (frame >= (int)intro_input.size())
    {
      rosalila()->graphics->grayscale_effect.set(1, 0.003);
      return rosalila()->receiver->isDown(0, button_map);
    }

    for (int i = 0; i < (int)intro_input[frame].size(); i++)
      if (intro_input[frame][i] == button_map[0])
        return true;
    return false;
  }
  return rosalila()->receiver->isDown(0, button_map);
}

bool Player::isOnIntro()
{
  return this->frame < (int)this->intro_input.size();
}
