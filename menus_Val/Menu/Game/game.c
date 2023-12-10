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
int gameWindow(int lg, int ht, const char* card_name) {
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

    SDL_Surface *loadedSurface = IMG_Load(card_name);
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
    // ===Texture_de_la_tête===
    SDL_Surface* headSurface = IMG_Load("Images/Eye.png");
    if (!headSurface) {
        printf("Erreur de chargement de l'image de la tête du serpent: %s\n", IMG_GetError());
        // Gérer l'erreur
    }

    SDL_Texture* headTexture = SDL_CreateTextureFromSurface(renderer, headSurface);
    SDL_FreeSurface(headSurface);  // Libérez la surface car elle n'est plus nécessaire
    if (!headTexture) {
        printf("Erreur de création de la texture de la tête du serpent: %s\n", SDL_GetError());
        // Gérer l'erreur
    }

    // ===Texture_du_corps===
    SDL_Surface* bodySurface = IMG_Load("Images/Body.png");
    if (!bodySurface) {
        printf("Erreur de chargement de l'image du corps du serpent: %s\n", IMG_GetError());
        // Gérer l'erreur
    }

    SDL_Texture* bodyTexture = SDL_CreateTextureFromSurface(renderer, bodySurface);
    SDL_FreeSurface(bodySurface); // Libérez la surface car elle n'est plus nécessaire
    if (!bodyTexture) {
        printf("Erreur de création de la texture du corps du serpent: %s\n", SDL_GetError());
        // Gérer l'erreur
    }

    // ===Texture_du_bonus===
    SDL_Surface* bonusSurface = IMG_Load("Images/Bonus.png");
    if (!bonusSurface) {
        printf("Erreur de chargement de l'image du bonus: %s\n", IMG_GetError());
        // Gérer l'erreur
    }

    SDL_Texture* bonusTexture = SDL_CreateTextureFromSurface(renderer, bonusSurface);
    SDL_FreeSurface(bonusSurface); // Libérez la surface car elle n'est plus nécessaire
    if (!bonusTexture) {
        printf("Erreur de création de la texture du bonus: %s\n", SDL_GetError());
        // Gérer l'erreur
    }



    // ===Le_serpent_et_les_bonus===
    GridSquare square = {((WINDOW_WIDTH / 2) / GRID_SIZE) * GRID_SIZE, ((WINDOW_HEIGHT / 2) / GRID_SIZE) * GRID_SIZE}; // Le serpent qui apparait au "centre" de la fenêtre
    GridSquare bonus = {(randCo(WINDOW_WIDTH / GRID_SIZE) * GRID_SIZE), (randCo(WINDOW_HEIGHT / GRID_SIZE) * GRID_SIZE)}; // Le bonus qui apparait à une position aléatoire
    
    SDL_Event e;
    int quit = 0;

    
    int moveInterval = 150; // Vitesse de déplacement, plus c'est élevé plus c'est lent
    int moveCounter = 0; // Comptage de chaque déplacement
    
    int newPos = 0;

    // ===Initialisation du Serpent===
    int snakeCapacity = 10; // Capacité initiale
    int snakeSize = 0; // Taille initiale
    GridSquare* snake = createSnake(snakeCapacity);

    // ===Score===
    int score = 0;

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
            
            int newX = square.x;
            int newY = square.y;

            int oldHeadX = square.x;
            int oldHeadY = square.y;

            switch (dir) {
                case UP:    square.y -= GRID_SIZE; newY -= GRID_SIZE; break;
                case DOWN:  square.y += GRID_SIZE; newY += GRID_SIZE; break;
                case LEFT:  square.x -= GRID_SIZE; newX -= GRID_SIZE; break;
                case RIGHT: square.x += GRID_SIZE; newX += GRID_SIZE; break;
                case STOP:  break;
            }

            for (int i = snakeSize - 1; i > 0; i--) {
                snake[i].x = snake[i - 1].x;
                snake[i].y = snake[i - 1].y;
            }

            // Si le serpent a un corps, le premier segment prend l'ancienne position de la tête
            if (snakeSize > 0) {
                snake[0].x = oldHeadX;
                snake[0].y = oldHeadY;
            }


            // Check if the new position is outside the window
            if (newX < 0 || newX >= WINDOW_WIDTH || newY < 0 || newY >= WINDOW_HEIGHT) {
                printf("The square is trying to leave the window!\n");
                quit = 1;
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                IMG_Quit();
                SDL_Quit();
                lostWindow(score);
                break;
            }
        }

        if ((square.x == bonus.x) && (square.y == bonus.y)){
            int newX, newY;

            if (snakeSize > 0) {
                // Utilisez la position du dernier segment du serpent
                newX = snake[snakeSize - 1].x;
                newY = snake[snakeSize - 1].y;
            } else {
                // Placez le nouveau segment derrière la tête du serpent
                switch (dir) {
                    case UP:    newY = square.y + GRID_SIZE; newX = square.x; break;
                    case DOWN:  newY = square.y - GRID_SIZE; newX = square.x; break;
                    case LEFT:  newX = square.x + GRID_SIZE; newY = square.y; break;
                    case RIGHT: newX = square.x - GRID_SIZE; newY = square.y; break;
                    default:    newX = square.x; newY = square.y; break; // ou une autre logique par défaut
                }
            }
                score += 50;
                updateBonusPosition(&bonus, WINDOW_WIDTH / GRID_SIZE, WINDOW_HEIGHT / GRID_SIZE, GRID_SIZE);
                createBodyPart(&snake, &snakeCapacity, &snakeSize, newX, newY);
                newPos = 0;
                printf("Score : %d\n",score);
        }
        // ===Nouvelle_apparition_du_bonus===
        if (newPos == 5000) {
            printf("Pos : %d\nsnek : %d, %d - Bonus : %d, %d\n", newPos, square.x, square.y, bonus.x, bonus.y);
            newPos = 0;
            updateBonusPosition(&bonus, WINDOW_WIDTH / GRID_SIZE, WINDOW_HEIGHT / GRID_SIZE, GRID_SIZE);
            if(score > 0){
                score -= 35;
            }
            printf("Score  %d\n",score);
        } else {
            newPos++;
        }

        // ===Mise_à_jour_de_la_position_du_carré===
        square.x = (square.x < 0) ? 0 : (square.x >= WINDOW_WIDTH) ? WINDOW_WIDTH - GRID_SIZE : square.x;
        square.y = (square.y < 0) ? 0 : (square.y >= WINDOW_HEIGHT) ? WINDOW_HEIGHT - GRID_SIZE : square.y;

        for (int i = 0; i < snakeSize; i++) {
            if (square.x == snake[i].x && square.y == snake[i].y) {
                printf("Collision détectée ! Fin du jeu.\n");
                quit = 1;  // Mettre fin au jeu
                SDL_DestroyTexture(bonusTexture);
                SDL_DestroyTexture(bodyTexture);
                SDL_DestroyTexture(headTexture);
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                IMG_Quit();
                SDL_Quit();
                lostWindow(score);
                break;
            }
        }


        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        // ===Player===
        SDL_Rect headRect = {square.x, square.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderCopy(renderer, headTexture, NULL, &headRect);
        // ===Bonus===
        // Afficher le bonus
        SDL_Rect bonusRect = {bonus.x, bonus.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderCopy(renderer, bonusTexture, NULL, &bonusRect);
        // Afficher chaque segment du corps du serpent
        for (int i = 0; i < snakeSize; i++) {
            SDL_Rect segmentRect = {snake[i].x, snake[i].y, GRID_SIZE, GRID_SIZE};
            SDL_RenderCopy(renderer, bodyTexture, NULL, &segmentRect);
        }



        SDL_RenderPresent(renderer);
    }

    // Nettoyage des ressources SDL
    SDL_DestroyTexture(bonusTexture);
    SDL_DestroyTexture(bodyTexture);
    SDL_DestroyTexture(headTexture);
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

// ================
// ===Le_Serpent===
// ================
// ===Initialisation===
GridSquare* createSnake(int initialCapacity) {
    return malloc(initialCapacity * sizeof(GridSquare));
}

// ===Agrandissement===
GridSquare* resizeSnake(GridSquare* list, int newCapacity) {
    GridSquare* temp = realloc(list, newCapacity * sizeof(GridSquare));
    return temp;
}

// ===Ajouter_d'un_élément===
void addToSnake(GridSquare* list, int index, int x, int y, const char* name) {
    list[index].x = x;
    list[index].y = y;
    snprintf(list[index].name, 10, "%s", name);
}

// ===Libéreration===
void freeSnake(GridSquare* list) {
    free(list);
}

// Créer et ajouter un carré à la liste avec un ID automatique
void createBodyPart(GridSquare** list, int* capacity, int* currentSize, int x, int y) {
    if (*currentSize >= *capacity) {
        *capacity *= 2;
        GridSquare* temp = resizeSnake(*list, *capacity);
        if (temp != NULL) {
            *list = temp;
        } else {
            // Gestion de l'échec de la réallocation
            return;
        }
    }

    // Attribuer un ID automatique basé sur currentSize
    snprintf((*list)[*currentSize].name, 10, "q%d", *currentSize);

    // Définir les coordonnées
    (*list)[*currentSize].x = x;
    (*list)[*currentSize].y = y;

    // Incrémenter la taille actuelle de la liste
    (*currentSize)++;
}
