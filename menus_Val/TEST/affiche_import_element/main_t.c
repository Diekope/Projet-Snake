#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Colored Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1080, 640,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Charger une police
    TTF_Font* font = TTF_OpenFont("/Users/ValQuiTravaille/Desktop/Université/L2/Programmation/Projet Snake/git/Projet-Snake/TEST/affiche_elements/Fonts/Arial Unicode.ttf", 12); // Remplacer par le chemin réel et la taille souhaitée
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Créer une surface de texte
    SDL_Color textColor = {255, 255, 255, 255}; // Couleur blanche
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Votre texte ici", textColor);
    if (!textSurface) {
        printf("Erreur de création de la surface de texte: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Créer une texture à partir de la surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // La surface n'est plus nécessaire

    if (!textTexture) {
        printf("Erreur de création de la texture de texte: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Event e;
    int running = 1;
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        int randomNumber = rand() % 255 + 1;
        int randomNumber2 = rand() % 255 + 1;
        int randomNumber3 = rand() % 255 + 1;

        SDL_SetRenderDrawColor(renderer, randomNumber, randomNumber2, randomNumber3, 255);
        SDL_RenderClear(renderer);

        // Rendre la texture de texte
        SDL_RenderCopy(renderer, textTexture, NULL, NULL); // Vous pouvez ajuster la position et la taille ici

        SDL_RenderPresent(renderer);

        SDL_Rect textRect;
        textRect.x = position_x;  // Position horizontale du texte
        textRect.y = position_y;  // Position verticale du texte
        textRect.w = largeur;     // Largeur du texte (peut être déterminée automatiquement)
        textRect.h = hauteur;     // Hauteur du texte (peut être déterminée automatiquement)


        SDL_Delay(600);
    }

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
