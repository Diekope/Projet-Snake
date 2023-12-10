// window.h
#ifndef WINDOW_H
#define WINDOW_H

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
    const char* card_name;  // Le nom de la carte
    const char* description; // Description de la carte
    int largeur;
    int hauteur;
} Card;

// ===Initialisation_de_base_pour_les_fenêtres=== (problème d'allocation mémoire à régler)
int initializeWindow(const char* title, int width, int height, SDL_Window** outWindow, SDL_Renderer** outRenderer);
// ===Destruction_des_fenêtres=== (problème d'allocation mémoire à régler)
void destroy_window(SDL_Texture* textTexture, TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window);

// ===Les_différentes_fenêtres_du_menu
int fenetre_acceuil(); // La fenetre d'acceuil
int partie(); // Nouvelle partie ou charger partie
int nMap(); // Choix de map en cas de nouvelle partie
int lostWindow(int score); // Quand on a perdu, retour au menu ou quitter le jeux

void drawButton(SDL_Renderer* renderer, TTF_Font* font, Button button); // Pour les boutons, on ne peut pas définir leur position dessus
int isMouseOverButton(Button button, int mouseX, int mouseY); // Pour le clic de la souris

void loadAndDisplayCards(SDL_Renderer* renderer, TTF_Font* font, Card* cards, int numCards); // Affichage des cartes
void cleanUpCards(Card* cards, int numCards); // Effacer les cartes


#endif //WINDOW_H