import QtQuick 2.12

Rectangle {

    id: board
    property int gridSpacing: 8

    width: adjusted_size(350, gridSpacing, game_model.nb_cols)
    height: adjusted_size(350, gridSpacing, game_model.nb_rows)
    color: "#b2aaaa"
    radius: 7

    // Ajuste la taille (largeur ou hauteur) du board pour que les bordures aient
    // exactement le spacing indiqué. Sans ça, il y a un léger écart à cause de
    // l'arrondi sur la taille des tuiles.
    function adjusted_size(size, spacing, nb_rows) {
        return nb_rows * Math.round((size-spacing)/nb_rows) + spacing
    }


    property bool dummyCompleted: false
    property int nbRunningAnimations: 0

    // Grille factice se superposant parfaitement au grid avec les tuiles du jeu.
    // Elle sert à accéder aux coordonnées des cellules du grid (pour l'animation)
    // et sert en tant que background.
    // On ne peut pas utiliser directement les coordonnées des tuiles du grid
    // car celles-ci se déplacent et cela conduirait à des anomalies.
    Grid {
        id: dummyGrid
        rows: game_model.nb_rows
        columns: game_model.nb_cols
        spacing: gridSpacing
        width: board.width - (1+columns)*spacing
        height: board.height - (1+rows)*spacing
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: spacing
        anchors.topMargin: spacing

        // Les tuiles factices
        Repeater {
            id: dummyTiles
            model: game_model.rowCount()
            delegate: Tile {
                state: ""
                width: Math.round(dummyGrid.width / dummyGrid.columns)
                height: Math.round(dummyGrid.height / dummyGrid.rows)

            }
        }
        Component.onCompleted: dummyCompleted = true;
        Component.onDestruction: dummyCompleted = false;
    }

    // Grille avec les tuiles du jeu
    Grid {
        id: grid
        rows: dummyGrid.rows
        columns: dummyGrid.columns
        spacing: dummyGrid.spacing
        width: dummyGrid.width
        height: dummyGrid.height
        x: dummyGrid.x
        y: dummyGrid.y
        z: 1

        // Les tuiles du jeu
        Repeater {
            id: tiles
            model: game_model
            delegate: Tile {
                state: tileValue
                width: Math.round(grid.width / grid.columns)

                height: Math.round(grid.height / grid.rows)
                tileInitialX: dummyCompleted ? dummyTiles.itemAt(index).x : x
                tileInitialY: dummyCompleted ? dummyTiles.itemAt(index).y : y
                x: dummyCompleted ? dummyTiles.itemAt(tileTargetIndex).x : x
                y: dummyCompleted ? dummyTiles.itemAt(tileTargetIndex).y : y
                onAnimationXStarted: { nbRunningAnimations++; z = 2; }
                onAnimationYStarted: { nbRunningAnimations++; z = 2; }
                onAnimationXFinished: { nbRunningAnimations--; z = 1; }
                onAnimationYFinished: { nbRunningAnimations--; z = 1; }
            }
        }
    }

    onNbRunningAnimationsChanged: {
        if (nbRunningAnimations == 0) game_model.update_moved_state();
    }

    focus: true

    Keys.onPressed: {
        if (!event.isAutoRepeat && nbRunningAnimations === 0) {
            if (event.key === Qt.Key_Up || event.key === Qt.Key_Z) game_model.move_top();
            else if (event.key === Qt.Key_Down || event.key === Qt.Key_S) game_model.move_down();
            else if (event.key === Qt.Key_Right || event.key === Qt.Key_D) game_model.move_right();
            else if (event.key === Qt.Key_Left || event.key === Qt.Key_Q) game_model.move_left();
            else if (event.key === Qt.Key_U) game_model.undo();
            else if (event.key === Qt.Key_R) game_model.reset();
        }
    }
}
