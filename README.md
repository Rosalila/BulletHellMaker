Rosalila Shoot 'Em Up Maker
===========================

[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/Rosalila/STG/blob/master/LICENSE)
[![Discord](https://img.shields.io/discord/650862206925275156.svg?label=Discord&logo=discord&color=7289DA&labelColor=2C2F33)](https://discord.gg/fGBbQZ8)
[![Travis](https://img.shields.io/travis/Rosalila/ShootEmUpMaker.svg?logo=travis)](https://travis-ci.org/Rosalila/ShootEmUpMaker)
[![Appveyor](https://img.shields.io/travis/Rosalila/ShootEmUpMaker.svg?logo=appveyor)](https://ci.appveyor.com/project/Turupawn/ShootEmUpMaker)

1. **Fast and modern development**: Create boss fights without coding.
2. **No string attached**: Free as in freedom, available even for commercial projects.
3. **Easy from start to deploy**: Easy to get started and easy to publish your game on stores such as Steam and Epic.

| Feature | Supported |
|----------|------------ |
| Playable characters                                          | ✔ |
| Boss fights                                                  | ✔ |
| Customize attack patterns, animations, sfx,  etc... via JSON | ✔ |
| Windows / Linux / MacOS                                      | ✔ |
| PC, XInput, PS4 and Steam input supported                    | ✔ |
| Steamworks achievements, stats and leaderboard               | ✔ |
| Store, replay and share your best runs                       | ✔ |
| Libre, MIT licensed                                          | ✔ |

![](img/game.png?raw=true)

## Getting started

Download the [boilerplate example](https://github.com/Rosalila/ShootEmUpExample/archive/master.zip) game and run the binary corresponding to your system (`./shmup` on Linux and `shmup.exe` on Windows).

## Usage

### Characters

Playable and enemy characters are defined with the same attributes on a `character.json` file.

![](img/player.gif?raw=true)

#### character.json

See [full character.json attributes](#characterjson-attributes).

*Example:*

```Json
{
  "velocity": "6",
  "animation_velocity": "15",
  "hp": "4",
  "initial_position": {
    "x": "100",
    "y": "500"
  },
  "sounds": {
    "hit": "sounds/hit.ogg"
  },
  "life_bar": {
    "x": "0",
    "y": "0",
    "width": "1920",
    "height": "8",
    "color":
    {
      "red": "65",
      "green": "255",
      "blue": "65"
    }
  },
  "states": [
    {
      "name": "start",
      "sprites": [
        {
          "path": "ship01.png"
        },
        {
          "path": "ship02.png"
        }
      ]
    },
    {
      "name": "destroyed",
      "sprites": [
        {
          "path": "ship01.png"
        }
      ]
    }
  ],
  "hitboxes": [
    {
      "x": "-10",
      "y": "-5",
      "width": "1",
      "height": "10",
      "angle": "0"
    }
  ]
}
```

### Bullets

Also define a collection of bullets with their sprites, sounds, etc...

#### bullets.json

See [full bullets.json attributes](#bulletsjson-attributes).

*Example:*

```Json
{
"bullets":
[
  {
    "name": "My Bullet",
    "damage": "7",
    "sprites": [
      {
        "path": "bullet.png"
      }
    ],
    "on_hit_sprites": [
      {
        "path": "on_hit.png"
      }
    ],
    "hitboxes": [
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

### Attack patterns

Attach bullets to patterns to create attacks. Name the character attacks as `Primary` and `Secondary` so they get automatically attached to the `a` and `b` buttons defined on the `config.json`. You can name enemy attack patterns as you want and then invoke them when you define the enemy behavior, see [Enemy behavior](#Enemy-behavior).

![](img/enemy.gif?raw=true)

#### attacks.json

See [full attacks.json attributes](#attacksjson-attributes).

*Example:*

```Json
{
"attacks":
[
  {
    "name": "primary",
    "pattern": [
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
    "pattern": [
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

Change enemy attributes or attacks in a given time or when health goes below certain amount.

#### modifiers.json

See [full modifiers.json attributes](#modifiersjson-attributes).

*Example:*

```Json
{
"modifiers":
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

### Stages

Add animated layers on the front and on the background for pure cosmetic purposes.

![](img/background.gif?raw=true)

#### stage.json

See [full stage.json attributes](#stagejson-attributes).

*Example:*

```Json
{
  "bullets_domain": {
    "x": "0",
    "y": "0",
    "width": "1920",
    "height": "1080"
  },
  "back_layers": [
    {
      "velocity_x": "0",
      "x": "0",
      "y": "0",
      "sprites": [
        {
          "type": "image",
          "path": "background.png"
        }
      ]
    },
    {
      "velocity_x": "-0.5",
      "x": "300",
      "y": "500",
      "separation_x": "1500",
      "animation_velocity": "5",
      "sprites": [
        {
          "type": "image",
          "path": "planet.png"
        }
      ]
    }
  ]
}
```

## General configuration

Organize the stages order and other stuff here.

#### config.json

See [full config.json attributes](#configjson-attributes).

*Example:*

```json
{
  "chars": {
    "char": {
      "name": "player"
    }
  },
  "stages": {
    "stage": [
      {
        "name": "FirstStage"
      },
      {
        "name": "SecondStage"
      }
    ]
  },
  "resolution": {
    "x": "1920",
    "y": "1080"
  },
  "screen_size": {
    "x": "1920",
    "y": "1080"
  },
  "fullscreen": {
    "enabled": "no"
  },
  "font": {
    "path": "assets/font.ttf",
    "red": "255",
    "green": "255",
    "blue": "255",
    "size": "30"
  },
  "notifications": {
    "background_path": "assets/notification_background.png"
  },
  "inputs": {
    "player": {
      "number": "0",
      "type": [
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

Follow the directory structure so the Shmup Maker detects your characters and stages.

```
Your game/
│   shmup
│
└───assets/
    │   config.json
    │   font.ttf
    │
    └───chars/
    │   └───Your Playable Character/
    │       │   bullets.json
    │       │   character.json
    │       │   attacks.json
    └───stages/
        └───Your Stage
            │   stage.json
            │
            └───Enemy
                │   bullets.json
                │   character.json
                │   modifiers.json
                │   attacks.json
```

## Build

Notice this guide is for compiling the Shoot 'Em Up Maker from source, if you wan't to create a game please refer to the [Getting started](#getting-started).
 docs
```bash
# Dependencies
sudo apt-get install g++ cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev libglew-dev

# Compile
mkdir build
cd build
cmake ..
make
```
