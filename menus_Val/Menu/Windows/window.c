#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "window.h"
#include "../Game/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// ==================================
// ===Initialisation des fenêtres===
// ==================================
int initializeWindow(const char* title, int width, int height, SDL_Window** outWindow, SDL_Renderer** outRenderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // ===Erreur_initialisation_SDL===
        printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }

    *outWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (*outWindow == NULL) {
        // ===Erreur_création_fenêtre===
        printf("Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *outRenderer = SDL_CreateRenderer(*outWindow, -1, SDL_RENDERER_ACCELERATED);
    if (*outRenderer == NULL) {
        // ===Erreur_création_rendu===
        printf("Erreur lors de la création du rendu: %s\n", SDL_GetError());
        SDL_DestroyWindow(*outWindow);
        SDL_Quit();
        return -1;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_ttf : %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    return 0; // Succès
}

// =============================
// ===On détruit les fenêtres===
// =============================
void destroy_window(SDL_Texture* textTexture, TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window) {
    // ===Quand_c'est_fini,_on_détruit_tout===
    if (textTexture != NULL) {
        SDL_DestroyTexture(textTexture); // Libère la texture
    }
    if (font != NULL) {
        TTF_CloseFont(font); // Ferme la police
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer); // On arrête le rendu
    }
    if (window != NULL) {
        SDL_DestroyWindow(window); // On ferme la fenêtre
    }
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

// ================
// ===Les_Cartes===
// ================
void loadAndDisplayCards(SDL_Renderer* renderer, TTF_Font* font, Card* cards, int numCards) {
    for (int i = 0; i < numCards; i++) {
        // ===Chargement_de_l'image_dans_la_texture===
        cards[i].image = IMG_LoadTexture(renderer, "Images/Snake.png");

        // ===Affichage_de_l'image===
        SDL_RenderCopy(renderer, cards[i].image, NULL, &cards[i].rect);

        // ===Affichage_de_la_description===
        SDL_Surface* surface = TTF_RenderText_Blended(font, cards[i].description, (SDL_Color){255, 255, 255});
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surface);

        int textW = 0, textH = 0;
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {cards[i].rect.x, cards[i].rect.y + cards[i].rect.h, textW, textH};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(textTexture);
    }
}

// ===Pour_les_effacer===
void cleanUpCards(Card* cards, int numCards) {
    for (int i = 0; i < numCards; i++) {
        SDL_DestroyTexture(cards[i].image);
    }
}



int fenetre_acceuil() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    // Initialisation de la fenêtre
    int lg = 570;
    int haut = 356;
    initializeWindow("Le Snake de la Diversité", lg, haut, &window, &renderer);

    if (initializeWindow("Le Snake de la Diversité", lg, haut, &window, &renderer) != 0) {
        return -1; // Échec de l'initialisation
    }

    // Logique de la fenêtre d'accueil (chargement des polices, création des boutons, etc.)
    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("../Menu/Fonts/GrinchedRegular.ttf", 100); // L'int à la fin est sa taille 
    TTF_Font* font2 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 25); // L'int à la fin est sa taille
    if (!font || !font2) {
        printf("Erreur de chargement des polices: %s\n", TTF_GetError());
        if (font) TTF_CloseFont(font);
            printf("Font 1\n");
        if (font2) TTF_CloseFont(font2);
            printf("Font 2\n");
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
        textRect.y = (haut - texteHauteur) / 1024;  // Centre verticalement
        textRect.w = texteLargeur; // Largeur
        textRect.h = texteHauteur; // Longeur
    }

    // ===================================
    // ===Pour_que_l'on_voie_la_fenêtre===
    // ===================================
    SDL_Event event;
    int running = 1;

    // Position_et_taille_des_boutons
    Button play = {{100, 100, 200, 50}, "Play"}; 
    play.rect.x = 40;
    play.rect.y = (haut - play.rect.h) / 2;

    Button autre = {{100, 100, 200, 50}, "Quitter"};
    autre.rect.x = play.rect.x+300;
    autre.rect.y = play.rect.y;

    while (running) {
        // Logique de l'événement et du rendu pour la fenêtre d'accueil
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

    // Nettoyage des ressources
    destroy_window(NULL, NULL, renderer, window);
    return 0; // Fin avec succès
}


int partie() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int lg = 570;
    int lag = 356;
    // Initialisation de la fenêtre
    if (initializeWindow("Le Snake de la Diversité - Choix du jeu", lg, lag, &window, &renderer) != 0) {
        return -1; // Échec de l'initialisation
    }

    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("../Menu/Fonts/GrinchedRegular.ttf", 100); // L'int à la fin est sa taille
    TTF_Font* font2 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 13); // L'int à la fin est sa taille
    if (!font || !font2) {
        if (font) TTF_CloseFont(font);
        if (font2) TTF_CloseFont(font2);
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

    // Position et taille des boutons
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
        // Logique de l'événement et du rendu pour la partie
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
                    nMap();
                    return 0; // à changer
                }else if (isMouseOverButton(charger, x, y)) {
                    printf("Autre!\n");
                    destroy_window(textTexture, font, renderer, window);
                    break; // à changer
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

    // Nettoyage des ressources
    destroy_window(NULL, NULL, renderer, window);
    return 0; // Fin avec succès
}


int nMap() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int lg = 770;
    int lag = 556; 
    // Initialisation de la fenêtre
    if (initializeWindow("Le Snake de la Diversité - Choix de la Carte", lg, lag, &window, &renderer) != 0) {
        return -1; // Échec de l'initialisation
    }

    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("../Menu/Fonts/GrinchedRegular.ttf", 100); // L'int à la fin est sa taille
    TTF_Font* font2 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 20); // L'int à la fin est sa taille

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
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Cartes", textColor); // Le texte (fonte du texte, contenu du texte, couleur du texte))

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
    if (TTF_SizeText(font, "Cartes", &texteLargeur, &texteHauteur)) {
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

    // ===Position_et_taille_du_bouton===
    Button retour = {{100, 100, 200, 50}, "Retour"};
    retour.rect.x = (lg - retour.rect.w) / 2;
    retour.rect.y = (lag - retour.rect.h) / 2 +250;

    // ===Le_nombre_de_Cartes===
    const int numCards = 6;
    Card cards[numCards] = {
        {{50, 150, 110, 110}, NULL, "Map 1", 10, 5}, // {Axe y, Axe x, Largeur, longueur}
        {{330, 150, 110, 110}, NULL, "Map 2", 15, 9},
        {{620, 150, 110, 110}, NULL, "Map 3", 20, 13},
        {{50, 300, 110, 110}, NULL, "Map 4", 25, 15},
        {{330, 300, 110, 110}, NULL, "Map 5", 30, 19},
        {{620, 300, 110, 110}, NULL, "Map 6", 35, 23}
    };

    while (running) {
        // Logique de l'événement et du rendu pour nMap
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(retour, x, y)) {
                    destroy_window(textTexture, font, renderer, window);
                    partie();
                }
                // Vérifier si le clic est sur une carte
                for (int i = 0; i < numCards; i++) {
                    if (x >= cards[i].rect.x && x <= (cards[i].rect.x + cards[i].rect.w) &&
                        y >= cards[i].rect.y && y <= (cards[i].rect.y + cards[i].rect.h)) {
                        // Action lorsque la carte est cliquée
                        printf("Carte %d cliquée : %s\n", i, cards[i].description);
                        gameWindow(cards[i].largeur, cards[i].hauteur);
                        break; // Sortir de la boucle si une carte a été cliquée
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 86, 27, 200);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        drawButton(renderer, font2, retour);

        loadAndDisplayCards(renderer, font2, cards, numCards); // Affiche les cartes

        SDL_RenderPresent(renderer);
    }

    // Nettoyage des ressources
    destroy_window(NULL, NULL, renderer, window);
    return 0; // Fin avec succès
}

