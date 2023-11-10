#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// Initialisation de SDL2 et TTF
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return false;
    }
    window = SDL_CreateWindow("Menu de sélection", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Charger une police
    font = TTF_OpenFont("../Fonts/Arial Unicode.ttf", 28); // Remplacer par le chemin vers votre police
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void close() {
    // Détruire la fenêtre
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    // Quitter les sous-systèmes SDL
    TTF_Quit();
    SDL_Quit();
}

int main() {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        bool quit = false;
        SDL_Event e;
        int selected = 0;
        char* options[3] = {"Option 1", "Option 2", "Quitter"};

        while (!quit) {
            // Gestion des événements
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            selected = (selected > 0) ? selected - 1 : 0;
                            break;
                        case SDLK_DOWN:
                            selected = (selected < 2) ? selected + 1 : 2;
                            break;
                        case SDLK_RETURN:
                            quit = true; // Vous pouvez gérer chaque option ici
                            break;
                    }
                }
            }

            // Effacer l'écran
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);

            // Dessiner les options ici en utilisant SDL2 et TTF
            // ...

            // Mettre à jour l'écran
            SDL_RenderPresent(renderer);
        }
    }

    // Libérer les ressources et fermer SDL
    close();
    return 0;
}
