#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 45;
const int SQUARE_SIZE = GRID_SIZE; // Le carré aura la taille d'une cellule du quadrillage

typedef struct {
    int x;
    int y;
} Square;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Grille sur image de fond", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    // Initialisation de SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("Erreur d'initialisation de SDL_image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Chargement de l'image
    SDL_Surface *loadedSurface = IMG_Load("image.png"); // Assurez-vous que le chemin est correct
    if (!loadedSurface) {
        printf("Erreur de chargement de l'image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Création d'une texture à partir de la surface
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (!backgroundTexture) {
        printf("Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    Square square = {WINDOW_WIDTH / 2 - SQUARE_SIZE / 2, WINDOW_HEIGHT / 2 - SQUARE_SIZE / 2};

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    square.y -= (square.y > 0) ? GRID_SIZE : 0; break;
                    case SDLK_DOWN:  square.y += (square.y < WINDOW_HEIGHT - SQUARE_SIZE) ? GRID_SIZE : 0; break;
                    case SDLK_LEFT:  square.x -= (square.x > 0) ? GRID_SIZE : 0; break;
                    case SDLK_RIGHT: square.x += (square.x < WINDOW_WIDTH - SQUARE_SIZE) ? GRID_SIZE : 0; break;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Dessin du quadrillage
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
        }
        for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
        }

        // Dessin du carré rouge
        SDL_Rect squareRect = {square.x, square.y, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &squareRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
