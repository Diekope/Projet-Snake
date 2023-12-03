#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include "../header/jeu.h"
#define general "data/general/info.txt"
/* infovide: louer une espace pour stocker les informations de utilisteur : [nom_user, mdp]*/
char **infovide(){
	char **info=(char **)malloc(2 * sizeof(char*)); // Allocation pour deux pointeurs
	for(int i=0;i<2;i++){
    		info[i] = malloc(10 * sizeof(char));
    	}
    	return info;
}
/**/
void libererinfo(char **info){
	for(int i=0;i<2;i++){
    		free(info[i]);
    	}
    	free(info);
}
void connexion(int *nb_partie_gagnee, int *nb_partie_gagnee_generale,char **info){
	FILE *test=fopen(general,"r");
	if(test==NULL){
		printf("base general introvable\n");
		test=fopen(general,"w");
		fprintf(test,"%d",0);
		printf("creation base general succès\n");
	}
	else{
		fscanf(test,"%d",nb_partie_gagnee_generale);
		printf("récupération info general avec succès\n");
	}
	printf("Qui vous êtes?\n");
	scanf("%s",info[0]);
	while(strlen(info[0])<6 || strlen(info[0])>10){
		printf("message erreur : la longueur de votre nom ne peut pas être supérieur de 9 et inférieur de 6\nQui vous êtes?\n");
		scanf("%s",info[0]);
	}
	char path_joueur[40]="data/player/";
	strncat(path_joueur,info[0],sizeof(path_joueur) - strlen(path_joueur) - 1);
	printf("%s\n",path_joueur);
	test=fopen(path_joueur,"r");
	
	if(test==NULL){
		printf("vous êtes nouveau.\nveuillez configurer le mot de passe de votre compte: ");
		scanf("%s",info[1]);
		while(strlen(info[1])<6 || strlen(info[1])>10){
			printf("la longueur de votre mot de passe droit avoir au moins 6 et au maximum 9 caractères\nnouveau saisit:");
			scanf("%s",info[1]);
		}
		test=fopen(path_joueur,"w");
		fwrite(info[1], sizeof(char), strlen(info[1]), test);
		fwrite(nb_partie_gagnee,sizeof(int),1,test);
		printf("creation de compte succès\nbienvenu joueur %s\n",info[0]);
		
	}
	else{
		fscanf(test,"%d",nb_partie_gagnee);
		fscanf(test,"%s",info[1]);
		printf("%s\n",info[1]);
		char test_mdp[10];
		int chance=2;
		printf("joueur trouvé\nentrez votre mot de passe: ");
		scanf("%s",test_mdp);
		while(strcmp(test_mdp,info[1])!=0 && chance>0){
			printf("il vous reste %d chances\n",chance);
			scanf("%s",test_mdp);
		}
		if(strcmp(test_mdp,info[1])==0){
			printf("connexion succès\nbienvenu %s\n",info[0]);
		}
		else{
			printf("connexion échec\n");
		}
		
	
	}
	fclose(test);
}
Noeud* generation_suivante(carte* terrain,serpent *snake,Noeud* fruit,direction d,int *pas,int *gameover){
	if(d!=init){
		clear(terrain);
		Noeud *tete=iemeNoeud(snake,1);
		int longueur=serpentlongueur(snake);
		int nouveau_x=valeurx(tete),nouveau_y=valeurY(tete);
		switch(d){
			case droite:
				nouveau_x++;
				break;
			case gauche:
				nouveau_x--;
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
		if(valeurx(fruit)==nouveau_x && valeurY(fruit)==nouveau_y){
			for(int i=1;i<=longueur;i++){
				iemeNoeud(snake,i)->val++;
				ajouter_terrain(terrain,iemeNoeud(snake,i),0);

			}
			inserer(snake,1,0,valeurx(fruit),valeurY(fruit));
			ajouter_terrain(terrain,iemeNoeud(snake,1),0);
			*pas=10;
			libererNoeud(fruit);
			fruit=generer_fruit(terrain,snake);
		}
		else{
			if(longueur>1){
				for(int i=longueur;i>=2;i--){
					changercoord(iemeNoeud(snake,i),valeurx(iemeNoeud(snake,i-1)),valeurY(iemeNoeud(snake,i-1)));
					ajouter_terrain(terrain,iemeNoeud(snake,i),0);
					
				}
			}
			changercoord(tete,nouveau_x,nouveau_y);
			ajouter_terrain(terrain,fruit,1);
			ajouter_terrain(terrain,iemeNoeud(snake,1),0);
			Noeud *test=test_toucher(terrain,snake);
			if(test!=NULL){
				ajouter_terrain(terrain,test,3);
				*gameover=1;
			}
		}
		
	}
	return fruit;
}
Noeud* test_toucher(carte *terrain,serpent *snake){
	int longueur=serpentlongueur(snake),colonne=nbcolonne(terrain),ligne=nbligne(terrain);
	Noeud *tete=iemeNoeud(snake,1);
	if(valeurx(tete)==1 || valeurx(tete)==colonne || valeurY(tete)==ligne-1 || valeurY(tete)==0){
		return tete;
	}
	if(longueur>1){
		for(int i=2;i<=longueur;i++){
			Noeud *corps=iemeNoeud(snake,i);
			if(valeurx(corps)==valeurx(tete) && valeurY(corps)==valeurY(tete)){
				printf("perdu!\n");
				return corps;
			}
			
		}
	}
	return NULL;
	
}
void ajouter_terrain(carte* terrain,Noeud *n,int choix){
	int x=valeurx(n),y=valeurY(n),val=contenu(n);
	if(choix<2){
		ajouterelt(terrain,x,y,alphabet(choix,val));
	}
	else{
		ajouterelt(terrain,x,y,'*');
	}
}
Noeud* generer_fruit(carte *terrain,serpent *snake){
	
	srand(time(NULL));
	int valeur=rand()%26,ligne=nbligne(terrain),colonne=nbcolonne(terrain);
	int x=rand()%(colonne-2)+1,y=rand()%(ligne-2)+1;
	while(valxy(terrain,x,y)!=' '){
		x=rand()%(colonne-2)+1;
		y=rand()%(ligne-2)+1;
		
	}
	Noeud *fruit=creerNoeud(valeur,x,y,NULL);
	ajouter_terrain(terrain,fruit,1);
	return fruit;
	
}
Noeud* initialisation(carte *terrain,serpent *snake,direction *d,int *pas){
	/*cas quitte normal*/
	*pas=10;
	int x=nbcolonne(terrain)/2,y=nbligne(terrain)/2;
	inserer(snake,1,0,x,y);
	ajouter_terrain(terrain,iemeNoeud(snake,1),0);
	Noeud *fruit=generer_fruit(terrain,snake);
	return fruit;
	
}
char alphabet(int choix, int val){
	char *alphab="ZYXWVUTSRQPONMLKJIHGFEDCBA";
	if(choix==0){
		return tolower(alphab[val%26]);
	}
	return alphab[val%26];
}
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
void clear(carte *terrain){
	int x,y,ligne=nbligne(terrain),colonne=nbcolonne(terrain);
	for(y=1;y<ligne-1;y++){
		for(x=2;x<colonne;x++){
			ajouterelt(terrain,x,y,' ');
		}
	}
}
void choisir_direction(direction *d,int *gameover){
	char choix=getchar();
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
				*d=init;
				*gameover=1;
				break;
			default:
				break;
		}
	}
}

