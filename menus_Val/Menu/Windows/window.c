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
void destroyWindow(SDL_Texture* texture, TTF_Font* font[], SDL_Renderer* renderer, SDL_Window* window, int fontCount) {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }

    for (int i = 0; i < fontCount; ++i) {
        if (font[i] != NULL) {
            TTF_CloseFont(font[i]);
        }
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
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
        cards[i].image = IMG_LoadTexture(renderer, cards[i].card_name);

        // ===Affichage_de_l'image===
        SDL_RenderCopy(renderer, cards[i].image, NULL, &cards[i].rect);

        // ===Affichage_de_la_description===
        SDL_Surface* surface = TTF_RenderText_Blended(font, cards[i].description, (SDL_Color){0, 0, 0});
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
    int lg = 770;
    int haut = 556;
    initializeWindow("Le Snake de la Diversité", lg, haut, &window, &renderer);

    if (initializeWindow("Le Snake de la Diversité", lg, haut, &window, &renderer) != 0) {
        return -1; // Échec de l'initialisation
    }

    // Logique de la fenêtre d'accueil (chargement des polices, création des boutons, etc.)
    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("../Menu/Fonts/GrinchedRegular.ttf", 150); // L'int à la fin est sa taille 
    TTF_Font* font1 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 40); // Play
    TTF_Font* font2 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 25); // Play
    TTF_Font* font3 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 10); // Quitter
    if (!font || !font2 || !font3) {
        printf("Erreur de chargement des polices: %s\n", TTF_GetError());
        if (font) TTF_CloseFont(font);
            printf("Font 1\n");
        if (font2) TTF_CloseFont(font2);
            printf("Font 2\n");
        if (font3) TTF_CloseFont(font3);
            printf("Font 3\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    TTF_Font* fonts[] = {font, font1, font2, font3}; // Inclure toutes les polices
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);

    // ===On génère le texte===
    SDL_Color textColor = {0, 0, 0, 255}; // Sa couleur
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
    int textWidth, textHeight;

    // Get the size of the text as it would be rendered with the current font
    if (TTF_SizeText(font, "SNAKE", &textWidth, &textHeight)) {
        printf("Erreur lors du calcul de la taille du texte: %s\n", TTF_GetError());
        // Handle error appropriately
    } else {
        textRect.x = (lg - textWidth) / 2; // Centre horizontalement
        textRect.y = (haut - textHeight) / 24 + 20; // Centre verticalement avec un décalage de 20 pixels vers le bas
        textRect.w = textWidth; // Largeur
        textRect.h = textHeight; // Hauteur
    }

    SDL_Surface* backgroundImageSurface = IMG_Load("Images/Acceuil.png");
    if (!backgroundImageSurface) {
        printf("Erreur de chargement de l'image de fond : %s\n", IMG_GetError());
        // Gérez l'erreur selon vos besoins
    }
    SDL_Texture* backgroundImageTexture = SDL_CreateTextureFromSurface(renderer, backgroundImageSurface);
    SDL_FreeSurface(backgroundImageSurface);
    if (!backgroundImageTexture) {
        printf("Erreur de création de la texture de l'image de fond : %s\n", SDL_GetError());
        // Gérez l'erreur selon vos besoins
    }


    // ===================================
    // ===Pour_que_l'on_voie_la_fenêtre===
    // ===================================
    SDL_Event event;
    int running = 1;

    // Position et taille des boutons
    int boutonLargeur = 250; // Largeur des boutons agrandie
    int boutonHauteur = 70;  // Hauteur des boutons agrandie
    int espacementVertical = 20; // Espace vertical entre les éléments

    Button play = {{0, 0, boutonLargeur, boutonHauteur}, "Play"};
    play.rect.x = (lg - play.rect.w) / 2;
    play.rect.y = textRect.y + textRect.h + espacementVertical;

    Button credits = {{0, 0, boutonLargeur/ 1.3, boutonHauteur/ 1.3}, "Credits"};
    credits.rect.x = (lg - credits.rect.w) /2;
    credits.rect.y = play.rect.y + play.rect.h + espacementVertical;

    Button quitter = {{0, 0, boutonLargeur/3, boutonHauteur/3}, "Quitter"};
    quitter.rect.x = lg - quitter.rect.w - 10;
    quitter.rect.y = haut - quitter.rect.h - 10; // En bas à droite

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
                    SDL_DestroyTexture(backgroundImageTexture);
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    partie();
                    break;
                }else if (isMouseOverButton(credits, x, y)) {
                    // Le bouton a été cliqué
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    SDL_DestroyTexture(backgroundImageTexture);
                    break;
                }else if (isMouseOverButton(quitter, x, y)) {
                    // Le bouton a été cliqué
                    SDL_DestroyTexture(backgroundImageTexture);
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    break;
                }
            }
        }
        // ===On_efface_ce_qu'il_y_avait_précédement===
        SDL_RenderClear(renderer);
        // Définir le rendu du fond avec la texture de l'image
        SDL_RenderCopy(renderer, backgroundImageTexture, NULL, NULL);
        // ===Son_texte===
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        // ===Les_boutons===
        drawButton(renderer, font1, play);
        drawButton(renderer, font2, credits);
        drawButton(renderer, font3, quitter);
        // ===Pour_que_ça_s'actualise===
        SDL_RenderPresent(renderer);
    }

    // Nettoyage des ressources
    SDL_DestroyTexture(backgroundImageTexture);
    destroyWindow(textTexture, fonts, renderer, window, fontCount);
    return 0; // Fin avec succès
}


int partie() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int lg = 770;
    int lag = 556;
    // Initialisation de la fenêtre
    if (initializeWindow("Le Snake de la Diversité - Choix du jeu", lg, lag, &window, &renderer) != 0) {
        return -1; // Échec de l'initialisation
    }

    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("../Menu/Fonts/GrinchedRegular.ttf", 100); // L'int à la fin est sa taille
    TTF_Font* font2 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 26); // Play - Load - Back

    if (!font || !font2) {
        if (font) TTF_CloseFont(font);
        if (font2) TTF_CloseFont(font2);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* fonts[] = {font, font2}; // Inclure toutes les polices
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);

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
        textRect.y = (lag - texteHauteur) / 24 + 20;  // Centre verticalement
        textRect.w = texteLargeur; // Largeur
        textRect.h = texteHauteur; // Longeur
    }

    SDL_Surface* backgroundImageSurface = IMG_Load("Images/Acceuil.png");
    if (!backgroundImageSurface) {
        printf("Erreur de chargement de l'image de fond : %s\n", IMG_GetError());
        // Gérez l'erreur selon vos besoins
    }
    SDL_Texture* backgroundImageTexture = SDL_CreateTextureFromSurface(renderer, backgroundImageSurface);
    SDL_FreeSurface(backgroundImageSurface);
    if (!backgroundImageTexture) {
        printf("Erreur de création de la texture de l'image de fond : %s\n", SDL_GetError());
        // Gérez l'erreur selon vos besoins
    }

    // ===================================
    // ===Pour_que_l'on_voie_la_fenêtre===
    // ===================================
    SDL_Event event;
    int running = 1;


    // Position et taille des boutons
    int boutonHauteur = 100;  // Hauteur de tous les boutons (plus grande)
    int espaceEntreBoutons = 20;  // Espace vertical entre les boutons

    Button nvPartie = {{100, 100, 220, 60}, "New Game"};
    nvPartie.rect.x = 40;  // Ajustez cette valeur pour décaler le bouton vers la gauche
    nvPartie.rect.y = (lag - boutonHauteur * 3 - espaceEntreBoutons * 2) / 2 + 2 * 3 * espaceEntreBoutons; // Décalage vers le bas

    Button charger = {{100, 100, 220, 60}, "Load Game"};
    charger.rect.x = 40;  // Ajustez cette valeur pour décaler le bouton vers la gauche
    charger.rect.y = nvPartie.rect.y + boutonHauteur + espaceEntreBoutons;

    Button retour = {{100, 100, 220, 60}, "Main Menu"};
    retour.rect.x = 40;  // Ajustez cette valeur pour décaler le bouton vers la gauche
    retour.rect.y = charger.rect.y + boutonHauteur + espaceEntreBoutons;




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
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    nMap();
                    break;
                }else if (isMouseOverButton(charger, x, y)) {
                    printf("Autre!\n");
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    break; // à changer
                }else if (isMouseOverButton(retour, x, y)) {
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    fenetre_acceuil();
                    break;
                }
            }
        }
        // ===On_efface_ce_qu'il_y_avait_précédement===
        SDL_RenderClear(renderer);
        // Définir le rendu du fond avec la texture de l'image
        SDL_RenderCopy(renderer, backgroundImageTexture, NULL, NULL);
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
    destroyWindow(textTexture, fonts, renderer, window, fontCount);
    SDL_DestroyTexture(backgroundImageTexture);
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
    TTF_Font* fonts[] = {font, font2}; // Inclure toutes les polices
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);

    // ===On génère le texte===
    SDL_Color textColor = {0, 0, 0, 255}; // Sa couleur
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

    SDL_Surface* backgroundImageSurface = IMG_Load("Images/Acceuil.png");
    if (!backgroundImageSurface) {
        printf("Erreur de chargement de l'image de fond : %s\n", IMG_GetError());
        // Gérez l'erreur selon vos besoins
    }
    SDL_Texture* backgroundImageTexture = SDL_CreateTextureFromSurface(renderer, backgroundImageSurface);
    SDL_FreeSurface(backgroundImageSurface);
    if (!backgroundImageTexture) {
        printf("Erreur de création de la texture de l'image de fond : %s\n", SDL_GetError());
        // Gérez l'erreur selon vos besoins
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
        {{50, 150, 110, 110}, NULL, "Images/Body.png", "Map 1", 10, 5}, // {Axe y, Axe x, Largeur, longueur}
        {{330, 150, 110, 110}, NULL, "Images/Acceuil.png", "Map 2", 15, 9},
        {{620, 150, 110, 110}, NULL, "Images/Body2.png", "Map 3", 20, 13},
        {{50, 300, 110, 110}, NULL, "Images/Bonus.png", "Map 4", 25, 15},
        {{330, 300, 110, 110}, NULL, "Images/Eye.png", "Map 5", 30, 19},
        {{620, 300, 110, 110}, NULL, "Images/Japon Cool +++.png", "Map 6", 35, 23}
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
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    partie();
                }
                // Vérifier si le clic est sur une carte
                for (int i = 0; i < numCards; i++) {
                    if (x >= cards[i].rect.x && x <= (cards[i].rect.x + cards[i].rect.w) &&
                        y >= cards[i].rect.y && y <= (cards[i].rect.y + cards[i].rect.h)) {
                        // Action lorsque la carte est cliquée
                        printf("Carte %d cliquée : %s\n", i, cards[i].description);
                        gameWindow(cards[i].largeur, cards[i].hauteur, cards[i].card_name);
                        break; // Sortir de la boucle si une carte a été cliquée
                    }
                }
            }
        }
        SDL_RenderClear(renderer);

        // Définir le rendu du fond avec la texture de l'image
        SDL_RenderCopy(renderer, backgroundImageTexture, NULL, NULL);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        drawButton(renderer, font2, retour);

        loadAndDisplayCards(renderer, font2, cards, numCards); // Affiche les cartes

        SDL_RenderPresent(renderer);
    }

    // Nettoyage des ressources
    destroyWindow(textTexture, fonts, renderer, window, fontCount);
    return 0; // Fin avec succès
}


int lostWindow(int score){
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    // Initialisation de la fenêtre
    int lg = 770;
    int haut = 556;
    initializeWindow("Snake de la diversité - Perdu !", lg, haut, &window, &renderer);
    // ==============
    // ===Le_Texte===
    // ==============
    // ===On_importe_la_police===
    TTF_Font* font = TTF_OpenFont("../Menu/Fonts/GrinchedRegular.ttf", 100); // L'int à la fin est sa taille
    TTF_Font* font1 = TTF_OpenFont("../Menu/Fonts/GrinchedRegular.ttf", 65); //
    TTF_Font* font2 = TTF_OpenFont("../Menu/Fonts/04B_30__.ttf", 28); // Main Menu - Replay


    if (!font || !font1 || !font2) {
        printf("Erreur de création de la fonte de texte: %s\n", TTF_GetError());
        if (font) TTF_CloseFont(font);
        if (font1) TTF_CloseFont(font1);
        if (font2) TTF_CloseFont(font2);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* fonts[] = {font, font1 , font2}; // Inclure toutes les polices
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);

    // ===On génère le texte===    
    SDL_Color textColor = {0, 0, 0, 255}; // Sa couleur
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Perdu", textColor); // Le texte (fonte du texte, contenu du texte, couleur du texte))

    char scoreText[50];
    sprintf(scoreText, "Score: %d", score); // Convertir le score en texte
    SDL_Surface* text2Surface = TTF_RenderText_Solid(font1, scoreText, textColor);


    // ===Gestion_des_erreurs===
    if (!textSurface || ! text2Surface) {
        printf("Erreur de création de la surface de texte: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // ===Texture_du_texte_?===
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Texture* subtitleTexture = SDL_CreateTextureFromSurface(renderer, text2Surface);
    SDL_FreeSurface(text2Surface); // Libérez la surface après la création de la texture

    // ===Gestion_des_erreurs===
    if (!textTexture || !subtitleTexture) {
        printf("Erreur de création de la texture de texte: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        TTF_CloseFont(font1);
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
    if (TTF_SizeText(font, "Replay", &texteLargeur, &texteHauteur)) {
        printf("Erreur lors du calcul de la taille du texte: %s\n", TTF_GetError());
    } else {
        textRect.x = (lg - texteLargeur) / 2;  // Centre horizontalement
        textRect.y = (haut - texteHauteur) / 24 + 20;  // Centre verticalement
        textRect.w = texteLargeur; // Largeur
        textRect.h = texteHauteur; // Longeur
    }

    SDL_Rect subtitleRect;
    int subtitleLarg = texteLargeur, subtitleHaut = texteHauteur;
    if (TTF_SizeText(font1, "Main Menu", &subtitleLarg, &subtitleHaut)) {
        printf("Erreur lors du calcul de la taille du texte: %s\n", TTF_GetError());
    } else {
        subtitleRect.x = (lg - subtitleLarg) / 2; // Centre horizontalement (en fonction de sa propre largeur)
        subtitleRect.y = textRect.y + texteHauteur + 15; // Positionnez en dessous du titre, avec un espace de 20 pixels
        subtitleRect.w = subtitleLarg; // Largeur
        subtitleRect.h = subtitleHaut; // Longueur

    }

    SDL_Surface* backgroundImageSurface = IMG_Load("Images/Acceuil.png");
    if (!backgroundImageSurface) {
        printf("Erreur de chargement de l'image de fond : %s\n", IMG_GetError());
        // Gérez l'erreur selon vos besoins
    }
    SDL_Texture* backgroundImageTexture = SDL_CreateTextureFromSurface(renderer, backgroundImageSurface);
    SDL_FreeSurface(backgroundImageSurface);
    if (!backgroundImageTexture) {
        printf("Erreur de création de la texture de l'image de fond : %s\n", SDL_GetError());
        // Gérez l'erreur selon vos besoins
    }

    // ===================================
    // ===Pour_que_l'on_voie_la_fenêtre===
    // ===================================
    SDL_Event event;
    int running = 1;


    // Position et taille des boutons
    int boutonHauteur = 100;  // Hauteur de tous les boutons (plus grande)
    int espaceEntreBoutons = 20;  // Espace vertical entre les boutons

    Button nvPartie = {{100, 100, 220, 60}, "Replay"};
    nvPartie.rect.x = 40;  // Ajustez cette valeur pour décaler le bouton vers la gauche
    nvPartie.rect.y = (haut - boutonHauteur * 3 - espaceEntreBoutons * 2) / 2 + 2 * 4 * espaceEntreBoutons; // Décalage vers le bas

    Button charger = {{100, 100, 220, 60}, "Main Menu"};
    charger.rect.x = 40;  // Ajustez cette valeur pour décaler le bouton vers la gauche
    charger.rect.y = nvPartie.rect.y + boutonHauteur + espaceEntreBoutons;


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
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    nMap();
                    break;
                }else if (isMouseOverButton(charger, x, y)) {
                    printf("Autre!\n");
                    destroyWindow(textTexture, fonts, renderer, window, fontCount);
                    fenetre_acceuil();
                    break; // à changer
                }
            }
        }
        // ===On_efface_ce_qu'il_y_avait_précédement===
        SDL_RenderClear(renderer);
        // Définir le rendu du fond avec la texture de l'image
        SDL_RenderCopy(renderer, backgroundImageTexture, NULL, NULL);
        // ===Son_texte===
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderCopy(renderer, subtitleTexture, NULL, &subtitleRect);
        // ===Les_boutons===
        drawButton(renderer, font2, nvPartie);
        drawButton(renderer, font2, charger);
        // ===Pour_que_ça_s'actualise===
        SDL_RenderPresent(renderer);
    }

    // Nettoyage des ressources
    destroyWindow(textTexture, fonts, renderer, window, fontCount);
    SDL_DestroyTexture(backgroundImageTexture);
    SDL_DestroyTexture(subtitleTexture);
    return 0; // Fin avec succès
}