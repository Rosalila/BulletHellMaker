Rosalila Shooter Engine
=======================

[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/Rosalila/STG/blob/master/LICENSE)

1. **Fast and modern development**: Create boss fights without coding.
2. **No string attached**: Free as in freedom, available even for commercial projects.
3. **Easy from start to deploy**: Easy to get started and easy to publish your game on stores such as Steam and Epic.

| Feature | Supported |
|----------|------------ |
| Playable characters | ✔ |
| Bossfights | ✔ |
| Customize attack patterns, animations, sfx,  etc... via JSON | ✔ |
| Windows / Linux / MacOS | ✔ |
| PC, XInput, PS4 and Steam input supported | ✔ |
| Steamworks achievements, stats and leaderboard | ✔ |
| Store, replay and share your best runs | ✔ |
| Libre, MIT licensed | ✔ |

![](img/game.png?raw=true)

## Usage

### Characters

Playable and enemy characters are defined with the same attributes on a `main.json` file. Notice the `Shield`, `Charge` and `Parry` attributes are optional.

#### main.json

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| velocity           | `integer`                             | ✔ | Movement velocity |
| animation_velocity | `integer`                             | ✔ | Animation speed |
| hp                 | `integer`                             | ✔ | Health, hit points |
| initial_position   | [initial_position](#initial_position) | ✔ | Character position at the beginning |
| sounds             | [sounds](#sounds)                     | ✔ | Sound effects |
| life_bar           | [life_bar](#life_bar)                 | ✔ | Life bar settings |
| sprites[]          | [sprites](#sprites)                   | ✔ | Sprite list |
| hitboxes[]         | [hitboxes](#hitboxes[])                 | ✔ | Hitboxes |

#### initial_position

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x      | `integer` | ✔ | Initial position on the x axis |
| y      | `integer` | ✔ | Initial position on the y axis |

#### sounds

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| hit | `file path` | ✔ | `.ogg` sfx to be played when the character is hit |

#### life_bar

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x      | `integer` | ✔ | Life bar position on the x axis |
| y      | `integer` | ✔ | Life bar position on the y axis |
| width  | `integer` | ✔ | Width of the life bar |
| height | `integer` | ✔ | Height of the life bar |
| color  | [color](#color) | ✔ | Color of the life bar |

#### color

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| red   | `integer` | ✔ | Amount of red color from 0 to 255 |
| green | `integer` | ✔ | Amount of green color from 0 to 255 |
| blue  | `integer` | ✔ | Amount of blue color from 0 to 255 |

#### sprites

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| state  | `string` | ✔ | Name of the state |
| path[] | `string` | ✔ | `.png` path array |

#### hitboxes[]

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x      | `integer` | ✔ | Hitbox position on the x axis |
| y      | `integer` | ✔ | Hitbox position on the y axis |
| width  | `integer` | ✔ | Hitbox width |
| height | `integer` | ✔ | Hitbox height |
| angle  | `integer` | ✔ | Hitbox rotation |

![](img/player.gif?raw=true)

example:

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
  "sprites": [
    {
      "state": "start",
      "sprite": [
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
      "sprite": [
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
  ]
}
```

### Bullets

Also define a collection of bullets with their sprites, sounds, etc...

bullets.xml:

```Json
{
"bullet":
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
    "images": [
      {
        "path": "bullet.png"
      }
    ],
    "on_hit_images": [
      {
        "path": "on_hit.png"
      }
    ],
    "hitbox": [
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

Attach bullet to patterns to create attacks. Name the character attacks as `Primary` and `Secondary` so they get automatically attached to the `a` and `b` buttons defined on the `config.json`. You can name enemy attack patterns as you want and then invoke them when you define the enemy behavior, see [Enemy behavior](#Enemy-behavior).

![](img/enemy.gif?raw=true)

patterns.json:

```Json
{
"type":
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

Change enemy attributes or attacks in a given time or when health goes below certain amount to create some sort of timeline defined behavior.

modifier.json:

```Json
{
"modifier":
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

Add annimated layers on the front and on the background for pure cosmetic purposes.

![](img/background.gif?raw=true)

main.xml:

```Json
{
  "bounds": {
    "x1": "0",
    "y1": "0",
    "x2": "1920",
    "y2": "1080"
  },
  "back_layer": [
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
        └───Your Playable Character/
            │   bullets.json
            │   main.json
            │   patterns.json
    └───stages/
        └───Your Stage
            │   config.json
            │
            └───Enemy
                │   bullets.json
                │   main.json
                │   modifiers.json
                │   patterns.json
```

## Getting started

Download the [boilerplate example](https://github.com/Rosalila/STGExample) game and run the binary corresponding to your system.

## Build

```bash
# Dependencies
sudo apt-get install g++ cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev libglew-dev

# Compile
mkdir build
cd build
cmake ..
make
```
