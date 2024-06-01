import QtQuick 2.12

Rectangle {

    id: tile
    color: "#d7d6d6"
    radius: 3
    property alias tileInitialX: tile.initialX
    property alias tileInitialY: tile.initialY
    // Coordonnées constantes de la tuile dans la grille.
    // Après une animation de déplacement, la tuile revient toujours
    // à sa position originale (de manière instannée), et sera dans son nouvel état (l'état suivant).
    property int initialX
    property int initialY
    property variant couleur_list:
        [ ["#7b68ee","#00ffff","#7fffd4","#8a2be2","#5f9ea0","#6495ed","#008b8b","#00ced1","#add8e6","#e6e6fa","#4b0082"],
          ["#9acd32","#006400","#bdb76b","#556b2f","#8fbc8f","#2f4f4f","#228b22","#008000","#adff2f","#f0fff0","#20b2aa"],
          ["#faebd7","#ffebcd","#e9967a","#ff1493","#ff00ff","#ff69b4","#cd5c5c","#f08080","#ffb6c1","#ff00ff","#c71585"]
    ]




    signal animationXStarted
    signal animationYStarted
    signal animationXFinished
    signal animationYFinished

    function choose_color(couleur ) {//selon le couleur choisi par le joueur ,on choisi la sous liste
        if (couleur==="green"){
            return 1
        }
        else if (couleur==="blue"){
            return 0
        }
        else if (couleur==="rose"){
            return 2
        }
    }

    Text {
        id: tile_value
        text: qsTr("")
        anchors.fill: parent
        font.pixelSize: 35
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        //font.family:  ""
        //font.bold: true
        font.styleName:police

    }

    // Définition des animations de translation de la tuile

    Behavior on x {
        SequentialAnimation {
            ScriptAction { script: tile.animationXStarted(); }
            // Déplacement de la tuile dans la direction jouée
            NumberAnimation {
                duration: 100
                easing.type: Easing.OutCirc
            }
            // Retour instantanné de la tuile à sa position originale
            // (puis transition instannée vers l'état suivant)
            NumberAnimation {
                to: initialX
                duration: 0
            }
            ScriptAction { script: tile.animationXFinished(); }
        }
    }

    Behavior on y {
        SequentialAnimation {
            ScriptAction { script: tile.animationYStarted(); }
            NumberAnimation {
                duration: 100
                easing.type: Easing.OutCirc
            }
            NumberAnimation {
                to: initialY
                duration: 0
            }
            ScriptAction { script: tile.animationYFinished(); }
        }
    }

    // Définition des différents états de la tuile selon sa valeur

    states: [

        State {
            name: "2"

            PropertyChanges {
                target: tile_value
                text: qsTr("2")



            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][0]

            }
        },
        State {
            name: "4"

            PropertyChanges {
                target: tile_value
                text: qsTr("4")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][1]

            }
        },
        State {
            name: "8"
            PropertyChanges {
                target: tile_value
                text: qsTr("8")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][2]

            }
        },
        State {
            name: "16"
            PropertyChanges {
                target: tile_value
                text: qsTr("16")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][3]

            }
        },
        State {
            name: "32"
            PropertyChanges {
                target: tile_value
                text: qsTr("32")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][4]

            }
        },
        State {
            name: "64"
            PropertyChanges {
                target: tile_value
                text: qsTr("64")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][5]

            }
        },
        State {
            name: "128"
            PropertyChanges {
                target: tile_value
                text: qsTr("128")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][6]

            }
        },
        State {
            name: "256"
            PropertyChanges {
                target: tile_value
                text: qsTr("256")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][7]

            }
        },
        State {
            name: "512"
            PropertyChanges {
                target: tile_value
                text: qsTr("512")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][8]

            }
        },
        State {
            name: "1024"
            PropertyChanges {
                target: tile_value
                text: qsTr("1024")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][9]

            }
        },
        State {
            name: "2048"
            PropertyChanges {
                target: tile_value
                text: qsTr("2048")
            }
            PropertyChanges {
                target: tile
                color:couleur_list[choose_color(couleur )][10]

            }
        }
    ]



}
