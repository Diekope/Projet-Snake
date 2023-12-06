CC := gcc
CFLAGS := `sdl2-config --cflags` 
LDFLAGS := `sdl2-config --libs`
TARGET := ff
SRC := ff.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean