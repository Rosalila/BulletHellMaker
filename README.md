Rosalila Studio's STG engine
============================
rosalilastudio.com
github.com/rosalila/stg

SDL dependencies
----------------
apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev
or
yum install SDL-devel SDL_mixer-devel SDL_image-devel SDL_ttf-devel
 
OpenGL dependencies
-------------------
yum install freeglut-devel
 
In "Linker settings" -> "Other linker options"
----------------------------------------------
*   lGL
*   lSDL
*   lSDL_image
*   lSDL_ttf
*   lSDL_mixer
 
In "Search directories" -> "Compiler"
-------------------------------------
*   include
