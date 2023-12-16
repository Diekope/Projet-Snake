# Projet Snake en C

## Description
Ce projet contient deux versions du jeu classique Snake, développées en C. La première version est conçue pour être exécutée dans un terminal, tandis que la deuxième utilise la bibliothèque SDL pour offrir une interface graphique. Le projet est actuellement toujours en cours de développement.

## Fonctionnalités
- **Version Terminal** : Une version minimaliste du jeu Snake jouable directement dans le terminal. Le serpent est composé d'un lettre de l'alphabet et au fur et à mesure du jeux, l'alphabet se complètre.
- **Version SDL** : Une version améliorée avec une interface graphique, offrant une expérience de jeu visuelle.

## Prérequis
- Compilateur C (comme GCC)
- Bibliothèque SDL (pour la version avec interface graphique)

## Installation
### Version Terminal
1. Clonez le dépôt : `git clone https://github.com/Diekope/Projet-Snake.git`
2. Naviguez dans le dossier du projet dans votre terminal. À la fin, votre chemin devrait ressembler à ceci : `Username\cheminVersLesFichiers`.
3. Exécutez le jeu : `./snake_terminal`.

### Version SDL
1. Assurez-vous d'avoir installé les bibliothèque SDL, SDL_ttf, SDL_image.
2. Si vous ne les avez pas, intallez les
- **Sur Windows** À l'aide de [Ce tutoriel pour Windows](https://www.youtube.com/watch?v=riUcuqWeN6w)
- **Sur MacOS avec VsCode** À l'aide de [Ce tutoriel pour MasOS](https://www.youtube.com/watch?v=Dyz9O7s7B8w)
4. Exécutez le jeu : `./acceuil`.

## Utilisation (En développement)
- Utilisez les touches directionnelles pour déplacer le serpent.
- Collectez des points et évitez de heurter les murs ou la queue du serpent.
- Essayez de battre votre meilleur score !
### Précautions:
- !!! pour la version terminal, ajoute cartes possibles mais les fichiers cartes devont restés dans le répertoire data/map!!!
- !!! pour assurer la bonne fonctionnement de snake le fichier carte ne doit pas être vide et la carte à l'interieur faut absolument sous forme de:<br />
l.1:  l c<br />
l.2:  ###...#<br />
l.3:  #     #<br />
l  :  .     .<br />
l  :  .     .<br />
l  :  .     .<br />
l.n:  ###...#<br />
- !!! la version terminal est dédiée uniquement pour linux !!!
## Les Contributeurs et leurs rôles
- **Benjamin** :
- **Yuyu** :
- **Mohan** :
- **Valentin** :

## Licence
[Inclure ici le type de licence si applicable]
