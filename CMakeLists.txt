cmake_minimum_required(VERSION 3.10)
project(ProjetING3 C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetING3 main.c
        src/jeu.h
        src/jeu.c
        src/menuPrincipal.c
        src/menuPrincipal.h
        src/partie.c
        src/partie.h
        src/carte.c
        src/carte.h
        src/collect.c
        src/collect.h
)

IF(WIN32)
    find_package(PkgConfig QUIET)
    IF(PkgConfig_FOUND)
        pkg_check_modules(ALLEGRO_MSYS2 IMPORTED_TARGET
                allegro-5
                allegro_font-5
                allegro_ttf-5
                allegro_image-5
                allegro_primitives-5
                allegro_audio-5
                allegro_acodec-5)
    ENDIF()

    IF(ALLEGRO_MSYS2_FOUND)
        target_link_libraries(ProjetING3 PkgConfig::ALLEGRO_MSYS2)
    ELSE()
        # Configuration d'origine du projet (inchangée)
        target_link_libraries(ProjetING3 -lmingw32 -lallegro-5.0.10-monolith-md-debug)
    ENDIF()
ELSE()
    INCLUDE_DIRECTORIES(/usr/local/Cellar/allegro/5.2.6.0/include)
    LINK_DIRECTORIES(/usr/local/Cellar/allegro/5.2.6.0/lib)
    file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.6.0/lib/*.dylib")
    message("LIBRARIES = ${LIBRARIES}")
    TARGET_LINK_LIBRARIES(ProjetING3 ${LIBRARIES})
ENDIF(WIN32)