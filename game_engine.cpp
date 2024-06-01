#include <vector>
#include <random>
#include <ctime>
#include "game_engine.h"

using namespace std;


Game_engine::Game_engine(int nb_rows, int nb_cols, int win_value):
    m_state(new Game_state(nb_rows, nb_cols)),
    m_grid(m_state->m_grid),
    m_indices_after_move(vector<int>(m_grid->get_size(), 0)),
    m_win_value(win_value),
    m_state_changed(false),
    m_rand_generator(time(0))
{
    reset_indices_after_move();
    add_new_tile();
    add_new_tile();
}


Game_engine::~Game_engine()
{
    delete_history();
}


/**
 * Réinitialise complètement la partie.
 */
void Game_engine::reset(int nb_rows, int nb_cols, int win_value)
{
    int best_score = m_state->m_best_score;
    delete_history();
    m_state = new Game_state(nb_rows, nb_cols, best_score);
    m_grid = m_state->m_grid;
    reset_indices_after_move();
    m_win_value = win_value;
    m_state_changed = false;
    add_new_tile();
    add_new_tile();
}


/**
 * Supprime tout l'historique des états de jeu.
 */
void Game_engine::delete_history()
{
    // On supprime en partant de l'état actuel en remontant jusqu'à l'état du début de la partie.
    Game_state *current = this->m_state;
    while (current != 0)
    {
        Game_state *previous = current->m_previous;
        delete current;
        current = previous;
    }
}


/**
 * Déplace les tuiles du grid dans la direction donnée
 * et met à jour m_indices_after_move, la liste des nouveaux indices
 * de chaque tuile dans la grille visible sur l'affichage.
 */
void Game_engine::move(int direction)
{
    // On fait une copie de l'état précédent
    // puis on applique le déplacement sur cette copie.
    Game_state *previous = m_state;
    m_state = new Game_state(*m_state);
    m_state->m_previous = previous;
    m_grid = m_state->m_grid;

    reset_indices_after_move();
    m_state_changed = false;

    // Déplacement pour TOP ou DOWN
    if (direction == Game_engine::TOP || direction == Game_engine::DOWN)
    {
        for (int j=0; j < m_grid->get_nb_cols(); j++)
        {
            // Indice de la ligne vers laquelle la prochaine tuile non vide trouvée
            // se déplacera, éventuellement en fusionnant.
            // Pour le cas DOWN, on parcours les lignes en partant de la fin.
            int next_insert_i = (direction == Game_engine::TOP) ? 0 : m_grid->get_nb_rows() - 1;
            for (int i=1; i < m_grid->get_nb_rows(); i++)
            {
                int i1 = (direction == Game_engine::TOP) ? i : m_grid->get_nb_rows() - 1 - i;
                // Déplacement de la tuile non vide en (i1,j) vers (next_insert_i,j).
                if (m_grid->get(i1, j) != 0)
                {
                    if (m_grid->get(next_insert_i, j) == 0) // Si la case est vide
                    {
                        move_tile(i1, j, next_insert_i, j);
                    }
                    else if (m_grid->get(next_insert_i, j) == m_grid->get(i1, j)) // S'il va y avoir fusion
                    {
                        move_tile(i1, j, next_insert_i, j);
                        if (direction == Game_engine::TOP) next_insert_i++;
                        else next_insert_i--;
                    }
                    else // S'il n'y aura pas fusion
                    {
                        if (direction == Game_engine::TOP) next_insert_i++;
                        else next_insert_i--;
                        move_tile(i1, j, next_insert_i, j);
                    }
                }
            }
        }
    }

    // Déplacement pour RIGHT ou LEFT
    if (direction == Game_engine::RIGHT || direction == Game_engine::LEFT)
    {
        for (int i=0; i < m_grid->get_nb_rows(); i++)
        {
            int next_insert_j = (direction == Game_engine::LEFT) ? 0 : m_grid->get_nb_cols() - 1;
            for (int j=1; j < m_grid->get_nb_cols(); j++)
            {
                int j1 = (direction == Game_engine::LEFT) ? j : m_grid->get_nb_cols() - 1 - j;
                if (m_grid->get(i, j1) != 0)
                {
                    if (m_grid->get(i, next_insert_j) == 0)
                    {
                        move_tile(i, j1, i, next_insert_j);
                    }
                    else if (m_grid->get(i, next_insert_j) == m_grid->get(i, j1))
                    {
                        move_tile(i, j1, i, next_insert_j);
                        if (direction == Game_engine::LEFT) next_insert_j++;
                        else next_insert_j--;
                    }
                    else
                    {
                        if (direction == Game_engine::LEFT) next_insert_j++;
                        else next_insert_j--;
                        move_tile(i, j1, i, next_insert_j);
                    }
                }
            }
        }
    }

    if (m_state_changed)
    {
        add_new_tile();
        m_state->m_nb_moves++;
    }
    else
        undo(); // Retour à l'état précédent si aucun déplacement n'a été réalisé.
}


void Game_engine::reset_indices_after_move()
{
    for (int i = 0; i < (int)m_indices_after_move.size(); i++)
        m_indices_after_move[i] = i;
}


/**
 * Déplace la tuile dans grid de (i0, j0) à (i1, j1)
 * avec éventuellement une fusion si (i1, j1) n'est pas vide.
 */
void Game_engine::move_tile(int i0, int j0, int i1, int j1)
{
    if (!(i0 == i1 && j0 == j1))
    {
        int tile_value = m_grid->get(i0, j0) + m_grid->get(i1, j1);

        // Mise à jour de is_win et score.
        if (tile_value == m_win_value)
            m_state->m_is_win = true;
        if (m_grid->get(i1, j1) > 0) // S'il y a eu une fusion.
        {
            m_state->m_score += tile_value;
            if (m_state->m_score > m_state->m_best_score)
                m_state->m_best_score = m_state->m_score;
        }

        // Déplacement de la tuile
        m_grid->set(i1, j1, tile_value);
        m_grid->set(i0, j0, 0);
        m_indices_after_move[m_grid->get_idx(i0, j0)] = m_grid->get_idx(i1, j1);

        m_state_changed = true;
    }
}


/**
 * Ajoute une tuile de valeur 2 ou 4 au hasard sur une case vide tirée au sort.
 */
void Game_engine::add_new_tile()
{
    // Construction de la liste des indices des tuiles vides.

    vector<int> empty_tiles(0);
    for (int i=0; i < m_grid->get_nb_rows(); i++)
        for (int j=0; j < m_grid->get_nb_cols(); j++)
        {
            if (m_grid->get(i, j) == 0)
            {
                empty_tiles.push_back(i);
                empty_tiles.push_back(j);
            }
        }

    // Ajout d'une nouvelle tuile.

    // On divise la taille du vecteur par 2 car les indices i et j sont mis côte à côte.
    uniform_int_distribution<int> int_dist(0, empty_tiles.size()/2 - 1);
    uniform_real_distribution<double> real_dist(0.0, 1.0);

    // Tirage d'une case vide.
    int rand_idx = 2 * int_dist(m_rand_generator);
    int rand_i = empty_tiles[rand_idx];
    int rand_j = empty_tiles[rand_idx+1];

    // Tirage d'un 2 ou 4.
    int rand_val = 2;
    if (real_dist(m_rand_generator) < m_proba_get_4) rand_val = 4;

    m_grid->set(rand_i, rand_j, rand_val);

    // S'il n'y avait plus qu'une tuile libre, alors avec la tuile qui vient d'être rajoutée,
    // la grille est remplie. La partie est alors perdue s'il n'y a pas de tuiles adjacentes identiques.
    if (empty_tiles.size() == 2 && !exist_same_adjacent_tiles())
        m_state->m_is_lose = true;
}


/**
 * Détermine s'il existe 2 tuiles adjacentes identiques dans le cas où il n'y a plus de case libre.
 */
bool Game_engine::exist_same_adjacent_tiles() const
{
    // On parcourt toute la grille à la recherche d'une possibilité de fusion.
    for (int i=0; i < m_grid->get_nb_rows(); i++)
    {
        for (int j=0; j < m_grid->get_nb_cols(); j++)
        {
            if (j < m_grid->get_nb_cols()-1 && m_grid->get(i, j) == m_grid->get(i, j+1)) return true;
            if (i < m_grid->get_nb_rows()-1 && m_grid->get(i, j) == m_grid->get(i+1, j)) return true;
        }
    }
    return false;
}


/**
 * Supprime l'état actuel et revient à l'état précédent.
 */
void Game_engine::undo()
{
    if (m_state->m_previous != 0)
    {
        Game_state *previous = m_state->m_previous;
        delete m_state;
        m_state = previous;
        m_grid = m_state->m_grid;
    }
}


Grid &Game_engine::get_grid() const
{
    return *m_state->m_grid;
}


int Game_engine::get_idx_after_move(int idx) const
{
    return m_indices_after_move[idx];
}


int Game_engine::get_nb_rows() const
{
    return m_grid->get_nb_rows();
}


int Game_engine::get_nb_cols() const
{
    return m_grid->get_nb_cols();
}


int Game_engine::get_win_value() const
{
    return m_win_value;
}


bool Game_engine::is_win() const
{
    return m_state->m_is_win;
}


bool Game_engine::is_lose() const
{
    return m_state->m_is_lose;
}


int Game_engine::get_score() const
{
    return m_state->m_score;
}


int Game_engine::get_best_score() const
{
    return m_state->m_best_score;
}


int Game_engine::get_nb_moves() const
{
    return m_state->m_nb_moves;
}


ostream& operator<<(ostream& out, const Game_engine &game)
{
    out << "Win: " << game.is_win()
        << " - Lose: " << game.is_lose()
        << " - Score: " << game.get_score()
        << " - Moves: " << game.get_nb_moves()
        << "\n\n"
        << game.get_grid();

    return out;
}
