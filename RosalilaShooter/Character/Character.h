#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <list>
#include <vector>

#include "../../Rosalila/Rosalila.h"
#include "Bullet.h"
#include "Pattern.h"
#include "Modifier.h"

class Character
{
public:
    std::string name;
    std::string directory;
    int hp;
    int max_hp;
    int iteration;
    bool visible;

    int velocity;
    bool shooting;
    std::string orientation;
    std::string current_type;

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
    std::map< std::string,std::vector<Image*> >sprites;

    //Patterns and bullets
    std::map<std::string,Bullet*> bullets;
    std::map<std::string, std::vector<Pattern*> > type;
    std::list<Pattern*>* active_patterns;

    //Color effect
    int current_color_effect_r;
    int current_color_effect_g;
    int current_color_effect_b;
    int current_color_effect_a;

    //Shake
    int current_screen_shake_x;
    int current_screen_shake_y;
    int shake_time;
    int shake_magnitude;

    int sound_channel_base;
    Image*life_bar;

    vector<Hitbox*> hitboxes;

    Image* flat_shadow_texture;
    vector<Point*>shadow_align_points_left;
    vector<Point*>shadow_align_points_right;
    vector<Point*>shadow_align_points_top;

    vector<Point*>inbetween_shadow_align_points_left;
    vector<Point*>inbetween_shadow_align_points_right;
    vector<Point*>inbetween_shadow_align_points_top;

    double x,y;

    Character(){}
    Character(std::string name,int sound_channel_base);
    virtual ~Character();
    void loadFromXML();
    void loadMainXML();
    void loadBulletsXML();
    void loadPatternsXML();
    vector<Modifier*> loadModifierXML(Node* modifier_node);
    Pattern* loadPatternXML(Node* pattern_node);

    void logic(int stage_velocity);
    void animationControl();
    void spellControl(int stage_velocity);
    virtual void addActivePattern(Pattern* pattern);

    void bottomRender();
    void topRender();
    void setOrientation(string orientation);
    bool collides(Hitbox hitbox,int hitbox_x,int hitbox_y,float hitbox_angle);
    void hit(int damage);
    void shakeScreen(int shake_magnitude, int shake_time);
    void deleteActivePatterns();
};

#endif
