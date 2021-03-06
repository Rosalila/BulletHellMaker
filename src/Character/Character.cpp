#include "Character/Character.h"
#include "Utility/Utility.h"

Character::Character(std::string name, int sound_channel_base)
{
  //Setting up the other variables
  this->frame = 0;
  this->name = name;
  this->directory = "";
  this->active_patterns = new std::list<Pattern *>;
  this->x = 500;
  this->y = 500;
  this->shooting = true;
  this->current_state = "start";
  this->current_type = "primary";
  this->visible = true;
  this->life_bar = NULL;

  //Sprites animation
  this->animation_iteration = 0;
  this->current_sprite = 0;

  this->life_bar_x = 0;
  this->life_bar_y = 0;

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

  if(life_bar)
    this->life_bar->color_filter.alpha = 255;
}

Character::~Character()
{
  for (map<string, vector<Image *>>::iterator i = sprites.begin(); i != sprites.end(); i++)
  {
    for (int j = 0; j < (int)(*i).second.size(); j++)
    {
      delete (*i).second[j];
    }
  }

  for (map<string, vector<Image *>>::iterator i = animation_images.begin(); i != animation_images.end(); i++)
  {
    for (int j = 0; j < (int)(*i).second.size(); j++)
    {
      delete (*i).second[j];
    }
  }

  for (map<string, Bullet *>::iterator i = bullets.begin(); i != bullets.end(); i++)
  {
    delete (*i).second;
  }

  for (map<string, vector<Pattern *>>::iterator i = type.begin(); i != type.end(); i++)
  {
    for (int j = 0; j < (int)(*i).second.size(); j++)
    {
      delete (*i).second[j];
    }
  }

  for (std::list<Pattern *>::iterator i = (*active_patterns).begin(); i != (*active_patterns).end(); i++)
  {
    delete (*i);
  }

  delete active_patterns;

  if (this->life_bar)
    delete this->life_bar;

  for (int i = 0; i < (int)hitboxes.size(); i++)
  {
    delete hitboxes[i];
  }
}

void Character::loadFromXML()
{
  loadMainXML();

  loadBulletsXML();

  loadPatternsXML();
}

void Character::loadMainXML()
{
  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "character.json");

  this->velocity = 5;
  if (root_node->hasAttribute("velocity"))
  {
    this->velocity = atoi(root_node->attributes["velocity"].c_str());
  }

  this->animation_velocity = 5;
  if (root_node->hasAttribute("animation_velocity"))
  {
    this->animation_velocity = atoi(root_node->attributes["animation_velocity"].c_str());
  }

  this->max_hp = 100;
  this->hp = 100;
  if (root_node->hasAttribute("hp"))
  {
    this->max_hp = atoi(root_node->attributes["hp"].c_str());
  }
  this->hp = this->max_hp;

  Node *initial_position_node = root_node->getNodeByName("initial_position");
  this->x = 100;
  if (initial_position_node->hasAttribute("x"))
  {
    this->x = atoi(initial_position_node->attributes["x"].c_str());
  }
  this->y = 500;
  if (initial_position_node->hasAttribute("y"))
  {
    this->y = atoi(initial_position_node->attributes["y"].c_str());
  }

  this->life_bar_x = 0;
  this->life_bar_y = 0;
  this->life_bar_rect_offset_x = 0;
  this->life_bar_rect_offset_y = 0;
  this->life_bar_rect_height = 0;
  this->life_bar_rect_width = 0;
  this->color.red = 0;
  this->color.green = 0;
  this->color.blue = 0;
  this->color.alpha = 255;

  Node *life_bar_node = root_node->getNodeByName("life_bar");
  
  if (life_bar_node)
  {
    if (life_bar_node->hasAttribute("x"))
      this->life_bar_x = atoi(life_bar_node->attributes["x"].c_str());

    if (life_bar_node->hasAttribute("y"))
      this->life_bar_y = atoi(life_bar_node->attributes["y"].c_str());

    if (life_bar_node->hasAttribute("width"))
      this->life_bar_rect_width = atoi(life_bar_node->attributes["width"].c_str());

    if (life_bar_node->hasAttribute("height"))
      this->life_bar_rect_height = atoi(life_bar_node->attributes["height"].c_str());

    if (life_bar_node->hasAttribute("rect_offset_x"))
      this->life_bar_rect_offset_x = atoi(life_bar_node->attributes["color_offset_x"].c_str());

    if (life_bar_node->hasAttribute("rect_offset_y"))
      this->life_bar_rect_offset_y = atoi(life_bar_node->attributes["color_offset_y"].c_str());

    if (life_bar_node->hasAttribute("image"))
    {
      this->life_bar = rosalila()->graphics->getImage(std::string(assets_directory) + directory + life_bar_node->attributes["image"]);
    }

    Node *color_node = life_bar_node->getNodeByName("color");
    if(color_node)
    {
      if (color_node->hasAttribute("red"))
        this->color.red = atoi(color_node->attributes["red"].c_str());

      if (color_node->hasAttribute("green"))
        this->color.green = atoi(color_node->attributes["green"].c_str());

      if (color_node->hasAttribute("blue"))
        this->color.blue = atoi(color_node->attributes["blue"].c_str());

      if (color_node->hasAttribute("alpha"))
        this->color.alpha = atoi(color_node->attributes["alpha"].c_str());
    }
  }

  vector<Node *> hitboxes_node = root_node->getNodesByName("hitboxes");

  for (int i = 0; i < (int)hitboxes_node.size(); i++)
  {
    Node *hitbox_node = hitboxes_node[i];

    int hitbox_x = atoi(hitbox_node->attributes["x"].c_str());
    int hitbox_y = atoi(hitbox_node->attributes["y"].c_str());
    int hitbox_width = atoi(hitbox_node->attributes["width"].c_str());
    int hitbox_height = atoi(hitbox_node->attributes["height"].c_str());
    int hitbox_angle = 0;
    if (hitbox_node->hasAttribute("angle"))
      hitbox_angle = atoi(hitbox_node->attributes["angle"].c_str());

    Hitbox *hitbox = new Hitbox(hitbox_x, hitbox_y, hitbox_width, hitbox_height, (float)hitbox_angle);
    this->hitboxes.push_back(hitbox);
  }

  Node *sounds_node = root_node->getNodeByName("sounds");

  if (sounds_node)
  {
    if (sounds_node->hasAttribute("hit"))
      rosalila()->sound->addSound(this->name + ".hit", std::string(assets_directory) + directory + sounds_node->attributes["hit"]);

    if (sounds_node->hasAttribute("destroyed"))
      rosalila()->sound->addSound(this->name + ".destroyed", std::string(assets_directory) + directory + sounds_node->attributes["destroyed"]);
  }

  vector<Node *> sprites_nodes = root_node->getNodesByName("states");

  for (int i = 0; i < (int)sprites_nodes.size(); i++)
  {
    std::vector<Image *> sprites_vector;

    std::string sprites_state = sprites_nodes[i]->attributes["name"];

    if (sprites_nodes[i]->hasAttribute("sound"))
    {
      std::string sprites_sound = sprites_nodes[i]->attributes["sound"];
      rosalila()->sound->addSound(name + "." + sprites_state, std::string(assets_directory) + directory + sprites_sound);
    }

    Node *conditions = sprites_nodes[i]->getNodeByName("conditions");
    if (conditions)
    {
      for(map<string,string>::iterator i = conditions->attributes.begin(); 
          i != conditions->attributes.end();
          i++)
      {
        state_triggers[sprites_state][(*i).first] = (*i).second;
      }
    }

    vector<Node *> sprite_nodes = sprites_nodes[i]->getNodesByName("sprites");

    for (int j = 0; j < (int)sprite_nodes.size(); j++)
    {
      string sprite_path = sprite_nodes[j]->attributes["path"];
      sprites_vector.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + sprite_path));
    }

    sprites[sprites_state] = sprites_vector;
  }

  vector<Node *> animations_nodes = root_node->getNodesByName("animations");

  for (int i = 0; i < (int)animations_nodes.size(); i++)
  {
    std::vector<Image *> images_vector;
    std::string name = animations_nodes[i]->attributes["name"];

    vector<Node *> animation_images_nodes = animations_nodes[i]->getNodesByName("images");
    
    for (int i = 0; i < (int)animation_images_nodes.size(); i++)
    {
      std::string path = animation_images_nodes[i]->attributes["path"];
      images_vector.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + this->directory + path));
    }

    this->animation_images[name] = images_vector;
  }


  delete root_node;
}

void Character::loadBulletsXML()
{
  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "bullets.json");

  vector<Node *> bullet_nodes = root_node->getNodesByName("bullets");

  for (int i = 0; i < (int)bullet_nodes.size(); i++)
  {
    std::string node_name = bullet_nodes[i]->attributes["name"];
    vector<string> random_sounds;

    if (bullet_nodes[i]->hasAttribute("sound_hit"))
    {
      std::string sound_hit = std::string(assets_directory) + directory + bullet_nodes[i]->attributes["sound_hit"];
      rosalila()->sound->addSound("bullet_hit." + node_name, sound_hit);
    }

    int randomize_sound_frequency = 1;
    if (bullet_nodes[i]->hasAttribute("randomize_sound_frequency"))
    {
      randomize_sound_frequency = atoi(bullet_nodes[i]->attributes["randomize_sound_frequency"].c_str());
    }

    int arpeggio_length = 1;
    if (bullet_nodes[i]->hasAttribute("arpeggio_length"))
    {
      arpeggio_length = atoi(bullet_nodes[i]->attributes["arpeggio_length"].c_str());
    }

    Node* sound_node = bullet_nodes[i]->getNodeByName("sound");

    int sound_channel = -1;
    if(sound_node)
    {
      if (sound_node->hasAttribute("channel"))
      {
        sound_channel = atoi(sound_node->attributes["channel"].c_str());
        if (sound_channel != -1)
          sound_channel += sound_channel_base;
      }
      if (sound_node->hasAttribute("path"))
      {
        std::string sound = std::string(assets_directory) + directory + sound_node->attributes["path"];
        rosalila()->sound->addSound("bullet." + node_name, sound);
        random_sounds.push_back("bullet." + node_name);
      }
    }

    int damage = 1;
    if (bullet_nodes[i]->hasAttribute("damage"))
    {
      damage = atoi(bullet_nodes[i]->attributes["damage"].c_str());
    }
    vector<Node *> sprites_nodes = bullet_nodes[i]->getNodesByName("sprites");
    vector<Image *> sprites_temp;
    for (int j = 0; j < (int)sprites_nodes.size(); j++)
    {
      sprites_temp.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + sprites_nodes[j]->attributes["path"]));
    }
    vector<Node *> hitbox_nodes = bullet_nodes[i]->getNodesByName("hitboxes");
    vector<Hitbox *> hitboxes_temp;
    for (int j = 0; j < (int)hitbox_nodes.size(); j++)
    {
      int x = 0;
      if (hitbox_nodes[j]->hasAttribute("x"))
      {
        x = atoi(hitbox_nodes[j]->attributes["x"].c_str());
      }

      int y = 0;
      if (hitbox_nodes[j]->hasAttribute("y"))
      {
        y = atoi(hitbox_nodes[j]->attributes["y"].c_str());
      }

      int width = 0;
      if (hitbox_nodes[j]->hasAttribute("width"))
      {
        width = atoi(hitbox_nodes[j]->attributes["width"].c_str());
      }

      int height = 0;
      if (hitbox_nodes[j]->hasAttribute("height"))
      {
        height = atoi(hitbox_nodes[j]->attributes["height"].c_str());
      }

      int angle = 0;
      if (hitbox_nodes[j]->hasAttribute("angle"))
      {
        angle = atoi(hitbox_nodes[j]->attributes["angle"].c_str());
      }

      hitboxes_temp.push_back(new Hitbox(x, y, width, height, (float)angle));
    }

    Node *onhit_node = bullet_nodes[i]->getNodeByName("on_hit_sprites");
    vector<Image *> sprites_onhit_temp;
    
    if (onhit_node)
    {
      vector<Node *> on_hit_sprites_node = bullet_nodes[i]->getNodesByName("on_hit_sprites");
      for (int j = 0; j < (int)on_hit_sprites_node.size(); j++)
      {
        sprites_onhit_temp.push_back(rosalila()->graphics->getImage(std::string(assets_directory) + directory + "sprites/" + on_hit_sprites_node[j]->attributes["path"]));
      }
    }
    Node *random_sound_node = bullet_nodes[i]->getNodeByName("RandomSound");

    if (random_sound_node)
    {
      vector<Node *> sound_nodes = random_sound_node->getNodesByName("Sound");

      for (int j = 0; j < (int)sound_nodes.size(); j++)
      {
        std::string sound = std::string(assets_directory) + directory + sound_nodes[j]->attributes["path"];
        rosalila()->sound->addSound("bullet." + node_name + sound_nodes[j]->attributes["path"], sound);
        random_sounds.push_back("bullet." + node_name + sound_nodes[j]->attributes["path"]);
      }
    }

    Node *color_node = bullet_nodes[i]->getNodeByName("color");

    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 255;
    if (color_node)
    {
      if (color_node->hasAttribute("red"))
        red = atoi(color_node->attributes["red"].c_str());
      if (color_node->hasAttribute("green"))
        green = atoi(color_node->attributes["green"].c_str());
      if (color_node->hasAttribute("blue"))
        blue = atoi(color_node->attributes["blue"].c_str());
      if (color_node->hasAttribute("alpha"))
        alpha = atoi(color_node->attributes["alpha"].c_str());
    }

    bullets[node_name] = new Bullet(node_name, sprites_temp, sprites_onhit_temp, Color(red, green, blue, alpha), hitboxes_temp, random_sounds, randomize_sound_frequency, arpeggio_length, damage, sound_channel);
  }

  delete root_node;
}

Pattern *Character::loadPatternXML(Node *pattern_node)
{
  std::string bullet_name = pattern_node->attributes["bullet"];
  Bullet *bullet = bullets[bullet_name];

  int acceleration = 0;
  if (pattern_node->hasAttribute("acceleration"))
    acceleration = atoi(pattern_node->attributes["acceleration"].c_str());

  int a_frequency = 0;
  if (pattern_node->hasAttribute("a_frequency"))
    a_frequency = atoi(pattern_node->attributes["a_frequency"].c_str());

  double additional_player_hp_change = 0;
  if (pattern_node->hasAttribute("additional_player_hp_change"))
    additional_player_hp_change = atof(pattern_node->attributes["additional_player_hp_change"].c_str());

  double additional_player_velocity_x = 0;
  if (pattern_node->hasAttribute("additional_player_velocity_x"))
    additional_player_velocity_x = atof(pattern_node->attributes["additional_player_velocity_x"].c_str());

  double additional_player_velocity_y = 0;
  if (pattern_node->hasAttribute("additional_player_velocity_y"))
    additional_player_velocity_y = atof(pattern_node->attributes["additional_player_velocity_y"].c_str());

  bool aim_player = false;
  if (pattern_node->hasAttribute("aim_player"))
    aim_player = pattern_node->attributes["aim_player"] == "yes";

  int angle = 0;
  if (pattern_node->hasAttribute("angle"))
    angle = atoi(pattern_node->attributes["angle"].c_str());

  int angle_change = 0;
  if (pattern_node->hasAttribute("angle_change"))
    angle_change = atoi(pattern_node->attributes["angle_change"].c_str());

  int ac_frequency = 0;
  if (pattern_node->hasAttribute("ac_frequency"))
    ac_frequency = atoi(pattern_node->attributes["ac_frequency"].c_str());

  int animation_velocity = 0;
  if (pattern_node->hasAttribute("animation_velocity"))
    animation_velocity = atoi(pattern_node->attributes["animation_velocity"].c_str());

  double auto_scale = 0;
  if (pattern_node->hasAttribute("auto_scale"))
    auto_scale = atof(pattern_node->attributes["auto_scale"].c_str());

  int bullet_rotation = 0;
  if (pattern_node->hasAttribute("bullet_rotation"))
    bullet_rotation = atoi(pattern_node->attributes["bullet_rotation"].c_str());

  int br_change = 0;
  if (pattern_node->hasAttribute("br_change"))
    br_change = atoi(pattern_node->attributes["br_change"].c_str());

  bool collides_bullets = false;
  if (pattern_node->hasAttribute("collides_bullets"))
    collides_bullets = pattern_node->attributes["collides_bullets"] == "yes";

  bool collides_opponent = true;
  if (pattern_node->hasAttribute("collides_opponent"))
    collides_opponent = pattern_node->attributes["collides_opponent"] == "yes";
  
  int cooldown = 0;
  if (pattern_node->hasAttribute("cooldown"))
    cooldown = atoi(pattern_node->attributes["cooldown"].c_str());

  int duration = -1;
  if (pattern_node->hasAttribute("duration"))
    duration = atoi(pattern_node->attributes["duration"].c_str());

  bool freeze = false;
  if (pattern_node->hasAttribute("freeze"))
    freeze = pattern_node->attributes["freeze"] == "yes";

  bool homing = false;
  if (pattern_node->hasAttribute("homing"))
    homing = pattern_node->attributes["homing"] == "yes";

  bool independent_br = false;
  if (pattern_node->hasAttribute("independent_br"))
    independent_br = pattern_node->attributes["independent_br"] == "yes";
  
  int max_velocity = 9999999;
  if (pattern_node->hasAttribute("max_velocity"))
    max_velocity = atoi(pattern_node->attributes["max_velocity"].c_str());

  int offset_x = 0;
  if (pattern_node->hasAttribute("offset_x"))
    offset_x = atoi(pattern_node->attributes["offset_x"].c_str());

  int offset_y = 0;
  if (pattern_node->hasAttribute("offset_y"))
    offset_y = atoi(pattern_node->attributes["offset_y"].c_str());

  int random_angle = 0;
  if (pattern_node->hasAttribute("random_angle"))
    random_angle = atoi(pattern_node->attributes["random_angle"].c_str());

  int startup = 0;
  if (pattern_node->hasAttribute("startup"))
    startup = atoi(pattern_node->attributes["startup"].c_str());

  int stop_ac_at = -1;
  if (pattern_node->hasAttribute("stop_ac_at"))
    stop_ac_at = atoi(pattern_node->attributes["stop_ac_at"].c_str());

  bool undestructable = false;
  if (pattern_node->hasAttribute("undestructable"))
    undestructable = pattern_node->attributes["undestructable"] == "yes";

  int velocity = 0;
  if (pattern_node->hasAttribute("velocity"))
    velocity = atoi(pattern_node->attributes["velocity"].c_str());

  bool aim_player_on_begin = false;
  if (pattern_node->hasAttribute("aim_player_on_begin"))
    aim_player_on_begin = pattern_node->attributes["aim_player_on_begin"] == "yes";

  std::map<int, vector<Modifier *>> *pattern_modifiers = new std::map<int, vector<Modifier *>>();

  vector<Node *> modifier_nodes = pattern_node->getNodesByName("modifiers");
  for (int i = 0; i < (int)modifier_nodes.size(); i++)
  {
    int at = atoi(modifier_nodes[i]->attributes["at"].c_str());
    (*pattern_modifiers)[at] = loadModifierXML(modifier_nodes[i]);
  }

  double player_velocity_override = 0;
  if (pattern_node->hasAttribute("player_velocity_override"))
    player_velocity_override = atof(pattern_node->attributes["player_velocity_override"].c_str());

  return new Pattern(velocity, max_velocity, acceleration, a_frequency, (float)angle, angle_change, stop_ac_at, ac_frequency, animation_velocity, auto_scale, bullet, offset_x, offset_y,
                     startup, cooldown, duration, random_angle, aim_player, bullet_rotation, br_change, independent_br, freeze, homing, collides_bullets, collides_opponent, undestructable, pattern_modifiers, &bullets,
                     additional_player_velocity_x, additional_player_velocity_y, additional_player_hp_change, player_velocity_override,
                     aim_player_on_begin
                     );
}

vector<Modifier *> Character::loadModifierXML(Node *modifier_node)
{
  vector<Modifier *> temp_modifiers;

  if (modifier_node->hasAttribute("bullet"))
  {
    std::string value = modifier_node->attributes["bullet"];
    temp_modifiers.push_back(new Modifier("bullet", value));
  }

  if (modifier_node->hasAttribute("velocity"))
  {
    std::string value = modifier_node->attributes["velocity"];
    temp_modifiers.push_back(new Modifier("velocity", value));
  }

  if (modifier_node->hasAttribute("max_velocity"))
  {
    std::string value = modifier_node->attributes["max_velocity"];
    temp_modifiers.push_back(new Modifier("max_velocity", value));
  }

  if (modifier_node->hasAttribute("acceleration"))
  {
    std::string value = modifier_node->attributes["acceleration"];
    temp_modifiers.push_back(new Modifier("acceleration", value));
  }

  if (modifier_node->hasAttribute("a_frequency"))
  {
    std::string value = modifier_node->attributes["a_frequency"];
    temp_modifiers.push_back(new Modifier("a_frequency", value));
  }

  if (modifier_node->hasAttribute("angle"))
  {
    std::string value = modifier_node->attributes["angle"];
    temp_modifiers.push_back(new Modifier("angle", value));
  }

  if (modifier_node->hasAttribute("angle_change"))
  {
    std::string value = modifier_node->attributes["angle_change"];
    temp_modifiers.push_back(new Modifier("angle_change", value));
  }

  if (modifier_node->hasAttribute("stop_ac_at"))
  {
    std::string value = modifier_node->attributes["stop_ac_at"];
    temp_modifiers.push_back(new Modifier("stop_ac_at", value));
  }

  if (modifier_node->hasAttribute("ac_frequency"))
  {
    std::string value = modifier_node->attributes["ac_frequency"];
    temp_modifiers.push_back(new Modifier("ac_frequency", value));
  }

  if (modifier_node->hasAttribute("animation_velocity"))
  {
    std::string value = modifier_node->attributes["animation_velocity"];
    temp_modifiers.push_back(new Modifier("animation_velocity", value));
  }

  if (modifier_node->hasAttribute("offset_x"))
  {
    std::string value = modifier_node->attributes["offset_x"];
    temp_modifiers.push_back(new Modifier("offset_x", value));
  }

  if (modifier_node->hasAttribute("offset_y"))
  {
    std::string value = modifier_node->attributes["offset_y"];
    temp_modifiers.push_back(new Modifier("offset_y", value));
  }

  if (modifier_node->hasAttribute("startup"))
  {
    std::string value = modifier_node->attributes["startup"];
    temp_modifiers.push_back(new Modifier("startup", value));
  }

  if (modifier_node->hasAttribute("cooldown"))
  {
    std::string value = modifier_node->attributes["cooldown"];
    temp_modifiers.push_back(new Modifier("cooldown", value));
  }

  if (modifier_node->hasAttribute("duration"))
  {
    std::string value = modifier_node->attributes["duration"];
    temp_modifiers.push_back(new Modifier("duration", value));
  }

  if (modifier_node->hasAttribute("random_angle"))
  {
    std::string value = modifier_node->attributes["random_angle"];
    temp_modifiers.push_back(new Modifier("random_angle", value));
  }

  if (modifier_node->hasAttribute("aim_player"))
  {
    std::string value = modifier_node->attributes["aim_player"];
    temp_modifiers.push_back(new Modifier("aim_player", value));
  }

  if (modifier_node->hasAttribute("bullet_rotation"))
  {
    std::string value = modifier_node->attributes["bullet_rotation"];
    temp_modifiers.push_back(new Modifier("bullet_rotation", value));
  }

  if (modifier_node->hasAttribute("br_change"))
  {
    std::string value = modifier_node->attributes["br_change"];
    temp_modifiers.push_back(new Modifier("br_change", value));
  }

  if (modifier_node->hasAttribute("independent_br"))
  {
    std::string value = modifier_node->attributes["independent_br"];
    temp_modifiers.push_back(new Modifier("independent_br", value));
  }
  if (modifier_node->hasAttribute("freeze"))
  {
    std::string value = modifier_node->attributes["freeze"];
    temp_modifiers.push_back(new Modifier("freeze", value));
  }

  if (modifier_node->hasAttribute("homing"))
  {
    std::string value = modifier_node->attributes["homing"];
    temp_modifiers.push_back(new Modifier("homing", value));
  }

  return temp_modifiers;
}

void Character::loadPatternsXML()
{
  Node *root_node = rosalila()->parser->getNodes(std::string(assets_directory) + directory + "attacks.json");

  vector<Node *> type_nodes = root_node->getNodesByName("attacks");

  for (int i = 0; i < (int)type_nodes.size(); i++)
  {
    std::string type_name = type_nodes[i]->attributes["name"];

    std::vector<Pattern *> patterns;

    vector<Node *> pattern_nodes = type_nodes[i]->getNodesByName("patterns");

    for (int j = 0; j < (int)pattern_nodes.size(); j++)
    {
      patterns.push_back(loadPatternXML(pattern_nodes[j]));
    }

    vector<Node *> repeat_nodes = type_nodes[i]->getNodesByName("repeat");

    for (int j = 0; j < (int)repeat_nodes.size(); j++)
    {
      int amount = atoi(repeat_nodes[j]->attributes["amount"].c_str());

      vector<Node *> pattern_nodes = repeat_nodes[j]->getNodesByName("patterns");

      for (int k = 0; k < (int)pattern_nodes.size(); k++)
      {
        for (int l = 0; l < amount; l++)
        {
          Pattern *p = loadPatternXML(pattern_nodes[k]);

          if (repeat_nodes[j]->hasAttribute("velocity"))
            p->velocity += atoi(repeat_nodes[j]->attributes["velocity"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("max_velocity"))
            p->max_velocity += atoi(repeat_nodes[j]->attributes["max_velocity"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("acceleration"))
            p->acceleration += atoi(repeat_nodes[j]->attributes["acceleration"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("a_frequency"))
            p->a_frequency += atoi(repeat_nodes[j]->attributes["a_frequency"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("angle"))
            p->angle += atoi(repeat_nodes[j]->attributes["angle"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("angle_change"))
            p->angle_change += atoi(repeat_nodes[j]->attributes["angle_change"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("stop_ac_at"))
            p->stop_ac_at += atoi(repeat_nodes[j]->attributes["stop_ac_at"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("ac_frequency"))
            p->ac_frequency += atoi(repeat_nodes[j]->attributes["ac_frequency"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("offset_x"))
            p->offset_x += atoi(repeat_nodes[j]->attributes["offset_x"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("offset_y"))
            p->offset_y += atoi(repeat_nodes[j]->attributes["offset_y"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("startup"))
          {
            p->repeat_startup_offset = atoi(repeat_nodes[j]->attributes["startup"].c_str()) * l;
            p->startup += p->repeat_startup_offset;
          }

          if (repeat_nodes[j]->hasAttribute("cooldown"))
          {
            p->repeat_cooldown_offset += atoi(repeat_nodes[j]->attributes["cooldown"].c_str()) * l;
            p->cooldown += p->repeat_cooldown_offset;
          }

          if (repeat_nodes[j]->hasAttribute("animation_velocity"))
            p->animation_velocity += atoi(repeat_nodes[j]->attributes["animation_velocity"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("random_angle"))
            p->random_angle += atoi(repeat_nodes[j]->attributes["random_angle"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("bullet_rotation"))
            p->bullet_rotation += atoi(repeat_nodes[j]->attributes["bullet_rotation"].c_str()) * l;

          if (repeat_nodes[j]->hasAttribute("br_change"))
            p->br_change += atoi(repeat_nodes[j]->attributes["br_change"].c_str()) * l;

          patterns.push_back(p);
        }
      }
    }

    type[type_name] = patterns;
  }

  delete root_node;
}

void Character::animationControl()
{
  if (current_state == "destroyed")
    visible = false;
  if (animation_iteration >= animation_velocity)
  {
    current_sprite++;
    if (current_sprite >= (int)sprites[current_state].size())
    {
      current_sprite = 0;
    }
    animation_iteration = 0;
  }
  if (getIterateSlowdownFlag())
    animation_iteration++;
}

void Character::bottomRender()
{
  if (shake_time > 0)
  {
    shake_time--;
    if (shake_time == 0)
    {
      current_screen_shake_x = 0;
      current_screen_shake_y = 0;
    }
    else
    {
      current_screen_shake_x = (rand() * 10000) % shake_magnitude;
      current_screen_shake_y = (rand() * 10000) % shake_magnitude;
    }
  }

  if(sprites.find(current_state) != sprites.end() && current_sprite < sprites[current_state].size())
  {
    Image * current_image = sprites[current_state][current_sprite];

    current_image->color_filter.red = color_filter_red;
    current_image->color_filter.green = color_filter_green;
    current_image->color_filter.blue = color_filter_blue;
    current_image->color_filter.alpha = color_filter_alpha;

    rosalila()->graphics->drawImage(current_image,
      (int)this->x - current_image->getWidth() / 2 + current_screen_shake_x,
      (int)this->y - current_image->getHeight() / 2 + current_screen_shake_y);
  }

  if (rosalila()->receiver->isKeyDown(SDLK_h))
  {
    for (int i = 0; i < (int)hitboxes.size(); i++)
    {
      rosalila()->graphics->drawRectangle((int)(hitboxes[i]->getX() + x),
		  (int)(hitboxes[i]->getY() + y),
		  hitboxes[i]->getWidth(), hitboxes[i]->getHeight(),
		  hitboxes[i]->angle, 100, 0, 0, 100);
    }
  }
}

void Character::topRender()
{
  vector<DrawableRectangle *> rectangles;
  for (std::list<Pattern *>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
      (*pattern)->render();

  double current_percentual_hp = 0;
  if(this->hp > 0)
    current_percentual_hp = (double)this->hp / (double)this->max_hp;
  double current_life_bar_width = life_bar_rect_width * current_percentual_hp;

  rosalila()->graphics->drawRectangle(life_bar_x, life_bar_y,
	  (int)current_life_bar_width, life_bar_rect_height,
      0, color.red, color.green, color.blue, 255);
  
  if(this->life_bar)
  {
    this->life_bar->color_filter.alpha = 255;
    rosalila()->graphics->drawImage(this->life_bar,this->life_bar_x, this->life_bar_y);
  }

  rosalila()->graphics->drawRectangles(rectangles);

  for (int i = 0; i < (int)rectangles.size(); i++)
    delete rectangles[i];
}

void Character::setState(string state)
{
  this->current_state = state;
  this->current_sprite = 0;
}

bool Character::hasState(string state)
{
  if(this->sprites.find(state) == this->sprites.end())
    return false;
  return true;
}

bool Character::collides(Hitbox hitbox, int hitbox_x, int hitbox_y, float hitbox_angle)
{
  if (!visible)
    return false;
  for (int i = 0; i < (int)hitboxes.size(); i++)
    if (hitboxes[i]->getPlacedHitbox(this->x, this->y).collides(hitbox))
      return true;
  return false;
}

void Character::hit(int damage)
{
  this->hp -= damage;
  if (hp < 0)
    hp = 0;
}

void Character::addActivePattern(Pattern *pattern, int new_pattern_x, int new_pattern_y)
{
  Pattern *new_pattern = new Pattern(pattern, new_pattern_x, new_pattern_y);
  float angle = new_pattern->angle;
  angle += new_pattern->getRandomAngle();
  new_pattern->angle = angle;
  active_patterns->push_back(new_pattern);
}

void Character::shakeScreen(int shake_magnitude, int shake_time)
{
  this->shake_magnitude = shake_magnitude;
  this->shake_time = shake_time;
}

void Character::deleteActivePatterns()
{
  std::list<Pattern *>::iterator i = active_patterns->begin();
  while (i != active_patterns->end())
  {
    Pattern *p = (Pattern *)*i;
    if(!p->is_hit && p->bullet->name != "cancel_reward")
      p->hit(sound_channel_base + 1, true);
    i++;
  }
}
