Rosalila Shooter Engine
=======================

[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/Rosalila/STG/blob/master/LICENSE)

1. **Fast and modern development**: Create playable characters, enemies and stages no sweat.
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

```Json
{
  "LifeBar": {
    "color_r": "65",
    "color_g": "255",
    "color_b": "65",
    "rect_offset_x": "60",
    "rect_offset_y": "10",
    "rect_width": "1280",
    "rect_height": "8"
  },
  "Attributes": {
    "velocity": "6",
    "animation_velocity": "15",
    "hp": "4",
    "initial_x": "100",
    "initial_y": "500"
  },
  "Shield": {
    "sprite": "shield.png",
    "max_shield": "500",
    "shield_fade": "40",
    "proration": "40"
  },
  "Charge": {
    "sprite": "charge.png",
    "max_charge": "300",
    "charge_velocity": "1",
    "x": "-40",
    "y": "0"
  },
  "Parry": {
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
  },
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
  "Sounds": {
    "hit": "sounds/hit.ogg"
  },
  "Sprites": [
    {
      "state": "start",
      "Sprite": [
        {
          "path": "states/3_balloons/01.png"
        },
        {
          "path": "states/3_balloons/02.png"
        },
        {
          "path": "states/3_balloons/03.png"
        },
        {
          "path": "states/3_balloons/04.png"
        },
        {
          "path": "states/3_balloons/05.png"
        }
      ]
    },
    {
      "state": "2 balloons",
      "conditions" :
      {
        "hp": "3"
      },
      "Sprite": [
        {
          "path": "states/2_balloons/01.png"
        },
        {
          "path": "states/2_balloons/02.png"
        },
        {
          "path": "states/2_balloons/03.png"
        },
        {
          "path": "states/2_balloons/04.png"
        },
        {
          "path": "states/2_balloons/05.png"
        }
      ]
    },
    {
      "state": "1 balloon",
      "conditions" :
      {
        "hp": "2"
      },
      "Sprite": [
        {
          "path": "states/1_balloon/01.png"
        },
        {
          "path": "states/1_balloon/02.png"
        },
        {
          "path": "states/1_balloon/03.png"
        },
        {
          "path": "states/1_balloon/04.png"
        },
        {
          "path": "states/1_balloon/05.png"
        }
      ]
    },
    {
      "state": "destroyed",
      "Sprite": [
        {
          "path": "move/01.png"
        },
        {
          "path": "move/02.png"
        },
        {
          "path": "move/03.png"
        },
        {
          "path": "move/04.png"
        },
        {
          "path": "move/05.png"
        },
        {
          "path": "move/06.png"
        },
        {
          "path": "move/07.png"
        },
        {
          "path": "move/08.png"
        }
      ]
    }
  ],
  "FlatShadow": {
    "image_path": "shadow.png",
    "CaseRight": {
      "Point": [
        {
          "x": "4",
          "y": "0"
        },
        {
          "x": "-3",
          "y": "25"
        }
      ],
      "Inbetween": {
        "Point": {
          "x": "0",
          "y": "-50"
        }
      }
    },
    "CaseLeft": [
      {
        "x": "4",
        "y": "0"
      },
      {
        "x": "-3",
        "y": "-25"
      }
    ],
    "CaseTop": [
      {
        "x": "4",
        "y": "0"
      },
      {
        "x": "-3",
        "y": "25"
      }
    ]
  }
}
```

### Define bullets

```Json
{
"Bullet":
[
  {
    "name": "bullet a",
    "damage": "7",
    "sound_channel": "-1",
    "sound": "sounds/shoot.ogg",
    "width": "20",
    "height": "6",
    "Color": {
      "red": "255",
      "green": "255",
      "blue": "255",
      "alpha": "255"
    },
    "Sprite": [
      {
        "path": "bullets/a/01.png"
      },
      {
        "path": "bullets/a/02.png"
      },
      {
        "path": "bullets/a/03.png"
      },
      {
        "path": "bullets/a/04.png"
      }
    ],
    "Hitbox": {
      "x": "-10",
      "y": "-3",
      "width": "20",
      "height": "6",
      "angle": "0"
    },
    "OnHit": [
      {
        "path": "kajira/01.png"
      },
      {
        "path": "kajira/02.png"
      },
      {
        "path": "kajira/03.png"
      },
      {
        "path": "kajira/04.png"
      },
      {
        "path": "kajira/05.png"
      },
      {
        "path": "kajira/06.png"
      },
      {
        "path": "kajira/07.png"
      },
      {
        "path": "kajira/08.png"
      }
    ]
  },
  {
    "name": "bullet b",
    "damage": "1",
    "sound_channel": "-1",
    "sound": "sounds/large.ogg",
    "width": "10",
    "height": "100",
    "Color": {
      "red": "255",
      "green": "255",
      "blue": "255",
      "alpha": "255"
    },
    "Sprite": {
      "path": "bullets/b/01.png"
    },
    "Hitbox": {
      "x": "-5",
      "y": "-50",
      "width": "10",
      "height": "100",
      "angle": "0"
    },
    "OnHit": [
      {
        "path": "kajira/01.png"
      },
      {
        "path": "kajira/02.png"
      },
      {
        "path": "kajira/03.png"
      },
      {
        "path": "kajira/04.png"
      },
      {
        "path": "kajira/05.png"
      },
      {
        "path": "kajira/06.png"
      },
      {
        "path": "kajira/07.png"
      },
      {
        "path": "kajira/08.png"
      }
    ]
  }
]
}
```

### Define patterns

```Json
{
"Type":
[
  {
    "name": "1",
    "Pattern": [
      {
        "bullet": "bullet a",
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
    "name": "2",
    "Pattern": {
      "bullet": "bullet b",
      "animation_velocity": "5",
      "velocity": "50",
      "acceleration": "0",
      "a_frequency": "0",
      "cooldown": "5",
      "offset_x": "45"
    }
  },
  {
    "name": "bomb",
    "Pattern": {
      "bullet": "bullet b",
      "undestructable": "yes",
      "collides_bullets": "yes",
      "collides_opponent": "no",
      "animation_velocity": "2",
      "angle": "0",
      "velocity": "-5",
      "max_velocity": "25",
      "acceleration": "0",
      "a_frequency": "1",
      "cooldown": "10",
      "offset_x": "-35",
      "Modifier": {
        "at": "1",
        "bullet": "bullet b",
        "acceleration": "1",
        "a_frequency": "5"
      }
    }
  }
]
}
```

### Define enemy timeline

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
    "pattern_type": "A"
  },
  {
    "life_at": "4000",
    "pattern_type": "B"
  },
  {
    "life_at": "3000",
    "pattern_type": "C"
  },
  {
    "life_at": "2000",
    "pattern_type": "D"
  }
]
}
```

### Define stages

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
      "randomize_color": "no",
      "velocity_x": "0",
      "x": "0",
      "y": "0",
      "separation_x": "1280",
      "frame": {
        "type": "image",
        "image_path": "layers/l0.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.03",
      "x": "300",
      "y": "400",
      "separation_x": "1280",
      "frame": {
        "type": "image",
        "image_path": "layers/l1.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.07",
      "x": "1320",
      "y": "120",
      "separation_x": "1280",
      "frame": {
        "type": "image",
        "image_path": "layers/l2.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.2",
      "x": "100",
      "y": "210",
      "separation_x": "1280",
      "frame": {
        "type": "image",
        "image_path": "layers/l3.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.2",
      "x": "1400",
      "y": "700",
      "separation_x": "1280",
      "frame": {
        "type": "image",
        "image_path": "layers/l4.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-1.2",
      "x": "0",
      "y": "90",
      "separation_x": "2000",
      "blend_effect": "yes",
      "frame": {
        "type": "image",
        "image_path": "layers/a1.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-1.2",
      "x": "15",
      "y": "125",
      "separation_x": "2000",
      "blend_effect": "yes",
      "frame": {
        "type": "image",
        "image_path": "layers/a2.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-1.2",
      "x": "10",
      "y": "100",
      "separation_x": "2000",
      "blend_effect": "yes",
      "frame": {
        "type": "image",
        "image_path": "layers/a3.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.5",
      "x": "1700",
      "y": "750",
      "separation_x": "2000",
      "blend_effect": "yes",
      "frame": {
        "type": "image",
        "image_path": "layers/b1.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.5",
      "x": "1715",
      "y": "760",
      "separation_x": "2000",
      "blend_effect": "yes",
      "frame": {
        "type": "image",
        "image_path": "layers/b2.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.5",
      "x": "1690",
      "y": "735",
      "separation_x": "2000",
      "blend_effect": "yes",
      "frame": {
        "type": "image",
        "image_path": "layers/b3.png"
      }
    },
    {
      "randomize_color": "no",
      "velocity_x": "-0.5",
      "x": "1695",
      "y": "145",
      "separation_x": "2000",
      "blend_effect": "yes",
      "frame": {
        "type": "image",
        "image_path": "layers/x2.png"
      }
    }
  ]
}
```

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