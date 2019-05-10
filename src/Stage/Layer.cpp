#include "Stage/Layer.h"

Layer::Layer(vector<LayerFrame *> layer_frames, int frame_duration, int depth_effect_x, int depth_effect_y, double x, double y, double velocity_x, double velocity_y, int separation_x, bool blend_effect,
             Color color)
{
  this->layer_frames = layer_frames;
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
  this->blend_effect = blend_effect;
  this->color = color;
}

void Layer::logic()
{
  this->x += velocity_x;
  this->y += velocity_y;
}

Layer::~Layer()
{
  rosalila()->utility->writeLogLine("Deleting layer.");
  for (int i = 0; i < (int)layer_frames.size(); i++)
  {
    delete layer_frames[i];
  }
}
