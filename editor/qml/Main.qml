import QtQuick

import editor

Window {
    id: root
    width: 1280
    height: 768
    minimumWidth: 1280
    minimumHeight: 768
    visible: true

    LHEView3D {
        anchors.fill: parent

        myData: {
            if (models.enabled)
                return models.myData
            if (places.enabled && places.myData)
                return places.myData.model
            if (characters.enabled && characters.myData)
                return characters.myData.model
            if (links.enabled && links.myData)
                return links.myData.model
            return null
        }
    }

    LHEProject {
        anchors.top: menu.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        enabled: (menu.selected === LHEMenu.ModuleType.Project)
    }

    LHEModels {
        id: models

        anchors.top: menu.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        enabled: (menu.selected === LHEMenu.ModuleType.Models)
    }

    LHEPlaces {
        id: places

        anchors.top: menu.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        enabled: (menu.selected === LHEMenu.ModuleType.Places)
    }

    LHECharacters {
        id: characters

        anchors.top: menu.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        enabled: (menu.selected === LHEMenu.ModuleType.Characters)
    }

    LHELinks {
        id: links

        anchors.top: menu.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        enabled: (menu.selected === LHEMenu.ModuleType.Links)
    }

    LHEMenu {
        id: menu

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
    }

    LHEToolBar {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: menu.left
        anchors.rightMargin: 10
    }

    Connections {
        target: MyManager
        function onBeginFileTransaction() {
            progress.visible = true
        }
        function onEndFileTransaction(result) {
            progress.visible = false
            if (result === MyFileManager.Result.Error) {
                message.text = qsTr("An error occurred!")
                message.visible = true;
            }
        }
    }

    LHEProgressBox {
        id: progress
        anchors.centerIn: parent
        visible: false
        onClicked: MyManager.requestFileTransactionInterruption()
    }

    LHEMessageBox {
        id: message
        anchors.centerIn: parent
        visible: false
        onClicked: message.visible = false
    }
}
