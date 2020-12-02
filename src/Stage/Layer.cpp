#include "Stage/Layer.h"
#include "Character/Player.h"

Layer::Layer(vector<LayerFrame *> layer_frames,
  std::map<int, vector<Modifier *>> *modifiers,
  std::string type,
  int frame_duration,
  int depth_effect_x,
  int depth_effect_y,
  double x,
  double y,
  double velocity,
  double angle,
  double angle_change,
  int separation_x,
  bool is_bounds_active,
  int bounds_x,
  int bounds_y,
  int bounds_width,
  int bounds_height,
  bool blend_effect,
  int obstacle_hit_points,
  Color color)
{
  this->layer_frames = layer_frames;
  this->modifiers = modifiers;
  this->type = type;
  this->frame_duration = frame_duration;
  this->depth_effect_x = depth_effect_x;
  this->depth_effect_y = depth_effect_y;
  this->current_frame = 0;
  this->time_elapsed = 0;
  this->x = x;
  this->y = y;
  this->velocity = velocity;
  this->angle = angle;
  this->angle_change = angle_change;
  this->separation_x = separation_x;
  this->is_bounds_active = is_bounds_active;
  this->bounds_x = bounds_x;
  this->bounds_y = bounds_y;
  this->bounds_width = bounds_width;
  this->bounds_height = bounds_height;
  this->blend_effect = blend_effect;
  this->obstacle_hit_points = obstacle_hit_points;
  this->color = color;

  this->frame = 0;
}

void Layer::modifiersControl()
{
  bool flag_iterator_change = false;

  vector<Modifier *> current_modifiers = (*this->modifiers)[frame];

  for (int i = 0; i < (int)current_modifiers.size(); i++)
  {
    Modifier *modifier = current_modifiers[i];
    if (modifier->variable == "frame")
    {
      this->frame = atoi(modifier->value.c_str());
      flag_iterator_change = true;
    }
    if (modifier->variable == "velocity")
    {
      this->velocity = atof(modifier->value.c_str());
    }
    if (modifier->variable == "angle")
    {
      this->angle = atof(modifier->value.c_str());
    }
    if (modifier->variable == "angle_change")
    {
      this->angle_change = atof(modifier->value.c_str());
    }
  }

  if (!flag_iterator_change)
    this->frame += 1/getSlowdown();
}

bool Layer::playerIsInBounds(Player* player)
{
  return pointIsInBounds(player->x, player->y);
}

bool Layer::pointIsInBounds(int x, int y)
{
  int image_height = 0;
  if(layer_frames.size() > 0)
    image_height = layer_frames[0]->height;
  
  bool is_in_bounds = false;
  for(int i = 0;
      separation_x!=0 && i + this->x + this->bounds_x < rosalila()->graphics->screen_width;
      i+=separation_x + this->layer_frames[this->current_frame]->width)
  {
    int current_bounds_x = this->x + this->bounds_x + i;
    int current_bounds_y = rosalila()->graphics->screen_height - image_height - this->y - this->bounds_y;
    if((x < current_bounds_x
      || y < current_bounds_y
      || x > current_bounds_x + this->bounds_width
      || y > current_bounds_y + this->bounds_height) == false)
      is_in_bounds = true;
  }
  return is_in_bounds;
}

bool Layer::playerIsInObstacle(Player* player)
{
  if(this->type != "obstacle" || this->obstacle_hit_points<=0)
    return false;
  return playerIsInBounds(player);
}

void Layer::logic(Player* player)
{
  modifiersControl();

  bool player_was_in_bounds = playerIsInBounds(player);

  double delta_x = cos(this->angle * PI / 180.0) * this->velocity / getSlowdown();
  double delta_y = sin(this->angle * PI / 180.0) * this->velocity / getSlowdown();

  this->x += delta_x;
  this->y -= delta_y;

  if(this->type=="obstacle")
  {
    for (std::list<Pattern *>::iterator iterator = player->active_patterns->begin();
      iterator != player->active_patterns->end();
      iterator++)
    {
      Pattern *pattern = (Pattern *)*iterator;
      if (!pattern->is_hit && this->obstacle_hit_points>0 && pointIsInBounds(pattern->x, pattern->y))
      {
        this->obstacle_hit_points -= pattern->bullet->damage;
        if(this->obstacle_hit_points <= 0)
        {
          pattern->hit(player->sound_channel_base + 1, false);        
        }
      }
    }
  }

  if(is_bounds_active
    && (this->type=="bomb" || this->type=="platform")
    && player_was_in_bounds)
  {
    player->x += delta_x;
    player->y += delta_y;
  }

  if(this->type=="obstacle"
    && this->obstacle_hit_points
    && playerIsInBounds(player))
  {
    if(delta_x>0)
      player->x = this->x+bounds_x+bounds_width;
    if(delta_x<0)
      player->x = this->x+bounds_x;
  }

  this->angle += this->angle_change;
}

Layer::~Layer()
{
  rosalila()->utility->writeLogLine("Deleting layer.");
  for (int i = 0; i < (int)layer_frames.size(); i++)
  {
    delete layer_frames[i];
  }
}
