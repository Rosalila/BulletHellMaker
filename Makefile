include Sources.mk
CC=g++
CFLAGS=-c -Wall -std=c++11
ifeq ($(API),steam)
  CFLAGS += -DSTEAM
endif
LDFLAGS=-pthread -lGL -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -L . -lsteam_api
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
