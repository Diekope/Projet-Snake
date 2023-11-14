#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Definition de la structure des statistiques du joueur

typedef struct dons{
	char nom[25];
	int nbr_parties;
	int meilleur_score;
}donnees;

//Definition de la structure pour les statistiques générales

typedef struct stat{
	int nbr_parties_jouees;
}stats;

//------------------Definition des fonctions------------------

void afficher_joueur();
int recherche_joueur(char identifiant[25]);
void modifier_joueur(char identifiant[25],donnees joueur_a_remplacer);
stats recherche_scoreboard();
donnees recup_joueur(char identifiant[25]);
void jeu(char identifiant[25]);

//------------Fonction Principale----------------

int main(){
	char id[25];
	donnees joueur;
	printf("Donner identifiant : ");
	fscanf(stdin,"%s",id);
	printf("Bienvenue %s",id);
	if (recherche_joueur(id)==0){
		FILE *fichier=fopen("./donnes.txt","a+");
		strcpy(joueur.nom,id);
		joueur.nbr_parties=0;
		joueur.meilleur_score=0;
		fwrite(&joueur,sizeof(joueur),1,fichier);
		fclose(fichier);
	}
	printf("\n");
	jeu(id);
}

//--------------------Fonction affichage du joueur----------------------

void afficher_joueur(){
	donnees joueur;
	FILE *fichier=fopen("./donnes.txt","r");
	while (fread(&joueur,sizeof(joueur),1,fichier)==1){
		printf("\n%s %d",joueur.nom,joueur.nbr_parties);
	}
	fclose(fichier);
}

//------------------Fonction de recherche du joueur-----------------------
//Cette fonction retourne 0 si le joueur n'existe pas et 1 si le joueur existe

int recherche_joueur(char identifiant[25]){
	donnees joueur;
	FILE *fichier=fopen("./donnes.txt","a+");
	while (fread(&joueur,sizeof(joueur),1,fichier)==1){
		if (strcmp(joueur.nom,identifiant)==0){
			fclose(fichier);
			return 1;
		}
	}
	fclose(fichier);
	return 0;
}

//--------------------Fonction modification du joueur------------------------
/*Cette fonction modifile la structure du joueur et modifie le fichier
dans lequel elle est contenue par l'utilisation d'un fichier temporaire
avant de la replacer dans son fichier initial "donnes.txt"*/

void modifier_joueur(char identifiant[25],donnees joueur_a_remplacer){
	donnees joueur;
	donnees joueur_trouve;
	FILE *fichier=fopen("./donnes.txt","r");
	FILE *fichier2=fopen("./temp.txt","w+");
	while (fread(&joueur,sizeof(joueur),1,fichier)==1){
		if (strcmp(identifiant,joueur.nom)==0){
			joueur_trouve=joueur_a_remplacer;
		}
		else{
			fwrite(&joueur,sizeof(joueur),1,fichier2);
		}
	}
	fwrite(&joueur_trouve,sizeof(joueur_trouve),1,fichier2);
	fclose(fichier2);
	fclose(fichier);
	fichier=fopen("./donnes.txt","w+");
	fichier2=fopen("./temp.txt","r");
	while (fread(&joueur,sizeof(joueur),1,fichier2)==1){
		fwrite(&joueur,sizeof(joueur),1,fichier);
	}
	fclose(fichier);
	fclose(fichier2);
}

//------------Recherche existence des statistiques générales----------------
/*Cette fonction recherche l'existence des statistiques générales
et elle retourne la structure du scoreboard*/

stats recherche_scoreboard(){
	stats sb={0};
	FILE *fichier=fopen("./statg.txt","r");
	if (fichier==NULL){
		fichier=fopen("./statg.txt","w");
		fwrite(&sb,sizeof(sb),1,fichier);
		fclose(fichier);
	}
	else{
		fread(&sb,sizeof(sb),1,fichier);
		fclose(fichier);
	}
	return sb;
}

//------------Fonction de récupération du joueur-----------------------------
/*Cette fonction est exécutée après la vérification de présence de la structure du joueur
dans le fichier et renverra la structure du joueur*/

donnees recup_joueur(char identifiant[25]){
	donnees joueur;
	FILE *fichier=fopen("./donnes.txt","r");
	while (strcmp(joueur.nom,identifiant)!=0){
		fread(&joueur,sizeof(joueur),1,fichier);
	}
	return joueur;
}

//----------------Fonction Jeu---------------
/*On récupère ici les statistiques du joueur et les statistiques
générales et contient le jeu*/

void jeu(char identifiant[25]){
	stats sb;
	char reponse='o';
	donnees joueur=recup_joueur(identifiant);
	sb=recherche_scoreboard();
	while (reponse=='o'){
		joueur.nbr_parties++;
		sb.nbr_parties_jouees++;
		printf("\nFin de partie");
		printf("\nStatistiques %s : ",identifiant);
		printf("\nNombre de parties jouées : %d",joueur.nbr_parties);
		printf("\nMeilleur Score : %d",joueur.meilleur_score);

		printf("\nSouhaitez vous rejouer ? (o/n) : ");
		fscanf(stdin,"%c",&reponse);
		while (reponse=='\n'){
			fscanf(stdin,"%c",&reponse);
		}
		while ((reponse != 'o') && (reponse!='n')){
			printf("\nDonner une reponse correct : ");
			fscanf(stdin,"%c",&reponse);
			while (reponse=='\n'){
				fscanf(stdin,"%c",&reponse);
			}
		}
		if (reponse=='n'){
			printf("\nFin du jeu");
			break;
		}

	}

		printf("\nStatistiques générales : ");
		printf("\nNombre de parties jouées : %d\n",sb.nbr_parties_jouees);
		modifier_joueur(identifiant,joueur);
		FILE *fichier=fopen("./statg.txt","w");
		fwrite(&sb,sizeof(sb),1,fichier);
		fclose(fichier);
}