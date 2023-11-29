BIN=snake
OBJECTS= obj/main.o obj/liste.o obj/noeud.o obj/carte.o obj/jeu.o
CC=gcc
all: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN)
obj/main.o: bin/main.c
	$(CC) -c bin/main.c -o obj/main.o -I header/
obj/noeud.o: src/noeud.c header/noeud.h
	$(CC) -c src/noeud.c -o obj/noeud.o -I header/
obj/liste.o: src/liste.c header/liste.h
	$(CC) -c src/liste.c -o obj/liste.o -I header/
obj/carte.o: src/carte.c header/carte.h
	$(CC) -c src/carte.c -o obj/carte.o -I header/
obj/jeu.o: src/jeu.c header/jeu.h
	$(CC) -c src/jeu.c -o obj/jeu.o -I header/
clean:
	rm obj/*o;rm snake
