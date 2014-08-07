import QtQuick 2.0
import Sailfish.Silica 1.0
import Virtaus 0.1

Page {
    id: page
    
    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent
        
        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            id: pullDownMenu
            MenuItem {
                id: aboutAction
                text: "About"
                onClicked: {
                    console.log("aboutAction clicked")
		    pageStack.push("AboutPage.qml")
                }
            }
            MenuItem {
                id: settiongsAction
                text: "Settings"
                onClicked: {
                    console.log("settiongsAction clicked")
		    pageStack.push("SettingsPage.qml")
                }

            }
        }
        
        // Tell SilicaFlickable the height of its content.
        contentHeight: childrenRect.height
        
        Column {
	    VideoStreamView {
		width: 640
		height: 480

		url: "http://webcam.modeemi.fi/mjpg/video.mjpg";
	    }
	}
    }
}
