//
// Created by Tom Cailluyer on 09/04/2025.
//

#ifndef CARTE_H
#define CARTE_H


#define MAP_WIDTH 50
#define MAP_HEIGHT 50
#define TILE_SIZE 64


typedef struct {
    int grid[MAP_HEIGHT][MAP_WIDTH];
} Carte;


typedef struct {
    int x, y;
} Personnage;

int chargerCarte(Carte *carte, const char *filename);

#endif // CARTE_H