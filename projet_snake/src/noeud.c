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

/*libère noeud*/

void libererNoeud(Noeud *n){

	if(n!=NULL){

		free(n);

	}

}

/*créer sent*/

Noeud* creerSent(){

	Noeud *N=creerNoeud(0,0,0,NULL);

	N->sent=1;

	return N;

}

/*test si noeud est un sent*/

int estSent(Noeud *n){

	return (n->sent==1);

}

/*test si noeud as un suivant*/

int aSuivant(Noeud *n){

	return(n->suiv!=NULL);

}

/*récupère le suivant*/

Noeud* suivant(Noeud *n){

	return n->suiv;

}

/*récupère sa valeur*/

int contenu(Noeud *n){

	return n->val;

}

/*récupère sa position x*/

int valeurx(Noeud *n){

	return n->x;

}

/*récupère sa position y*/

int valeurY(Noeud *n){

	return n->y;

}

/*changer la cordonnée de noeud*/

void changercoord(Noeud* n,int x,int y){

	n->x=x;

	n->y=y;

}

/*changer tous les valeurs*/

void changerCont(Noeud *n, int val,int x,int y){

	n->val=val;

	n->x=x;

	n->y=y;

}

/*changer le suivant de noeud n*/

void changerSuiv(Noeud *n, Noeud *p){

	n->suiv=p;

}

