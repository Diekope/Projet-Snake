#include <SDL.h>
#include <SDL_image.h>
#include <time.h>

void interpolateColor(SDL_Color* currentColor, SDL_Color startColor, SDL_Color endColor, float progress) {
    currentColor->r = (Uint8)(startColor.r + (endColor.r - startColor.r) * progress);
    currentColor->g = (Uint8)(startColor.g + (endColor.g - startColor.g) * progress);
    currentColor->b = (Uint8)(startColor.b + (endColor.b - startColor.b) * progress);
}

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

    // Définir les couleurs de départ et de fin
    SDL_Color startColor = {255, 0, 0, 255}; // Rouge
    SDL_Color endColor = {0, 255, 0, 255};   // Vert
    SDL_Color currentColor;
    const int transitionFrames = 180; // Nombre de frames pour la transition
    int frame = 0;

    SDL_Event e;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        float progress = (float)frame / transitionFrames;
        interpolateColor(&currentColor, startColor, endColor, progress);

        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        frame++;
        if (frame > transitionFrames) {
            frame = 0;
            // Optionnel: Changer les couleurs de début et de fin pour la prochaine transition
            SDL_Color temp = startColor;
            startColor = endColor;
            endColor = temp;
        }

        SDL_Delay(16); // Environ 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
