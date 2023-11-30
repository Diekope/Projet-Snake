#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 32; // Taille de chaque cellule du quadrillage

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Grille colorée SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc pour le fond
        SDL_RenderClear(renderer);

        // Remplir le quadrillage avec des couleurs aléatoires
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
                // Générer une couleur aléatoire
                Uint8 r = rand() % 256;
                Uint8 g = rand() % 256;
                Uint8 b = rand() % 256;
                SDL_SetRenderDrawColor(renderer, r, g, b, 255); // Couleur aléatoire pour la cellule
                SDL_Rect rect = {x, y, GRID_SIZE, GRID_SIZE};
                SDL_RenderFillRect(renderer, &rect); // Remplir la cellule avec la couleur
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}