#ifndef JEU_H
#define JEU_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "carte.h"

#define WIDTH 2200
#define HEIGHT 1500
#define FPS 30
typedef struct {
    int slots[4];
    bool ouvert;
} Inventaire;
typedef struct {
    int slots[2];
    int actif;
} InventaireArme;
typedef struct {
    int nbJoueurs;
    Carte carte;
    Personnage perso;
    int cameraX;
    int cameraY;
} Partie;

typedef struct {
    ALLEGRO_DISPLAY *fenetre;
    ALLEGRO_EVENT_QUEUE *fifo;
    ALLEGRO_TIMER *timer;
    Partie *partie;
    ALLEGRO_FONT *bangers40;
    Inventaire inventaire;
    char messageMarchand[128];
    int tempsMessage;
    InventaireArme inventaireArme;
    bool finDeJeu;
} Jeu;

void initialiserJeu(Jeu *pJeu);

void libererMemoireJeu(Jeu *pJeu);

void mettreEnPause(Jeu *pJeu);

void creerPartie(Jeu *pJeu);

#endif