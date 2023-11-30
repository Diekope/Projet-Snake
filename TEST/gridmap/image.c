#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 32;

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
    SDL_Surface *loadedSurface = IMG_Load("image.png"); // Remplacez par le chemin de votre image
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
    SDL_FreeSurface(loadedSurface); // La surface n'est plus nécessaire après la création de la texture

    if (!backgroundTexture) {
        printf("Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Boucle principale
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        // Gestion des événements
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Nettoyage de l'écran
        SDL_RenderClear(renderer);

        // Affichage de l'image de fond
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Dessin du quadrillage par-dessus
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur noire pour le quadrillage
// ... [Votre code initial]

// Dessin du quadrillage par-dessus avec des lignes fines
// Couleur noire pour le quadrillage
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT); // Lignes verticales
        }
        for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y); // Lignes horizontales
        }

// ... [Le reste de votre boucle de rendu]
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
                SDL_Rect gridCell = {x, y, GRID_SIZE, GRID_SIZE};
                SDL_RenderDrawRect(renderer, &gridCell);
            }
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}