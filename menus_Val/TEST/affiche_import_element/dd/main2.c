#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL
    IMG_Init(IMG_INIT_PNG);  // Initialisation de SDL_image pour le format PNG

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Image Display",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charge l'image dans une surface SDL
    SDL_Surface* imageSurface = IMG_Load("Snake.png");
    if (!imageSurface) {
        printf("Erreur de chargement de l'image: %s\n", IMG_GetError());
        return -1;
    }

    // Crée une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface); // La surface n'est plus nécessaire après la création de la texture



    SDL_Event e;
    int running = 1;
    Uint32 lastTime = 0, currentTime;
    float angle = 0.0;
    while (running) {
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 3000) { // 3000 ms = 3 seconds
            angle += 90; // Pivote de 90 degrés toutes les 3 secondes
            if (angle >= 360) {
                angle = 0;
            }
            lastTime = currentTime;
        }

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }
         SDL_RenderClear(renderer);

        // Render the texture to the screen with rotation
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, angle, NULL, SDL_FLIP_NONE);

        // Update the screen
        SDL_RenderPresent(renderer);

        // You could add a delay here if you wanted to control the frame rate
        SDL_Delay(16); // Approximately 60 frames per second
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
