#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#define NB_OPTIONS 3

// Fonction pour obtenir une touche du clavier
int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

// Fonction pour afficher le menu et le voyant
void afficherMenu(int selection, char *options[]) {
    system("clear");
    printf("Choix:\n");
    for (int i = 0; i < NB_OPTIONS; i++) {
        if (i == selection) {
            // Marquer l'option sélectionnée avec un cercle plein
            printf("\033[31m(\033[0m\033[1;31m•\033[0m\033[31m)\033[0m %s\n", options[i]);
        } else {
            // Les autres options avec un cercle vide
            printf("( ) %s\n", options[i]);
        }
    }
}

int main() {
    char *options[NB_OPTIONS] = {
        "option 1",
        "option 2",
        "quitter"
    };
    int selection = 0;
    int touche;

    while (1) {
        afficherMenu(selection, options);
        
        touche = getch(); // Lire une touche sans appuyer sur Entrée
        
        switch (touche) {
            case 65: // Flèche haut
                if (selection > 0) selection--;
                break;
            case 66: // Flèche bas
                if (selection < NB_OPTIONS - 1) selection++;
                break;
            case 10: // Entrée
                // Exécutez l'action pour l'option sélectionnée
                if (selection == NB_OPTIONS - 1) {
                    exit(0); // Quitter si l'option "Quitter" est sélectionnée
                }
                printf("Option %d sélectionnée\n", selection + 1);
                sleep(1);
                break;
        }
    }

    return 0;
}
