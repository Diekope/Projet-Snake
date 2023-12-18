// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #include "save.h"
// #include "../Game/game.h"

// void sauvegarderEtatJeu(const GameState *etat) {
//     FILE *file = fopen("../Saves/sauvegarde.txt", "w");
//     if (file != NULL) {
//         // Écrire le nom de la carte
//         fprintf(file, "Nom de la Carte: %s\n", etat->nomCarte);

//         // Écrire les skins
//         fprintf(file, "Skin de la Tête: %s\n", etat->skinTete);
//         fprintf(file, "Skin du Corps: %s\n", etat->skinCorps);
//         fprintf(file, "Skin du Bonus: %s\n", etat->skinBonus);

//         // Écrire le score
//         fprintf(file, "Score: %d\n", etat->score);

//         // Écrire la position de la tête du serpent
//         fprintf(file, "Position de la Tête: (%d, %d)\n", etat->teteSerpent.x, etat->teteSerpent.y);

//         // Écrire la taille du serpent (sans compter la tête)
//         fprintf(file, "Taille du Serpent: %d\n", etat->tailleSerpent);

//         // Écrire les positions des segments du corps du serpent
//         for (int i = 0; i < etat->tailleSerpent; i++) {
//             fprintf(file, "Segment %d: (%d, %d)\n", i, etat->corpsSerpent[i].x, etat->corpsSerpent[i].y);
//         }

//         fclose(file);
//     } else {
//         printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
//     }
// }



// void remplirEtSauvegarder(GameState *etat, GridSquare *tete, Snake *corpsSerpent, int score, const char* card_name, const char* headSkin, const char* bodySkin, const char* bonusSkin) {
//     // Copier les noms dans la structure GameState
//     strncpy(etat->nomCarte, card_name, sizeof(etat->nomCarte) - 1);
//     etat->nomCarte[sizeof(etat->nomCarte) - 1] = '\0';

//     strncpy(etat->skinTete, headSkin, sizeof(etat->skinTete) - 1);
//     etat->skinTete[sizeof(etat->skinTete) - 1] = '\0';

//     strncpy(etat->skinCorps, bodySkin, sizeof(etat->skinCorps) - 1);
//     etat->skinCorps[sizeof(etat->skinCorps) - 1] = '\0';

//     strncpy(etat->skinBonus, bonusSkin, sizeof(etat->skinBonus) - 1);
//     etat->skinBonus[sizeof(etat->skinBonus) - 1] = '\0';

//     // Copier les autres données
//     etat->score = score;

//     // Copier la position de la tête du serpent
//     etat->teteSerpent.x = tete->x;
//     etat->teteSerpent.y = tete->y;

//     // La taille du serpent est la longueur du corps, la tête n'est pas comptée
//     etat->tailleSerpent = corpsSerpent->length;

//     // Allouer ou réallouer la mémoire pour le corps du serpent dans GameState
//     GridSquare *nouveauCorps = realloc(etat->corpsSerpent, corpsSerpent->length * sizeof(GridSquare));
//     if (nouveauCorps == NULL) {
//         // Gestion de l'erreur d'allocation mémoire
//         perror("Erreur d'allocation de mémoire pour corpsSerpent");
//         return;
//     }
//     etat->corpsSerpent = nouveauCorps;

//     // Copier les données de position du corps du serpent
//     for (int i = 0; i < corpsSerpent->length; i++) {
//         etat->corpsSerpent[i].x = corpsSerpent->body[i].x;
//         etat->corpsSerpent[i].y = corpsSerpent->body[i].y;
//     }

//     // Appeler la fonction de sauvegarde
//     sauvegarderEtatJeu(etat);
// }
