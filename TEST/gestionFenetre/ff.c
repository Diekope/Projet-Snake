#include <SDL.h>

#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void drawMainMenu(SDL_Renderer* renderer) {
    // Couleur de fond pour le menu principal
    SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255); // Un bleu foncé pour le fond
    SDL_RenderClear(renderer);

    // Ici, vous pourriez ajouter du texte pour les options du menu, des images, etc.
    // Par exemple, dessiner un titre de menu simple
    // Note: Vous auriez besoin d'une texture déjà chargée pour le texte du titre
    SDL_Rect menuTitleRect = {100, 50, 600, 100}; // Position et dimensions pour le titre
    SDL_RenderCopy(renderer, menuTitleTexture, NULL, &menuTitleRect);

    // Afficher d'autres éléments du menu...
}

// Fonction pour dessiner la page du jeu
void drawGame(SDL_Renderer* renderer) {
    // Couleur de fond pour le jeu
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Un gris pour le fond
    SDL_RenderClear(renderer);

    // Dessiner les éléments du jeu
    // Par exemple, dessiner un joueur
    SDL_Rect playerRect = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 20, 20};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour le joueur
    SDL_RenderFillRect(renderer, &playerRect);

    // Dessiner plus d'éléments du jeu...
}


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
    SDL_Event e;
    bool quit = false;

int main(int argc, char const *argv[])
{
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        
        }

        // Nettoyer l'affichage
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur fond
        SDL_RenderClear(renderer);

    
        switch (currentPage) {
            case PAGE_MAIN_MENU:
                drawMainMenu(renderer);
                break;
            case PAGE_GAME:
                drawGame(renderer);
                break;
        
        }

        // Met à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;



}