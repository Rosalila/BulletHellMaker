CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lGL -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -L . -lsteam_api
ROSALILA_SHOOTER_SOURCES=RosalilaShooter/STG.cpp RosalilaShooter/Character/Bullet.cpp RosalilaShooter/Character/Character.cpp RosalilaShooter/Character/Enemy.cpp RosalilaShooter/Character/Modifier.cpp RosalilaShooter/Character/Pattern.cpp RosalilaShooter/Character/Player.cpp RosalilaShooter/Menus/ControllerConfig.cpp RosalilaShooter/Menus/Intro.cpp RosalilaShooter/Menus/StageSelect.cpp RosalilaShooter/Menus/StageModsSelect.cpp RosalilaShooter/Stage/Layer.cpp RosalilaShooter/Stage/LayerFrame.cpp RosalilaShooter/Stage/Stage.cpp RosalilaShooter/Utility/Utility.cpp
include Rosalila/Sources.mk
SOURCES=main.cpp $(ROSALILA_SHOOTER_SOURCES) $(ROSALILA_SOURCES)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Flatshot

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-find . -name '*.o' -exec rm -r {} \;
	-rm -f $(EXECUTABLE)
