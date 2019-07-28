Rosalila Shooter Engine
=======================

[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/Rosalila/STG/blob/master/LICENSE)

1. **Fast and modern development**: Create boss fights without coding.
2. **No string attached**: Free as in freedom, available even for commercial projects.
3. **Easy from start to deploy**: Easy to get started and easy to publish your game on stores such as Steam and Epic.

| Feature | Supported |
|----------|------------ |
| Windows / Linux / MacOS | ✔ |
| PC, XInput, PS4 and Steam input supported | ✔ |
| Steamworks achievements, stats and leaderboard | ✔ |
| Store, replay and share your best runs | ✔ |
| Libre, MIT licensed | ✔ |

## Usage

### Define characters

Playable and enemy characters are defined with the same attributes on a `main.json` file. Notice the `Shield`, `Charge` and `Parry` attributes are optional.

main.json:

```Json
{
  "velocity": "6",
  "animation_velocity": "15",
  "hp": "4",
  "InitialPosition": {
    "x": "100",
    "y": "500"
  },
  "Sounds": {
    "hit": "sounds/hit.ogg"
  },
  "LifeBar": {
    "x": "0",
    "y": "0",
    "width": "1920",
    "height": "8",
    "Color":
    {
      "red": "65",
      "green": "255",
      "blue": "65"
    }
  },
  "Sprites": [
    {
      "state": "start",
      "Sprite": [
        {
          "path": "ship01.png"
        },
        {
          "path": "ship02.png"
        }
      ]
    },
    {
      "state": "destroyed",
      "Sprite": [
        {
          "path": "ship01.png"
        }
      ]
    }
  ],
  "Hitboxes": [
    {
      "x": "-10",
      "y": "-5",
      "width": "1",
      "height": "10",
      "angle": "0"
    },
    {
      "x": "-10",
      "y": "-5",
      "width": "1",
      "height": "10",
      "angle": "60"
    },
    {
      "x": "-1",
      "y": "-0",
      "width": "1",
      "height": "10",
      "angle": "-60"
    }
  ],
  "Shield OPTIONAL": {
    "sprite": "shield.png",
    "max_shield": "500",
    "shield_fade": "40",
    "proration": "40"
  },
  "Charge OPTIONAL": {
    "sprite": "charge.png",
    "max_charge": "300",
    "charge_velocity": "1",
    "x": "-40",
    "y": "0"
  },
  "Parry OPTIONAL": {
    "duration": "60",
    "sound": "sounds/parry.ogg",
    "Parrying": {
      "sprite": "parrying.png",
      "x": "-40",
      "y": "-35"
    },
    "Parryed": {
      "sprite": "parryed.png",
      "x": "-40",
      "y": "-35"
    },
    "Hitboxes": [
      {
        "x": "-21",
        "y": "-25",
        "width": "5",
        "height": "50",
        "angle": "0"
      },
      {
        "x": "-21",
        "y": "-19",
        "width": "5",
        "height": "46",
        "angle": "60"
      },
      {
        "x": "19",
        "y": "-3",
        "width": "5",
        "height": "46",
        "angle": "-60"
      }
    ]
  }
}
```

### Define bullets

Also define a collection of bullets with their sprites, sounds, etc...

bullets.xml:

```Json
{
"Bullet":
[
  {
    "name": "My Bullet",
    "damage": "7",
    "width": "20",
    "height": "6",
    "Sound OPTIONAL": {
      "channel": "-1",
      "path": "sounds/shoot.ogg"
    },
    "Images": [
      {
        "path": "bullet.png"
      }
    ],
    "OnHitImages": [
      {
        "path": "on_hit.png"
      }
    ],
    "Hitbox": [
      {
        "x": "-10",
        "y": "-3",
        "width": "20",
        "height": "6",
        "angle": "0"
      }
    ]
  }
]
}
```

### Define attack patterns

Attach bullet to patterns to create attacks. Name the character attacks as `Primary` and `Secondary` so they get automatically attached to the `a` and `b` buttons defined on the `config.json`. You can name enemy attack patterns as you want and then invoke them when you define the enemy behavior, see [Enemy behavior](#Enemy-behavior). 

patterns.json:

```Json
{
"Type":
[
  {
    "name": "primary",
    "Pattern": [
      {
        "bullet": "My Bullet",
        "animation_velocity": "5",
        "angle": "0",
        "random_angle": "0",
        "velocity": "20",
        "max_velocity": "25",
        "acceleration": "0",
        "a_frequency": "0",
        "cooldown": "15",
        "offset_x": "60",
        "offset_y": "0"
      }
    ]
  },
  {
    "name": "secondary",
    "Pattern": [
      {
        "bullet": "My Bullet",
        "animation_velocity": "5",
        "angle": "45",
        "random_angle": "0",
        "velocity": "20",
        "max_velocity": "25",
        "acceleration": "0",
        "a_frequency": "0",
        "cooldown": "15",
        "offset_x": "60",
        "offset_y": "0"
      }
    ]
  }
]
}
```

### Enemy behavior

Change enemy attributes or attacks in a given time or when health goes below certain amount to create some sort of timeline defined behavior.

modifier.json:

```Json
{
"Modifier":
[
  {
    "at": "0",
    "velocity": "0",
    "angle": "180"
  },
  {
    "at": "125",
    "velocity": "0",
    "angle_change": "0"
  },
  {
    "at": "200",
    "pattern_type": "Pattern A"
  },
  {
    "life_at": "4000",
    "pattern_type": "Pattern B"
  },
  {
    "life_at": "3000",
    "pattern_type": "Pattern C"
  },
  {
    "life_at": "2000",
    "pattern_type": "Pattern D"
  }
]
}
```

### Define stages

Add annimated layers on the front and on the background for pure cosmetic purposes.

main.xml:

```Json
{
  "Bounds": {
    "x1": "0",
    "y1": "0",
    "x2": "1920",
    "y2": "1080"
  },
  "Misc": {
    "velocity": "0"
  },
  "BackLayer": [
    {
      "velocity_x": "0",
      "x": "0",
      "y": "0",
      "frame": {
        "type": "image",
        "image_path": "background.png"
      }
    },
    {
      "velocity_x": "-0.5",
      "x": "300",
      "y": "500",
      "separation_x": "1500",
      "frame_duration": "5",
      "frame": [
        {
          "type": "image",
          "image_path": "planet.png"
        }
      ]
    }
  ]
}
```

## General configuration

Organize the stages order and other stuff here.

config.json:

```json
{
  "Chars": {
    "char": {
      "name": "player"
    }
  },
  "Stages": {
    "stage": [
      {
        "name": "FirstStage"
      },
      {
        "name": "SecondStage"
      }
    ]
  },
  "Resolution": {
    "x": "1920",
    "y": "1080"
  },
  "ScreenSize": {
    "x": "1920",
    "y": "1080"
  },
  "Fullscreen": {
    "enabled": "no"
  },
  "Font": {
    "path": "assets/font.ttf",
    "red": "255",
    "green": "255",
    "blue": "255",
    "size": "30"
  },
  "Notifications": {
    "background_path": "assets/notification_background.png"
  },
  "Inputs": {
    "Player": {
      "number": "0",
      "Type": [
        {
          "name": "keyboard",
          "button": [
            {
              "name": "8",
              "key": "up"
            },
            {
              "name": "2",
              "key": "down"
            },
            {
              "name": "4",
              "key": "left"
            },
            {
              "name": "6",
              "key": "right"
            },
            {
              "name": "a",
              "key": "z"
            },
            {
              "name": "b",
              "key": "x"
            },
            {
              "name": "c",
              "key": "c"
            },
            {
              "name": "back",
              "key": "q"
            }
          ]
        },
        {
          "name": "gamepad",
          "button": [
            {
              "name": "8",
              "key": "up"
            },
            {
              "name": "2",
              "key": "down"
            },
            {
              "name": "4",
              "key": "left"
            },
            {
              "name": "6",
              "key": "right"
            },
            {
              "name": "a",
              "key": "1"
            },
            {
              "name": "b",
              "key": "2"
            },
            {
              "name": "c",
              "key": "3"
            },
            {
              "name": "back",
              "key": "4"
            }
          ]
        }
      ]
    }
  }
}
```

## Directory structure

TODO

## Example game

TODO

## Build & Run

```bash
# Dependencies
sudo apt-get install g++ cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev libglew-dev

# Compile
mkdir build
cd build
cmake ..
make

# Run
cd ..
cp build/shmup .
./shmup
```