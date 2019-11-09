#include "Stage/Layer.h"

Layer::Layer(vector<LayerFrame *> layer_frames,
  std::map<int, vector<Modifier *>> *modifiers,
  int frame_duration,
  int depth_effect_x,
  int depth_effect_y,
  double x,
  double y,
  double velocity_x,
  double velocity_y,
  int separation_x,
  bool is_bounds_active,
  int bounds_x,
  int bounds_y,
  int bounds_width,
  int bounds_height,
  bool blend_effect,
  Color color)
{
  this->layer_frames = layer_frames;
  this->modifiers = modifiers;
  this->frame_duration = frame_duration;
  this->depth_effect_x = depth_effect_x;
  this->depth_effect_y = depth_effect_y;
  this->current_frame = 0;
  this->time_elapsed = 0;
  this->x = x;
  this->y = y;
  this->velocity_x = velocity_x;
  this->velocity_y = velocity_y;
  this->separation_x = separation_x;
  this->is_bounds_active = is_bounds_active;
  this->bounds_x = bounds_x;
  this->bounds_y = bounds_y;
  this->bounds_width = bounds_width;
  this->bounds_height = bounds_height;
  this->blend_effect = blend_effect;
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
    if (modifier->variable == "velocity_x")
    {
      this->velocity_x = atof(modifier->value.c_str());
    }
    if (modifier->variable == "velocity_y")
    {
      this->velocity_y = atof(modifier->value.c_str());
    }
  }

  if (!flag_iterator_change)
    this->frame++;
}

bool playerIsInBounds(Player* player, int x, int y, int width, int height)
{
  if(player->x < x || player->y < y || player->x > x + width || player->y > y + height)
    return false;
  return true;
}

void Layer::logic(Player* player)
{
  modifiersControl();

  this->x += velocity_x;
  this->y += velocity_y;


  if(is_bounds_active)
  {
    int image_height = 0;
    if(layer_frames.size() > 0)
      image_height = layer_frames[0]->height;
    if(playerIsInBounds(player,
        x + bounds_x,
        rosalila()->graphics->screen_height - image_height - y - bounds_y,
        bounds_width,
        bounds_height)
      )
    {
      player->x += velocity_x;
      player->y -= velocity_y;
    }
  }
}

Layer::~Layer()
{
  rosalila()->utility->writeLogLine("Deleting layer.");
  for (int i = 0; i < (int)layer_frames.size(); i++)
  {
    delete layer_frames[i];
  }
}
