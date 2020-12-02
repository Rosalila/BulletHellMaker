#include "Character/Player.h"
#include "Utility/Utility.h"

Player::Player(std::string name, int sound_channel_base, vector<string> intro_input, vector<string> replay_input, string game_mode)
{
  this->enemy = NULL;
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
  this->frame = 0;
  this->slow_bar = NULL;
  this->shield_image = NULL;

  this->bomb_channel = -1;

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

  // velocity
  this->original_velocity = 0;
  this->primary_weapon_velocity = 0;
  this->secondary_weapon_velocity = 0;

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

  // buffs and debuffs
  this->additional_velocity_x = 0;
  this->additional_velocity_y = 0;
  this->additional_hp_change = 0;
  this->velocity_override = 0;

  // Bomb
  for(int i=29; i>=0; i--)
  {
    std::string image_number = rosalila()->utility->toString(i+1);
    if(image_number.size()==1)
      image_number = "0" + image_number;
    this->bomb_images.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/bomb_background/" + image_number+ ".png"));
    //this->bomb_images[i]->color_filter.alpha = 255;
  }
  this->current_bomb_frame = 0;
  this->current_bomb_image = 0;
  this->bomb_image_duration = 5;
  this->is_bomb_active = false;
  
  this->dash_button_was_down_last_frame = false;

  this->jump_tutorial_image = rosalila()->graphics->getImage(std::string(assets_directory) + "misc/jump.png");
  this->jump_tutorial_was_pressed = false;
  this->jump_tutorial_frame = 0;
}

Player::~Player()
{
  if (this->slow_bar)
    delete this->slow_bar;
  if (this->shield_image)
    delete this->shield_image;

  for (int i = 0; i < (int)bomb_images.size(); i++)
  {
    delete bomb_images[i];
  }

  for (int i = 0; i < (int)hitbox_animation_vector.size(); i++)
  {
    delete hitbox_animation_vector[i];
  }
}

void Player::loadPlayerFromXML()
{
  loadFromXML();

  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "character.json");

  Node *sounds_node = root_node->getNodeByName("sounds");
  if (sounds_node)
  {
    if (sounds_node->hasAttribute("bomb"))
      rosalila()->sound->addSound(this->name + ".bomb", std::string(assets_directory) + directory + sounds_node->attributes["bomb"]);
    if (sounds_node->hasAttribute("dash"))
      rosalila()->sound->addSound(this->name + ".dash", std::string(assets_directory) + directory + sounds_node->attributes["dash"]);
  }

  if (root_node->hasAttribute("velocity"))
    this->original_velocity = atoi(root_node->attributes["velocity"].c_str());
  
  if (root_node->hasAttribute("primary_weapon_velocity"))
    this->primary_weapon_velocity = atoi(root_node->attributes["primary_weapon_velocity"].c_str());
  else
    this->primary_weapon_velocity = this->original_velocity;

  if (root_node->hasAttribute("secondary_weapon_velocity"))
    this->secondary_weapon_velocity = atoi(root_node->attributes["secondary_weapon_velocity"].c_str());
  else
    this->secondary_weapon_velocity = this->original_velocity;
  
  this->dash_velocity = 10;
  this->dash_friction = 10;
  Node *dash_node = root_node->getNodeByName("dash");
  if (dash_node)
  {
    if (dash_node->hasAttribute("velocity"))
      this->dash_velocity = atoi(dash_node->attributes["velocity"].c_str());
    if (dash_node->hasAttribute("friction"))
      this->dash_friction = atoi(dash_node->attributes["friction"].c_str());
  }

  this->current_slow = 0;
  this->max_slow = -1;
  this->slow_decrement = 3;
  this->slow_increment = 1;
  this->slow_cooldown_increment = 2;
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
  Node *slow_node = root_node->getNodeByName("slow");
  if (slow_node)
  {
    if (slow_node->hasAttribute("max"))
    {
      this->current_slow = atoi(slow_node->attributes["max"].c_str());
      this->max_slow = atoi(slow_node->attributes["max"].c_str());
    }
    if (slow_node->hasAttribute("decrement"))
      this->slow_decrement = atoi(slow_node->attributes["decrement"].c_str());
    if (slow_node->hasAttribute("increment"))
      this->slow_increment = atoi(slow_node->attributes["increment"].c_str());
    if (slow_node->hasAttribute("cooldown_increment"))
      this->slow_cooldown_increment = atoi(slow_node->attributes["cooldown_increment"].c_str());


    Node *slow_bar_node = slow_node->getNodeByName("slow_bar");
    if (slow_bar_node)
    {
      if (slow_bar_node->hasAttribute("x"))
        this->slow_bar_x = atoi(slow_bar_node->attributes["x"].c_str());
      if (slow_bar_node->hasAttribute("y"))
        this->slow_bar_y = atoi(slow_bar_node->attributes["y"].c_str());

      Node *slow_bar_color_active_node = slow_bar_node->getNodeByName("active_color");
      if (slow_bar_color_active_node)
      {
        if (slow_bar_color_active_node->hasAttribute("r"))
          this->slow_bar_color.red = atoi(slow_bar_color_active_node->attributes["r"].c_str());
        if (slow_bar_color_active_node->hasAttribute("g"))
          this->slow_bar_color.green = atoi(slow_bar_color_active_node->attributes["g"].c_str());
        if (slow_bar_color_active_node->hasAttribute("b"))
          this->slow_bar_color.blue = atoi(slow_bar_color_active_node->attributes["b"].c_str());
        if (slow_bar_color_active_node->hasAttribute("a"))
          this->slow_bar_color.alpha = atoi(slow_bar_color_active_node->attributes["a"].c_str());
      }
      
      Node *slow_bar_color_cooldown_node = slow_bar_node->getNodeByName("cooldown_color");
      if (slow_bar_color_cooldown_node)
      {
        if (slow_bar_color_cooldown_node->hasAttribute("r"))
          this->slow_bar_cooldown_color.red = atoi(slow_bar_color_cooldown_node->attributes["r"].c_str());
        if (slow_bar_color_cooldown_node->hasAttribute("g"))
          this->slow_bar_cooldown_color.green = atoi(slow_bar_color_cooldown_node->attributes["g"].c_str());
        if (slow_bar_color_cooldown_node->hasAttribute("b"))
          this->slow_bar_cooldown_color.blue = atoi(slow_bar_color_cooldown_node->attributes["b"].c_str());
        if (slow_bar_color_cooldown_node->hasAttribute("a"))
          this->slow_bar_cooldown_color.alpha = atoi(slow_bar_color_cooldown_node->attributes["a"].c_str());
      }

      Node *slow_bar_rectangle_node = slow_bar_node->getNodeByName("rectangle");
      if (slow_bar_rectangle_node)
      {
        if (slow_bar_rectangle_node->hasAttribute("x"))
          this->slow_bar_rect_offset_x = atoi(slow_bar_rectangle_node->attributes["x"].c_str());
        if (slow_bar_rectangle_node->hasAttribute("y"))
          this->slow_bar_rect_offset_y = atoi(slow_bar_rectangle_node->attributes["y"].c_str());
        if (slow_bar_rectangle_node->hasAttribute("height"))
          this->slow_bar_rect_height = atoi(slow_bar_rectangle_node->attributes["height"].c_str());
        if (slow_bar_rectangle_node->hasAttribute("width"))
          this->slow_bar_rect_width = atoi(slow_bar_rectangle_node->attributes["width"].c_str());
      }
    }
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

  double delta_y = 0;
  double delta_x = 0;

  double current_velocity = velocity;
  if(velocity_override != 0)
    current_velocity = velocity_override;

  if (up_pressed && !down_pressed && !left_pressed && !right_pressed) //8
  {
    delta_y = -current_velocity / getSlowdown();
  }
  if (!up_pressed && down_pressed && !left_pressed && !right_pressed) //2
  {
    delta_y = current_velocity / getSlowdown();
  }
  if (!up_pressed && !down_pressed && left_pressed && !right_pressed) //4
  {
    delta_x = -current_velocity / getSlowdown();
  }
  if (!up_pressed && !down_pressed && !left_pressed && right_pressed) //6
  {
    delta_x = current_velocity / getSlowdown();
  }
 
  if (!up_pressed && down_pressed && left_pressed && !right_pressed) //1
  {
    this->pressed_1_last_frame = true;
    delta_x = cos(225 * PI / 180) * current_velocity / getSlowdown();
    delta_y = -sin(225 * PI / 180) * current_velocity / getSlowdown();
  }
  if (!up_pressed && down_pressed && !left_pressed && right_pressed) //3
  {
    this->pressed_3_last_frame = true;
    delta_x = cos(315 * PI / 180) * current_velocity / getSlowdown();
    delta_y = -sin(315 * PI / 180) * current_velocity / getSlowdown();
  }
  if (up_pressed && !down_pressed && left_pressed && !right_pressed) //7
  {
    this->pressed_7_last_frame = true;
    delta_x = cos(135 * PI / 180) * current_velocity / getSlowdown();
    delta_y = -sin(135 * PI / 180) * current_velocity / getSlowdown();
  }
  if (up_pressed && !down_pressed && !left_pressed && right_pressed) //9
  {
    this->pressed_9_last_frame = true;
    delta_x = cos(45 * PI / 180) * current_velocity / getSlowdown();
    delta_y = -sin(45 * PI / 180) * current_velocity / getSlowdown();
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

  bool dash_pressed = !this->dash_button_was_down_last_frame && isDownWrapper("c");
  this->dash_button_was_down_last_frame = isDownWrapper("c");

  if(dash_pressed)
  {
    this->onDash();
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

  double last_x = this->x;
  double last_y = this->y;
  bool player_was_in_platform = this->stage->playerIsInPlatform();

  this->x += delta_x + additional_velocity_x + this->dash_extra_velocity_x;
  this->y += delta_y + additional_velocity_y + this->dash_extra_velocity_y;

  if(this->stage->playerIsInObstacle()
      ||(player_was_in_platform
      && !this->stage->playerIsInPlatform()
      && (this->dash_extra_velocity_x == 0 && this->dash_extra_velocity_y == 0)
      && !isSlowEnabled())
    )
  {
    this->x = last_x;
    this->y = last_y;
  }

  if(this->x < 0)
    this->x = 0;
  if(this->y < 0)
    this->y = 0;
  if(this->x > rosalila()->graphics->screen_width)
    this->x = rosalila()->graphics->screen_width;
  if(this->y > rosalila()->graphics->screen_height)
    this->y = rosalila()->graphics->screen_height;

  if(this->dash_extra_velocity_x < 0)
  {
    this->dash_extra_velocity_x += this->dash_friction;
    if(this->dash_extra_velocity_x > 0)
      this->dash_extra_velocity_x = 0;
  }
  if(this->dash_extra_velocity_x > 0)
  {
    this->dash_extra_velocity_x-=this->dash_friction;
    if(this->dash_extra_velocity_x < 0)
      this->dash_extra_velocity_x = 0;
  }
  if(this->dash_extra_velocity_y < 0)
  {
    this->dash_extra_velocity_y+=this->dash_friction;
    if(this->dash_extra_velocity_y > 0)
      this->dash_extra_velocity_y = 0;
  }
  if(this->dash_extra_velocity_y > 0)
  {
    this->dash_extra_velocity_y-=this->dash_friction;
    if(this->dash_extra_velocity_y < 0)
      this->dash_extra_velocity_y = 0;
  }

  if (isDownWrapper("a"))
  {
    current_input_replay_store += "a";
    this->shooting = true;
    this->current_type = "primary";
    this->velocity = this->primary_weapon_velocity;
  }else if (isDownWrapper("b"))
  {
    current_input_replay_store += "b";
    this->shooting = true;
    this->current_type = "secondary";
    this->velocity = this->secondary_weapon_velocity;
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
    this->velocity = this->original_velocity;
    this->shooting = false;
  }

  if (!getGameOver())
    replay_storage.push_back(current_input_replay_store);
}

void Player::logic(int stage_velocity)
{
  animationControl();
  if (this->hp != 0)
  {
    inputControl();
  }
  else
  {
    this->onDefeated();
  }

  // Die if out of bounds
  if(!getGameOver()
    && !this->stage->playerIsInPlatform()
    && !isSlowEnabled()
    && this->current_state != "dash up"
    && this->current_state != "dash down"
    && this->current_state != "dash left"
    && this->current_state != "dash right")
  {
    this->hit(9999);
  }

  //Enable or disable slow
  if (!slow_in_cooldown
      && isDownWrapper("b"))
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

  if(!slow_in_cooldown && isDownWrapper("d") && !this->is_bomb_active)
  {
    this->onBomb();
  }

  bombLogic();

  for (auto pattern : type[current_type])
  {
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

  for (std::list<Pattern *>::iterator iterator = active_patterns->begin(); iterator != active_patterns->end(); iterator++)
  {
    Pattern *active_pattern = (Pattern *)*iterator;
    active_pattern->logic(stage_velocity);
  }


  iteration++;

  current_shield -= shield_fade;
  if (current_shield < 0)
    current_shield = 0;

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

  // Shadow control
  if(isSlowEnabled() && frame%10 == 0)
  {
    this->shadows.push_back(new ShadowControl(this->current_state, this->current_sprite, 128, this->x, this->y));
  }

  for(auto shadow : shadows)
  {
    shadow->alpha-=2;
  }

  if(shadows.size() > 0 && shadows.front()->alpha <= 0)
  {
    delete shadows.front();
    shadows.pop_front();    
  }

  this->frame++;
}

void Player::bottomRender()
{
  for(auto shadow : shadows)
  {
    Image* shadow_image = this->sprites[shadow->state][shadow->image_number];

    int previous_red = shadow_image->color_filter.red;
    int previous_green = shadow_image->color_filter.green;
    int previous_blue = shadow_image->color_filter.blue;
    int previous_alpha = shadow_image->color_filter.alpha;

    shadow_image->color_filter.alpha = shadow->alpha;
    shadow_image->color_filter.red = 0;
    shadow_image->color_filter.green = 0;

    rosalila()->graphics->drawImage(shadow_image,
      shadow->x - shadow_image->getWidth() / 2 + current_screen_shake_x,
      shadow->y - shadow_image->getHeight() / 2 + current_screen_shake_y);
    
    shadow_image->color_filter.red = previous_red;
    shadow_image->color_filter.green = previous_green;
    shadow_image->color_filter.blue = previous_blue;
    shadow_image->color_filter.alpha = previous_alpha;
  }

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
}

void Player::topRender()
{
  Character::topRender();

  double current_percentual_slow = 0;
  if(this->current_slow > 0)
    current_percentual_slow = (double)this->current_slow / (double)this->max_slow;
  double current_slow_bar_width = slow_bar_rect_width * current_percentual_slow;

  if(!slow_in_cooldown)
  {
    rosalila()->graphics->drawRectangle(slow_bar_x, slow_bar_y,
      (int)current_slow_bar_width, slow_bar_rect_height,
        0, slow_bar_color.red, slow_bar_color.green, slow_bar_color.blue, slow_bar_color.alpha);
  }else
  {
    rosalila()->graphics->drawRectangle(slow_bar_x, slow_bar_y,
      (int)current_slow_bar_width, slow_bar_rect_height,
        0, slow_bar_cooldown_color.red, slow_bar_cooldown_color.green, slow_bar_cooldown_color.blue, slow_bar_cooldown_color.alpha);
  }
  
  if(hitbox_animation_vector.size()>0)
  {
    this->hitbox_animation_vector[hitbox_animation_current_image]->color_filter.alpha = 255;
    rosalila()->graphics->drawImage(hitbox_animation_vector[hitbox_animation_current_image],
                                    this->x + this->hitbox_animation_x,
                                    this->y + this->hitbox_animation_y);
    if(this->frame % hitbox_animation_velocity == 0)
    {
      hitbox_animation_current_image++;
      if(hitbox_animation_current_image >= hitbox_animation_vector.size())
        hitbox_animation_current_image = 0;
    }
  }

  if(!jump_tutorial_was_pressed)
  {
    if(this->frame % 25 == 0)
    {
      jump_tutorial_frame++;
      if(jump_tutorial_frame == 2)
        jump_tutorial_frame = 0;
    }
    if(jump_tutorial_frame == 1)
    {
      jump_tutorial_image->color_filter.alpha = 255;
      rosalila()->graphics->drawImage(jump_tutorial_image, 50,750);
    }
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

  
  Node* hitbox_animation_node = root_node->getNodeByName("hitbox_animation");
  this->hitbox_animation_velocity = 0;
  if (hitbox_animation_node->hasAttribute("animation_velocity"))
    this->hitbox_animation_velocity = atoi(hitbox_animation_node->attributes["animation_velocity"].c_str());
  
  this->hitbox_animation_x = 0;
  if (hitbox_animation_node->hasAttribute("x"))
    this->hitbox_animation_x = atoi(hitbox_animation_node->attributes["x"].c_str());

  this->hitbox_animation_y = 0;
  if (hitbox_animation_node->hasAttribute("y"))
    this->hitbox_animation_y = atoi(hitbox_animation_node->attributes["y"].c_str());

  vector<Node *> hitbox_animation_sprites_nodes = hitbox_animation_node->getNodesByName("sprites");
  for (int i = 0; i < (int)hitbox_animation_sprites_nodes.size(); i++)
  {
    string sprite_path = hitbox_animation_sprites_nodes[i]->attributes["path"];
    hitbox_animation_vector.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + sprite_path));
  }

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
  delete root_node;
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

void Player::bombLogic()
{
  if(this->is_bomb_active)
  {
    this->enemy->deleteActivePatterns();

    this->current_slow-=6;
    if(current_slow <= 0)
    {
      onBombFinished();
    }

    current_bomb_frame++;
    if(current_bomb_frame % bomb_image_duration == 0)
    {
      current_bomb_image++;
      if(current_bomb_image >= bomb_images.size())
        current_bomb_image = 0;
    }
  }
}

void Player::onBomb()
{
  if (rosalila()->sound->soundExists(name + ".bomb"))
    this->bomb_channel = rosalila()->sound->playSound(name + ".bomb", -1, 99, 0);

  this->current_bomb_image = 0;
  this->current_bomb_frame = 0;
  this->is_bomb_active = true;
  int bomb_duration = this->current_slow/6 + 6;
  rosalila()->graphics->screen_shake_effect.set(15, bomb_duration, 0, 0);
}

void Player::onBombFinished()
{
  this->is_bomb_active = false;
  if(bomb_channel != -1)
  {
    rosalila()->sound->stopSound(this->bomb_channel);
    this->bomb_channel = -1;
  }
}

void Player::onDefeated()
{
  if (current_state != "destroyed" && rosalila()->sound->soundExists(name + ".destroyed"))
    rosalila()->sound->playSound(name + ".destroyed", 1, 0, this->x);
  this->setState("destroyed");
}

void Player::onDash()
{
  this->jump_tutorial_was_pressed = true;

  if (rosalila()->sound->soundExists(name + ".dash"))
    rosalila()->sound->playSound(name + ".dash", -1, 0, this->x);
  
  if(isDownWrapper("6")
      && this->current_state != "dash right"
      && this->hasState("dash right"))
  {
    dash_extra_velocity_x = dash_velocity;
    this->setState("dash right");
  }
  if(isDownWrapper("4")
      && this->current_state != "dash left"
      && this->hasState("dash left"))
  {
    dash_extra_velocity_x = -dash_velocity;
    this->setState("dash left");
  }
  if(isDownWrapper("2")
      && this->current_state != "dash down"
      && this->hasState("dash down"))
  {
    dash_extra_velocity_y = dash_velocity;
    this->setState("dash down");
  }
  if(isDownWrapper("8")
      && this->current_state != "dash up"
      && this->hasState("dash up"))
  {
    dash_extra_velocity_y = -dash_velocity;
    this->setState("dash up");
  }
}