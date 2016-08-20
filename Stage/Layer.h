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

    int random_color_r;
    int random_color_g;
    int random_color_b;

    int frame_duration;
    int depth_effect_x;
    int depth_effect_y;
    int alignment_x;
    int alignment_y;
    int separation_x;

    int current_frame;
    int time_elapsed;

    Layer(vector<Image*> textures,vector <int> textures_size_x,
          vector <int> textures_size_y,int frame_duration,
          int depth_effect_x,int depth_effect_y,
          int alignment_x,int alignment_y,int separation_x,
          int random_color_r,int random_color_g,int random_color_b);

    ~Layer();
};

#endif
