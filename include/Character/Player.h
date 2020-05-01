#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <list>
#include <vector>

#include "Character/Bullet.h"
#include "Character/Pattern.h"
#include "Character/Character.h"
#include "Stage/Stage.h"

class ShadowControl
{
public:
  std::string state;
  int image_number;
  int alpha;
  int x;
  int y;
  ShadowControl(std::string state, int image_number, int alpha, int x, int y)
  {
    this->state = state;
    this->image_number = image_number;
    this->alpha = alpha;
    this->x = x;
    this->y = y;
  }
};

class Player : public Character
{
public:
  Character* enemy;
  Stage* stage;
  //Slow bar variables
  string game_mode;
  Image *slow_bar;
  int slow_decrement;
  int slow_increment;
  int slow_cooldown_increment;
  bool slow_in_cooldown;
  int slow_bar_x;
  int slow_bar_y;
  int slow_bar_rect_offset_x;
  int slow_bar_rect_offset_y;
  int slow_bar_rect_height;
  int slow_bar_rect_width;
  Color slow_bar_color;
  Color slow_bar_cooldown_color;

  //Input control
  bool pressed_1_last_frame, pressed_2_last_frame, pressed_3_last_frame, pressed_4_last_frame,
    pressed_6_last_frame, pressed_7_last_frame, pressed_8_last_frame, pressed_9_last_frame;
  int pressed_1_last_frameX, pressed_2_last_frameX, pressed_3_last_frameX, pressed_4_last_frameX,
    pressed_6_last_frameX, pressed_7_last_frameX, pressed_8_last_frameX, pressed_9_last_frameX;
  
  //Velocity
  double original_velocity;
  double primary_weapon_velocity;
  double secondary_weapon_velocity;

  //Dash
  int dash_last_tap_frame;
  int dash_velocity;
  int dash_extra_velocity_x, dash_extra_velocity_y;

  //Shield
  int current_shield;
  int max_shield;
  int shield_fade;
  double proration;
  Image *shield_image;

  //Charge
  int max_charge;
  int charge_velocity;
  int charge_sprite_x;
  int charge_sprite_y;
  Image *charge_image;
  bool charge_ready;
  int charging_sound_channel;

  //Parry
  int current_parry_frame;
  int parry_duration;
  Image *parrying_image;
  int parrying_x, parrying_y;
  Image *parryed_image;
  int parryed_x, parryed_y;
  vector<Hitbox *> parry_hitboxes;

  vector<Image *> parry_sprites;

  int frame;

  int parries_left;
  int invulnerable_frames_left;
  int current_charge;

  vector<string> intro_input;
  vector<string> replay_input;
  vector<string> replay_storage;

  // buffs & debuffs
  double additional_velocity_x;
  double additional_velocity_y;
  double additional_hp_change;
  double velocity_override;

  // Bomb animation
  std::vector<Image *> bomb_images;
  int current_bomb_frame;
  int current_bomb_image;
  int bomb_image_duration;
  bool is_bomb_animation_active;
  bool is_bomb_active;

  // Shadow
  std::list<ShadowControl*> shadows;

  Player(std::string name, int sound_channel_base, vector<string> intro_input, vector<string> replay_input, string game_mode);
  ~Player();
  void loadFromXML();
  void loadPlayerFromXML();
  void inputControl();
  void logic(int stage_velocity);
  double getShieldPercentage();
  void hit(int damage);
  bool isParrying();
  bool isInvulnerable();
  void parry(bool infinite_parries);
  bool collidesParry(Hitbox hitbox, int hitbox_x, int hitbox_y, float hitbox_angle);
  void bottomRender();
  void topRender();
  bool isDownWrapper(string button_map);
  void exit();
  bool isOnIntro();
  void activateBomb();
  void disableBomb();
  void bombLogic();
};

#endif
