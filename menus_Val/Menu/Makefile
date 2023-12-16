CC := gcc
CFLAGS := $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer
TTF_LDFLAGS := -lSDL2_ttf

# Exécutables cibles
TARGET1 := acceuil
TARGET2 := game

# Fichiers source
SRC1 := acceuil.c Windows/window.c Game/game.c Saves/save.c
SRC2 := game.c fonctions.c

all: $(TARGET1) #$(TARGET2)

$(TARGET1): $(SRC1)
	$(CC) $(CFLAGS) $(SRC1) $(LDFLAGS) $(TTF_LDFLAGS) -o $(TARGET1)

#$(TARGET2): $(SRC2)
#	$(CC) $(CFLAGS) $(SRC2) $(LDFLAGS) $(TTF_LDFLAGS) -o $(TARGET2)

clean:
	rm -f $(TARGET1)

.PHONY: all clean
