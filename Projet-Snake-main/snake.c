#include <stdio.h>
#include <stdlib.h>
/*#include <SDL2/SDL.h>*/
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <termios.h>
int fruit_x,fruit_y;
int size=20;
int erreur=0, stop=0,longueur=0;
char keyboard;
int fruit_existe=0;
typedef struct matrice
{
    int ligne;
    int colonne;
    char **elt;
} matrice;
typedef struct  corps_
{
    int x;
    int y;
    int val;
}corps;
typedef enum direction_{
    init=0,
    o,
    k,
    m,
    l,
    q
}direction;
corps snake[25];
corps head;
matrice *map;
direction d=init;
void matrice_create(int l,int c){
    int i;
    map=(matrice *)malloc(sizeof(matrice));
    map->colonne=c;
    map->ligne=l;
    map->elt=(char**)malloc(l*sizeof(char*));
    for(i=0;i<l;i++){
        map->elt[i]=(char*)malloc(c*sizeof(char));
    }
    
}
void free_matrice(){
    int i;
    for(i=0;i< map->ligne;i++){
        free(map->elt[i]);
    }
    free(map->elt);
    free(map);
}
void read_carte(FILE *file){
    int c,l;
    char cara;
    if(file==NULL){
    	stop=1;
    	erreur=1;
    	perror("fopen:fichier non existe\n");
    	return;
    }
    fscanf(file,"%d",&l);
    fscanf(file,"%d",&c);
    matrice_create(l,c);
    fscanf(file,"%c",&cara);
    for(l=0;l< map->ligne;l++){
        for(c=0;c<= map->colonne;c++){
            fscanf(file,"%c",&cara);
            if(cara!='\n'){
                map->elt[l][c]=cara;
            }
        }
       
    }
    fclose(file);
}
void print_map(){
    int l,c;
    printf("\n");
    for(l=0;l< map->ligne;l++){
        for(c=0;c< map->colonne;c++){
            printf("%c",map->elt[l][c]);
        }
        printf("\n");
    }
}
char choose(int val,int ch){
    char *alphab="zyxwvutsrqponmlkjihgfedcba";
    if(ch==1){
            return alphab[val];}
    else{
            return toupper(alphab[val]);
        }
}
int test_touch(){
	if(longueur>0){
		for(int i=0;i<longueur;i++){
			if(snake[i].x==head.x && snake[i].y==head.y){
				return 1;
			}
		}
	}
	return 0;
}
char *set_something_in_map(int y,int x,char val,int ch){
    //printf("elt[%d][%d]=%c mode:%d\n",y,x,map->elt[y][x],ch);
    if(y==head.y && x==head.x){
    	if(test_touch()==1){
    		map->elt[y][x]='+';
    		stop=1;
    		return "perdu!";
    	};
    }
    if(y>0 && x>0 && y<=map->ligne-2 && x<=map->colonne-2 && (map->elt[y][x]==' ' || ch==-1 || (ch==1 && y==head.y && x==head.x && x==fruit_x && y==fruit_y))){
    	if(ch<26 && ch>-1){
        	map->elt[y][x]=choose(val,ch);
        	if(ch==0){
        		fruit_existe=1;	
        	}
        }
        else{
        	map->elt[y][x]=' ';
        }
    }
    else if(y==0 || x==0 || y == map->ligne-1 || x==map->colonne-1 || (map->elt[y][x]!=' ' && y!=head.y && x!=head.x)){
        map->elt[y][x]='+';
        stop=1;
        
        return "perdu!";
    }
    else{
        return "error!";
    }
    return "succes";
}
void fruit(){
    srand(time(NULL));
    if(fruit_existe==0){
    	fruit_x=rand()% (map->colonne -2)+1;
    	fruit_y=rand()% (map->ligne -2)+1;
	    while(map->elt[fruit_y][fruit_x]!=' '){
	    	fruit_x=rand()% (map->colonne-2)+1;
	    	fruit_y=rand()% (map->ligne -2)+1;
	    }
    set_something_in_map(fruit_y,fruit_x,longueur,0);
    fruit_existe=1;
    }
}
void choose_direction(char keyboard){
	switch(keyboard){
		case 'o':
			d=o;
			break;
		case 'k':
			d=k;
			break;
		case 'm':
			d=m;
			break;
		case 'l':
			d=l;
			break;
		case 'q':
			d=q;
			stop=1;
			break;
		default:
			break;
	}
}

void entree(){
    keyboard=getchar();
    if(!((keyboard=='o'&& d==4) || (keyboard=='k' && d==3) || (keyboard=='m' && d==2)|| (keyboard=='l' && d==1))){
    		choose_direction(keyboard);
    }
}
void initialization(){
	head.x=map->colonne/2;
	head.y=map->ligne/2;
	head.val=0;
	set_something_in_map(head.y,head.x,head.val,1);
}
void clear_(int x,int y){
	set_something_in_map(y,x,26,-1);
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
void move_snake(){
	int i;
	clear_(head.x,head.y);
	corps last;
	if(longueur==0){
		last.x=head.x;
		last.y=head.y;
		last.val=longueur+1;
	}
	else{
		last.x=snake[longueur-1].x;
		last.y=snake[longueur-1].y;
		last.val=longueur+1;
	}
	
	if(longueur>0){
		clear_(snake[0].x,snake[0].y);
		for(i=longueur-1;i>0;i--){
		clear_(snake[i].x,snake[i].y);
		snake[i].x=snake[i-1].x;
		snake[i].y=snake[i-1].y;
		}
		snake[0].x=head.x;
		snake[0].y=head.y;
	}
	switch(d){
	case o:
		head.y--;
		break;
	case k:
		head.x--;
		break;
	case m:
		head.x++;
		break;
	case l:
		head.y++;
		break;
	default:
		break;
	}
	if(head.x==fruit_x && head.y==fruit_y && fruit_existe==1){
		longueur++;
		clear_(fruit_x,fruit_y);
		fruit_existe=0;
		if(longueur==25){
			printf("gagné");
			stop=1;
			return;
		}
		snake[longueur-1]=last;
	}
	char *info=set_something_in_map(head.y,head.x,head.val,1);
	//printf("head:x %d y%d\ninfo:%s\n",head.x,head.y,info);
	char *info1;
	if(longueur>0){
		for(i=0;i<longueur;i++){
		//printf("neaud %d :x %d y %d\n",i,snake[i].x,snake[i].y);
		info1= set_something_in_map(snake[i].y,snake[i].x,snake[i].val,1);
		//printf("%s\n",info1);
		}
	}
	if(strcmp(info,"perdu!")==0 || strcmp(info,"erreor!")==0){
		stop=1;
		//printf("%s\n",info);
	};
}

int main(){
    FILE *fichier=fopen("map.txt","r");
    read_carte(fichier);
   
    if(erreur==1){
    	return EXIT_FAILURE;
    }
    initialization();
    while(stop==0){
    	while(!kbhit()){
    	printf("\e[H\e[2J");/*vider le terminal*/
    	fruit();
    	move_snake();
    	//printf("fruit_existe: %d longueur:%d stop:%d fruit_x:%d fruit_y:%d \n",fruit_existe,longueur,stop,fruit_x,fruit_y);
    	print_map();
    	sleep(1);
    	if(stop!=0){
    		break;
    	}
    	}
    	entree();
    	
    }
      
    free_matrice();
	return EXIT_SUCCESS;
}
