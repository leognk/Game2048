#ifndef GRID_H
#define GRID_H

#include <iostream>
using namespace std;


/**
 * Classe représentant une grille (un tableau d'entiers) pour le jeu de 2048.
 */
class Grid
{

public:

    Grid(int nb_rows, int nb_cols, int value=0);
    Grid(const Grid &g);
    ~Grid();

    void set_all(int value);
    void set(int i, int j, int value);

    int get(int i, int j) const;
    int get(int idx) const;
    int get_nb_rows() const;
    int get_nb_cols() const;
    int get_size() const;
    int get_idx(int i, int j) const;
    int get_i(int idx) const;
    int get_j(int idx) const;

    Grid& operator=(const Grid &g);
    friend ostream& operator<<(ostream& out, const Grid &g);

private:

    void alloc(int nb_rows, int nb_cols);
    void free();

    int m_nb_rows, m_nb_cols;
    int** m_T; // Le tableau d'entiers
};


#endif // GRID_H
