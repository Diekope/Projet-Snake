#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/carte.h"

/*lire le terrain dans un fichier
l'exemple de composition d'un fichier de la carte:

l c
##...# total c colonne et l ligne 
#    #
.    .
.    .
.    .
##...#
méthode de lecture: caractère par caractère
*/
carte* lire_terrain(char *path){
	int c,l;
	char cara;
	FILE *file=fopen(path,"r");
	if(file==NULL){
		printf("fopen:fichier non existe\n");
		return NULL;
	}
	fscanf(file,"%d",&l);
	fscanf(file,"%d",&c);
	fscanf(file,"%c",&cara);
	carte *terrain=(carte *)malloc(sizeof(carte));
	terrain->ligne=l;
	terrain->colonne=c;
	terrain->elt=(char**)malloc(l*sizeof(char*));
	for(int i=0;i<l;i++){
		terrain->elt[i]=(char*)malloc(c*sizeof(char));
	}
	for(l=0;l<terrain->ligne;l++){
		for(c=0;c<=terrain->colonne;c++){
			fscanf(file,"%c",&cara);
			if(cara!='\n'){
				terrain->elt[l][c]=cara;
			}
			else{
				terrain->elt[l][c]='\0';
			}
		}
	}
	fclose(file);
	return terrain;
}
/*retourne nb de ligne*/
int nbligne(carte *terrain){
	return terrain->ligne;
}
/*retourne nb de colonne*/
int nbcolonne(carte *terrain){
	return terrain->colonne;
}
/*ajoute un element dans le terrain*/
void ajouterelt(carte *terrain,int x,int y,char val){
	terrain->elt[y][x]=val;
}
/*retourne la valeur qui se trouve dans cette position*/
char valxy(carte *terrain,int x,int y){
	return terrain->elt[y][x];
}
/*libere la structure*/
void liberer_terrain(carte *terrain){
	for(int i=0;i<terrain->ligne;i++){
		free(terrain->elt[i]);
	}
	free(terrain->elt);
	free(terrain);
}
/*affiche la struture*/
void afficher_terrain(carte *terrain){
	int i,j;
	for(i=0;i<terrain->ligne;i++){
		for( j=0;j<terrain->colonne;j++){
		printf("%c",terrain->elt[i][j]);
		}
		printf("\n");
	}
}
