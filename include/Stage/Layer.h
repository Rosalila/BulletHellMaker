#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include "Rosalila/Rosalila.h"
#include "Stage/LayerFrame.h"
#include "Character/Modifier.h"
using namespace std;

class Player;

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

  int frame;

  // Bounds
  bool is_bounds_active;
  int bounds_x;
  int bounds_y;
  int bounds_width;
  int bounds_height;

  map<int, vector<Modifier *>> *modifiers;

  Layer(vector<LayerFrame *> layer_frames,
        std::map<int, vector<Modifier *>> *modifiers,
        int frame_duration,
        int depth_effect_x, int depth_effect_y,
        double x, double y,
        double velocity_x, double velocity_y,
        int separation_x,
        bool is_bounds_active,
        int bounds_x,
        int bounds_y,
        int bounds_width,
        int bounds_height,
        bool blend_effect,
        Color color);
  
  bool playerIsInBounds(Player* player);
  void modifiersControl();
  void logic(Player* player);

  ~Layer();
};

#endif
