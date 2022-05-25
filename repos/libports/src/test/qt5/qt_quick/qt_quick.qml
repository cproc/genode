/*
 * QML example from the Qt5 Quick Start Guide
 *
 * http://qt-project.org/doc/qt-5.0/qtquick/qtquick-quickstart-essentials.html
 */

import QtQuick 2.0
import QtQuick.Controls 2.13

Rectangle {
    width: 200
    height: 100

//	Item {

		Action {
			id: copyAction
//			text: qsTr("&Copy")
			icon.source: "file://redstone.png"
		}

		ToolButton {
			id: toolButton
			action: copyAction
		}

//		Image {
//	        id: img
//			anchors.fill: parent
//			source: "redStone.png"
//		}

//	}
}

