#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/carte.h"
#define path "../data/map/map.txt"

carte* lire_terrain(){
	int c,l;
	char cara;
	FILE *file=fopen(path,"r");
	if(file==NULL){
		printf("fopen:fichier non existe\n");
		return NULL;
	}
	fscanf(file,"%d",&l);
	fscanf(file,"%d",&c);
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
		}
	}
	fclose(file);
	return terrain;
}
int nbligne(carte *terrain){
	return terrain->ligne;
}
int nbcolonne(carte *terrain){
	return terrain->colonne;
}
void ajouterelt(carte *terrain,int x,int y,char val){
	terrain->elt[y][x]=val;
}
char valxy(carte *terrain,int x,int y){
	return terrain->elt[y][x];
}
void liberer_terrain(carte *terrain){
	for(int i=0;i<terrain->ligne;i++){
		free(terrain->elt[i]);
	}
	free(terrain->elt);
	free(terrain);
}
void afficher_terrain(carte *terrain){
	int i,j;
	for(i=0;i<terrain->ligne;i++){
		for( j=0;j<=terrain->colonne;j++){
		printf("%c",terrain->elt[i][j]);
		}
		printf("\n");
	}
}
