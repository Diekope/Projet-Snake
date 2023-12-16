#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../header/jeu.h"
#define taille 100
int main(){
	/*initialisation*/
	direction d=init;
	int pas,gameover=0;
	char nom[20];
	printf("entrez votre identifiant:\n");
	scanf("%s",nom);
	joueur player=connexion(nom);
	printf("%s\n",player.carte);
	carte *terrain=recupere_carte(&player);
	serpent *snake=lire_serpent(nom,player);
	Noeud* fruit= initialisation(terrain,snake,&d,&pas,player);
	/*1er affichage*/
	afficher_terrain(terrain);
	int choix;
	while(gameover==0){
		while(!kbhit()){
			if(gameover==1){/*cas serpent est mort*/
				printf("vous voulez commencer une nouvelle partie?\n1.oui\n2.non\n");
				scanf("%d",&choix);
				while(choix !=1 && choix!=2){
					printf("valeur inconnu veuillez refaire le saisit\n1.oui\n2.non\n");
					scanf("%d",&choix);
				}
				if (choix ==1){
					printf("ok\n");
					gameover=0;
					player.d=0;
					libererserpent(snake);
					snake=lire_serpent(nom,player);
					liberer_terrain(terrain);
					terrain=recupere_carte(&player);
					fruit=initialisation(terrain,snake,&d,&pas,player);
					afficher_terrain(terrain);
				}
				else{
					player.d=0;
					player.carte[0]='\0';
					printf("à la prochaine\n");
					break;
				}
			}
			else{
				if(d!=init){
					if(pas==0){
						pas=10;
						libererNoeud(fruit);
						fruit=generer_fruit(terrain,snake);
					}
					printf("\e[H\e[2J");
					
					fruit=generation_suivante(terrain,snake,fruit,d,&pas,&gameover,&player);
					afficherserpent(snake);
					int longueur=serpentlongueur(snake);
					printf("longueur:%d,socre:%d\n",longueur,longueur-1);
					afficher_terrain(terrain);
					pas--;
					sleep(2);
				}
			}
		}
		choisir_direction(&d,&gameover);/*récupère la valeur entrée*/
		if(gameover==2){/*cas quitte avec q*/
				switch(d){
				 case init:
				 	break;
				 case haute:
				 	player.d=1;
				 	break;
				 case bas:
				 	player.d=2;
				 	break;
				 case gauche:
				 	player.d=3;
				 	break;
				 case droite:
				 	player.d=4;
				 	break;
				 default:
				 	break;
				}
				printf("d %d\n",player.d);
				player.fx=valeurx(fruit);
				player.fy=valeurY(fruit);
				player.fval=contenu(fruit);
				player.pas=pas;
				sauvegarde_serpent(nom,snake);
				break;
			}
		if(gameover==3){/*cas relancer jeu*/
			gameover=0;
			player.d=0;
			libererserpent(snake);
			snake=lire_serpent(nom,player);
			terrain=recupere_carte(&player);
			fruit=initialisation(terrain,snake,&d,&pas,player);
			afficher_terrain(terrain);
		}
	}
	/*libèrer tous d'après le sauvegarde de player*/
	enregistrer_joueur(&player,nom);
	libererNoeud(fruit);
	libererserpent(snake);
	liberer_terrain(terrain);
	return EXIT_SUCCESS;
}
