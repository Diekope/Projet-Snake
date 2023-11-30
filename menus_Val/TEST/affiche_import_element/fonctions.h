// button.h
#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct {
    SDL_Rect rect;     // Dimensions et position
    const char* text;  // Texte du bouton
} Button;

void drawButton(SDL_Renderer* renderer, TTF_Font* font, Button button); // Pour les boutons, on ne peut pas définir leur position dessus
int isMouseOverButton(Button button, int mouseX, int mouseY); // Pour le clic de la souris

int fenetre_acceuil(); // La fenetre d'acceuil
void destroy_window(SDL_Texture* textTexture, TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window);
int partie();
#endif // FONCTIONS_H