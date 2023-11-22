CC := gcc
CFLAGS := $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image
TTF_LDFLAGS := -lSDL2_ttf

# Exécutables cibles
TARGET1 := main
TARGET2 := main_t
TARGET3 := main_2

# Fichiers source
SRC1 := main.c
SRC2 := main_t.c
SRC3 := main_2.c

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(SRC1)
	$(CC) $(CFLAGS) $(SRC1) $(LDFLAGS) -o $(TARGET1)

$(TARGET2): $(SRC2)
	$(CC) $(CFLAGS) $(SRC2) $(LDFLAGS) $(TTF_LDFLAGS) -o $(TARGET2)

$(TARGET3): $(SRC3)
	$(CC) $(CFLAGS) $(SRC3) $(LDFLAGS) -o $(TARGET3)

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)

.PHONY: all clean
