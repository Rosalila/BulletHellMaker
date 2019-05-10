#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include "Rosalila/Rosalila.h"
#include "Stage/LayerFrame.h"
using namespace std;

class Layer
{
public:
  vector<LayerFrame *> layer_frames;

  Color color;

  int frame_duration;
  int depth_effect_x;
  int depth_effect_y;
  double x;
  double y;
  double velocity_x;
  double velocity_y;
  int separation_x;

  bool blend_effect;

  int current_frame;
  int time_elapsed;

  Layer(vector<LayerFrame *> layer_frames,
        int frame_duration,
        int depth_effect_x, int depth_effect_y,
        double x, double y,
        double velocity_x, double velocity_y,
        int separation_x,
        bool blend_effect,
        Color color);

  void logic();

  ~Layer();
};

#endif
