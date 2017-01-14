#include "Layer.h"

Layer::Layer(vector<LayerFrame*> layer_frames,int frame_duration,int depth_effect_x,int depth_effect_y,int alignment_x,int alignment_y,int separation_x,
        Color color)
{
    this->layer_frames=layer_frames;
    this->frame_duration=frame_duration;
    this->depth_effect_x=depth_effect_x;
    this->depth_effect_y=depth_effect_y;
    this->current_frame=0;
    this->time_elapsed=0;
    this->alignment_x=alignment_x;
    this->alignment_y=alignment_y;
    this->separation_x=separation_x;
    this->color = color;
}

Layer::~Layer()
{
    rosalila()->utility->writeLogLine("Deleting layer.");
    for(int i=0;i<(int)layer_frames.size();i++)
    {
        delete layer_frames[i];
    }
}
