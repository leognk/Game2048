import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
Window {
    property alias police: mycombo.currentText
    property alias couleur: mycombo1.currentText


    visible: true
    width: 720
    height: 480
    color: "#ebe8e8"
    title: qsTr("Game 2048")

    Item {
        id: el3
        x: -39
        y: 0
        width: 720
        height: 486

        Text {
            id: element
            x: 582
            width: 162
            color: "#771a70"
            text: qsTr("2048")
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 436
            anchors.right: parent.right
            anchors.rightMargin: -24
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 45
            font.styleName: police

        }

        // La grille du jeu (peut être réinitialisée)
        Loader {
            id: boardLoader
            source: "Board.qml"



            function reload(nb_rows, nb_cols, win_value) {
                source = "";
                game_model.reset(nb_rows, nb_cols, win_value);
                source = "Board.qml";
            }

            x: 113
            anchors.verticalCenter: parent.verticalCenter
            focus: true
            onFocusChanged: focus = true;

            // Initialement, la grille sera de taille 4x4.
            Component.onCompleted: reload(4, 4, 2048);

        }

        Text {
            id: el1
            x: 642
            y: 56
            width: 80
            height: 40
            text: qsTr("SCORE")
            anchors.rightMargin: 20
            font.pixelSize: 20

            verticalAlignment: Text.AlignVCenter
            style: Text.Normal
            font.underline: true
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.styleName: police


        }

        Rectangle {
            id: bestlabel
            x: 642
            y: 185
            width: 80
            height: 40
            color: "#ad7fa8"
            radius: 10
            anchors.rightMargin: 20


            Text {
                id: best
                width: 80
                height: 40
                text: game_model.best_score
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 30
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                font.styleName: police

            }
        }

        Text {
            id: el2
            x: 642
            y: 139
            width: 80
            height: 40
            text: qsTr("BEST")
            anchors.rightMargin: 20
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            font.underline: true
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.styleName: police


        }




        ButtonGroup{
            id:group1
        }

        GroupBox{
            x: 627
            y: 231
            width: 95
            height: 200
            title:"size"

            font.styleName: police


            ColumnLayout{
                anchors.left: parent.left
                RadioButton{
                    ButtonGroup.group:group1
                    text:"4x4"
                    checked: true
                    font.styleName: police


                    onClicked: {
                        boardLoader.reload(4, 4, 2048)
                    }
                }
                RadioButton{
                    ButtonGroup.group:group1
                    text:"5x5"
                    font.styleName: police


                    onClicked: {
                        boardLoader.reload(5, 5, 2048)
                    }
                }
                RadioButton{
                    ButtonGroup.group:group1
                    text:"6x6"
                    font.styleName: police


                    onClicked: {
                        boardLoader.reload(6, 6, 2048)
                    }
                }


            }


        }



        ComboBox{
            id:mycombo
            x: 635
            y: 438
            width: 85
            height: 42
            font.styleName: police

            model: ListModel{
                ListElement{text:"Bold"}
                ListElement{text:"Light Italic"}
                ListElement{text:"Bold Italic"}
                ListElement{text:"Medium"}
                ListElement{text:"Light"}

            }

        }
        ComboBox{ //Bonus choix des degradation du couleur
            id:mycombo1
            x: 524
            y: 439
            width: 85
            height: 39
            font.styleName: police

            model: ListModel{
                ListElement{text:"green"}
                ListElement{text:"blue"}
                ListElement{text:"rose"}

            }

        }

        Button {
            id: btnback
            x: 228
            y: 437
            width: 120
            height: 40
            text: qsTr("BACK")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            font.styleName: police

            onClicked:{
                game_model.undo();
            }
        }

        Button {
            id: btnew
            x: 65
            y: 437
            width: 120
            height: 40
            text: qsTr("NEW")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            font.styleName: police

            onClicked:{
                boardLoader.reload(game_model.nb_rows, game_model.nb_cols, game_model.win_value);
            }
        }

        Button {
            id: btnquit
            x: 382
            y: 439
            width: 120
            height: 40
            text: qsTr("QUIT")
            font.styleName: police

            onClicked: Qt.quit()
        }

        Rectangle {
            id: scorelabel
            x: 642
            y: 97
            width: 80
            height: 40
            color: "#ad7fa8"
            radius: 10
            anchors.rightMargin: 20
            Text {

                id: score
                x: 627
                y: 138
                width: 80
                height: 40
                text: game_model.score
                anchors.fill: parent
                font.pixelSize: 30
                verticalAlignment: Text.AlignVCenter
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                horizontalAlignment: Text.AlignHCenter
            }

        }

        Text {
            id: el5
            x: 121
            y: 8
            width: 370
            height: 20
            text: qsTr("Combine the tiles and get the 2048 tile")
            anchors.rightMargin: 20
            font.pixelSize: 25
            font.styleName: police



            verticalAlignment: Text.AlignVCenter
            font.underline: false
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            id: rectangle1
            x: 134
            y: 205
            width: 308
            height: 60
            visible: game_model.is_lose
            color: "#c36f6f"




            Text {
                id: text2
                x: 0
                y: 0
                width: 308
                height: 60
                text: qsTr("Unfortunelly you loose")//message de perte
                font.pixelSize: 25
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.styleName: police

            }
        }
        Rectangle {
            id: rectangle2
            x: 134
            y: 205
            width: 308
            height: 60
            visible: game_model.is_win
            color: "#bdd6a9"




            Text {
                id: text3
                x: 0
                y: 0
                width: 308
                height: 60
                text: qsTr("Good JOB,YOU WIN")//Message de gain
                font.pixelSize: 25
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.styleName: police

            }
        }



    }
}
