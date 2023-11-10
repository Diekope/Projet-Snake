#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdbool.h>

// Définition des dimensions de la fenêtre
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Déclaration de quelques fonctions nécessaires
void DrawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect);
bool PointInRect(int x, int y, SDL_Rect *rect);
void HandleTextInput(SDL_Event *event, char *textInput, bool *textInputActive);

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("SDL2 Text Input and Button", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Création et initialisation des éléments GUI
    SDL_Rect textInputRect = {100, 100, 200, 30}; // x, y, width, height
    SDL_Rect buttonRect = {100, 200, 200, 50}; // x, y, width, height
    char textInputContent[256] = ""; // Pour stocker le texte saisi
    bool isTextInputActive = false;

    // Configuration de la police et de la couleur du texte
    TTF_Font *font = TTF_OpenFont("Fonts/Arial Unicode.ttf", 28);
    SDL_Color textColor = {0, 0, 0, 255}; // Noir

    SDL_Event event;
    bool isRunning = true;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (PointInRect(event.button.x, event.button.y, &buttonRect)) {
                    // Le bouton a été cliqué, exécuter l'action du bouton
                    printf("Bouton cliqué!\n");
                }
            }

            // Gérer la saisie de texte
            HandleTextInput(&event, textInputContent, &isTextInputActive);
        }

        // Dessiner l'interface
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
        SDL_RenderClear(renderer);

        // Dessiner le champ de texte
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
        SDL_RenderDrawRect(renderer, &textInputRect);
        DrawText(renderer, font, textInputContent, textColor, &textInputRect);

        // Dessiner le bouton
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
        SDL_RenderDrawRect(renderer, &buttonRect);
        DrawText(renderer, font, "Cliquer ici", textColor, &buttonRect);

        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void DrawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

bool PointInRect(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && y >= rect->y && x <= rect->x + rect->w && y <= rect->y + rect->h;
}

void HandleTextInput(SDL_Event *event, char *textInput, bool *textInputActive) {
    if (*textInputActive) {
        switch (event->type) {
            case SDL_KEYDOWN:
                // Gestion des touches spéciales (par exemple: backspace)
                if (event->key.keysym.sym == SDLK_BACKSPACE && strlen(textInput) > 0) {
                    textInput[strlen(textInput) - 1] = '\0';
                }
                break;
            case SDL_TEXTINPUT:
                // Ajout du nouveau texte à la chaîne
                strcat(textInput, event->text.text);
                break;
        }
    }
}
