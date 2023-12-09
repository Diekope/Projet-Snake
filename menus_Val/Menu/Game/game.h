// window.h
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// ===Pour_les_fenetres_de_jeux_des_maps===
typedef struct {
    int x, y;   // Coordonnées du segment
    char name[10]; // Nom du segment (ex: q1, q2, ...)
} GridSquare;

typedef struct {
    GridSquare *segments; // Tableau dynamique de segments
    int length;           // Longueur actuelle du serpent
} Snake;

// ===Fenêtre_de_jeux===
SDL_Window* CreateWindow(const char* title, int width, int height);
int gameWindow(int lg, int ht);

// ===Bonus===
void initializeRandom();
int randCo(int x);
void updateBonusPosition(GridSquare *bonus, int gridWidth, int gridHeight, int gridSize);

// ===Snake===
GridSquare* createSnake(int initialCapacity);
// ===Agrandissement===
GridSquare* resizeSnake(GridSquare* list, int newCapacity);
// ===Ajouter_d'un_élément===
void addToSnake(GridSquare* list, int index, int x, int y, const char* name);

// ===Libéreration===
void freeSnake(GridSquare* list);

void createBodyPart(GridSquare** list, int* capacity, int* currentSize, int x, int y);

#endif // GAME_H