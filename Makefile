include Sources.mk
CC = cl.exe
CFLAGS = /EHsc /MD
SOURCES = main.cpp include/dirent.cpp $(ROSALILA_SHOOTER_SOURCES)
LIBRARIES = rosalila.lib SDL2.lib SDL2main.lib SDL2_image.lib SDL2_mixer.lib SDL2_ttf.lib SDL2test.lib steam_api.lib opengl32.lib

build:
 $(CC) $(CFLAGS) $(SOURCES) /I include /link /LIBPATH:lib $(LIBRARIES) /SUBSYSTEM:windows

clean:
 del *.obj
 del *.exe
