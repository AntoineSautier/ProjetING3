#include "src/jeu.h"
#include "src/menuPrincipal.h"

int main(void) {
    Jeu jeu = {0};
    initialiserJeu(&jeu);
    menuPrincipal(&jeu);
    libererMemoireJeu(&jeu);
    return 0;
}

