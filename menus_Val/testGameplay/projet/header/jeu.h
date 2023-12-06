#ifndef JEU_H
#define JEU_H
#include "carte.h"
typedef enum direction_{
	init=0,
	haute,
	bas,
	gauche,
	droite
	
}direction;
char **infovide();
void libererinfo(char **info);
void connexion(int *nb_partie_gagnee, int *nb_partie_gagnee_generale,char **info);
Noeud* generation_suivante(carte* terrain,serpent *snake,Noeud* fruit,direction d,int *pas,int *gameover);/*calculer les nouveaux coordonnées de snake et de positionner les noeud dans le terrain à la fin*/
Noeud* initialisation(carte *terrain,serpent *snake,direction *d,int *pas);/*si la structure carte n'est pas sauvegardé dans le fichier map/temporaire/tmp_map/nom_joueur.txt alors connexion normale sinon on récupère la structure carte puis aller sur le dossier map/temporaire/tmp_fruit pour cercher nom_joueur.txt si le fichier existe alors on récupère la structure à l'interieur +nombre de pas resté*/
void ajouter_terrain(carte* terrain,Noeud *n,int choix);
void choisir_direction(direction *d,int *gameover);
Noeud* test_toucher(carte *terrain,serpent *snake);
Noeud* generer_fruit(carte *terrain,serpent *snake);
char alphabet(int choix, int val);
int kbhit(void);
void clear(carte *terrain);

#endif //JEU_H
