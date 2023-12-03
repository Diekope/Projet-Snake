// button.h
#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// ===Pour_les_boutons===
typedef struct {
    SDL_Rect rect;     // Dimensions et position
    const char* text;  // Texte du bouton
} Button;

// ===Pour_les_maps===
typedef struct {
    SDL_Rect rect;          // Position et dimensions de l'image de la carte
    SDL_Texture* image;     // Texture pour l'image de la carte
    const char* description; // Description de la carte
} Card;


void drawButton(SDL_Renderer* renderer, TTF_Font* font, Button button); // Pour les boutons, on ne peut pas définir leur position dessus
int isMouseOverButton(Button button, int mouseX, int mouseY); // Pour le clic de la souris

void loadAndDisplayCards(SDL_Renderer* renderer, TTF_Font* font, Card* cards, int numCards);
void cleanUpCards(Card* cards, int numCards);

int fenetre_acceuil(); // La fenetre d'acceuil
void destroy_window(SDL_Texture* textTexture, TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window);
int partie();
int nMap();

#endif // FONCTIONS_H