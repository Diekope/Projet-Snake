#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include "../header/jeu.h"
#define taille 100

/*lecture de la carte

si la variable carte de la structure player est vide ->demande de en l'entrée la carte

sinon on récupère la carte s'il existe sinon annonce erreur et demande à utilisateur de recommencer une partie et la nouvelle carte
----------------------------------------------------------------------------------------------
!!! précaution: les cartes devont être créées et restées dans le répertoire  ../data/map/ !!!!

*/

carte *recupere_carte(joueur *player){

	carte *terrain;

	char path_m[taille]="../data/map/";

	printf("rep:%s\n",player->carte);

	if(strlen(player->carte)==0){

		printf("donnez le nom de la carte taille max :20\n");

		scanf("%s",player->carte);

		strcat(path_m,player->carte);

		terrain= lire_terrain(path_m);

		while(terrain==NULL){

			printf("la carte n'existe pas dans data/map veuillez créer le map ou saisir un autre\n");

			scanf("%s",player->carte);

			char path_m[taille]="../data/map/";

			strcat(path_m,player->carte);

			terrain= lire_terrain(path_m);

		}

	}

	else{

		strcat(path_m,player->carte);

		terrain= lire_terrain(path_m);

		if(terrain ==NULL){

			printf("carte perdu!\nveuillez recommencer une autre partie!\n");

			char path_m[taille]="../data/map/";

			printf("donnez le nom de la carte taille max :20\n");

			scanf("%s",player->carte);

			player->pas=10;

			strcat(path_m,player->carte);

			terrain= lire_terrain(path_m);

			while(terrain==NULL){

				printf("la carte n'existe pas dans data/map veuillez créer le map ou saisir un autre\n");

				scanf("%s",player->carte);

				char path_m[taille]="../data/map/";

				strcat(path_m,player->carte);

				terrain= lire_terrain(path_m);

			}

		}

	}

	return terrain;

}

/*enregistrer la structure joueur dans un fichier ../data/player/nom_de_utilisateur_saisit*/

void enregistrer_joueur(joueur *player,char nom[20]){

	char path[taille]="../data/player/";

	strcat(path,nom);

	FILE* fp=fopen(path,"w");

	fwrite(player, sizeof(joueur), 1,fp);

	fclose(fp);

};

/*créer une structure joueur vide*/

joueur joueur_vide(){

	joueur player;

	player.score=0;

	player.carte[0]='\0';

	player.pas=10;

	player.d=0;

	player.fx=0;

	player.fy=0;

	player.fval=0;

	return player;

}

/*s'il existe déjà un fichier avec nom = le nom saisit par l'utilisateur 

alors on récupère la structure dans ce fichier et demande s'il veut reprendre la partie(s'il existe une partie

enregistré) sinon on réinitialise player.carte en une chaine de caractère vide

s'il n'exite pas un fichier avec nom = le nom saisit par l'utilisateur alors créer un fichier avec le nom saisit

*/

joueur connexion(char nom[20]){

	/*0:jeu commence normalement sinon on lire la partie*/

	joueur player=joueur_vide();

	char path_p[taille]="../data/player/";

	

	printf("%s %ld\n",nom,strlen(nom));

	strcat(path_p,nom);

	

	FILE* fp_joueur=fopen(path_p,"rb");

	if(fp_joueur==NULL){

		int choix=0;

		printf("Vous etes nouveau\n");

		fp_joueur=fopen(path_p,"wb");

		printf("\nBienvenue %s\n",nom);

	}

	else{

		fread(&player,sizeof(joueur), 1,fp_joueur);

		printf("Bonjour %s\n",nom);

	}

	fclose(fp_joueur);

	return player;

	

	

}

/*test l'état de fichier serpent ../data/sauvegarde/nom_utilisateur_saisit.txt

s'il existe une partie en mémoire alors on fait appel à la fonction recup_serpent

et on supprime le fichier d'après la lecture

*/

serpent *lire_serpent(char nom[20],joueur player){

	serpent *snake=serpentvide();

	char path[taille]="../data/sauvegarde/";

	if (player.d!=0){

		strcat(path,nom);

		strcat(path,".txt");

		printf("%s\n",path);

		FILE *fp=fopen(path,"r");

		if (fp==NULL){

			perror("fopen fichier serpent");

		}

		

		snake=recup_serpent(fp,snake);

		fclose(fp);

		//remove(path);

	}

	printf("touch\n");

	return snake;

}

/*lire le serpent dans le fichier

exemple de fichier serpent:

x

y

x1

y1

.

.

.

xn

yn

avec x et y des entiers naturels

*/

serpent *recup_serpent(FILE *fp,serpent *snake){

	int x,y,sl;

	int val=0;

	int nb_noeud=1;

	

	if (feof(fp)){

		printf("Fichier vide");

	}

	fscanf(fp,"%d",&sl);

	for (int i=0;i<sl;i++){

		fscanf(fp,"%d",&x);

		fscanf(fp,"%d",&y);

		inserer(snake,nb_noeud, val,x,y);

		printf("x=%d y=%d\n",x,y);

		val++;

		nb_noeud++;

		}

	return snake;

}

/*enregistrer le serpent dans un fichier ../data/sauvegarde/nom_utilisateur_saisit.txt en cas quitte avec q*/

void sauvegarde_serpent(char nom[20],serpent *snake){

	char path[taille]="../data/sauvegarde/";

	strcat(path,nom);

	strcat(path,".txt");

	FILE *fp=fopen(path,"w");

	Noeud *actuel=snake->sentAvt;

	fprintf(fp,"%d\n",serpentlongueur(snake));

	while(actuel!=NULL){

		if(estSent(actuel)==0){

			fprintf(fp,"%d\n",actuel->x);

			if(actuel->suiv!=NULL){

				fprintf(fp,"%d\n",actuel->y);

			}

			else{

				fprintf(fp,"%d",actuel->y);

			}

		};

		actuel=actuel->suiv;

	}

	fclose(fp);

}

/*elle genere la generation suivante*/

Noeud* generation_suivante(carte* terrain,serpent *snake,Noeud* fruit,direction d,int *pas,int *gameover,joueur *player){

	if(d!=init){/*si le serpent n'est pas immobile*/

		clear(terrain);/*effacer tous les elements de la carte*/

		Noeud *tete=iemeNoeud(snake,1);/*recupere la tête de serpent*/

		int longueur=serpentlongueur(snake);/*la longueur de serpent*/

		int nouveau_x=valeurx(tete),nouveau_y=valeurY(tete);/*recupere les cordonnees de la tete*/

		switch(d){

			case droite:/*si va droite*/

				nouveau_x++;/*(x++,y)*/

				break;

			case gauche:/*si va gauche*/

				nouveau_x--;/*(x--,y)*/

				break;

			case haute:

				nouveau_y--;

				break;

			case bas:

				nouveau_y++;

				break;

			default:

				break;

		}

		/*recupere x,y de fruit: si les nouvelles cordonnees de la tete=les cordonnees de fruit*/

		if(valeurx(fruit)==nouveau_x && valeurY(fruit)==nouveau_y){

			for(int i=1;i<=longueur;i++){/*la valeur de chaque noeud +1*/

				iemeNoeud(snake,i)->val++;

				ajouter_terrain(terrain,iemeNoeud(snake,i),0);/*on dessine ce noeud dans la carte*/



			}

			player->score++;

			inserer(snake,1,0,valeurx(fruit),valeurY(fruit));/*on insere la nouvelle tete*/

			ajouter_terrain(terrain,iemeNoeud(snake,1),0);/*on dessine la tete*/

			*pas=10;/*reinitialise le pas*/

			libererNoeud(fruit);/*libere espace*/

			fruit=generer_fruit(terrain,snake);/*generer un nouveau*/

		}

		else{

			/*cas normal*/

			if(longueur>1){/*si la taille de serpent >1*/

			/*de queue à 2eme noeud : les cordonnees i = les cordonnées de noeud i-1*/

				for(int i=longueur;i>=2;i--){

					changercoord(iemeNoeud(snake,i),valeurx(iemeNoeud(snake,i-1)),valeurY(iemeNoeud(snake,i-1)));

					ajouter_terrain(terrain,iemeNoeud(snake,i),0);/*dessine dans la carte*/

					

				}

			}

			/*on change les cordonnees de la tete*/

			changercoord(tete,nouveau_x,nouveau_y);

			/*on redessine le fruit et la tete*/

			ajouter_terrain(terrain,fruit,1);

			ajouter_terrain(terrain,iemeNoeud(snake,1),0);

			Noeud *test=test_toucher(terrain,snake);/*test si la tete a touche le mur ou son corps*/

			if(test!=NULL){/*si la test retourne pas nulle*/

				ajouter_terrain(terrain,test,3);/*alors on change intersection en * */

				*gameover=1;/*jeu arrete*/

			}

		}

		

	}

	return fruit;/*retourne le fruit*/

}

/*test si la tete as touche le corps ou le mur*/

Noeud* test_toucher(carte *terrain,serpent *snake){

	int longueur=serpentlongueur(snake),colonne=nbcolonne(terrain),ligne=nbligne(terrain);

	Noeud *tete=iemeNoeud(snake,1);

	/*si la tete touche le mur*/

	if(valeurx(tete)==0 || valeurx(tete)==colonne-1 || valeurY(tete)==ligne-1 || valeurY(tete)==0){

		return tete;/*on retourne la tete*/

	}

	if(longueur>1){/*si la taille de serpent >1*/

		for(int i=2;i<=longueur;i++){/*on parcours le serpent*/

			Noeud *corps=iemeNoeud(snake,i);/*on recupere ieme noeud*/

			/*si ses cordonnees = les cordonnees de la tete*/

			if(valeurx(corps)==valeurx(tete) && valeurY(corps)==valeurY(tete)){

				printf("perdu!\n");

				return corps;/*retourne le corps*/

			}

			

		}

	}

	return NULL;/*si pas touche alors NULL*/

	

}

/*ajouter des choses dans le terrain*/

void ajouter_terrain(carte* terrain,Noeud *n,int choix){

	int x=valeurx(n),y=valeurY(n),val=contenu(n);

	if(choix<2){

		ajouterelt(terrain,x,y,alphabet(choix,val));/*ajoute le noeud dans le terrain, si choix =0 alors c'est un noeud de serpent sinon fruit*/

	}

	else{

		ajouterelt(terrain,x,y,'*');/*sinon ajoute * */

	}

}

/*générer le fruit*/

Noeud* generer_fruit(carte *terrain,serpent *snake){

	/*genere fruit*/

	srand(time(NULL));

	int valeur=rand()%26,ligne=nbligne(terrain),colonne=nbcolonne(terrain);

	int x=rand()%(colonne-2)+1,y=rand()%(ligne-2)+1;

	/*tant que la position (x,y) n'est pas vide */

	while(valxy(terrain,x,y)!=' '){

		x=rand()%(colonne-2)+1;

		y=rand()%(ligne-2)+1;

		

	}

	/*cree le noeud fruit*/

	Noeud *fruit=creerNoeud(valeur,x,y,NULL);

	/*dessine dans le terrain*/

	ajouter_terrain(terrain,fruit,1);

	return fruit;

	

}

/*initialise le jeu*/

Noeud* initialisation(carte *terrain,serpent *snake,direction *d,int *pas,joueur player){

	/*cas quitte normal*/

	Noeud *fruit;

	/*cas normal: nouvelle partie*/

	if(player.d ==0){

		*d=init;

		*pas=10;

		int x=nbcolonne(terrain)/2,y=nbligne(terrain)/2;

		inserer(snake,1,0,x,y);

		ajouter_terrain(terrain,iemeNoeud(snake,1),0);

		fruit=generer_fruit(terrain,snake);

	}

	else{/*cas lecture: récupère la partie précédante*/

		*pas=player.pas;

		fruit=creerNoeud(player.fval,player.fx,player.fy, NULL);

		ajouter_terrain(terrain,fruit,1);

		*d=player.d;

		

	}

	return fruit;

	

}

/*choisir la lettre correspond à val*/

char alphabet(int choix, int val){

	char *alphab="ZYXWVUTSRQPONMLKJIHGFEDCBA";

	if(choix==0){/*choix 0 alors minuscule*/

		return tolower(alphab[val%26]);

	}

	return alphab[val%26];/*majuscule*/

}

/*test si l'utilisateur as fait de saisit ou non dans le terminal*/

int kbhit(void){

	struct timeval intervalle_t;

	fd_set fds;

	intervalle_t.tv_sec=0;

	intervalle_t.tv_usec=0;

	FD_ZERO(&fds);

	FD_SET(STDIN_FILENO,&fds);

	select(STDIN_FILENO+1,&fds,NULL,NULL,&intervalle_t);

	return FD_ISSET(STDIN_FILENO,&fds);

}

/*efface tous sauf les murs*/

void clear(carte *terrain){

	int x,y,ligne=nbligne(terrain),colonne=nbcolonne(terrain);

	for(y=1;y<ligne-1;y++){

		for(x=1;x<colonne-1;x++){

			ajouterelt(terrain,x,y,' ');

		}

	}

}

/*recupere le valeur saisit par l'utilisateur*/

void choisir_direction(direction *d,int *gameover){

	char choix=getchar();

	/*si pas de contre sens*/

	if(!((choix=='o' && *d==2) || (choix =='l' && *d==1) ||(choix=='m' && *d==3) ||(choix=='k' && *d==4))){

		switch(choix){

			case 'o':

				*d=haute;

				break;

			case 'l':

				*d=bas;

				break;

			case 'm':

				*d=droite;

				break;

			case 'k':

				*d=gauche;

				break;

			case 'q':

				*gameover=2;

				break;

			default:

				break;

		}

	}

}