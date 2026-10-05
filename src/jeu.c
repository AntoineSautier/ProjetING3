#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "jeu.h"
#include <allegro5/allegro_image.h>
#include "partie.h"
#include "stdio.h"


void initialiserJeu(Jeu *const pJeu) {
    assert(al_init());
    assert(al_init_image_addon());
    assert(al_init_primitives_addon());
    assert(al_install_keyboard());
    (al_init_font_addon());
    assert(al_init_ttf_addon());
    al_set_new_window_position(0,0);
    pJeu->fenetre = al_create_display(WIDTH, HEIGHT);
    assert(pJeu->fenetre);
    al_set_window_title(pJeu->fenetre, "PIEW PIEW");

    pJeu->timer = al_create_timer(1.0 / FPS);
    assert(pJeu->timer);

    pJeu->fifo = al_create_event_queue();
    assert(pJeu->fifo);
    al_register_event_source(pJeu->fifo, al_get_display_event_source(pJeu->fenetre));
    al_register_event_source(pJeu->fifo, al_get_timer_event_source(pJeu->timer));
    al_register_event_source(pJeu->fifo, al_get_keyboard_event_source());

    pJeu->bangers40 = al_load_ttf_font("../fonts/Bangers-Regular.ttf", 40, 0);
    assert(pJeu->bangers40);

    chargerTuiles();

    al_start_timer(pJeu->timer);

    for (int i = 0; i < 4; i++) {
        pJeu->inventaire.slots[i] = 0;
    }
    pJeu->inventaire.ouvert = false;
    pJeu->messageMarchand[0] = '\0';
    pJeu->tempsMessage = 0;
    for (int i = 0; i < 2; i++) pJeu->inventaireArme.slots[i] = 0;
    pJeu->inventaireArme.actif = 0;
}

void libererMemoireJeu(Jeu *const pJeu) {
    printf("Liberation de la memoire.\n");
    al_destroy_display(pJeu->fenetre);
    pJeu->fenetre = NULL;
    al_destroy_event_queue(pJeu->fifo);
    pJeu->fifo = NULL;
    al_destroy_font(pJeu->bangers40);
    pJeu->bangers40 = NULL;
    al_destroy_timer(pJeu->timer);
    pJeu->timer = NULL;
    if (pJeu->partie != NULL) {
        free(pJeu->partie);
        pJeu->partie = NULL;
    }
}


    void mettreEnPause(Jeu *pJeu) {
        ALLEGRO_EVENT event;

        ALLEGRO_BITMAP *imgPause = al_load_bitmap("../imagejeux/ecranpause.png");
        assert(imgPause);

        al_clear_to_color(al_map_rgb(0, 0, 0));

        // 🔥 Dessine l'image en l'étirant pour remplir toute la fenêtre
        al_draw_scaled_bitmap(
            imgPause,
            0, 0,
            al_get_bitmap_width(imgPause),
            al_get_bitmap_height(imgPause),
            0, 0,
            al_get_display_width(al_get_current_display()),
            al_get_display_height(al_get_current_display()),
            0
        );

        al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 10, 10, 0, "PAUSE...");
        al_draw_text(pJeu->bangers40, al_map_rgb(255, 255, 255), 10, 10 + al_get_font_line_height(pJeu->bangers40), 0,
                     "Appuyez sur espace pour reprendre");

        al_flip_display();

        while (1) {
            al_wait_for_event(pJeu->fifo, &event);
            switch (event.type) {
                case ALLEGRO_EVENT_KEY_DOWN:
                    if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        al_destroy_bitmap(imgPause);
                        return;
                    }
                break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    al_destroy_bitmap(imgPause);
                return;
            }
        }
    }