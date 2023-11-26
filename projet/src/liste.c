#include <stdio.h>
#include <stdlib.h>
#include "../header/liste.h"

serpent* serpentvide(){
	serpent *l=(serpent *)malloc(sizeof(serpent));
	l->sentAvt=creerSent();
	l->sentArr=creerSent();
	changerSuiv(l->sentAvt,l->sentArr);
	return l;
};
void libererserpent(serpent *l){
	Noeud *n=l->sentAvt;
	while(n!=NULL){
		l->sentAvt=n->suiv;
		libererNoeud(n);
		n=l->sentAvt;
	}
	free(l);
}
int estserpentvide(serpent *l){
	return(suivant(l->sentAvt)==l->sentArr);
}
Noeud* iemeNoeud(serpent *l,int i){
	int j=0;
	Noeud *resultat;
	resultat=l->sentAvt;
	while(resultat!=NULL && j<i){
		resultat=resultat->suiv;
		j++;
	}
	return resultat;
}
int iemeElt(serpent *l,int i){
	return contenu(iemeNoeud(l,i));
}
void inserer(serpent *l,int i, int val,int x,int y){
	Noeud *prec=iemeNoeud(l,i-1);
	Noeud *n =creerNoeud(val,x,y,suivant(prec));
	changerSuiv(prec,n);
}
void afficherserpent(serpent *l){
	if(l!=NULL){
		Noeud *actuel=l->sentAvt;
		printf("[ ");
		while(actuel!=NULL){
			if(estSent(actuel)==0){
			printf("%d :(%d,%d) ",actuel->val,actuel->x,actuel->y);};
			actuel=actuel->suiv;
		}
		printf("]\n");
	}
}
int serpentlongueur(serpent* l){
    int lg = 0;
    if (l == NULL){
        exit(EXIT_FAILURE);
    }
    Noeud *actuel = l->sentAvt;

    while (actuel != NULL){
        lg++;
        actuel = actuel->suiv;
    }
    return lg-2;
}
