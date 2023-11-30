#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// =============================
// ===On détruit les fenêtres===
// =============================
void destroy_window(SDL_Texture* textTexture, TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window){
    // ===Quand_c'est_fini,_on_détruit_tout===
    SDL_DestroyTexture(textTexture); // Libère la texture
    TTF_CloseFont(font); // Ferme la police
    SDL_DestroyRenderer(renderer); // On arrête le rendu
    SDL_DestroyWindow(window); // On ferme la fenêtre
    TTF_Quit();
    SDL_Quit();
}

// =================
// ===Les_Boutons===
// =================
void drawButton(SDL_Renderer* renderer, TTF_Font* font2, Button button) {
    // ===Dessinez_le_rectangle_du_bouton==
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
    SDL_RenderFillRect(renderer, &button.rect); // Remplissage

    // ===Calcul_de_la_taille_du_texte===
    int texteLargeur2, texteHauteur2;
    TTF_SizeText(font2, button.text, &texteLargeur2, &texteHauteur2);

    // ===On_centre_le_texte_dans_le_boutton===
    SDL_Rect textRect;
    textRect.x = button.rect.x + (button.rect.w - texteLargeur2) / 2; // Centre horizontalement dans le bouton
    textRect.y = button.rect.y + (button.rect.h - texteHauteur2) / 2; // Centre verticalement dans le bouton
    textRect.w = texteLargeur2;
    textRect.h = texteHauteur2;

    // ===On_affiche_le_texte_dans_le_bouton===
    SDL_Surface* textSurface = TTF_RenderText_Solid(font2, button.text, (SDL_Color){255, 255, 255, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Nettoyage
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
// ===Pour_le_clic_de_la_souris===
int isMouseOverButton(Button button, int mouseX, int mouseY) {
    return (mouseX > button.rect.x &&
            mouseX < button.rect.x + button.rect.w &&
            mouseY > button.rect.y &&
            mouseY < button.rect.y + button.rect.h);
}

int fenetre_acceuil(){
    // ======================================
    // ===Initialisatins_des_bibliothèques===
    // ======================================
    // ===Initialisation_de_la_fenetre===
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }
    // ===Initialisation_du_texte===
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // ==============================
    // ===La_fenêtre_et_sa_contenu===
    // ==============================
    // ===La_taille===
    int lg = 570, lag = 356; 
    // ===La_création===
    SDL_Window* window = SDL_CreateWindow(
        "Le Snake de la Diversité", // ===Le_titre_de_la_fenêtre===
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        lg, lag,
        SDL_WINDOW_SHOWN
    );
    // ===Condition_de_la_fenêtre===
    if (!window) {
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // ===Pour_que_la_fenêtre_s'affiche===
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("/Users/ValQuiTravaille/Desktop/Université/L2/Programmation/Projet Snake/git/Projet-Snake/menus_Val/TEST/affiche_elements/Fonts/Arial Unicode.ttf", 100); // L'int à la fin est sa taille
    TTF_Font* font2 = TTF_OpenFont("/Users/ValQuiTravaille/Desktop/Université/L2/Programmation/Projet Snake/git/Projet-Snake/menus_Val/TEST/affiche_elements/Fonts/Arial Unicode.ttf", 20); // L'int à la fin est sa taille

    // ===Gestion_des_erreurs===
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // ===On génère le texte===
    SDL_Color textColor = {255, 255, 255, 255}; // Sa couleur
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "SNAKE", textColor); // Le texte (fonte du texte, contenu du texte, couleur du texte))

    // ===Gestion_des_erreurs===
    if (!textSurface) {
        printf("Erreur de création de la surface de texte: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // ===Texture_du_texte_?===
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    // ===Gestion_des_erreurs===
    if (!textTexture) {
        printf("Erreur de création de la texture de texte: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // ===Taille_et_position===
    SDL_Rect textRect;
    // ===Taille_du_texte===
    int texteLargeur = 2048, texteHauteur = 1120;
    if (TTF_SizeText(font, "SNAKE", &texteLargeur, &texteHauteur)) {
        printf("Erreur lors du calcul de la taille du texte: %s\n", TTF_GetError());
    } else {
        textRect.x = (lg - texteLargeur) / 2;  // Centre horizontalement
        textRect.y = (lag - texteHauteur) / 1024;  // Centre verticalement
        textRect.w = texteLargeur; // Largeur
        textRect.h = texteHauteur; // Longeurç
    }

    // ===================================
    // ===Pour_que_l'on_voie_la_fenêtre===
    // ===================================
    SDL_Event event;
    int running = 1;

    Button play = {{100, 100, 200, 50}, "Play"}; // Position et taille originales
    play.rect.x = 40;
    play.rect.y = (lag - play.rect.h) / 2;

    Button autre = {{100, 100, 200, 50}, "Quitter"};
    autre.rect.x = play.rect.x+300;
    autre.rect.y = play.rect.y;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(play, x, y)) {
                    // Le bouton a été cliqué
                    destroy_window(textTexture, font, renderer, window);
                    partie();
                }else if (isMouseOverButton(autre, x, y)) {
                    // Le bouton a été cliqué
                    destroy_window(textTexture, font, renderer, window);
                    return 0;
                }
            }
        }
        // ===La_couleur===
        SDL_SetRenderDrawColor(renderer, 0, 100, 27, 255);
        // ===On_efface_ce_qu'il_y_avait_précédement===
        SDL_RenderClear(renderer);
        // ===Son_texte===
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        // ===Les_boutons===
        drawButton(renderer, font2, play);
        drawButton(renderer, font2, autre);
        // ===Pour_que_ça_s'actualise===
        SDL_RenderPresent(renderer);
    }
    return 0;
}
// ========================================================================================================================================================
// ========================================================================================================================================================


// ========================================================================================================================================================
// ========================================================================================================================================================
int partie(){
    // ======================================
    // ===Initialisatins_des_bibliothèques===
    // ======================================
    // ===Initialisation_de_la_fenetre===
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }
    // ===Initialisation_du_texte===
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // ==============================
    // ===La_fenêtre_et_sa_contenu===
    // ==============================
    // ===La_taille===
    int lg = 570, lag = 356; 
    // ===La_création===
    SDL_Window* window = SDL_CreateWindow(
        "Le Snake de la Diversité - Choix du jeux", // ===Le_titre_de_la_fenêtre===
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        lg, lag,
        SDL_WINDOW_SHOWN
    );
    // ===Condition_de_la_fenêtre===
    if (!window) {
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // ===Pour_que_la_fenêtre_s'affiche===
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("/Users/ValQuiTravaille/Desktop/Université/L2/Programmation/Projet Snake/git/Projet-Snake/menus_Val/TEST/affiche_elements/Fonts/Arial Unicode.ttf", 100); // L'int à la fin est sa taille
    TTF_Font* font2 = TTF_OpenFont("/Users/ValQuiTravaille/Desktop/Université/L2/Programmation/Projet Snake/git/Projet-Snake/menus_Val/TEST/affiche_elements/Fonts/Arial Unicode.ttf", 20); // L'int à la fin est sa taille

    // ===Gestion_des_erreurs===
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // ===On génère le texte===
    SDL_Color textColor = {255, 255, 255, 255}; // Sa couleur
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Jouer", textColor); // Le texte (fonte du texte, contenu du texte, couleur du texte))

    // ===Gestion_des_erreurs===
    if (!textSurface) {
        printf("Erreur de création de la surface de texte: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // ===Texture_du_texte_?===
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    // ===Gestion_des_erreurs===
    if (!textTexture) {
        printf("Erreur de création de la texture de texte: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // ===Taille_et_position===
    SDL_Rect textRect;
    // ===Taille_du_texte===
    int texteLargeur = 2048, texteHauteur = 1120;
    if (TTF_SizeText(font, "Jouer", &texteLargeur, &texteHauteur)) {
        printf("Erreur lors du calcul de la taille du texte: %s\n", TTF_GetError());
    } else {
        textRect.x = (lg - texteLargeur) / 2;  // Centre horizontalement
        textRect.y = (lag - texteHauteur) / 1024;  // Centre verticalement
        textRect.w = texteLargeur; // Largeur
        textRect.h = texteHauteur; // Longeur
    }

    // ===================================
    // ===Pour_que_l'on_voie_la_fenêtre===
    // ===================================
    SDL_Event event;
    int running = 1;

    Button nvPartie = {{100, 100, 200, 50}, "Nouvelle Partie"};
    nvPartie.rect.x = 40;
    nvPartie.rect.y = (lag - nvPartie.rect.h) / 2;

    Button charger = {{100, 100, 200, 50}, "Charger une partie"};
    charger.rect.x = nvPartie.rect.x+300;
    charger.rect.y = nvPartie.rect.y;

    Button retour = {{100, 100, 200, 50}, "Retour"};
    retour.rect.x = (lg - retour.rect.w) / 2;
    retour.rect.y = charger.rect.y+100;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(nvPartie, x, y)) {
                    printf("Play !\n");
                    destroy_window(textTexture, font, renderer, window);
                    return -2; // à changer
                }else if (isMouseOverButton(charger, x, y)) {
                    printf("Autre!\n");
                    destroy_window(textTexture, font, renderer, window);
                    return -3; // à changer
                }else if (isMouseOverButton(retour, x, y)) {
                    destroy_window(textTexture, font, renderer, window);
                    fenetre_acceuil();
                }
            }
        }
        // ===La_couleur===
        SDL_SetRenderDrawColor(renderer, 0, 86, 27, 200);
        // ===On_efface_ce_qu'il_y_avait_précédement===
        SDL_RenderClear(renderer);
        // ===Son_texte===
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        // ===Les_boutons===
        drawButton(renderer, font2, nvPartie);
        drawButton(renderer, font2, charger);
        drawButton(renderer, font2, retour);
        // ===Pour_que_ça_s'actualise===
        SDL_RenderPresent(renderer);
    }
    return 0;
}
// ========================================================================================================================================================
// ========================================================================================================================================================


// ========================================================================================================================================================
// ========================================================================================================================================================