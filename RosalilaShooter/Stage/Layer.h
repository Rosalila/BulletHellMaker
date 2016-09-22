#ifndef LAYER_H
#define LAYER_H

#include<vector>
#include "../../Rosalila/Rosalila.h"
#include "LayerFrame.h"
using namespace std;

class Layer
{
public:
    vector<LayerFrame*> layer_frames;

    Color color;

    int frame_duration;
    int depth_effect_x;
    int depth_effect_y;
    int alignment_x;
    int alignment_y;
    int separation_x;

    int current_frame;
    int time_elapsed;

    Layer(vector<LayerFrame*> layer_frames,
          int frame_duration,
          int depth_effect_x,int depth_effect_y,
          int alignment_x,int alignment_y,int separation_x,
          Color color);

    ~Layer();
};

#endif
