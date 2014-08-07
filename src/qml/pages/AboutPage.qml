import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    PageHeader { title: "About" }

    Label { 
	anchors.centerIn    : parent
        width		    : parent.width
        wrapMode	    : Text.WrapAtWordBoundaryOrAnywhere
        horizontalAlignment : Text.AlignHCenter
	text		    : "Virtaus Webcam Stream Viewer"
    }
}
