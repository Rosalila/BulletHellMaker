#include "Layer.h"

Layer::Layer(vector<Image*> textures,vector <int> textures_size_x,vector <int> textures_size_y,int frame_duration,int depth_effect_x,int depth_effect_y,int alignment_x,int alignment_y,int separation_x,
        int random_color_r,int random_color_g,int random_color_b)
{
    this->textures=textures;
    this->textures_size_x=textures_size_x;
    this->textures_size_y=textures_size_y;
    this->frame_duration=frame_duration;
    this->depth_effect_x=depth_effect_x;
    this->depth_effect_y=depth_effect_y;
    this->current_frame=0;
    this->time_elapsed=0;
    this->alignment_x=alignment_x;
    this->alignment_y=alignment_y;
    this->separation_x=separation_x;
    this->random_color_r=random_color_r;
    this->random_color_g=random_color_g;
    this->random_color_b=random_color_b;
}

Layer::~Layer()
{
    Rosalila()->Utility->writeLogLine("Deleting layer.");
    for(;!textures.empty();)
    {
        Image*image=textures.back();
        textures.pop_back();
        delete image;
    }
}
