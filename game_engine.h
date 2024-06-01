#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <random>
#include "game_state.h"


/**
 * Moteur de jeu du 2048
 *
 * Il contient l'état de jeu actuel (état de la partie en cours) et le met à jour suivant les actions de jeu
 * effectuées par l'utilisateur.
 * Tout l'historique des états de jeu depuis le début de la partie est conservé pour pouvoir revenir en arrière.
 * Des informations sur l'état de la partie en cours peuvent être récupérées.
 */
class Game_engine
{

public:

    Game_engine(int nb_rows, int nb_cols, int win_value);
    ~Game_engine();

    static const int TOP=0, DOWN=1, RIGHT=2, LEFT=3; // Directions possibles pour un déplacement des tuiles.

    void move(int direction); // Déplace les tuiles dans la direction indiquée.
    void undo(); // Revient à l'état précédent.
    void reset(int nb_rows, int nb_cols, int win_value);

    Grid &get_grid() const;
    int get_idx_after_move(int idx) const;
    int get_nb_rows() const;
    int get_nb_cols() const;
    int get_win_value() const;
    bool is_win() const;
    bool is_lose() const;
    int get_score() const;
    int get_best_score() const;
    int get_nb_moves() const;

    friend ostream& operator<<(ostream& out, const Game_engine& game);


protected:

    void delete_history();
    void add_new_tile();
    void reset_indices_after_move();
    void move_tile(int i0, int j0, int i1, int j1);
    bool exist_same_adjacent_tiles() const;

    Game_state *m_state; // Pointeur vers l'état de jeu actuel.
    Grid *m_grid; // Pointeur vers la grille de m_state.
    vector<int> m_indices_after_move; // Liste des nouveaux indices des tuiles dans la grille après déplacement.
    int m_win_value; // Valeur de la tuile gagnante. (exemple: 2048)
    const double m_proba_get_4 = 0.1; // Probabilité de voir apparaître une tuile de valeur 4 au lieu de 2.

    bool m_state_changed;
    mt19937 m_rand_generator;
};


#endif // GAME_ENGINE_H
