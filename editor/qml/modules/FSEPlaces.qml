import QtQuick
import QtQuick.Layouts

import editor

FSEModule {
    property MyPlace myData: null

    id: root
    title: qsTr("Places")

    MySelectionWrapper {
        id: mng
        model: MyModels.placeModel
        onCurrentDataChanged: root.myData = currentData
    }

    selection: FSEList {
        model: mng.model
        currentIndex: mng.currentIndex
        removeEnabled: mng.currentData && !mng.currentData.hasRef
        duplicateEnabled: mng.currentData

        onCreateClicked: { MyCommands.projectCommand.createPlace() }
        onRemoveClicked: { MyCommands.projectCommand.removePlace(mng.currentData) }
        onDuplicateClicked: { MyCommands.projectCommand.duplicatePlace(mng.currentData) }
        onItemClicked: (index) => { mng.currentIndex = index }
    }

    placement: FSEObject {
        object: root.myData
        filters: {
            "type": MyModel.Type.Place,
        }
    }
}
