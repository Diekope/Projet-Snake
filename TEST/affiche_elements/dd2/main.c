#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

SDL_Rect textInputRect = {100, 100, 600, 40}; // x, y, width, height
SDL_Rect buttonRect = {100, 200, 200, 50}; // x, y, width, height
char textInputContent[256] = ""; // Pour stocker le texte saisi

bool InitSDL() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialisation de TTF
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    // Création de la fenêtre
    window = SDL_CreateWindow("SDL2 TextInput and Button", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Création du rendu
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Chargement de la police
    font = TTF_OpenFont("Fonts/Arial Unicode.ttf", 28);// Remplacez par le chemin vers votre police
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void CloseSDL() {
    // Destruction des ressources
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    // Quitter les sous-systèmes de SDL
    TTF_Quit();
    SDL_Quit();
}

void HandleTextInput(SDL_Event e) {
    if (e.type == SDL_TEXTINPUT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE)) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && strlen(textInputContent) > 0) {
            // Supprimer le dernier caractère
            textInputContent[strlen(textInputContent) - 1] = '\0';
        } else if (e.type == SDL_TEXTINPUT) {
            // Ajouter le nouveau caractère
            strcat(textInputContent, e.text.text);
        }
    }
}

void RenderTextInput() {
    // Définir la couleur de fond du champ de texte
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    SDL_RenderFillRect(renderer, &textInputRect);

    // Assurez-vous que la police est chargée et que le contenu du texte n'est pas NULL ou vide
    if (font == NULL || textInputContent == NULL || strlen(textInputContent) == 0) {
        // Gérer l'erreur : la police n'est pas chargée ou le contenu du texte est vide
        return;
    }

    // Afficher le texte saisi
    SDL_Color textColor = {0, 0, 0, 255}; // Noir
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textInputContent, textColor);
    
    // Vérifiez si la surface de texte est valide
    if (!textSurface) {
        // Gérer l'erreur : textSurface est NULL
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    // Vérifiez si la texture est valide
    if (!textTexture) {
        // Gérer l'erreur : textTexture est NULL
        SDL_FreeSurface(textSurface); // Libérer la surface du texte avant de retourner
        return;
    }
    
    // Copier le texte dans le champ de texte
    SDL_Rect textRect = { textInputRect.x + 5, textInputRect.y + 5, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Libérer les ressources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


void RenderButton() {
    // Définir la couleur du bouton
    SDL_SetRenderDrawColor(renderer, 0, 120, 215, 255); // Bleu
    SDL_RenderFillRect(renderer, &buttonRect);

    // Afficher le texte du bouton
    SDL_Color textColor = {255, 255, 255, 255}; // Blanc
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Click Me!", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    // Positionnement du texte sur le bouton
    SDL_Rect textRect = {
        buttonRect.x + (buttonRect.w - textSurface->w) / 2,
        buttonRect.y + (buttonRect.h - textSurface->h) / 2,
        textSurface->w, textSurface->h
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Libérer les ressources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

int main() {
    if (!InitSDL()) {
        printf("Failed to initialize!\n");
        CloseSDL();
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w && y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                    // Le bouton a été cliqué
                }
            }
            HandleTextInput(e);
        }

        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // Gris pour le fond
        SDL_RenderClear(renderer);

        RenderTextInput();
        RenderButton();

        SDL_RenderPresent(renderer);
    }

    CloseSDL();
    
    return 0;
}
