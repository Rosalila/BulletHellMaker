Rosalila Shoot 'Em Up Maker
===========================

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

![](img/player.gif?raw=true)

#### main.json

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

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| velocity           | `integer`                             | ✔ | Movement velocity |
| animation_velocity | `integer`                             | ✔ | Animation speed |
| hp                 | `integer`                             | ✔ | Health, hit points |
| initial_position   | [initial_position](#initial_position) | ✔ | Character position at the beginning |
| sounds             | [sounds](#sounds)                     | ✔ | Sound effects |
| life_bar           | [life_bar](#life_bar)                 | ✔ | Life bar settings |
| states             | [states[]](#state)                    | ✔ | Sprite list |
| hitboxes           | [hitbox[]](#hitbox)                   | ✔ | Hitboxes |

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

#### state

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| name    | `string`            | ✔ | Name of the state |
| sprites | [sprite[]](#sprite) | ✔ | `.png` path array |

#### sprite

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| path | `path` | ✔ | `.png` file path |

#### hitbox

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x      | `integer` | ✔ | Hitbox position on the x axis |
| y      | `integer` | ✔ | Hitbox position on the y axis |
| width  | `integer` | ✔ | Hitbox width |
| height | `integer` | ✔ | Hitbox height |
| angle  | `integer` | ✔ | Hitbox rotation |

### Bullets

Also define a collection of bullets with their sprites, sounds, etc...

#### bullets.xml

```Json
{
"bullet":
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

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| name           | `string`          | ✔ | Bullet unique name identifier |
| damage         | `integer`         | ✔ | Bullet attack damage to be inflicted on impact |
| sound          | [sound](#sound)   |   | Sound to be played when emited |
| images         | `string[]`        | ✔ | Sprite list |
| on_hit_sprites | `string[]`        | ✔ | Sprite list to be displayed on bullet collision |
| hitboxes       | [hitbox[]](#hitbox) | ✔ | Hitbox list |

#### sound

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| channel | `integer`   | ✔ | Sound channel, -1 means auto assign |
| path    | `file path` | ✔ | `.ogg` sound effect file |

#### hitbox

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x      | `integer` | ✔ | Hitbox position on the x axis |
| y      | `integer` | ✔ | Hitbox position on the y axis |
| width  | `integer` | ✔ | Hitbox width |
| height | `integer` | ✔ | Hitbox height |
| angle  | `integer` | ✔ | Hitbox rotation |

### Attack patterns

Attach bullet to patterns to create attacks. Name the character attacks as `Primary` and `Secondary` so they get automatically attached to the `a` and `b` buttons defined on the `config.json`. You can name enemy attack patterns as you want and then invoke them when you define the enemy behavior, see [Enemy behavior](#Enemy-behavior).

![](img/enemy.gif?raw=true)

#### patterns.json

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

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| type | [type[]](#type)| ✔ | A collection of attack patterns form an attack type |

#### type

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| name     | `string`             | ✔ | Pattern collection unique name |
| patterns | [pattern[]](#pattern) | ✔ | Single bullet pattern |

#### pattern

| Attribute | Type | Required | Default | Description |
|-----------|------|----------|---------|-------------|
| bullet                       | `string`     | ✔ |           | Bullet identifier |
| acceleration                 | `integer`    |   | `0`       | Change of velocity added every frame |
| a_frequency                  | `integer`    |   | `0`       | Frequency that the acceleration is applied |
| additional_player_hp_change  | `double`     |   | `0`       | Player's hit points through time, use this to apply poison or heal buffs |
| additional_player_velocity_x | `integer`    |   | `0`       | Player's velocity change, use this to pull or push him or to in conjuction with to `additional_player_velocity_y` to reduce the general velocity |
| additional_player_velocity_y | `integer`    |   | `0`       | Player's velocity change, use this to push him up or down or to in conjuction with to `additional_player_velocity_x` to reduce the general velocity |
| aim_player                   | `yes/no`     |   | `no`      | Overrides the bullet angle to now point to the player |
| angle                        | `integer`    |   | `0`       | Rotation |
| angle_change                 | `integer`    |   | `0`       | Rotation change through time |
| ac_frequency                 | `integer`    |   | `0`       | Angle change frequency |
| animation_velocity           | `integer`    |   | `0`       | Sprite animation speed |
| auto_scale                   | `double`     |   | `0`       | Bullet scale change through time |
| bullet_rotation              | `integer`    |   | `0`       | Rotates the bullet sprites withouth affecting other attributes such as the angle or hitboxes |
| br_change                    | `integer`    |   | `0`       | Bullet rotation change through time |
| collides_bullets             | `yes/no`     |   | `no`      | If the bullet collides with an opponent's one, both would be destroyed |
| collides_opponent            | `yes/no`     |   | `yes`     | The bullet is able to hit the opponent |
| cooldown                     | `integer`    |   | `0`       | Amount of frames between each bullet emision |
| duration                     | `integer`    |   | `-1`      | Amount of frames before the bullet gets destroyed, -1 is infinite |
| freeze                       | `yes/no`     |   | `no`      | All bullet changes, such as `velocity`, `acceleration` or `angle_chage` are stopped |
| homming                      | `yes/no`     |   | `no`      | Bullet will track the player |
| independent_br               | `yes/no`     |   | `no`      | The `bullet_rotation` is independent from the `angle` |
| max_velocity                 | `yes/no`     |   | `9999999` | Velocity cap |
| offset_x                     | `integer`    |   | `0`       | Pixels offset relative to the character on the x axis on emision |
| offset_y                     | `integer`    |   | `0`       | Pixels offset relative to the character on the y axis on emision |
| random_angle                 | `integer`    |   | `0`       | Adds a random angle from 0 to the provided number |
| startup                      | `integer`    |   | `0`       | Amount of frames before emiting the bullet for the first time |
| stop_ac_at                   | `integer`    |   | `-1`      | Amount of frames before stopping the `angle_change`, -1 means never |
| undestructable               | `yes/no`     |   | `no`      | The bullet can be destroyed by other bullets |
| velocity                     | `integer`    |   | `0`       | Speed |
| modifier                     | `modifier[]` |   |           | Modifies a pattern in a specific frame |

#### modifier

| Attribute | Type | Required | Default | Description |
|-----------|------|----------|---------|-------------|
| at          | `integer`          | ✔ | Modifier's frame of execution |
| `attribute` | `attribute's type` | ✔ | Attribute to be changed, e.g. `{at: "10", velocity: "3", homming: "true"}` |

### Enemy behavior

Change enemy attributes or attacks in a given time or when health goes below certain amount to create some sort of timeline defined behavior.

#### modifier.json

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

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| modifiers | [modifier[]](#modifier) | ✔ | Bullet identifier |

#### modifier

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| at           | `integer`  | ✔ | Modifier's frame of execution |
| life_at      | `integer`  | ✔ | Defines the amount of hit points needed to be reached to execute the modifier |
| velocity     | `integer`  | ✔ | New enemy velocity to be set |
| angle_change | `integer`  | ✔ | New enemy angle change to be set |
| pattern_type | `integer`  | ✔ | New enemy pattern to be set |

### Stages

Add animated layers on the front and on the background for pure cosmetic purposes.

![](img/background.gif?raw=true)

#### main.json

```Json
{
  "bullets_domain": {
    "x": "0",
    "y": "0",
    "width": "1920",
    "height": "1080"
  },
  "back_layer": [
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

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| bullets_domain | [bullets_domain](#bullets_domain) | ✔ | Bullet bounds, bullets outside this range will be automatically distroyed |
| back_layer     | [layer[]](#layer)                 | ✔ | Animated layers to be displayed behind of characters and bullets |
| front_layer    | [layer[]](#layer)                 | ✔ | Animated layers to be displayed on front of characters and bullets |

#### bullets_domain

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x      | `integer` | ✔ | Position of the bullets domain on the x axis |
| y      | `integer` | ✔ | Position of the bullets domain on the y axis |
| width  | `integer` | ✔ | Bullets domain width |
| height | `integer` | ✔ | Bullets domain height |

#### layer

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x                  | `integer`         | ✔ | Initial position on the x axis |
| y                  | `integer`         | ✔ | Initial position on the y axis |
| separation_x       | `integer`         | ✔ | Amount of pixels between every animation repetition |
| velocity_x         | `double`          | ✔ | Layer velocity on the x axis, use this to make parallax effect |
| animation_velocity | `integer`         | ✔ | Animation speed |
| sprites            | [sprite[]](#sprite) | ✔ | Animation sprites |

#### sprite

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| type | `image` | ✔ | Type of frame |
| path | `path`  | ✔ | Path to the `.png` image |

## General configuration

Organize the stages order and other stuff here.

#### config.json

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

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| `chars`         | [char[]](#char)                 | ✔ | Playable character |
| `stages`        | [stage[]](#stage)               | ✔ | Playable character |
| `resolution`    | [resolution](#resolution)       | ✔ | Screen resolution, in pixels |
| `screen_size`   | [screen_size](#screen_size)     | ✔ | Screen size, in pixels |
| `fullscreen`    | [fullscreen](#fullscreen)       | ✔ | Fullscreen settings |
| `inputs`        | [inputs](#inputs)               | ✔ | Input settings |
| `font`          | [font](#font)                   |   | Fullscreen settings |
| `notifications` | [notifications](#notifications) |   | Notification settings |

#### char

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| name | `string` | ✔ | Character name, must match the directory under `assets/chars/` |

#### stage

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| name | `string` | ✔ | Stage name, must match the directory under `assets/stages/` |

#### resolution

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x | `integer` | ✔ | Resolution width |
| y | `integer` | ✔ | Resolution height |

#### screen_size

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| x | `integer` | ✔ | Screen width |
| y | `integer` | ✔ | Screen height |

#### fullscreen

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| enabled | `yes/no` | ✔ | Defines whether the game will go fullscreen or not |

#### font

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| path  | `file path` | ✔ | Path to the `.ttf` file |
| red   | `integer`   | ✔ | Amount of red color from 0 to 255 |
| green | `integer`   | ✔ | Amount of green color from 0 to 255 |
| blue  | `integer`   | ✔ | Amount of blue color from 0 to 255 |
| size  | `integer`   | ✔ | Font size |

#### notifications

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| background_path  | `file path` | ✔ | Path to the notification box `.png` background |

#### inputs

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| player[]  | [player](#player) | ✔ | Array of player's button configuration |


#### player

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| number | `integer`     | ✔ | Player number from 0 onwards... |
| type[] | [type](#type) | ✔ | Array of types of inputs (gamepad or keyboard) |

#### type

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| name     | `keyboard/gamepad` | ✔ | Type of input configuration |
| button[] | [button](#button)  | ✔ | Button key-name mapping |

#### button

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| name | `string` | ✔ | Name of the button to be used on your game |
| key  | `string` | ✔ | Key associated to the name |

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
    │       │   main.json
    │       │   patterns.json
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
