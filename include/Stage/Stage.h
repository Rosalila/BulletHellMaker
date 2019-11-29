#ifndef STAGE_H
#define STAGE_H

#include "Stage/Layer.h"
#include "Utility/Utility.h"
#include <map>

class Player;

class Stage
{
public:
  Player* player;
  
  std::vector<Layer*> back,front;
  std::string music_path;
  int bound_x1,bound_y1,bound_x2,bound_y2;
  int iterator;
  string name;
  bool iterate_slowdown_flag;
  int current_slowdown_iteration;

  int layer_transparency;

  bool is_mod;
  string path;

  Stage(Player* player);
  ~Stage();
  bool playerIsInBounds();
  void dibujarBack();
  void dibujarFront();
  void drawLayer(Layer*layer);
  void loadFromXML(std::string name, bool is_mod);
  LayerFrame* getFrameFromNode(Node* frame_node);
  void logic();
  void playMusic();
};

#endif
