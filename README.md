# Projet Snake en C

## Description
Ce projet contient deux versions du jeu classique Snake, développées en C. La première version est conçue pour être exécutée dans un terminal, tandis que la deuxième utilise la bibliothèque SDL pour offrir une interface graphique riche.

## Fonctionnalités
- **Version Terminal** : Une version minimaliste du jeu Snake jouable directement dans le terminal.
- **Version SDL** : Une version améliorée avec une interface graphique, offrant une expérience de jeu visuelle.

## Prérequis
- Compilateur C (comme GCC)
- Bibliothèque SDL (pour la version avec interface graphique)

## Installation
### Version Terminal
1. Clonez le dépôt : `git clone https://github.com/Diekope/Projet-Snake.git`
2. Naviguez dans le dossier du projet.
3. Compilez le projet pour la version terminal : `gcc -o snake_terminal snake_terminal.c` (ajustez le nom du fichier source si nécessaire).
4. Exécutez le jeu : `./snake_terminal`.

### Version SDL
1. Assurez-vous d'avoir installé la bibliothèque SDL.
2. Suivez les étapes 1 et 2 comme ci-dessus.
3. Compilez le projet pour la version SDL : `gcc -o snake_sdl snake_sdl.c $(sdl2-config --cflags --libs)` (ajustez les noms de fichiers et les flags selon votre configuration).
4. Exécutez le jeu : `./snake_sdl`.

## Utilisation
- Utilisez les touches directionnelles pour déplacer le serpent.
- Collectez des points et évitez de heurter les murs ou la queue du serpent.
- Essayez de battre votre meilleur score !

## Contribution
Les contributions à ce projet sont les bienvenues. Si vous avez des suggestions ou des améliorations, n'hésitez pas à ouvrir une issue ou un pull request.

## Licence
[Inclure ici le type de licence si applicable]
