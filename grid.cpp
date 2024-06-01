#include <string>
#include "grid.h"


Grid::Grid(int nb_rows, int nb_cols, int value)
{
    alloc(nb_rows, nb_cols);
    set_all(value);
}


Grid::Grid(const Grid &g)
{
    alloc(g.m_nb_rows, g.m_nb_cols);

    for (int i=0; i < g.m_nb_rows; i++)
        for (int j=0; j < g.m_nb_cols; j++)
            m_T[i][j] = g.m_T[i][j];
}


Grid::~Grid()
{
    if (m_T != 0)
    {
        free();
        m_T = 0;
    }
}


/**
 * Alloue en mémoire le tableau m_T.
 */
void Grid::alloc(int nb_rows, int nb_cols)
{
    m_nb_rows = nb_rows;
    m_nb_cols = nb_cols;
    m_T = new int*[nb_rows];
    for (int i=0; i < nb_rows; i++)
        m_T[i] = new int[nb_cols];
}


/**
 * Supprime le tableau d'entiers m_T.
 */
void Grid::free()
{
    for (int i=0; i < m_nb_rows; i++)
    {
        delete [] m_T[i];
    }
    delete [] m_T;
}


/**
 * Affecte la valeur donnée à toutes les cases du tableau m_T.
 */
void Grid::set_all(int value)
{
    for(int i=0; i < m_nb_rows; i++)
        for(int j=0; j < m_nb_cols; j++)
            m_T[i][j] = value;
}


void Grid::set(int i, int j, int value)
{
    m_T[i][j] = value;
}


int Grid::get(int i, int j) const
{
    return m_T[i][j];
}


int Grid::get(int idx) const
{
    return m_T[get_i(idx)][get_j(idx)];
}


int Grid::get_nb_rows() const
{
    return m_nb_rows;
}


int Grid::get_nb_cols() const
{
    return m_nb_cols;
}


int Grid::get_size() const
{
    return m_nb_rows * m_nb_cols;
}


int Grid::get_idx(int i, int j) const
{
    return i * m_nb_cols + j;
}


int Grid::get_i(int idx) const
{
    return idx / m_nb_cols;
}


int Grid::get_j(int idx) const
{
    return idx % m_nb_cols;
}


Grid& Grid::operator=(const Grid &g)
{
    if (this != &g) {
        free();
        alloc(g.m_nb_rows, g.m_nb_cols);
        for(int i=0; i < m_nb_rows; i++)
            for(int j=0; j < m_nb_cols; j++)
                m_T[i][j] = g.m_T[i][j];
    }
    return *this;
}


ostream& operator<<(ostream& out, const Grid& g)
{
    const int tile_width = 6;

    for(int i=0; i < g.m_nb_rows; i++) {
        for(int j=0; j < g.m_nb_cols; j++)
        {
            string str_tile = (g.m_T[i][j] == 0) ? "." : to_string(g.m_T[i][j]);
            int nb_spaces = tile_width - str_tile.size();
            out << string(nb_spaces/2, ' ') + str_tile + string(nb_spaces - nb_spaces/2, ' ');
        }
        if (i < g.m_nb_rows-1) out << "\n\n";
    }

    return out;
}
