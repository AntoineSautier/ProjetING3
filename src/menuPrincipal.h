#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
#include "jeu.h"

void menuPrincipal(Jeu *pJeu);

void dessinerMenuPrincipal(Jeu *pJeu);

bool toucheEnfonceeDansMenuPrincipal(Jeu *pJeu, int keycode);

void detruireMenuPrincipal(void);

void afficherEcranHistoire(Jeu *pJeu);

void afficherEcranTouche(Jeu *pJeu);

#endif