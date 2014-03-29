import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    allowedOrientations: [Orientation.Landscape];
    initialPage: videoStreamComponent
    Component {
        id: videoStreamComponent
        VideoStream { }
    }
}


