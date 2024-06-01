#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "grid.h"


/**
 * Représentation d'un état de jeu contenant toutes les informations
 * relatives à cet état.
 * Cette classe est uniquement utilisée dans la classe Game_engine.
 */
class Game_state
{

public:

    friend class Game_engine;
    Game_state(int nb_rows, int nb_cols, int best_score=0);
    Game_state(const Game_state& state);
    ~Game_state();

    Grid *get_grid();
    Game_state *get_previous();

private:

    Grid *m_grid;
    Game_state *m_previous; // Pointeur vers l'état de jeu précédent.
    bool m_is_win;
    bool m_is_lose;
    int m_score;
    int m_best_score;
    int m_nb_moves; // Nombre de mouvements effectués depuis le début de la partie.
};


#endif // GAME_STATE_H
