#include <SDL.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 20;
const int SQUARE_SIZE = GRID_SIZE; // La taille du carré sera la même que celle du quadrillage

typedef struct {
    int x;
    int y;
} Square;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Déplacement dans le quadrillage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    Square square = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}; // Initialisation du carré au centre de l'écran

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                // Déplacement du carré avec les touches du clavier
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    square.y -= GRID_SIZE; break;
                    case SDLK_DOWN:  square.y += GRID_SIZE; break;
                    case SDLK_LEFT:  square.x -= GRID_SIZE; break;
                    case SDLK_RIGHT: square.x += GRID_SIZE; break;
                }
            }
        }

        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fond blanc
        SDL_RenderClear(renderer);

        // Dessin du quadrillage
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir pour le quadrillage
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT); // Lignes verticales
        }
        for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y); // Lignes horizontales
        }

        // Dessin du carré rouge
        SDL_Rect squareRect = {square.x, square.y, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour le carré
        SDL_RenderFillRect(renderer, &squareRect);

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);

        // Délai pour contrôler la vitesse de rafraîchissement
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
