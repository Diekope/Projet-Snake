// window.h
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// ===Pour_les_fenetres_de_jeux_des_maps===
typedef struct {
    int x;  // Largeur de la fenêtre
    int y;  // Hauteur de la fenêtre
} GridSquare;

// ===Fenêtre_de_jeux===
SDL_Window* CreateWindow(const char* title, int width, int height);
int gameWindow(int lg, int ht);

// ===Bonus===
void initializeRandom();
int randCo(int x);
void updateBonusPosition(GridSquare *bonus, int gridWidth, int gridHeight, int gridSize);

#endif // GAME_H