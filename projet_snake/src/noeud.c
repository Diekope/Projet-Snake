#include <stdio.h>
#include <stdlib.h>
#include "../header/noeud.h"
/*la meme chose de ce qu'on avait vu en cours juste 2 variables de +*/
Noeud* creerNoeud(int val,int x,int y, Noeud *n){
	Noeud *N=(Noeud*)malloc(sizeof(Noeud));
	if(N==NULL){
		exit(EXIT_FAILURE);
	}
	N->val=val;
	N->x=x;
	N->y=y;
	N->sent=0;
	N->suiv=n;
	return N;
}
void libererNoeud(Noeud *n){
	if(n!=NULL){
		free(n);
	}
}
Noeud* creerSent(){
	Noeud *N=creerNoeud(0,0,0,NULL);
	N->sent=1;
	return N;
}
int estSent(Noeud *n){
	return (n->sent==1);
}
int aSuivant(Noeud *n){
	return(n->suiv!=NULL);
}
Noeud* suivant(Noeud *n){
	return n->suiv;
}
int contenu(Noeud *n){
	return n->val;
}
int valeurx(Noeud *n){
	return n->x;
}
int valeurY(Noeud *n){
	return n->y;
}
void changercoord(Noeud* n,int x,int y){
	n->x=x;
	n->y=y;
}
void changerCont(Noeud *n, int val,int x,int y){
	n->val=val;
	n->x=x;
	n->y=y;
}
void changerSuiv(Noeud *n, Noeud *p){
	n->suiv=p;
}