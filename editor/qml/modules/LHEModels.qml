import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs

import editor

LHEModule {
    property MyModel myData: null

    id: root
    title: qsTr("Models")

    MySelectionManager {
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

        RowLayout {
            height: 40

            spacing: 5

            LHEText {
                text: qsTr("Source")

                Layout.preferredWidth: 80
            }

            LHEMenuButton {
                text: qsTr("Choose...")
                onClicked: dialog.open()
            }
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
