CC := gcc
CFLAGS := `sdl2-config --cflags` -Wall -Wextra
LDFLAGS := `sdl2-config --libs` -lSDL2_ttf

# Nom du fichier exécutable
TARGET := main
# Vos fichiers source
SRCS := main.c
# Générer des noms de fichiers objets à partir des fichiers source
OBJS := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
