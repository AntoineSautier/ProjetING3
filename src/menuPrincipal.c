#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "menuPrincipal.h"
#include "partie.h"
#include <unistd.h>
#include <stdio.h>
#include <allegro5/allegro_image.h>



void menuPrincipal(Jeu *const pJeu) {
    ALLEGRO_EVENT event;
    bool fin = false;
    while (!fin) {
        al_wait_for_event(pJeu->fifo, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                return;
            }
            case ALLEGRO_EVENT_KEY_DOWN: {
                fin = toucheEnfonceeDansMenuPrincipal(pJeu, event.keyboard.keycode);
                break;
            }
            case ALLEGRO_EVENT_TIMER: {
                dessinerMenuPrincipal(pJeu);
                break;
            }
            default: {

            }
        }
    }
}

bool toucheEnfonceeDansMenuPrincipal(Jeu *pJeu, int const keycode) {
    switch (keycode) {
        case ALLEGRO_KEY_1: {
            nouvellePartie(pJeu);
            break;
        }
        case ALLEGRO_KEY_2: {
            if (pJeu->partie) {
                partie(pJeu);
            }
            break;
        }
        case ALLEGRO_KEY_3: {
            afficherEcranHistoire(pJeu);
            break;
        }

        case ALLEGRO_KEY_4: {
            afficherEcranTouche(pJeu);
            break;
        }
        case ALLEGRO_KEY_ESCAPE: {
            return true;
        }
        default: {
            printf("Touche de keycode %d enfoncee mais ignoree.\n", keycode);
        }
    }
    return false;
}
void afficherEcranHistoire(Jeu *pJeu) {
    ALLEGRO_BITMAP *imgHistoire = al_load_bitmap("../imagejeux/ecranhistoire.png");
    assert(imgHistoire);

    bool quitter = false;
    ALLEGRO_EVENT event;

    while (!quitter) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(
            imgHistoire,
            0, 0,
            al_get_bitmap_width(imgHistoire),
            al_get_bitmap_height(imgHistoire),
            0, 0,
            al_get_display_width(al_get_current_display()),
            al_get_display_height(al_get_current_display()),
            0
        );

        al_flip_display();

        al_wait_for_event(pJeu->fifo, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    quitter = true;
                }
            break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                quitter = true;
            break;
            default:
                break;
        }
    }

    al_destroy_bitmap(imgHistoire);
}
void afficherEcranTouche(Jeu *pJeu) {
    ALLEGRO_BITMAP *imgTouche = al_load_bitmap("../imagejeux/ecrantouche.png");
    assert(imgTouche);

    bool quitter = false;
    ALLEGRO_EVENT event;

    while (!quitter) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(
            imgTouche,
            0, 0,
            al_get_bitmap_width(imgTouche),
            al_get_bitmap_height(imgTouche),
            0, 0,
            al_get_display_width(al_get_current_display()),
            al_get_display_height(al_get_current_display()),
            0
        );
        al_flip_display();

        al_wait_for_event(pJeu->fifo, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    quitter = true;
                }
            break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                quitter = true;
            break;
            default:
                break;
        }
    }

    al_destroy_bitmap(imgTouche);
}
void dessinerMenuPrincipal(Jeu *const pJeu) {
    al_init_image_addon();
    static ALLEGRO_BITMAP *background = NULL;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Répertoire courant d'exécution : %s\n", cwd);
    }

    if (!background) {
        background = al_load_bitmap("../src/image/imagemenu2.png");
        if (!background) {
            fprintf(stderr, "Erreur de chargement de l'image du menu principal\n");
            return;
        }
    }

    al_clear_to_color(al_map_rgb(240, 240, 240));

    al_draw_scaled_bitmap(
        background,
        0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
        0, 0, WIDTH, HEIGHT, 0
    );

    float y = 50.0;
    //al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 100, y, 0, "1 : Nouvelle partie");
    y += al_get_font_line_height(pJeu->bangers40);

    if (pJeu->partie) {
        //al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 100, y, 0, "2 : Reprendre la partie");
        y += al_get_font_line_height(pJeu->bangers40);
    }

    //al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 100, y, 0, "3 : Règles du jeu");

    al_flip_display();
}

void detruireMenuPrincipal() {
    static ALLEGRO_BITMAP *background = NULL;
    if (background) {
        al_destroy_bitmap(background);
        background = NULL;
    }
}