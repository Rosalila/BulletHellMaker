#ifndef MYEVENTRECEIVER_H
#define MYEVENTRECEIVER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stdarg.h>
using namespace std;

#include "SDL2/SDL.h"
#include "../RosalilaUtility/RosalilaUtility.h"

const int max_joystick_inputs = 50;

class ROSALILA_DLL RosalilaReceiver
{
  public:
    bool button_up_flag;
    map<int /*players*/,
        map<std::string /*types*/,
            map<std::string, std::string> /*buttons*/
        >
    > inputs;

    int joystick;

    bool is_key_pressed[322];
    bool is_joy1_pressed[max_joystick_inputs];
    bool is_joy2_pressed[max_joystick_inputs];
    bool hat_up_pressed_player_1;
    bool hat_down_pressed_player_1;
    bool hat_left_pressed_player_1;
    bool hat_right_pressed_player_1;
    bool stick_up_pressed_player_1;
    bool stick_down_pressed_player_1;
    bool stick_left_pressed_player_1;
    bool stick_right_pressed_player_1;
    bool joy_up_pressed_player_2;
    bool joy_down_pressed_player_2;
    bool joy_left_pressed_player_2;
    bool joy_right_pressed_player_2;

    void init();
    bool isKeyDown(int keycode);
    bool isKeyPressed(int keycode);
    bool isJoyDown(int gamepad_number, int joyCode);
    bool isJoyPressed(int gamepad_number, int joyCode);
    void updateInputs();
    void unpressAllInputs();

    bool isPressed(int player, std::string name);
    bool isDown(int player, std::string name);
};

#endif // MYEVENTRECEIVER_H>
