#ifndef SAVE_H
#define SAVE_H

#include "../Game/game.h"

typedef struct {
    int x;
    int y;
} Head;

typedef struct {
    char nomCarte[100];   // Nom de la carte
    char skinTete[100];   // Skin de la tête du serpent
    char skinCorps[100];  // Skin du corps du serpent
    char skinBonus[100];  // Skin du bonus
    int score;            // Score du joueur
    Head teteSerpent;     // Coordonnées de la tête du serpent
    int tailleSerpent;    // Taille du serpent (sans compter la tête)
    GridSquare *corpsSerpent; // Positions des segments du corps du serpent
} GameState;


void sauvegarderEtatJeu(const GameState *etat);
void remplirEtSauvegarder(GameState *etat, GridSquare *tete, Snake *corpsSerpent, int score, const char* card_name, const char* headSkin, const char* bodySkin, const char* bonusSkin);

#endif // SAVE_H