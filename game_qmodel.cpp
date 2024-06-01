#include "game_qmodel.h"

Game_qmodel::Game_qmodel(QObject *parent) :
    QAbstractListModel(parent),
    // On doit initialiser avec la taille de la grille maximale qui sera utilisée dans QML,
    // sinon un étrange bug d'affichage se produit lors d'un changement de la taille de la grille.
    Game_engine(6, 6, 2048),
    next_state(0),
    next_grid(0)
{
    connect(this, &Game_qmodel::state_changed, this, &Game_qmodel::update_grid);
}

/**
 * Effectue un déplacement dans la direction donnée.
 *
 * Le déplacement se fait en 2 temps :
 * - D'abord, on calcule m_indices_after_move, la liste des nouveaux des indices des tuiles
 * dans la grille, afin d'effectuer l'animation de la translation.
 * - Ensuite, on met à jour l'état de la partie m_state afin d'afficher la nouvelle grille. (fait dans update_moved_state)
 */
void Game_qmodel::move(int direction)
{
    Game_engine::move(direction);

    // On fait revenir m_state à l'état d'avant le déplacement pour pouvoir faire d'abord l'animation.
    if (m_state_changed)
    {
        next_state = m_state;
        next_grid = m_grid;
        m_state = m_state->get_previous();
        m_grid = m_state->get_grid();
        update_delegate(TargetIndexRole);
    }
}

void Game_qmodel::move_top()
{
    move(Game_engine::TOP);
}

void Game_qmodel::move_down()
{
    move(Game_engine::DOWN);
}

void Game_qmodel::move_right()
{
    move(Game_engine::RIGHT);
}

void Game_qmodel::move_left()
{
    move(Game_engine::LEFT);
}

/**
 * Met à jour l'état de jeu actuel après un déplacement (après l'animation).
 */
void Game_qmodel::update_moved_state()
{
    if (m_state_changed)
    {
        m_state = next_state;
        m_grid = next_grid;
        next_state = 0;
        next_grid = 0;
        emit state_changed();
    }
}

void Game_qmodel::undo()
{
    Game_engine::undo();
    emit state_changed();
}

void Game_qmodel::reset(int nb_rows, int nb_cols, int win_value)
{
    Game_engine::reset(nb_rows, nb_cols, win_value);
    emit state_changed();
}

void Game_qmodel::reset()
{
    reset(get_nb_rows(), get_nb_cols(), get_win_value());
}

/**
 * Méthode dérivée de QAbstractListModel
 * Renvoie le nombre d'éléments de la grille.
 */
int Game_qmodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_grid->get_size();
}

/**
 * Méthode dérivée de QAbstractListModel
 * Renvoie le role demandé pour la tuile à l'indice demandé pour le delegate dans QML.
 */
QVariant Game_qmodel::data(const QModelIndex &index, int role) const
{
    // Si l'index est en dehors de la grille, on renvoie un QVariant invalide.
    if (!(0 <= index.row() && index.row() < m_grid->get_size()))
        return QVariant();

    if (role == ValueRole)
    {
        int tile_value = m_grid->get(index.row());
        return QString::number(tile_value); // Si tile_value est nul, la tuile par défaut (vide) sera prise.
    }

    else if (role == TargetIndexRole)
        return get_idx_after_move(index.row());

    return QVariant();
}

/**
 * Envoie un signal au repeater défini dans le Board de QML pour mettre à jour l'affichage par le delegate.
 */
void Game_qmodel::update_delegate(int role)
{
    emit dataChanged(createIndex(0, 0), createIndex(m_grid->get_size()-1, 0), {role});
}

void Game_qmodel::update_grid()
{
    update_delegate(ValueRole);
}

/**
 * Méthode dérivée de QAbstractListModel
 * On expose les noms des roles pour qu'ils puissent être utilisés par un delegate dans QML.
 */
QHash<int, QByteArray> Game_qmodel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ValueRole] = "tileValue";
    roles[TargetIndexRole] = "tileTargetIndex";
    return roles;
}
