import QtQuick

import editor

Item {
    property alias imageHeight: image.height
    property alias imageWidth: image.width
    property alias source: image.source
    signal clicked()

    implicitHeight: image.height
    implicitWidth: image.width
    Image {
        id: image
        anchors.top: parent.top
        anchors.left: parent.left

        mipmap: true
        fillMode: Image.PreserveAspectFit
    }
    MouseArea {
        anchors.fill: parent
        onClicked: (mouse)=> { if (mouse.button === Qt.LeftButton) parent.clicked() }
    }
}
