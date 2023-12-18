#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include "../header/jeu.h"

#define taille 100

int main(){

	/*initialisation*/

	direction d=init;

	int pas,gameover=0,choix2;

	char nom[20];

	printf("entrez votre identifiant:\n");

	scanf("%s",nom);

	joueur player=connexion(nom);

	

	if(strlen(player.carte)!=0){

			char path_s[taille]="../data/sauvegarde/";

			int choose=0;

			while(choose!=1 && choose!=2 && choose!=3){

				printf("\n1)Charger partie");

				printf("\n2)Nouvelle partie");

				printf("\n3)Quitter\nChoix:");

				scanf("%d",&choose);

			}

			if(choose==1){

				strcat(path_s,nom);

				strcat(path_s,".txt");

				FILE *fp_joueur=fopen(path_s,"r");

				if(fp_joueur==NULL){

					printf("memoire perdu!\nvous devez recommencer\n");

					player.carte[0]='\0';

					player.d=0;

				}

				else{

					printf("%ld\n",strlen(player.carte));

					printf("bien reçu\n");

				}

				fclose(fp_joueur);

			}

			else if (choose==2){

				player.carte[0]='\0';

				player.d=0;

			}

			else{

				return EXIT_SUCCESS;

			}

	}

	else{

		int choose=0;

			while(choose!=1 && choose!=2 && choose!=3){

				printf("\n1)Nouvelle partie");

				printf("\n2)Quitter\nChoix: ");

				scanf("%d",&choose);

			}

			if (choose==1){

				player.carte[0]='\0';

				player.d=0;

			}

			else{

				return EXIT_SUCCESS;

			}

	}

	

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

					libererNoeud(fruit);

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

			choix2=0;

			printf("1)Sauvegarder et quitter\n2)Recommencer à zéro\n3)Nouvelle carte\n4)Sortir du jeu");

			scanf("%d",&choix2);

			if (choix2==1){

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

			else if(choix2==2){/*cas relancer jeu*/

				gameover=0;

				player.d=0;

				libererserpent(snake);

				liberer_terrain(terrain);

				libererNoeud(fruit);

				snake=lire_serpent(nom,player);

				terrain=recupere_carte(&player);

				fruit=initialisation(terrain,snake,&d,&pas,player);

				afficher_terrain(terrain);

			}

			else if (choix2==3){

				gameover=0;

				player.d=0;

				player.carte[0]='\0';

				libererserpent(snake);

				liberer_terrain(terrain);

				libererNoeud(fruit);

				snake=lire_serpent(nom,player);

				terrain=recupere_carte(&player);

				fruit=initialisation(terrain,snake,&d,&pas,player);

				afficher_terrain(terrain);

			}

			else if(choix2==4){

				printf("\nFin de la partie\n");

				break;

			}

		}

	}

	/*libèrer tous d'après le sauvegarde de player*/

	enregistrer_joueur(&player,nom);

	libererNoeud(fruit);

	libererserpent(snake);

	liberer_terrain(terrain);

	return EXIT_SUCCESS;

}