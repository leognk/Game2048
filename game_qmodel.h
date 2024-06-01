#ifndef GAME_QMODEL_H
#define GAME_QMODEL_H

#include <QAbstractListModel>
#include "game_engine.h"

/**
 * Cette classe adapte la classe Game_engine pour être
 * un QAbstractListModel utilisable comme model dans QML.
 */
class Game_qmodel : public QAbstractListModel, Game_engine
{
    Q_OBJECT

public:
    explicit Game_qmodel(QObject *parent=0);

    // Actions autorisées
    Q_INVOKABLE void move_top();
    Q_INVOKABLE void move_down();
    Q_INVOKABLE void move_right();
    Q_INVOKABLE void move_left();
    Q_INVOKABLE void update_moved_state();
    Q_INVOKABLE void undo();
    Q_INVOKABLE void reset(int nb_rows, int nb_cols, int win_value);
    Q_INVOKABLE void reset();

    // Informations sur l'état de la partie
    Q_PROPERTY(int nb_rows READ get_nb_rows NOTIFY state_changed)
    Q_PROPERTY(int nb_cols READ get_nb_cols NOTIFY state_changed)
    Q_PROPERTY(int win_value READ get_win_value NOTIFY state_changed)
    Q_PROPERTY(bool is_win READ is_win NOTIFY state_changed)
    Q_PROPERTY(bool is_lose READ is_lose NOTIFY state_changed)
    Q_PROPERTY(int score READ get_score NOTIFY state_changed)
    Q_PROPERTY(int best_score READ get_best_score NOTIFY state_changed)
    Q_PROPERTY(int nb_moves READ get_nb_moves NOTIFY state_changed)

    // Méthodes dérivées de QAbstractListModel
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    // Les roles de notre model utilisés par le delegate dans QML.
    enum TileRoles {
        ValueRole, // Valeur de la tuile en QString (pour le changement d'état)
        TargetIndexRole, // Indice de la tuile dans la grille vers laquelle la tuile traitée se déplace.
    };

public slots:
    void update_grid();

signals:
    void state_changed();

private:
    void move(int direction);
    void update_delegate(int role);
    // Méthode dérivée de QAbstractListModel
    QHash<int, QByteArray> roleNames() const;

    Game_state *next_state;
    Grid *next_grid;

};

#endif // GAME_QMODEL_H
