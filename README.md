Rosalila STG Engine
===================
rosalilastudio.com
github.com/rosalila/stg


Build instructions using Code::Blocks:

1. Install the dependencies
---------------------------
* apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev codeblocks g++
* yum install SDL-devel SDL_mixer-devel SDL_image-devel SDL_ttf-devel codeblocks gcc-c++

2. Link the libraries
---------------------
In Code::Blocks: "Linker settings" -> "Other linker options"
Add the following:
*   -lSDL2
*   -lSDL2_image
*   -lSDL2_ttf
*   -lSDL2_mixer
*   -lSDL2_net
*   -lGL

4. Clone the Rosalila Engine
----------------------------
*   Create a folder named Rosalila/
*   Clone the Rosalila Engine (https://github.com/Rosalila/engine) code inside.

5. Add a game assets
--------------------
*   Create a folder named assets/
*   Clone the Rosalila Engine (https://github.com/Rosalila/STGAssets) code inside.

6. Celebrate
------------
Compile the project and celebrate.

Support
-------
This software is licensed under the LGPL License.

Please file issues [click here] at Github. 

Please visit us at [Rosalila Studio].

[Rosalila Studio]:http://www.rosalilastudio.com/
[click here]:https://github.com/Rosalila/STG/issues

Contributing
------------

Create your feature branch (git checkout -b my-new-feature)

Commit your changes (git commit -am 'Add some feature')

Push to the branch (git push origin my-new-feature)

Create a new Pull Request
