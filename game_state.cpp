#include "game_state.h"

using namespace std;


Game_state::Game_state(int nb_rows, int nb_cols, int best_score):
    m_grid(new Grid(nb_rows, nb_cols, 0)),
    m_previous(0),
    m_is_win(false),
    m_is_lose(false),
    m_score(0),
    m_best_score(best_score),
    m_nb_moves(0)
{
}

Game_state::Game_state(const Game_state &state):
    m_grid(new Grid(*state.m_grid)),
    m_previous(state.m_previous),
    m_is_win(state.m_is_win),
    m_is_lose(state.m_is_lose),
    m_score(state.m_score),
    m_best_score(state.m_best_score),
    m_nb_moves(state.m_nb_moves)
{
}

Game_state::~Game_state()
{
    delete m_grid;
}

Grid *Game_state::get_grid()
{
    return m_grid;
}

Game_state *Game_state::get_previous()
{
    return m_previous;
}
