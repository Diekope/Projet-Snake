CC := gcc
CFLAGS := $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image
TTF_LDFLAGS := -lSDL2_ttf

# Exécutables cibles
TARGET1 := acceuil

# Fichiers source
SRC1 := acceuil.c

all: $(TARGET1) 

$(TARGET1): $(SRC1)
	$(CC) $(CFLAGS) $(SRC1) $(LDFLAGS) $(TTF_LDFLAGS) -o $(TARGET1)


clean:
	rm -f $(TARGET1)

.PHONY: all clean
