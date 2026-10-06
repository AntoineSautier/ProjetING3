#include "partie.h"
#include <allegro5/allegro_primitives.h>
#include "collect.h"
#include "stdio.h"

#include <allegro5/allegro_primitives.h>
#include "collect.h"
#include "stdio.h"
bool touches[ALLEGRO_KEY_MAX] = { false };
char messageTonneau[100] = "";

char messageCactus[100] = "";
int compteurDeplacement = 0;
const int delaiDeplacement = 5;
void dessinerInventaire(Jeu *pJeu);
static ALLEGRO_BITMAP *spritePerso = NULL;

static ALLEGRO_BITMAP *ecranFin = NULL;
int compteurFin = 0;
#define NB_TILES 20
int currentInventoryIndex = 0;
const int totalInventorySlots = 4;


ALLEGRO_BITMAP *tiles[NB_TILES];
ALLEGRO_BITMAP *redimensionner(ALLEGRO_BITMAP *src, int w, int h) {
    ALLEGRO_BITMAP *resized = al_create_bitmap(w, h);
    if (!resized) return NULL;

    ALLEGRO_BITMAP *target_backup = al_get_target_bitmap();
    al_set_target_bitmap(resized);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_draw_scaled_bitmap(src, 0, 0,
                          al_get_bitmap_width(src), al_get_bitmap_height(src),
                          0, 0, w, h, 0);
    al_set_target_bitmap(target_backup);

    al_destroy_bitmap(src);
    return resized;
}

void chargerTuiles(void) {
    const char* fichiers[] = {
        NULL,
        "../imagejeux/lyon_trottoir.png",  // 1 : trottoir (avant : desert1.PNG)
        "../imagejeux/lyon_route.png",     // 2 : route    (avant : desert2.PNG)
        "../imagejeux/lyon_rhone.png",     // 3 : Rhone    (avant : eau.PNG)
        "../imagejeux/lyon_arbre1.png",    // 4 : platane  (avant : cactus1.PNG)
        "../imagejeux/lyon_arbre2.png",    // 5 : tilleul  (avant : cactus2.PNG)
        "../imagejeux/herbe1.PNG",
        "../imagejeux/cactusbleu.PNG",
        "../imagejeux/pngcactus.PNG",
        "../imagejeux/totem.PNG",
        "../imagejeux/ton1.PNG",
        "../imagejeux/ton2.PNG",
        "../imagejeux/lasso.png",
        "../imagejeux/revol.png",
        "../imagejeux/bois.png",
        "../imagejeux/lyon_toit.png",      // 15 : immeuble, toit en tuiles (bloquant)
        "../imagejeux/lyon_citroen.png",   // 16 : ancien garage Citroen / ECE (bloquant)
        "../imagejeux/lyon_herbe.png",     // 17 : herbe, parcs et berges
        "../imagejeux/lyon_ece.png",       // 18 : enseigne ECE (bloquant)
        "../imagejeux/lyon_ardoise.png",   // 19 : immeuble, toit en ardoise (bloquant)
    };

    for (int i = 1; i < NB_TILES; i++) {
        if (!fichiers[i]) {
            fprintf(stderr, "❌ Tuile %d non définie !\n", i);
            tiles[i] = NULL;
            continue;
        }

        ALLEGRO_BITMAP *original = al_load_bitmap(fichiers[i]);
        if (!original) {
            fprintf(stderr, "❌ Image introuvable : %s\n", fichiers[i]);
            tiles[i] = NULL;
            continue;
        }

        printf("✅ Tuile %d chargee avec succes : %s\n", i, fichiers[i]); // Message de débogage

        int w = TILE_SIZE;
        int h = TILE_SIZE;
        if (i == 4 || i == 5 || i == 6 || i == 8) {
            w *= 2;
            h *= 2;
        } else if (i == 8) {
            w *= 3;
            h *= 3;
        }
        tiles[i] = redimensionner(original, w, h);

        if (!tiles[i]) {
            fprintf(stderr, "❌ Erreur lors du redimensionnement de la tuile %d\n", i);
        }
    }
}

void dessinerPartie(Jeu *const pJeu) {
    if (pJeu->finDeJeu && ecranFin) {
        al_draw_scaled_bitmap(
            ecranFin,
            0, 0,
            al_get_bitmap_width(ecranFin),
            al_get_bitmap_height(ecranFin),
            0, 0,
            WIDTH, HEIGHT,
            0
        );
        al_flip_display();
        return;
    }
    al_clear_to_color(al_map_rgb(0, 0, 0));
    dessinerCarte(pJeu, &(pJeu->partie->carte));
    dessinerPersonnage(pJeu, &(pJeu->partie->perso));
    //al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 0, 0, 0, "Partie en cours !");
    //al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 0, al_get_font_line_height(pJeu->bangers40), 0,
                 //"ECHAP : Retour au menu principal");
    //al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 0, al_get_font_line_height(pJeu->bangers40) * 2, 0,
                // "ESPACE : Mettre en pause");
    dessinerPersonnage(pJeu, &(pJeu->partie->perso));
    dessinerInventaire(pJeu);
    if (pJeu->tempsMessage > 0) {
        al_draw_filled_rectangle(100, HEIGHT - 120, WIDTH - 100, HEIGHT - 60, al_map_rgb(0, 0, 0));
        al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT - 100, ALLEGRO_ALIGN_CENTER, pJeu->messageMarchand);
    }
    // Afficher le message si le personnage est sur un cactus
    if (messageCactus[0] != '\0') {
        al_draw_filled_rectangle(100, HEIGHT - 180, WIDTH - 100, HEIGHT - 150, al_map_rgb(0, 0, 0));
        al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT - 170, ALLEGRO_ALIGN_CENTER, messageCactus);
    }
    if (messageTonneau[0] != '\0') {
        al_draw_filled_rectangle(100, HEIGHT - 180, WIDTH - 100, HEIGHT - 150, al_map_rgb(0, 0, 0));
        al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT - 170, ALLEGRO_ALIGN_CENTER, messageTonneau);
    }
    int baseX = WIDTH - 2 * TILE_SIZE - 50;
    int y = HEIGHT - TILE_SIZE - 120;


    for (int i = 0; i < 2; i++) {
        int x = baseX + i * (TILE_SIZE + 10);
        ALLEGRO_COLOR fond = (i == pJeu->inventaireArme.actif) ? al_map_rgb(255, 0, 0) : al_map_rgb(180, 180, 180);

        al_draw_filled_rectangle(x, y, x + TILE_SIZE, y + TILE_SIZE, fond);
        al_draw_rectangle(x, y, x + TILE_SIZE, y + TILE_SIZE, al_map_rgb(0, 0, 0), 2);

        int id = pJeu->inventaireArme.slots[i];
        if (id >= 1 && id < NB_TILES && tiles[id]) {
            al_draw_bitmap(tiles[id], x, y, 0);
        }
    }
    al_flip_display();
}
int chargerCarte(Carte *carte, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (fscanf(fp, "%d", &carte->grid[i][j]) != 1) {
                fprintf(stderr, "Erreur de lecture à la ligne %d, colonne %d\n", i, j);
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);
    return 1;
}
void mettreAJourCamera(Jeu *pJeu);
void creerPartie(Jeu *const pJeu) {
    assert(pJeu->partie == NULL);
    pJeu->partie = calloc(1, sizeof(Partie));


    if (!chargerCarte(&(pJeu->partie->carte), "../src/carte.txt")) {
        fprintf(stderr, "Erreur lors du chargement de la carte.\n");

        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1)
                    pJeu->partie->carte.grid[i][j] = 2;
                else
                    pJeu->partie->carte.grid[i][j] = 1;
            }
        }
    }


    pJeu->partie->perso.x = SPAWN_X;
    pJeu->partie->perso.y = SPAWN_Y;
    mettreAJourCamera(pJeu); // centre la camera sur le point de depart
}
void mettreAJourCamera(Jeu *pJeu) {

    int newCameraX = pJeu->partie->perso.x * TILE_SIZE - WIDTH / 2;
    int newCameraY = pJeu->partie->perso.y * TILE_SIZE - HEIGHT / 2;


    if (newCameraX < 0) newCameraX = 0;
    if (newCameraY < 0) newCameraY = 0;

    int maxCameraX = MAP_WIDTH * TILE_SIZE - WIDTH;
    int maxCameraY = MAP_HEIGHT * TILE_SIZE - HEIGHT;
    if (newCameraX > maxCameraX) newCameraX = maxCameraX;
    if (newCameraY > maxCameraY) newCameraY = maxCameraY;

    pJeu->partie->cameraX = newCameraX;
    pJeu->partie->cameraY = newCameraY;
}
void dessinerCarte(Jeu *const pJeu, Carte *carte) {
    int cameraX = pJeu->partie->cameraX;
    int cameraY = pJeu->partie->cameraY;

    int startCol = cameraX / TILE_SIZE;
    int startRow = cameraY / TILE_SIZE;
    int endCol = (cameraX + WIDTH) / TILE_SIZE;
    int endRow = (cameraY + HEIGHT) / TILE_SIZE;


    if (endCol >= MAP_WIDTH) endCol = MAP_WIDTH - 1;
    if (endRow >= MAP_HEIGHT) endRow = MAP_HEIGHT - 1;

    for (int i = startRow; i <= endRow; i++) {
        for (int j = startCol; j <= endCol; j++) {
            int tile = carte->grid[i][j];
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE - cameraY;


            if (tiles[1]) {
                al_draw_bitmap(tiles[1], screenX, screenY, 0);
            }


            if ((tile == 2 || tile == 3 || (tile >= 15 && tile < NB_TILES)) && tiles[tile]) {
                al_draw_bitmap(tiles[tile], screenX, screenY, 0);
            }
        }
    }

    for (int i = startRow; i <= endRow; i++) {
        for (int j = startCol; j <= endCol; j++) {
            int tile = carte->grid[i][j];
            if (tile == 4 || tile == 5 || tile == 6 || tile == 7 || tile == 8|| tile == 9|| tile == 10|| tile == 11|| tile == 12|| tile == 13|| tile == 14) {
                int screenX = j * TILE_SIZE - cameraX;
                int screenY = i * TILE_SIZE - cameraY;
                int h = al_get_bitmap_height(tiles[tile]);
                al_draw_bitmap(tiles[tile], screenX, screenY - (h - TILE_SIZE), 0);
            }

        }
    }
}


void dessinerPersonnage(Jeu *const pJeu, Personnage *perso) {
    int cameraX = pJeu->partie->cameraX;
    int cameraY = pJeu->partie->cameraY;
    int x = perso->x * TILE_SIZE - cameraX;
    int y = perso->y * TILE_SIZE - cameraY;

    if (spritePerso) {
        al_draw_scaled_bitmap(
            spritePerso,
            0, 0,
            al_get_bitmap_width(spritePerso),
            al_get_bitmap_height(spritePerso),
            x, y,
            TILE_SIZE * 2, TILE_SIZE * 2, // 💥 ici : 2 fois plus grand
            0
        );
    } else {
        al_draw_filled_rectangle(x, y, x + TILE_SIZE * 2, y + TILE_SIZE * 2, al_map_rgb(255, 0, 0));
    }
}
void partie(Jeu *pJeu) {
    assert(pJeu->partie != NULL);

    spritePerso = al_load_bitmap("../imagejeux/spritedetourer.png");
    if (!spritePerso) {
        printf("Erreur chargement sprite personnage\n");
        exit(1);
    } else {
        printf("Sprite personnage chargé avec succès\n");
    }
    ecranFin = al_load_bitmap("../imagejeux/ecranfin.png");
    if (!ecranFin) {
        fprintf(stderr, "Erreur chargement ecranfin.png\n");
        exit(1);
    }


    ALLEGRO_EVENT event;
    while (1) {
        al_wait_for_event(pJeu->fifo, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                printf("Retour au menu");
                return;
            }
            case ALLEGRO_EVENT_KEY_DOWN:
                touches[event.keyboard.keycode] = true;
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    printf("RETOUR");
                    return;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_E) {
                    pJeu->inventaire.ouvert = !pJeu->inventaire.ouvert;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_S) {
                    supprimerItemInventaire(pJeu);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_Q) {
                    if (pJeu->inventaire.ouvert) {
                        naviguerInventaire(pJeu, -1);
                    }
                } else if (event.keyboard.keycode == ALLEGRO_KEY_D) {
                    if (pJeu->inventaire.ouvert) {
                        naviguerInventaire(pJeu, 1);
                    }
                } else if (event.keyboard.keycode == ALLEGRO_KEY_C) {
                    recolterBois(pJeu);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                    int x = pJeu->partie->perso.x;
                    int y = pJeu->partie->perso.y;
                    Carte *carte = &(pJeu->partie->carte);

                    bool voisinMarchand =
                        (x > 0 && carte->grid[y][x - 1] == 8) ||
                        (x < MAP_WIDTH - 1 && carte->grid[y][x + 1] == 8) ||
                        (y > 0 && carte->grid[y - 1][x] == 8) ||
                        (y < MAP_HEIGHT - 1 && carte->grid[y + 1][x] == 8);

                    if (!voisinMarchand) break;

                    int cactusBleus = 0;
                    for (int i = 0; i < 4; i++) {
                        if (pJeu->inventaire.slots[i] == 7)
                            cactusBleus++;
                    }

                    if (cactusBleus >= 3) {
                        int enleves = 0;
                        for (int i = 0; i < 4 && enleves < 3; i++) {
                            if (pJeu->inventaire.slots[i] == 7) {
                                pJeu->inventaire.slots[i] = 0;
                                enleves++;
                            }
                        }
                        for (int i = 0; i < 4; i++) {
                            if (pJeu->inventaire.slots[i] == 0) {
                                pJeu->inventaire.slots[i] = 9;
                                break;
                            }
                        }
                        snprintf(pJeu->messageMarchand, sizeof(pJeu->messageMarchand),
                                 "Merci ! Voici le totem du cactus squelette !");
                    } else {
                        snprintf(pJeu->messageMarchand, sizeof(pJeu->messageMarchand),
                                 "Tu n’as pas assez de cactus bleus continue d'explorer le desert afin d'en trouver plus...");
                    }

                    pJeu->tempsMessage = FPS * 3;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_T) {
                    pJeu->inventaireArme.actif = (pJeu->inventaireArme.actif + 1) % 2;
                    printf(" Arme active : slot %d (id %d)\n",
                        pJeu->inventaireArme.actif,
                        pJeu->inventaireArme.slots[pJeu->inventaireArme.actif]);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_Y) {
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

                        if (objet == 10) {
                            for (int j = 0; j < 2; j++) {
                                if (pJeu->inventaireArme.slots[j] == 0) {
                                    pJeu->inventaireArme.slots[j] = 12;
                                    printf("🪢 Lasso reçu !\n");

                                    if (i == 0) carte->grid[y][x - 1] = 1;
                                    if (i == 1) carte->grid[y][x + 1] = 1;
                                    if (i == 2) carte->grid[y - 1][x] = 1;
                                    if (i == 3) carte->grid[y + 1][x] = 1;
                                    break;
                                }
                            }
                        }

                        if (objet == 11) {
                            for (int j = 0; j < 2; j++) {
                                if (pJeu->inventaireArme.slots[j] == 0) {
                                    pJeu->inventaireArme.slots[j] = 13;
                                    printf("🔫 Revolver reçu !\n");

                                    if (i == 0) carte->grid[y][x - 1] = 1;
                                    if (i == 1) carte->grid[y][x + 1] = 1;
                                    if (i == 2) carte->grid[y - 1][x] = 1;
                                    if (i == 3) carte->grid[y + 1][x] = 1;
                                    break;
                                }
                            }
                        }
                    }
                }else if (event.keyboard.keycode == ALLEGRO_KEY_O) {
                    if (!pJeu->finDeJeu) {
                        for (int i = 0; i < 4; i++) {
                            if (pJeu->inventaire.slots[i] == 9) {
                                pJeu->finDeJeu = true;
                                compteurFin = FPS * 10;
                                printf("🎉 Fin du jeu activée !\n");
                                break;
                            }
                        }

                    } else {
                        pJeu->inventaire.ouvert = !pJeu->inventaire.ouvert;
                    }
                }

                break;

            case ALLEGRO_EVENT_KEY_UP:
                touches[event.keyboard.keycode] = false;
                break;
            case ALLEGRO_EVENT_TIMER: {
                if (pJeu->finDeJeu) {
                    compteurFin--;
                    if (compteurFin <= 0) {
                        printf("💤 Fin terminée, retour au menu\n");
                        return;
                    }
                }
                if (pJeu->tempsMessage > 0) {
                    pJeu->tempsMessage--;
                }
                compteurDeplacement++;
                if (compteurDeplacement >= delaiDeplacement) {
                    compteurDeplacement = 0;
                    int nouveauX = pJeu->partie->perso.x;
                    int nouveauY = pJeu->partie->perso.y;

                    if (touches[ALLEGRO_KEY_UP])    nouveauY--;
                    if (touches[ALLEGRO_KEY_DOWN])  nouveauY++;
                    if (touches[ALLEGRO_KEY_LEFT])  nouveauX--;
                    if (touches[ALLEGRO_KEY_RIGHT]) nouveauX++;

                    if (nouveauX >= 0 && nouveauX < MAP_WIDTH && nouveauY >= 0 && nouveauY < MAP_HEIGHT) {
                        int tuile = pJeu->partie->carte.grid[nouveauY][nouveauX];

                        if (!estTuileBlocante(tuile)) {
                            pJeu->partie->perso.x = nouveauX;
                            pJeu->partie->perso.y = nouveauY;
                            mettreAJourCamera(pJeu);

                            int px = pJeu->partie->perso.x;
                            int py = pJeu->partie->perso.y;
                            Carte *carte = &(pJeu->partie->carte);

                            if (carte->grid[py][px] == 7) {
                                for (int i = 0; i < 4; i++) {
                                    if (pJeu->inventaire.slots[i] == 0) {
                                        pJeu->inventaire.slots[i] = 7;
                                        carte->grid[py][px] = 1;
                                        printf(" Ajout cactus bleu dans slot[%d] → valeur = %d\n", i, pJeu->inventaire.slots[i]);
                                        break;
                                    }
                                }
                            }
                            if (carte->grid[py][px] == 14) {
                                for (int i = 0; i < 4; i++) {
                                    if (pJeu->inventaire.slots[i] == 0) {
                                        pJeu->inventaire.slots[i] = 14;
                                        carte->grid[py][px] = 1;
                                        printf(" Ajout bois dans slot[%d] → valeur = %d\n", i, pJeu->inventaire.slots[i]);
                                        break;
                                    }
                                }
                            }
                        } else {
                            printf("Collision avec une tuile bloquante : %d\n", tuile);
                        }
                    }
                }
                int px = pJeu->partie->perso.x;
                int py = pJeu->partie->perso.y;
                Carte *carte = &(pJeu->partie->carte);

                if (carte->grid[py][px] == 4 || carte->grid[py][px] == 5 || carte->grid[py][px] == 6) {
                    snprintf(messageCactus, sizeof(messageCactus), "APPUYER SUR C POUR COUPER");
                } else {
                    messageCactus[0] = '\0'; // Efface le message si le personnage n'est pas sur un cactus
                }
                int ax = pJeu->partie->perso.x;
                int ay = pJeu->partie->perso.y;
                Carte *pcarte = &(pJeu->partie->carte);

                bool voisinTonneau =
                    (px > 0 && (carte->grid[py][px - 1] == 10 || carte->grid[py][px - 1] == 11)) ||
                    (px < MAP_WIDTH - 1 && (carte->grid[py][px + 1] == 10 || carte->grid[py][px + 1] == 11)) ||
                    (py > 0 && (carte->grid[py - 1][px] == 10 || carte->grid[py - 1][px] == 11)) ||
                    (py < MAP_HEIGHT - 1 && (carte->grid[py + 1][px] == 10 || carte->grid[py + 1][px] == 11));

                if (voisinTonneau) {
                    snprintf(messageTonneau, sizeof(messageTonneau), "APPUYER SUR Y POUR OUVRIR LE TONNEAU");
                } else {
                    messageTonneau[0] = '\0'; // Efface le message si le personnage n'est pas à côté d'un tonneau
                }
                dessinerPartie(pJeu);
                break;
            }
        }
    }
}


void nouvellePartie(Jeu *const pJeu) {
    creerPartie(pJeu);
    partie(pJeu);
}

bool estTuileBlocante(int tuile) {
    // Liste des tuiles considérées comme bloquantes
    return tuile==3|| tuile == 6 ||tuile==10||tuile==11|| tuile==15 || tuile==16 || tuile==18 || tuile==19;
}

void partieToucheEnfoncee(Jeu *const pJeu, const int keycode) {
    int nouveauX = pJeu->partie->perso.x;
    int nouveauY = pJeu->partie->perso.y;

    switch (keycode) {
        case ALLEGRO_KEY_UP:    nouveauY--; break;
        case ALLEGRO_KEY_DOWN:  nouveauY++; break;
        case ALLEGRO_KEY_LEFT:  nouveauX--; break;
        case ALLEGRO_KEY_RIGHT: nouveauX++; break;
        case ALLEGRO_KEY_SPACE:
            mettreEnPause(pJeu);
            return;
        case ALLEGRO_KEY_E:
            pJeu->inventaire.ouvert = !pJeu->inventaire.ouvert;
            return;
        case ALLEGRO_KEY_ESCAPE:
            printf("RETOUR");
            return;
        case ALLEGRO_KEY_R: {
            int x = pJeu->partie->perso.x;
            int y = pJeu->partie->perso.y;
            Carte *carte = &(pJeu->partie->carte);

            bool voisinMarchand =
                (x > 0 && carte->grid[y][x - 1] == 8) ||
                (x < MAP_WIDTH - 1 && carte->grid[y][x + 1] == 8) ||
                (y > 0 && carte->grid[y - 1][x] == 8) ||
                (y < MAP_HEIGHT - 1 && carte->grid[y + 1][x] == 8);

            if (!voisinMarchand) return;

            int cactusBleus = 0;
            for (int i = 0; i < 4; i++) {
                if (pJeu->inventaire.slots[i] == 7)
                    cactusBleus++;
            }

            if (cactusBleus >= 3) {
                int enleves = 0;
                for (int i = 0; i < 4 && enleves < 3; i++) {
                    if (pJeu->inventaire.slots[i] == 7) {
                        pJeu->inventaire.slots[i] = 0;
                        enleves++;
                    }
                }
                for (int i = 0; i < 4; i++) {
                    if (pJeu->inventaire.slots[i] == 0) {
                        pJeu->inventaire.slots[i] = 9;
                        break;
                    }
                }
                snprintf(pJeu->messageMarchand, sizeof(pJeu->messageMarchand),
                         "Merci ! Voici le totem du cactus squelette !");
            } else {
                snprintf(pJeu->messageMarchand, sizeof(pJeu->messageMarchand),
                         "Tu n’as pas assez de cactus bleus continue d'explorer le desert afin d'en trouver plus...");
            }

            pJeu->tempsMessage = FPS * 3;
            return;
        }
        case ALLEGRO_KEY_T:
            pJeu->inventaireArme.actif = (pJeu->inventaireArme.actif + 1) % 2;
            printf(" Arme active : slot %d (id %d)\n",
                pJeu->inventaireArme.actif,
                pJeu->inventaireArme.slots[pJeu->inventaireArme.actif]);
            return;
        case ALLEGRO_KEY_Y: {
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

                if (objet == 10) {
                    for (int j = 0; j < 2; j++) {
                        if (pJeu->inventaireArme.slots[j] == 0) {
                            pJeu->inventaireArme.slots[j] = 12;
                            printf("🪢 Lasso reçu !\n");

                            if (i == 0) carte->grid[y][x - 1] = 1;
                            if (i == 1) carte->grid[y][x + 1] = 1;
                            if (i == 2) carte->grid[y - 1][x] = 1;
                            if (i == 3) carte->grid[y + 1][x] = 1;
                            return;
                        }
                    }
                }

                if (objet == 11) {
                    for (int j = 0; j < 2; j++) {
                        if (pJeu->inventaireArme.slots[j] == 0) {
                            pJeu->inventaireArme.slots[j] = 13;
                            printf("🔫 Revolver reçu !\n");

                            if (i == 0) carte->grid[y][x - 1] = 1;
                            if (i == 1) carte->grid[y][x + 1] = 1;
                            if (i == 2) carte->grid[y - 1][x] = 1;
                            if (i == 3) carte->grid[y + 1][x] = 1;
                            return;
                        }
                    }
                }
            }
            return;
        }
        default:
            printf("Touche ignorée : %d\n", keycode);
            return;
    }

    if (nouveauX >= 0 && nouveauX < MAP_WIDTH && nouveauY >= 0 && nouveauY < MAP_HEIGHT) {
        int tuile = pJeu->partie->carte.grid[nouveauY][nouveauX];

        if (!estTuileBlocante(tuile)) {
            pJeu->partie->perso.x = nouveauX;
            pJeu->partie->perso.y = nouveauY;
            mettreAJourCamera(pJeu);
        } else {
            printf("Collision avec une tuile bloquante : %d\n", tuile);
        }
    }

    int px = pJeu->partie->perso.x;
    int py = pJeu->partie->perso.y;
    Carte *carte = &(pJeu->partie->carte);

    if (carte->grid[py][px] == 7) {
        for (int i = 0; i < 4; i++) {
            if (pJeu->inventaire.slots[i] == 0) {
                pJeu->inventaire.slots[i] = 7;
                carte->grid[py][px] = 1;
                printf(" Ajout cactus bleu dans slot[%d] → valeur = %d\n", i, pJeu->inventaire.slots[i]);
                break;
            }
        }
    }
}
void naviguerInventaire(Jeu *pJeu, int direction) {
    currentInventoryIndex += direction;

    if (currentInventoryIndex < 0) {
        currentInventoryIndex = totalInventorySlots - 1;
    } else if (currentInventoryIndex >= totalInventorySlots) {
        currentInventoryIndex = 0;
    }

    printf("Slot actuel de l'inventaire : %d\n", currentInventoryIndex);
}
void dessinerInventaire(Jeu *pJeu) {
    if (!pJeu->inventaire.ouvert) return;

    int x = WIDTH / 2 - 2 * TILE_SIZE;
    int y = HEIGHT / 2 - TILE_SIZE;

    for (int i = 0; i < totalInventorySlots; i++) {
        int slotX = x + i * (TILE_SIZE + 8);
        ALLEGRO_COLOR fond = (i == currentInventoryIndex) ? al_map_rgb(255, 0, 0) : al_map_rgb(197, 209, 115); // Mettre en évidence le slot actuel
        al_draw_filled_rectangle(slotX, y, slotX + TILE_SIZE, y + TILE_SIZE, fond);
        al_draw_rectangle(slotX, y, slotX + TILE_SIZE, y + TILE_SIZE, al_map_rgb(0, 0, 0), 2);

        int id = pJeu->inventaire.slots[i];
        printf(" slot[%d] = %d, tile ok ? %s\n", i, id, (tiles[id] ? "OUI" : "NON"));

        if (id >= 1 && id < NB_TILES && tiles[id]) {
            al_draw_bitmap(tiles[id], slotX, y, 0);
        }
    }
}

void supprimerItemInventaire(Jeu *pJeu) {
    if (pJeu->inventaire.ouvert) {
        if (pJeu->inventaire.slots[currentInventoryIndex] != 0) {
            pJeu->inventaire.slots[currentInventoryIndex] = 0;
            printf("Item supprimé du slot %d\n", currentInventoryIndex);
        } else {
            printf("Aucun item à supprimer dans le slot %d\n", currentInventoryIndex);
        }
    } else {
        printf("L'inventaire n'est pas ouvert\n");
    }
}


void libererTuiles() {
    for (int i = 1; i <= 6; i++) {
        if (tiles[i]) {
            al_destroy_bitmap(tiles[i]);
        }
    }
}