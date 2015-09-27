Rosalila STG Engine
===================
rosalilastudio.com
github.com/rosalila/stg


Build instructions using Code::Blocks:

1. Install the dependencies
---------------------------
* apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev freeglut3-dev
* yum install SDL-devel SDL_mixer-devel SDL_image-devel SDL_ttf-devel freeglut-devel 

2. Link the libraries
---------------------
In Code::Blocks: "Linker settings" -> "Other linker options"
Add the following:
*   -lGL
*   -lglut
*   -lGLU
*   -lSDL
*   -lSDL_image
*   -lSDL_ttf
*   -lSDL_mixer

4. Clone the Rosalila Engine
----------------------------
*   Create a folder named Rosalila/
*   Clone the Rosalila Engine (https://github.com/Rosalila/engine) code inside.
 
5. Tell the compiler to look for the Rosalila Engine
----------------------------------------------------
In Code::Blocks: "Search directories" -> "Compiler"
Add the following:
*   Rosalila

6. Add a game assets
--------------------
Download a complete game assets and put it in your root directory.
You should add the following
*   chars/
*   stages/
*   menu/
*   misc/
*   config.xml

7. Celebrate
------------
Well done.

Support
--------------------
This software is licensed under the GPL License.

Please file issues [click here] at Github. 

Please visit us at [Rosalila Studio].

[Rosalila Studio]:http://www.rosalilastudio.com/
[click here]:https://github.com/Rosalila/STG/issues

Contributing
--------------------

Create your feature branch (git checkout -b my-new-feature)

Commit your changes (git commit -am 'Add some feature')

Push to the branch (git push origin my-new-feature)

Create a new Pull Request
