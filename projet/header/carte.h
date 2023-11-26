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
#endif //CARTE_H_INCLUDED

