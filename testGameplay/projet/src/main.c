#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../header/jeu.h"
int main(){
	/*initialisation*/
	direction d=init;
	int pas,gameover=0;
	carte* terrain= lire_terrain();
	serpent *snake=serpentvide();
	Noeud* fruit= initialisation(terrain,snake,&d,&pas);
	/*1er affichage*/
	afficher_terrain(terrain);
	/*si etat jeu est normal*/
	while(gameover==0){
		while(!kbhit()){/*saisit ?*/
			if(gameover==1){/*quitte la boucle*/
				break;
			}
			if(d!=init){/*si pas reste en immobile*/
				if(pas==0){/*s'il reste 0 pas*/
					pas=10;/*reinitialise le pas en 10*/
					libererNoeud(fruit);/*libere ancien fruit*/
					fruit=generer_fruit(terrain,snake);/*genere un nouveau*/
				}
				printf("\e[H\e[2J");/*efface le terminal*/
				/*genere puis dessine le nouveau serpent dans la carte*/
				fruit=generation_suivante(terrain,snake,fruit,d,&pas,&gameover);
				afficherserpent(snake);/*affiche la liste chainee(juste pour le test, possible de l'enleve d'apres)*/
				int longueur=serpentlongueur(snake);/*la taille de serpent*/
				printf("longueur:%d,socre:%d\n",longueur,longueur-1);/*affiche sa taille et son score*/
				afficher_terrain(terrain);/*affiche la carte*/
				pas--;
				sleep(2);
			}
		}
		choisir_direction(&d,&gameover);/*recupere la valeur saisie*/
	}
	libererNoeud(fruit);/*libere tous avant de finir*/
	libererserpent(snake);
	liberer_terrain(terrain);
	return EXIT_SUCCESS;
}
