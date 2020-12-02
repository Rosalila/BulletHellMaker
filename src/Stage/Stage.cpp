#include "Stage/Stage.h"
#include "../Character/Player.h"

Stage::Stage(Player* player)
{
  this->player = player;
  this->iterator = 0;
  this->iterate_slowdown_flag = false;
  this->current_slowdown_iteration = 0;
  this->layer_transparency = 255;
  this->is_mod = false;
  this->path = "";
}

Stage::~Stage()
{
  rosalila()->utility->writeLogLine("Deleting stage.");
  for (; !back.empty();)
  {
    Layer *layer = back.back();
    back.pop_back();
    delete layer;
  }
  for (; !front.empty();)
  {
    Layer *layer = front.back();
    front.pop_back();
    delete layer;
  }
}

void Stage::drawLayer(Layer *layer)
{
  //Animation speed
  if (layer->time_elapsed > layer->frame_duration)
  {
    layer->current_frame++;
    layer->time_elapsed = 0;
  }

  //Loop animation
  if (getIterateSlowdownFlag())
    layer->time_elapsed++;
  
  if(isSlowEnabled())
  {
    layer->color.red-=5;
    if(layer->color.red<0)
      layer->color.red=0;
  }else if(!getGameOver())
  {
    layer->color.red+=10;
    if(layer->color.red>255)
      layer->color.red = 255;
  }
  

  if (layer->current_frame >= (int)layer->layer_frames.size())
    layer->current_frame = 0;

  if (getGameOver())
  {
    if (getPlayerWon())
    {
      layer_transparency = 128;
    }
    else
    {
      layer->color.red -= 3;
      layer->color.green -= 3;
      layer->color.blue -= 3;
      if (layer->color.red < 0)
        layer->color.red = 0;
      if (layer->color.green < 0)
        layer->color.green = 0;
      if (layer->color.blue < 0)
        layer->color.blue = 0;
    }
  }

  //Paint

  LayerFrame *current_layer_frame = layer->layer_frames[layer->current_frame];

  if(layer->type != "obstacle"
      || (layer->type == "obstacle" && layer->obstacle_hit_points > 0))
  {
    int frame_width = current_layer_frame->width;
    int frame_heigth = current_layer_frame->height;

    vector<DrawableRectangle *> rectangles;

    for (int i = 0; i < rosalila()->graphics->screen_width / (frame_width + layer->separation_x) + 2; i++)
    {
      int pos_x = layer->x + i * (frame_width + layer->separation_x);
      int pos_y = rosalila()->graphics->screen_height - frame_heigth - layer->y;

      if (current_layer_frame->type == "image")
      {
        Image *image = current_layer_frame->image;
        //TODO: Fix paralax
        //TODO: Review Layer and LayerFrame name, variable names and stuff
        image->width = frame_width;
        image->height = frame_heigth;
        image->blend_effect = layer->blend_effect;
        image->color_filter.red =  layer->color.red;
        image->color_filter.green =  layer->color.green;
        image->color_filter.blue =  layer->color.blue;
        image->color_filter.alpha =  layer->color.alpha;
        rosalila()->graphics->drawImage(image,pos_x, pos_y);

        if (rosalila()->receiver->isKeyDown(SDLK_h))
        {
          rosalila()->graphics->drawRectangle(pos_x + layer->bounds_x,
                                              rosalila()->graphics->screen_height - current_layer_frame->height - layer->y - layer->bounds_y,
                                              layer->bounds_width,
                                              layer->bounds_height,
                                              0,
                                              0, 100, 0, 100);
        }
      }
      if (current_layer_frame->type == "rectangle")
      {
        rectangles.push_back(new DrawableRectangle(pos_x, pos_y, frame_width, frame_heigth, 0, Color(layer->color.red, layer->color.green, layer->color.blue, layer->color.alpha)));
      }
    }
    rosalila()->graphics->drawRectangles(rectangles);

    for (int i = 0; i < (int)rectangles.size(); i++)
    {
      delete rectangles[i];
    }

    if (layer->x < -frame_width - layer->separation_x)
    {
      layer->x += frame_width + layer->separation_x;
    }
  }

  // TODO redo paralax
  /*
    if(layer->depth_effect_x>0)
    {
        if(rosalila()->graphics->camera_x/layer->depth_effect_x>frame_width+layer->separation_x+layer->x)
        {
            layer->x+=frame_width+layer->separation_x;
        }
    }else if(layer->depth_effect_x<0)
    {
        if(rosalila()->graphics->camera_x*-layer->depth_effect_x > frame_width+layer->separation_x+layer->x)
        {
            layer->y+=frame_width+layer->separation_x;
        }
    }
    */
}

bool Stage::playerIsInPlatform()
{
  for (int i = 0; i < (int)back.size(); i++)
  {
    Layer *layer = back[i];
    if (layer->type=="platform" && layer->playerIsInBounds(player))
      return true;
  }
  return false;
}

bool Stage::playerIsInObstacle()
{
  for (int i = 0; i < (int)back.size(); i++)
  {
    Layer *layer = back[i];
    if (layer->playerIsInObstacle(player))
      return true;
  }
  return false;
}

void Stage::dibujarBack()
{
  if(this->player->is_bomb_active)
  {
    rosalila()->graphics->drawImage(this->player->bomb_images[this->player->current_bomb_image],
                                    0,
                                    0);
  }
  for (int i = 0; i < (int)back.size(); i++)
  {
    Layer *layer = back[i];
    if(this->player->is_bomb_active && layer->type == "bomb")
      drawLayer(layer);
    else if(!this->player->is_bomb_active && layer->type != "bomb")
      drawLayer(layer);
  }
  //    rosalila()->graphics->clearScreen(Color(0,0,0,0));
}

void Stage::dibujarFront()
{
  for (int i = 0; i < (int)front.size(); i++)
  {
    Layer *layer = front[i];
    drawLayer(layer);
  }
}

vector<Modifier *> loadLayerModifierXML(Node *modifier_node)
{
  vector<Modifier *> temp_modifiers;

  if (modifier_node->hasAttribute("velocity"))
  {
    std::string value = modifier_node->attributes["velocity"];
    temp_modifiers.push_back(new Modifier("velocity", value));
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
  if (modifier_node->hasAttribute("frame"))
  {
    std::string value = modifier_node->attributes["frame"];
    temp_modifiers.push_back(new Modifier("frame", value));
  }

  return temp_modifiers;
}

void Stage::loadFromXML(std::string name, bool is_mod)
{
  this->name = name;

  this->is_mod = is_mod;
  if (!is_mod)
  {
    path = std::string(assets_directory) + "stages/";
  }
  else
  {
    path = std::string(assets_directory) + "mods/stages/";
  }

  rosalila()->utility->writeLogLine("Loading stage from XML.");

  Node *root_node = rosalila()->parser->getNodes(path + name + "/stage.json");

  //Load settings
  music_path = path + name + "/music.ogg";

  Node *bullets_domain_node = root_node->getNodeByName("bullets_domain");
  this->bound_x1 = 0;
  this->bound_y1 = 0;
  this->bound_x2 = rosalila()->graphics->screen_width;
  this->bound_y2 = rosalila()->graphics->screen_height;

  if(bullets_domain_node)
  {
    if (bullets_domain_node->hasAttribute("x"))
      this->bound_x1 = atoi(bullets_domain_node->attributes["x"].c_str());

    if (bullets_domain_node->hasAttribute("y"))
      this->bound_y1 = atoi(bullets_domain_node->attributes["y"].c_str());

    if (bullets_domain_node->hasAttribute("width"))
      this->bound_x2 = this->bound_x1 + atoi(bullets_domain_node->attributes["width"].c_str());

    if (bullets_domain_node->hasAttribute("height"))
      this->bound_y2 = this->bound_y1 + atoi(bullets_domain_node->attributes["height"].c_str());
  }


  rosalila()->utility->writeLogLine("Loading stage's BackLayers.");

  vector<int> random_colors_r;
  vector<int> random_colors_g;
  vector<int> random_colors_b;

  int u = rosalila()->utility->getNonSeededRandomNumber() % 3 + 1;
  for (int i = 0; i < u; i++)
  {
    int random_number = rosalila()->utility->getNonSeededRandomNumber() % (*getColorPaletteR()).size();

    random_colors_r.push_back((*getColorPaletteR())[random_number]);
    random_colors_g.push_back((*getColorPaletteG())[random_number]);
    random_colors_b.push_back((*getColorPaletteB())[random_number]);
  }

  map<string, list<int>> randomized_appereance;
  map<string, int> max_layers;
  int current_layer = 0;

  vector<Node *> backlayer_nodes = root_node->getNodesByName("back_layers");

  for (int i = 0; i < (int)backlayer_nodes.size(); i++)
  {
    if (backlayer_nodes[i]->hasAttribute("randomize_appereance") && backlayer_nodes[i]->attributes["randomize_appereance"] == "yes")
    {
      string random_group = "";
      if (backlayer_nodes[i]->hasAttribute("random_group"))
      {
        random_group = backlayer_nodes[i]->attributes["random_group"];
      }
      randomized_appereance[random_group].push_back(current_layer);
    }
    current_layer++;
  }

  max_layers["a"] = 1 + rosalila()->utility->getNonSeededRandomNumber() % 2;
  max_layers["b"] = 1 + rosalila()->utility->getNonSeededRandomNumber() % 3;

  for (map<string, list<int>>::iterator randomized_appereance_iterator = randomized_appereance.begin();
       randomized_appereance_iterator != randomized_appereance.end();
       randomized_appereance_iterator++)
  {
    list<int> current_list = (*randomized_appereance_iterator).second;
    while ((int)current_list.size() > max_layers[(*randomized_appereance_iterator).first])
    {
      int random_to_remove = rosalila()->utility->getNonSeededRandomNumber() % current_list.size();
      list<int>::iterator remove_iterator = current_list.begin();
      for (int i = 0; i < random_to_remove; i++)
      {
        remove_iterator++;
      }
      current_list.erase(remove_iterator);
    }
    randomized_appereance[(*randomized_appereance_iterator).first] = current_list;
  }

  current_layer = 0;

  for (int i = 0; i < (int)backlayer_nodes.size(); i++)
  {
    bool included = false;

    if (!backlayer_nodes[i]->hasAttribute("randomize_appereance") || backlayer_nodes[i]->attributes["randomize_appereance"] != "yes")
    {
      included = true;
    }
    else
    {

      for (map<string, list<int>>::iterator randomized_appereance_iterator = randomized_appereance.begin();
           randomized_appereance_iterator != randomized_appereance.end();
           randomized_appereance_iterator++)
      {
        list<int> current_list = (*randomized_appereance_iterator).second;
        for (list<int>::iterator i = current_list.begin();
             i != current_list.end();
             i++)
        {
          if (current_layer == (*i))
          {
            included = true;
            break;
          }
        }
      }
    }
    current_layer++;
    if (!included)
    {
      continue;
    }

    std::string type = "";
    if (backlayer_nodes[i]->hasAttribute("type"))
      type = backlayer_nodes[i]->attributes["type"];

    int frame_duration = 5;
    if (backlayer_nodes[i]->hasAttribute("animation_velocity"))
      frame_duration = atoi(backlayer_nodes[i]->attributes["animation_velocity"].c_str());

    if (backlayer_nodes[i]->hasAttribute("randomize_frame_duration"))
      frame_duration += rosalila()->utility->getNonSeededRandomNumber() % atoi(backlayer_nodes[i]->attributes["randomize_frame_duration"].c_str());

    int depth_effect_x = 0;
    if (backlayer_nodes[i]->hasAttribute("depth_effect_x"))
      depth_effect_x = atoi(backlayer_nodes[i]->attributes["depth_effect_x"].c_str());

    if (backlayer_nodes[i]->hasAttribute("randomize_depth_effect_x"))
      depth_effect_x += rosalila()->utility->getNonSeededRandomNumber() % atoi(backlayer_nodes[i]->attributes["randomize_depth_effect_x"].c_str());

    int depth_effect_y = 0;
    if (backlayer_nodes[i]->hasAttribute("depth_effect_y"))
      depth_effect_y = atoi(backlayer_nodes[i]->attributes["depth_effect_y"].c_str());

    if (backlayer_nodes[i]->hasAttribute("randomize_depth_effect_y"))
      depth_effect_y += rosalila()->utility->getNonSeededRandomNumber() % atoi(backlayer_nodes[i]->attributes["randomize_depth_effect_y"].c_str());

    int x = 0;
    if (backlayer_nodes[i]->hasAttribute("x"))
      x = atoi(backlayer_nodes[i]->attributes["x"].c_str());

    if (backlayer_nodes[i]->hasAttribute("randomize_x"))
      x += rosalila()->utility->getNonSeededRandomNumber() % atoi(backlayer_nodes[i]->attributes["randomize_x"].c_str());

    int y = 0;
    if (backlayer_nodes[i]->hasAttribute("y"))
      y = atoi(backlayer_nodes[i]->attributes["y"].c_str());

    if (backlayer_nodes[i]->hasAttribute("randomize_y"))
      y += rosalila()->utility->getNonSeededRandomNumber() % atoi(backlayer_nodes[i]->attributes["randomize_y"].c_str());

    double velocity = 0;
    if (backlayer_nodes[i]->hasAttribute("velocity"))
      velocity = atof(backlayer_nodes[i]->attributes["velocity"].c_str());

    double angle = 0;
    if (backlayer_nodes[i]->hasAttribute("angle"))
      angle = atof(backlayer_nodes[i]->attributes["angle"].c_str());

    double angle_change = 0;
    if (backlayer_nodes[i]->hasAttribute("angle_change"))
      angle_change = atof(backlayer_nodes[i]->attributes["angle_change"].c_str());

    int separation_x = 0;
    if (backlayer_nodes[i]->hasAttribute("separation_x"))
      separation_x = atoi(backlayer_nodes[i]->attributes["separation_x"].c_str());

    bool blend_effect = false;
    if (backlayer_nodes[i]->hasAttribute("blend_effect"))
      blend_effect = backlayer_nodes[i]->attributes["blend_effect"] == "yes";

    if (backlayer_nodes[i]->hasAttribute("randomize_separation_x"))
      separation_x += rosalila()->utility->getNonSeededRandomNumber() % atoi(backlayer_nodes[i]->attributes["randomize_separation_x"].c_str());

    vector<Node *> frame_nodes = backlayer_nodes[i]->getNodesByName("sprites");

    vector<LayerFrame *> layer_frames;

    for (int j = 0; j < (int)frame_nodes.size(); j++)
    {
      layer_frames.push_back(getFrameFromNode(frame_nodes[j]));
    }

    Color color(255, 255, 255, 255);

    if (backlayer_nodes[i]->hasAttribute("randomize_color") && backlayer_nodes[i]->attributes["randomize_color"] == "yes")
    {
      int random_number_pos = rosalila()->utility->getNonSeededRandomNumber() % random_colors_r.size();
      color.red = random_colors_r[random_number_pos];
      color.green = random_colors_g[random_number_pos];
      color.blue = random_colors_b[random_number_pos];
    }

    if (backlayer_nodes[i]->hasAttribute("alpha"))
      color.alpha = atoi(backlayer_nodes[i]->attributes["alpha"].c_str());

    // Load modifiers
    std::map<int, vector<Modifier *>> *layer_modifiers = new std::map<int, vector<Modifier *>>();

    vector<Node *> modifier_nodes = backlayer_nodes[i]->getNodesByName("modifiers");
    for (int i = 0; i < (int)modifier_nodes.size(); i++)
    {
      int at = atoi(modifier_nodes[i]->attributes["at"].c_str());
      (*layer_modifiers)[at] = loadLayerModifierXML(modifier_nodes[i]);
    }

    // Load bounds
    bool is_bounds_active = false;
    int bounds_x = 0;
    int bounds_y = 0;
    int bounds_width = 0;
    int bounds_height = 0;

    Node* bounds_node = backlayer_nodes[i]->getNodeByName("bounds");
    if (bounds_node)
    {
      if (bounds_node->hasAttribute("is_active"))
        is_bounds_active = bounds_node->attributes["is_active"] == "yes";
      if (bounds_node->hasAttribute("x"))
        bounds_x = atoi(bounds_node->attributes["x"].c_str());
      if (bounds_node->hasAttribute("y"))
        bounds_y = atoi(bounds_node->attributes["y"].c_str());
      if (bounds_node->hasAttribute("width"))
        bounds_width = atoi(bounds_node->attributes["width"].c_str());
      if (bounds_node->hasAttribute("height"))
        bounds_height = atoi(bounds_node->attributes["height"].c_str());
    }

    // Load obstacle info
    int obstacle_hit_points = 5;
    Node* obstacle_node = backlayer_nodes[i]->getNodeByName("obstacle");
    if (obstacle_node)
    {
      if (obstacle_node->hasAttribute("obstacle_hit_points"))
        obstacle_hit_points = atoi(obstacle_node->attributes["obstacle_hit_points"].c_str());
    }

    back.push_back(new Layer(layer_frames,
                             layer_modifiers,
                             type,
                             frame_duration,
                             depth_effect_x, depth_effect_y,
                             x, y,
                             velocity,
                             angle,
                             angle_change,
                             separation_x,
                             is_bounds_active,
                             bounds_x,
                             bounds_y,
                             bounds_width,
                             bounds_height,
                             blend_effect,
                             obstacle_hit_points,
                             color));
  }

  rosalila()->utility->writeLogLine("Loading stage's FrontLayers.");

  vector<Node *> frontlayer_nodes = root_node->getNodesByName("front_layers");
  //Load front layer
  for (int i = 0; i < (int)frontlayer_nodes.size(); i++)
  {
    std::string type = "";
    if (backlayer_nodes[i]->hasAttribute("type"))
      type = backlayer_nodes[i]->attributes["type"];
    
    int frame_duration = 0;
    if (frontlayer_nodes[i]->hasAttribute("animation_velocity"))
      frame_duration = atoi(frontlayer_nodes[i]->attributes["animation_velocity"].c_str());

    int depth_effect_x = 0;
    if (frontlayer_nodes[i]->hasAttribute("depth_effect_x"))
      depth_effect_x = atoi(frontlayer_nodes[i]->attributes["depth_effect_x"].c_str());

    int depth_effect_y = 0;
    if (frontlayer_nodes[i]->hasAttribute("depth_effect_y"))
      depth_effect_y = atoi(frontlayer_nodes[i]->attributes["depth_effect_y"].c_str());

    int x = 0;
    if (frontlayer_nodes[i]->hasAttribute("x"))
      x = atoi(frontlayer_nodes[i]->attributes["x"].c_str());

    if (frontlayer_nodes[i]->hasAttribute("randomize_x"))
      x += rosalila()->utility->getNonSeededRandomNumber() % atoi(frontlayer_nodes[i]->attributes["randomize_x"].c_str());

    int y = 0;
    if (frontlayer_nodes[i]->hasAttribute("y"))
      y = atoi(frontlayer_nodes[i]->attributes["y"].c_str());

    if (frontlayer_nodes[i]->hasAttribute("randomize_y"))
      y += rosalila()->utility->getNonSeededRandomNumber() % atoi(frontlayer_nodes[i]->attributes["randomize_y"].c_str());

    double velocity = 0;
    if (frontlayer_nodes[i]->hasAttribute("velocity"))
      velocity = atoi(frontlayer_nodes[i]->attributes["velocity"].c_str());

    double angle = 0;
    if (frontlayer_nodes[i]->hasAttribute("angle"))
      angle = atoi(frontlayer_nodes[i]->attributes["angle"].c_str());

    double angle_change = 0;
    if (frontlayer_nodes[i]->hasAttribute("angle_change"))
      angle_change = atoi(frontlayer_nodes[i]->attributes["angle_change"].c_str());

    int separation_x = 0;
    if (frontlayer_nodes[i]->hasAttribute("separation_x"))
      separation_x = atoi(frontlayer_nodes[i]->attributes["separation_x"].c_str());

    bool blend_effect = false;
    if (frontlayer_nodes[i]->hasAttribute("blend_effect"))
      blend_effect = frontlayer_nodes[i]->attributes["blend_effect"] == "yes";

    std::vector<Image *> textures;
    std::vector<int> textures_size_x;
    std::vector<int> textures_size_y;

    vector<Node *> frame_nodes = frontlayer_nodes[i]->getNodesByName("sprites");

    vector<LayerFrame *> layer_frames;

    for (int j = 0; j < (int)frame_nodes.size(); j++)
    {
      layer_frames.push_back(getFrameFromNode(frame_nodes[j]));
    }

    Color color(255, 255, 255, 255);

    if (frontlayer_nodes[i]->hasAttribute("randomize_color") && frontlayer_nodes[i]->attributes["randomize_color"] == "yes")
    {
      int random_number_pos = rosalila()->utility->getNonSeededRandomNumber() % random_colors_r.size();
      color.red = random_colors_r[random_number_pos];
      color.green = random_colors_g[random_number_pos];
      color.blue = random_colors_b[random_number_pos];
    }

    if (frontlayer_nodes[i]->hasAttribute("alpha"))
      color.alpha = atoi(frontlayer_nodes[i]->attributes["alpha"].c_str());

    // Load modifiers
    std::map<int, vector<Modifier *>> *layer_modifiers = new std::map<int, vector<Modifier *>>();

    vector<Node *> modifier_nodes = frontlayer_nodes[i]->getNodesByName("modifiers");
    for (int i = 0; i < (int)modifier_nodes.size(); i++)
    {
      int at = atoi(modifier_nodes[i]->attributes["at"].c_str());
      (*layer_modifiers)[at] = loadLayerModifierXML(modifier_nodes[i]);
    }

    // Load bounds
    bool is_bounds_active = false;
    int bounds_x = 0;
    int bounds_y = 0;
    int bounds_width = 0;
    int bounds_height = 0;

    Node* bounds_node = frontlayer_nodes[i]->getNodeByName("bounds");
    if (bounds_node)
    {
      if (bounds_node->hasAttribute("is_active"))
        is_bounds_active = bounds_node->attributes["is_active"] == "yes";
      if (bounds_node->hasAttribute("x"))
        bounds_x = atoi(bounds_node->attributes["x"].c_str());
      if (bounds_node->hasAttribute("y"))
        bounds_y = atoi(bounds_node->attributes["y"].c_str());
      if (bounds_node->hasAttribute("width"))
        bounds_width = atoi(bounds_node->attributes["width"].c_str());
      if (bounds_node->hasAttribute("height"))
        bounds_height = atoi(bounds_node->attributes["height"].c_str());
    }

    // Load obstacle info
    int obstacle_hit_points = 5;
    Node* obstacle_node = frontlayer_nodes[i]->getNodeByName("obstacle");
    if (obstacle_node)
    {
      if (obstacle_node->hasAttribute("obstacle_hit_points"))
        obstacle_hit_points = atoi(obstacle_node->attributes["obstacle_hit_points"].c_str());
    }

    front.push_back(new Layer(layer_frames,
                              layer_modifiers,
                              type,
                              frame_duration,
                              depth_effect_x, depth_effect_y,
                              x, y,
                              velocity,
                              angle,
                              angle_change,
                              separation_x,
                              is_bounds_active,
                              bounds_x,
                              bounds_y,
                              bounds_width,
                              bounds_height,
                              blend_effect,
                              obstacle_hit_points,
                              color));
  }

  delete root_node;
  rosalila()->utility->writeLogLine("Stage loaded succesfully from Json.");
}

LayerFrame *Stage::getFrameFromNode(Node *frame_node)
{
  Image *image_temp = NULL;
  string type;
  int width = 0;
  int height = 0;

  if (frame_node->hasAttribute("type"))
    type = frame_node->attributes["type"];

  if (frame_node->hasAttribute("path"))
  {
    string image_path = path + name + "/images/" + frame_node->attributes["path"];
    image_temp = rosalila()->graphics->getImage(image_path);

    width = image_temp->getWidth();
    height = image_temp->getHeight();
  }

  if (frame_node->hasAttribute("width"))
    width = atoi(frame_node->attributes["width"].c_str());
  if (frame_node->hasAttribute("height"))
    height = atoi(frame_node->attributes["height"].c_str());

  return new LayerFrame(image_temp, type, width, height);
}

void Stage::logic()
{
  if (getIterateSlowdownFlag())
    iterator++;

  for (int i = 0; i < (int)back.size(); i++)
  {
    Layer *layer = back[i];
    layer->logic(player);
  }

  for (int i = 0; i < (int)front.size(); i++)
  {
    Layer *layer = front[i];
    layer->logic(player);
  }
}

void Stage::playMusic()
{
  rosalila()->sound->playMusic(this->music_path, -1);
}
