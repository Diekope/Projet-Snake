#ifndef CARTE_H_INCLUDED
#define CARTE_H_INCLUDED
#include "liste.h"
typedef struct carte_{
	int ligne;
	int colonne;
	char **elt;
	
}carte;
void afficher_terrain(carte *terrain);
void liberer_terrain(carte *terrain);
carte *lire_terrain();
int nbligne(carte *terrain);
int nbcolonne(carte *terrain);
void ajouterelt(carte *terrain,int x,int y,char val);
char valxy(carte *terrain,int x,int y);
#endif //CARTE_H_INCLUDED

