//
// Created by andre on 10/05/2025.
//

#include "collect.h"
#include "stdio.h"
#include "partie.h"




void recolterBois(Jeu *pJeu) {
    int x = pJeu->partie->perso.x;
    int y = pJeu->partie->perso.y;
    Carte *carte = &(pJeu->partie->carte);

    int objetsVoisins[] = {
        (x > 0) ? carte->grid[y][x - 1] : 0,
        (x < MAP_WIDTH - 1) ? carte->grid[y][x + 1] : 0,
        (y > 0) ? carte->grid[y - 1][x] : 0,
        (y < MAP_HEIGHT - 1) ? carte->grid[y + 1][x] : 0
    };

    for (int i = 0; i < 4; i++) {
        int objet = objetsVoisins[i];

        if (objet == 4 || objet == 5 ) {
            for (int j = 0; j < 4; j++) {
                if (pJeu->inventaire.slots[j] == 0) {
                    pJeu->inventaire.slots[j] = 14;
                    printf("🪵 Bois collecté !\n");

                    if (i == 0) carte->grid[y][x - 1] = 1;
                    if (i == 1) carte->grid[y][x + 1] = 1;
                    if (i == 2) carte->grid[y - 1][x] = 1;
                    if (i == 3) carte->grid[y + 1][x] = 1;
                    break;
                }
            }
            break;
        }
    }
}



