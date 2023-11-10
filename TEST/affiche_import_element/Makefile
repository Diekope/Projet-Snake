CC := gcc
CFLAGS := `sdl2-config --cflags` -lSDL2_image
LDFLAGS := `sdl2-config --libs`
TARGET := main
SRC := main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean