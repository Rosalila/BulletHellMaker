#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <list>
#include <vector>

#include "Rosalila/Rosalila.h"
#include "Character/Bullet.h"
#include "Character/Pattern.h"
#include "Character/Modifier.h"

class AnimationControl
{
public:
  std::string name;
  int current_frame;
  int animation_velocity;
  int x;
  int y;
  bool delete_flag;
  AnimationControl(std::string name, int animation_velocity, int x, int y)
  {
    this->name = name;
    this->animation_velocity = animation_velocity;
    this->x = x;
    this->y = y;
    this->current_frame = 0;
    this->delete_flag = false;
  }
};

class Character
{
public:
  std::string name;
  std::string directory;
  int hp;
  int frame;
  int max_hp;
  int iteration;
  bool visible;

  int velocity;
  bool shooting;
  std::string current_state;
  std::string current_type;
  std::map < /*current state*/ std::string, /*trigger validation*/ std::map< std::string, std::string > >state_triggers;
  std::map< std::string, vector<Image*> > animation_images;
  std::list<AnimationControl*> animation_controls;

  //Life bar variables
  int current_slow;
  int max_slow;
  int life_bar_x;
  int life_bar_y;
  int life_bar_rect_offset_x;
  int life_bar_rect_offset_y;
  int life_bar_rect_height;
  int life_bar_rect_width;
  Color color;

  //Sprites animation
  int animation_velocity;
  int animation_iteration;
  int current_sprite;
  std::map<std::string, std::vector<Image *> > sprites;

  //Patterns and bullets
  std::map<std::string, Bullet *> bullets;
  std::map<std::string, std::vector<Pattern *> > type;
  std::list<Pattern *> *active_patterns;

  //Color effect
  int color_filter_red;
  int color_filter_green;
  int color_filter_blue;
  int color_filter_alpha;

  //Shake
  int current_screen_shake_x;
  int current_screen_shake_y;
  int shake_time;
  int shake_magnitude;

  int sound_channel_base;
  Image *life_bar;

  vector<Hitbox *> hitboxes;

  double x, y;

  Character() {}
  Character(std::string name, int sound_channel_base);
  virtual ~Character();
  void loadFromXML();
  void loadMainXML();
  void loadBulletsXML();
  void loadPatternsXML();
  vector<Modifier *> loadModifierXML(Node *modifier_node);
  Pattern *loadPatternXML(Node *pattern_node);

  void animationControl();
  virtual void addActivePattern(Pattern *pattern);

  void bottomRender();
  void topRender();
  void setState(string state);
  bool hasState(string state);
  bool collides(Hitbox hitbox, int hitbox_x, int hitbox_y, float hitbox_angle);
  void hit(int damage);
  void shakeScreen(int shake_magnitude, int shake_time);
  void deleteActivePatterns();
};

#endif
