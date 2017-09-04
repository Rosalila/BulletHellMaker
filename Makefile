include Sources.mk
CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lGL -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -L . -lsteam_api
SOURCES=main.cpp $(ROSALILA_SHOOTER_SOURCES) $(ROSALILA_SOURCES)
OBJECTS=$(SOURCES:.cpp=.o) libsecret.a
ifeq ($(API),secret)
  LDFLAGS += -lcurl -pthread -lz -I secret_include
  CFLAGS += -DSECRET
  SOURCES += RosalilaShooter/Menus/StageSecretSelect.cpp
  OBJECTS += libsecret.a
endif
ifeq ($(API),steam)
  CFLAGS += -DSTEAM
endif
EXECUTABLE=Flatshot

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ -I secret_include

clean:
	-find . -name '*.o' -exec rm -r {} \;
	-rm -f $(EXECUTABLE)
