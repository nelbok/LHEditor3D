import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs

import editor

LHEModule {
    property MyModel myData: null

    id: root
    title: qsTr("Models")

    MySelectionWrapper {
        id: mng
        model: MyModels.modelModel
        onCurrentDataChanged: root.myData = currentData
    }

    selection: LHEList {
        model: mng.model
        currentIndex: mng.currentIndex
        removeEnabled: mng.currentData && !mng.currentData.hasRef
        duplicateEnabled: mng.currentData

        onCreateClicked: { MyCommands.projectCommand.createModel() }
        onRemoveClicked: { MyCommands.projectCommand.removeModel(mng.currentData) }
        onDuplicateClicked: { MyCommands.projectCommand.duplicateModel(mng.currentData) }
        onItemClicked: (index) => { mng.currentIndex = index }
    }

    placement: LHEEntity {
        entity: root.myData

        LHEComboBox {
            name: qsTr("Type")
            model: [
                { uuid: MyModel.Type.Character, name: qsTr("Character") },
                { uuid: MyModel.Type.Link, name: qsTr("Link") },
                { uuid: MyModel.Type.Place, name: qsTr("Place") },
            ]
            currentIndex: (root.myData) ? indexOfValue(root.myData.type) : -1
            onActivated: { if (root.myData) MyCommands.modelCommand.setType(root.myData, valueAt(currentIndex)) }
        }
    }

    partA: ColumnLayout {
        spacing: 5
        enabled: root.myData

        RowLayout {
            height: 40

            spacing: 5

            LHEText {
                text: qsTr("Source")

                Layout.preferredWidth: 80
            }

            LHEMenuButton {
                text: (root.myData && root.myData.sourcePath.toString() !== "") ? root.myData.sourcePath : qsTr("Choose...")
                elide: Text.ElideMiddle
                onClicked: dialog.open()
            }
        }

        LHETextField {
            name: qsTr("QML name")
            value: (root.myData) ? root.myData.qmlName : "-"
            enabled: false
        }
    }

    Connections {
        target: MyManager.balsam
        function onErrorOccurred() { message.open() }
    }

    FileDialog {
        id: dialog
        fileMode: FileDialog.OpenFile
        nameFilters: [
            "Wavefront (*.obj)",
            "COLLADA (*.dae)",
            "FBX (*.fbx)",
            "STL (*.stl)",
            "PLY (*.ply)",
            "GLTF2 (*.gltf *.glb)",
        ]
        onAccepted: MyManager.balsam.generate(myData, selectedFile)
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        text: qsTr("An error occurred while parsing the file.")
    }
}
