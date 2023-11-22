#include <SDL.h>
#include <SDL_image.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Colored Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1080, 640,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Event e;
    int running = 1;
    int toggleColor = 0;

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Générer un nombre aléatoire entre 1 et 255
        int randomNumber = rand() % 255 + 1;
        int randomNumber2 = rand() % 255 + 1;
        int randomNumber3 = rand() % 255 + 1;
        int intensite = rand() % 255 + 1;

        // Toggle the color every 3 seconds
        SDL_SetRenderDrawColor(renderer, randomNumber, randomNumber2, randomNumber3, intensite);

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Wait for 3 seconds

        toggleColor = !toggleColor; // Switch color for the next iteration
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
