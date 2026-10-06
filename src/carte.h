#ifndef CARTE_H
#define CARTE_H


#define MAP_WIDTH 50
#define MAP_HEIGHT 50
#define TILE_SIZE 64

// Point de depart du joueur : devant l'entree de l'ECE Lyon (colonne, ligne)
#define SPAWN_X 29
#define SPAWN_Y 22


typedef struct {
    int grid[MAP_HEIGHT][MAP_WIDTH];
} Carte;


typedef struct {
    int x, y;
} Personnage;

int chargerCarte(Carte *carte, const char *filename);

#endif // CARTE_H