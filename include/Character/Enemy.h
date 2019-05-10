#ifndef ENEMY_H
#define ENEMY_H

#include <map>
#include <list>
#include <vector>

#include "Rosalila/Rosalila.h"
#include "Character/Bullet.h"
#include "Character/Pattern.h"
#include "Character/Character.h"
#include "Character/Player.h"

class Enemy : public Character
{
public:
  Player *player;
  map<int, vector<Modifier *>> modifiers;
  map<int, vector<Modifier *>> life_at_modifiers;
  double angle;
  double angle_change;
  bool is_mod;
  int last_frame_hp;

  Enemy(string name, Player *player, int sound_channel_base, bool is_mod);
  ~Enemy();
  void loadModifiersFromXML();
  void logic(int stage_velocity, string stage_name);
  void modifiersControl();
  virtual void addActivePattern(Pattern *pattern);
  void bottomRender();
  void topRender();
};

#endif
