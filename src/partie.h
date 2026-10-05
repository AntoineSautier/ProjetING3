#ifndef PARTIE_H
#define PARTIE_H
#include "jeu.h"
#include "carte.h"

void nouvellePartie(Jeu *pJeu);
void partie(Jeu *pJeu);
void dessinerPartie(Jeu *pJeu);
void partieToucheEnfoncee(Jeu *pJeu, int keycode);

void chargerTuiles(void);
void libererTuiles(void);
void dessinerCarte(Jeu *const pJeu, Carte *carte);
void dessinerPersonnage(Jeu *const pJeu, Personnage *perso);
void nouvellePartie(Jeu *pJeu);

void partie(Jeu *pJeu);

void dessinerPartie(Jeu *pJeu);

void partieToucheEnfoncee(Jeu *pJeu, int keycode);

bool estTuileBlocante(int tuile);

void naviguerInventaire(Jeu *pJeu, int direction) ;

void supprimerItemInventaire(Jeu *pJeu) ;

#endif