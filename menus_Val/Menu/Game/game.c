#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "game.h"
#include "../Windows/window.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ===Le_programme_va_changer_à_l'avenir===

// ===Création_de_la_fenêtre_de_jeux===
SDL_Window* CreateWindow(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return NULL;
    }

    SDL_Window* window = SDL_CreateWindow(title,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          width, height,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return NULL;
    }
    SDL_Event e;
    int quit = 1;
    while (quit != 0) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 0;
            }
            // Autres gestionnaires d'événements ici
        }
        // Mise à jour de la logique du jeu et du rendu ici
    }


    return window;
}

// ===Les_directions_du_serpent===
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// ===Fenêtre_de_jeux===
// Elle est divisée en une grille de petits carrés d'une taille changeable
// Sa taille est donc de x carrés de longueur et h de hauteur
// Le serpent est un carré rouge qui se déplace de carrés en carrés
// Le bonus sont des carrés qui apparaissent à une position aléatoire et :
//          Soit toutes les x intérations sont regénérés à une autre position
//          Soit sont absorbés par le serpent et réapparaissent à une autre position (Pas encore implémenté la partie taille du serpent)
int gameWindow(int lg, int ht) {
    //
    const int GRID_SIZE = 35; // Taille du carré
    const int WINDOW_WIDTH = lg * GRID_SIZE; // Largeur de la fenêtre
    const int WINDOW_HEIGHT = ht * GRID_SIZE;// Hauteur de la fenêtre


    enum Direction dir = STOP; // Pour arrêter le jeux
    enum Direction lastDir = STOP; // Pour la dernière direction

    // =======================
    // Initialisations de base
    // =======================
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Snake de la diversité - Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur d'initialisation de SDL_image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Surface *loadedSurface = IMG_Load("Images/Japon Cool +++.png");
    if (!loadedSurface) {
        printf("Erreur de chargement de l'image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (!backgroundTexture) {
        printf("Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // ===Le_serpent_et_les_bonus===
    GridSquare square = {((WINDOW_WIDTH / 2) / GRID_SIZE) * GRID_SIZE, ((WINDOW_HEIGHT / 2) / GRID_SIZE) * GRID_SIZE}; // Le serpent qui apparait au "centre" de la fenêtre
    GridSquare bonus = {(randCo(WINDOW_WIDTH / GRID_SIZE) * GRID_SIZE), (randCo(WINDOW_HEIGHT / GRID_SIZE) * GRID_SIZE)}; // Le bonus qui apparait à une position aléatoire
    
    SDL_Event e;
    int quit = 0;

    
    int moveInterval = 105; // Vitesse de déplacement, plus c'est élevé plus c'est lent
    int moveCounter = 0; // Comptage de chaque déplacement
    
    int newPos = 0;

    // ===Gestion_des_évenements===
    while (!quit) {
        // ===Pour_quitter===
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                // ===Déplacement_du_serpent_à_l'aide_des_flêches directionnelles===
                // ===Lettre_q_pour_quitter===
                // ===On_ne_peut_pas_retourner_directement_sur_ses_pas===
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    if (lastDir != DOWN) dir = UP; break;
                    case SDLK_DOWN:  if (lastDir != UP) dir = DOWN; break;
                    case SDLK_LEFT:  if (lastDir != RIGHT) dir = LEFT; break;
                    case SDLK_RIGHT: if (lastDir != LEFT) dir = RIGHT; break;
                    case SDLK_q: quit = 1; SDL_DestroyTexture(backgroundTexture); SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); IMG_Quit(); SDL_Quit();nMap(); break;
                }
            }
        }

        // ===Déplacement_automatique===
        if (moveCounter++ >= moveInterval) {
            moveCounter = 0;
            lastDir = dir; // Pour que l'on sache quelle était la dernière touche directionnelle
            switch (dir) {
                case UP:    square.y -= GRID_SIZE; break;
                case DOWN:  square.y += GRID_SIZE; break;
                case LEFT:  square.x -= GRID_SIZE; break;
                case RIGHT: square.x += GRID_SIZE; break;
                case STOP:  break;
            }
        }

        // ===Nouvelle_apparition_du_bonus===
        if ((newPos == 5000) || ((square.x == bonus.x) && (square.y == bonus.y))) {
            newPos = 0;
            updateBonusPosition(&bonus, WINDOW_WIDTH / GRID_SIZE, WINDOW_HEIGHT / GRID_SIZE, GRID_SIZE);

        } else {
            newPos++;
        }

        // ===Mise_à_jour_de_la_position_du_carré===
        square.x = (square.x < 0) ? 0 : (square.x >= WINDOW_WIDTH) ? WINDOW_WIDTH - GRID_SIZE : square.x;
        square.y = (square.y < 0) ? 0 : (square.y >= WINDOW_HEIGHT) ? WINDOW_HEIGHT - GRID_SIZE : square.y;


        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        // ===Player===
        SDL_Rect squareRect = {square.x, square.y, GRID_SIZE, GRID_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &squareRect);
        // ===Bonus===
        SDL_Rect squareRect2 = {bonus.x, bonus.y, GRID_SIZE, GRID_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &squareRect2);


        SDL_RenderPresent(renderer);
    }

    // Nettoyage des ressources SDL
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

// ===Nombre_aléatoire===
void initializeRandom() {
    srand((unsigned)time(NULL));
}

// ===Génération_d'un_nombre_entre_0_et_x_pour_les_coordonnées===
int randCo(int x) {
    return (rand() % x);
}

// ===Apparition_du_bonus===
void updateBonusPosition(GridSquare *bonus, int gridWidth, int gridHeight, int gridSize) {
    bonus->x = (randCo(gridWidth) * gridSize);
    bonus->y = (randCo(gridHeight) * gridSize);
}
