#ifndef LAYER_H
#define LAYER_H

#include "../Rosalila/TinyXml/tinyxml.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
#include "../Rosalila/RosalilaInputs/RosalilaInputs.h"

class Layer
{
public:
    std::vector <Image*> textures;
    std::vector <int> textures_size_x;
    std::vector <int> textures_size_y;

    //External logic
    int frame_duration,depth_effect_x,depth_effect_y,alignment_x,alignment_y;
    int separation_x;
    //Internal logic
    int current_frame,time_elapsed;
    Layer(vector<Image*> textures,vector <int> textures_size_x,vector <int> textures_size_y,int frame_duration,int depth_effect_x,int depth_effect_y,int alignment_x,int alignment_y,int separation_x)
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
    }

    ~Layer()
    {
        writeLogLine("Deleting layer.");
        for(;!textures.empty();)
        {
            Image*image=textures.back();
            textures.pop_back();
            delete image;
        }
    }
};

#endif
