#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../header/jeu.h"
int main(){
	direction d=init;
	int pas,gameover=0;
	carte* terrain= lire_terrain();
	serpent *snake=serpentvide();
	Noeud* fruit= initialisation(terrain,snake,&d,&pas);
	afficher_terrain(terrain);
	while(gameover==0){
		while(!kbhit()){
			if(gameover==1){
				break;
			}
			if(d!=init){
				if(pas==0){
					pas=10;
					libererNoeud(fruit);
					fruit=generer_fruit(terrain,snake);
				}
				printf("\e[H\e[2J");
				fruit=generation_suivante(terrain,snake,fruit,d,&pas,&gameover);
				afficherserpent(snake);
				int longueur=serpentlongueur(snake);
				printf("longueur:%d,socre:%d\n",longueur,longueur-1);
				afficher_terrain(terrain);
				pas--;
				sleep(2);
			}
		}
		choisir_direction(&d,&gameover);
	}
	libererNoeud(fruit);
	libererserpent(snake);
	liberer_terrain(terrain);
	return EXIT_SUCCESS;
}
