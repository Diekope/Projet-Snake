#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 700;
const int GRID_SIZE = 45;
const int SQUARE_SIZE = GRID_SIZE; // Assurez-vous que SQUARE_SIZE est égal à GRID_SIZE

typedef struct {
    int x;
    int y;
} GridSquare;

GridSquare makeGridAlignedSquare(int x, int y) {
    GridSquare square;
    square.x = (x / GRID_SIZE) * GRID_SIZE; // Aligner sur la grille en X
    square.y = (y / GRID_SIZE) * GRID_SIZE; // Aligner sur la grille en Y
    return square;
}

int main(int argc, char *argv[]) {
    // ... [Initialisation de SDL, SDL_image, création de la fenêtre et du rendu, et chargement de la texture d'arrière-plan]
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
    
    // Initialisation du carré aligné sur la grille
    GridSquare square = makeGridAlignedSquare(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                // Calculer la nouvelle position potentielle du carré
                int newX = square.x;
                int newY = square.y;
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    newY -= GRID_SIZE; break;
                    case SDLK_DOWN:  newY += GRID_SIZE; break;
                    case SDLK_LEFT:  newX -= GRID_SIZE; break;
                    case SDLK_RIGHT: newX += GRID_SIZE; break;
                }
                // Vérifier si la nouvelle position est à l'intérieur des limites de la fenêtre
                if (newX >= 0 && newX < WINDOW_WIDTH && newY >= 0 && newY < WINDOW_HEIGHT) {
                    square.x = newX;
                    square.y = newY;
                }
            }
        }
        

        // Nettoyage de l'écran
        SDL_RenderClear(renderer);

        // Affichage de l'image de fond
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Dessin du carré rouge
        SDL_Rect squareRect = {square.x, square.y, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour le carré
        SDL_RenderFillRect(renderer, &squareRect);
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

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);
    }

    // ... [Nettoyage et sortie]
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
