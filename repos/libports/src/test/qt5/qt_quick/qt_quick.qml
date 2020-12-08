/*
 * QML example from the Qt5 Quick Start Guide
 *
 * http://qt-project.org/doc/qt-5.0/qtquick/qtquick-quickstart-essentials.html
 */

import QtQuick 2.0


/* root object */

Item {


/* button 1 */

Rectangle {
    id: "button1"
    objectName: "button1" /* for lookup from C++ code */
    width: 200
    height: 100
    color: "red"
    signal clicked()      /* Qt signal */

    Text {
        anchors.centerIn: parent
        text: "Hello, World!"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parent.color = "blue"

            /* emit Qt signal */
            button1.clicked()
        }
    }

    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Return) {
            color = "green";
            event.accepted = true;
        }
    }
}


}
