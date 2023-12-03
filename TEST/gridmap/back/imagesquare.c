#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 45;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Grille avec carré image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur d'initialisation de SDL_image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Surface *backgroundSurface = IMG_Load("image.png");
    if (!backgroundSurface) {
        fprintf(stderr, "Erreur de chargement de l'image de fond: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        fprintf(stderr, "Erreur de création de la texture de fond: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Surface *squareSurface = IMG_Load("banana.png");
    if (!squareSurface) {
        fprintf(stderr, "Erreur de chargement de l'image du carré: %s\n", IMG_GetError());
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Texture *squareTexture = SDL_CreateTextureFromSurface(renderer, squareSurface);
    SDL_FreeSurface(squareSurface);
    if (!squareTexture) {
        fprintf(stderr, "Erreur de création de la texture du carré: %s\n", SDL_GetError());
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Rect squareRect = {WINDOW_WIDTH / 2 - GRID_SIZE / 1.1, WINDOW_HEIGHT / 2 - GRID_SIZE / 1.5, GRID_SIZE, GRID_SIZE};

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    squareRect.y -= (squareRect.y > 0) ? GRID_SIZE : 0; break;
                    case SDLK_DOWN:  squareRect.y += (squareRect.y < WINDOW_HEIGHT - GRID_SIZE) ? GRID_SIZE : 0; break;
                    case SDLK_LEFT:  squareRect.x -= (squareRect.x > 0) ? GRID_SIZE : 0; break;
                    case SDLK_RIGHT: squareRect.x += (squareRect.x < WINDOW_WIDTH - GRID_SIZE) ? GRID_SIZE : 0; break;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, squareTexture, NULL, &squareRect);

        // Dessin du quadrillage par-dessus avec des lignes fines
// Couleur noire pour le quadrillage
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT); // Lignes verticales
        }
        for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y); // Lignes horizontales
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(squareTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
