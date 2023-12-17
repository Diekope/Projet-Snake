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
typedef struct joueur_{
	int score;
	int pas;
	char carte[20];/*NULL par defaut sinon demande au joueur s'il veut continuer ou non*/
	int d;
	int fx;
	int fy;
	int fval;
}joueur;
carte *recupere_carte(joueur *player);
serpent *lire_serpent(char nom[20],joueur player);
serpent *recup_serpent(FILE *fp,serpent *snake);
void sauvegarde_serpent(char nom[20],serpent *snake);
void enregistrer_joueur(joueur *player,char nom[20]);
joueur joueur_vide();
joueur connexion(char nom[20]);
Noeud* generation_suivante(carte* terrain,serpent *snake,Noeud* fruit,direction d,int *pas,int *gameover,joueur *player);/*calculer les nouveaux coordonnées de snake et de positionner les noeud dans le terrain à la fin*/
Noeud* initialisation(carte *terrain,serpent *snake,direction *d,int *pas,joueur player);/*si la structure carte n'est pas sauvegardé dans le fichier map/temporaire/tmp_map/nom_joueur.txt alors connexion normale sinon on récupère la structure carte puis aller sur le dossier map/temporaire/tmp_fruit pour cercher nom_joueur.txt si le fichier existe alors on récupère la structure à l'interieur +nombre de pas resté*/
void ajouter_terrain(carte* terrain,Noeud *n,int choix);
void choisir_direction(direction *d,int *gameover);
Noeud* test_toucher(carte *terrain,serpent *snake);
Noeud* generer_fruit(carte *terrain,serpent *snake);
char alphabet(int choix, int val);
int kbhit(void);
void clear(carte *terrain);

#endif //JEU_H