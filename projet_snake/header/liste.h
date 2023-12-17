#ifndef LISTE_H_INCLUDED
#define LISTE_H_INCLUDED

#include "noeud.h"

typedef struct serpent_ {
    Noeud *sentAvt;
    Noeud *sentArr;
} serpent;

serpent* serpentvide();
void libererserpent(serpent *l);
int estserpentvide(serpent *l);
Noeud *iemeNoeud(serpent *l,int i);
int iemeElt(serpent *l,int i);
int serpentlongueur(serpent* l);
void inserer(serpent *l,int i, int val,int x,int y);
/* ------------pour le test---------*/
void afficherserpent(serpent *l);
#endif //LISTE_H_INCLUDED