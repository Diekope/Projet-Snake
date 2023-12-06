#ifndef NOEUD_H_INCLUDED
#define NOEUD_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>


typedef struct noeud Noeud;
struct noeud{
	int val;
	int x;
	int y;
	int sent;
	Noeud *suiv;
};
Noeud* creerNoeud(int val,int x,int y, Noeud *n);
void libererNoeud(Noeud *n);
Noeud *creerSent();
int estSent(Noeud *n);
int aSuivant(Noeud *n);
Noeud* suivant(Noeud *n);
int contenu(Noeud *n);
int valeurx(Noeud *n);
int valeurY(Noeud *n);
void changercoord(Noeud* n,int x,int y);
void changerCont(Noeud *n, int val,int x,int y);
void changerSuiv(Noeud *n,Noeud *p);
#endif //NOEUD_H_INCLUDED
