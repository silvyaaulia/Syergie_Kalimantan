import QtQuick 2.7
import QtQuick.Window 2.1
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1
import QtLocation 5.7
import QtPositioning 5.7



Window {
    id: window
    visible: true
    width: 1350
    height: 750
    color: "#f3f3f4"
    property alias text4Text: text4.text
    title: qsTr("Dynamic Positioning")

    function updateValue() {
        //button
        button1.value = mqttvalue.buttona1()
    }



    Item{
        id: title
        width: 400
        height: 73
        Text {
            id: dynamic
            x: 26
            y: 14
            color: "#313131"
            text: qsTr("Dynamic")
            font.letterSpacing: 3
            font.weight: Font.Medium
            font.family: "Roboto"
            fontSizeMode: Text.FixedSize
            font.pixelSize: 35
        }

        Text {
            id: positioning
            x: 190
            y: 14
            color: "#313131"
            text: qsTr("Positioning")
            font.letterSpacing: 3
            font.weight: Font.Medium
            font.family: "Roboto"
            fontSizeMode: Text.FixedSize
            font.pixelSize: 35
        }
    }

    /*=======HEADER=======*/
    Item{
        id: header
        x: 785
        y: 0
        width: 565
        height: 121

        /*=======MODE=======*/
        Item{
            id: mode
            x: 59
            y: 30
            width: 61
            height: 64
            Text {
                x: 9
                y: -5
                width: 40
                height: 37
                text: qsTr("A")
                horizontalAlignment: Text.AlignHCenter
                color: "#313131"
                font.pointSize: 30
            }
            Text {
                x: 8
                y: 48
                width: 40
                height: 17
                text: qsTr("Mode")
                horizontalAlignment: Text.AlignHCenter
                color: "#313131"
                font.pointSize: 8
            }
        }

        /*=======TRAINING=======*/
        Item{
            id: trainingGroup
            x: 126
            y: 21
            width: 62
            height: 73
            Text {
                x: 7
                y: 57
                width: 47
                height: 13
                text: qsTr("Training")
                font.letterSpacing: 1
                font.pointSize: 8
                fontSizeMode: Text.Fit
                //color: "#313131"
            }
            Button {
                id: trainingbutton
                x: 8
                y: 9
                width: 46
                height: 40
                //visible: false
                visible: {
                    trainingbutton.checked = false
                    trainingbutton.checkable = true
                }

                Rectangle {
                    id: rectangle
                    x: 0
                    y: 0
                    width: 45
                    height: 40
                    color: trainingbutton.checked ? "#d3d3ff" : "#f3f3f4"
                }

                Text {
                    id: text4
                    x: -485
                    y: -6
                    width: 87
                    height: 21
                    color: "#cb2525"
                    text: qsTr("training on")
                    font.pixelSize: 16
                    visible: trainingbutton.checked
                }
                Image {
                    id: image2
                    x: 7
                    y: 2
                    width: 32
                    height: 37
                    source: "icon_training.png"
                }

            }
        }


        /*=======MISSION=======*/
        Item{
            id: missionGroup
            x: 207
            y: 21
            width: 106
            height: 73
            Text {
                x: 32
                y: 57
                text: qsTr("Mission")
                font.letterSpacing: 1
                font.pointSize: 8
                fontSizeMode: Text.Fit
                color: "#313131"
            }

            Button {
                id: startbutton
                x: 8
                y: 9
                width: 42
                height: 40
                visible: {
                    startbutton.checked = false;
                    startbutton.checkable = true;

                }

                Text {
                    id: text5
                    x: -614
                    y: 21
                    width: 87
                    height: 20
                    color: "#cb2525"
                    text: qsTr("mission on")
                    font.pixelSize: 16
                    visible: startbutton.checked

                }

                Text {
                    id: text9
                    x: 3
                    y: 14
                    text: qsTr("Start")
                    font.pixelSize: 12
                }

            }

            Button {
                id: addbutton
                x: 60
                y: 9
                width: 41
                height: 40
                visible: {
                    addbutton.checked = false;
                    addbutton.checkable = true;
                }

                Text {
                    id: text8
                    x: 3
                    y: 14
                    text: qsTr("Add")
                    font.pixelSize: 12
                }

                Text {
                    id: text6
                    x: -573
                    y: 47
                    width: 87
                    height: 20
                    color: "#cb2525"
                    text: qsTr("add clicked")
                    font.pixelSize: 16
                    visible: addbutton.checked

                }

            }

            /*-------add waypoint-------*/

            /*-------start-------*/

        }

        /*=======TRACKING=======*/
        Item{
            id: trackingGroup
            x: 335
            y: 21
            width: 86
            height: 73
            Text {
                x: 21
                y: 55
                text: qsTr("Tracking")
                font.letterSpacing: 1
                font.pointSize: 8
                fontSizeMode: Text.Fit
                color: "#313131"
            }

            /*-------clear-------*/
            Button {
                id: clearbutton
                x: 3
                y: 9
                width: 38
                height: 40
                visible: {
                    clearbutton.checked = false;
                    clearbutton.checkable = true;
                }

                Text {
                    id: text7
                    x: -518
                    y: 21
                    width: 87
                    height: 20
                    color: "#cb2525"
                    text: qsTr("clear clicked")
                    font.pixelSize: 16
                    visible: clearbutton.checked

                }

                Text {
                    id: text10
                    x: 1
                    y: 16
                    text: qsTr("Clear")
                    font.pixelSize: 12
                }

            }

            /*-------switch(view-hide)-------*/
            Switch {
                id: control2
                x: 48
                y: 10
                //signal checked
                text: control2.checked ? qsTr("Show") : qsTr("Hide")

                indicator: Rectangle {
                    implicitWidth: 28
                    implicitHeight: 8
                    x: control2.leftPadding
                    y: parent.height / 3 - height / 3
                    radius: 6
                    color: control2.checked ? "#487048" : "#4f545b"
                    // border.color: control.checked ? "#487048" : "#4f545b"

                    Rectangle { //bulet
                        x: control2.checked ? parent.width - width : 0
                        y: -3
                        width: 14
                        height: 14
                        radius: 6
                        color: control2.checked ? "#4dd880" : "#aeb2b7"
                        //border.color: control.checked ? (control.down ? "#31a010" : "#ffffff") : "#999999"
                    }
                }

                contentItem: Text {
                    x: -24
                    y: 9
                    color: "#313131"
                    text: control2.text
                    horizontalAlignment: Text.AlignHCenter
                    font.letterSpacing: 1
                    font.pointSize: 7
                    wrapMode: Text.WordWrap
                    //font: control2.font
                    opacity: enabled ? 1.0 : 0.3
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: control2.indicator.width + control2.spacing
                }

            }
        }

        /*=======DGPS=======*/
        Item{
            id: dgps
            x: 464
            y: 21
            width: 70
            height: 73
            Text {
                x: 21
                y: 55
                text: qsTr("DGPS")
                font.letterSpacing: 1
                font.pointSize: 8
                fontSizeMode: Text.Fit
                color: "#313131"
            }
        }
        ToolSeparator {
            id: toolSeparator1
            x: 110
            y: 17
            width: 13
            height: 77
        }

        ToolSeparator {
            id: toolSeparator2
            x: 192
            y: 17
            width: 13
            height: 77
        }

        ToolSeparator {
            id: toolSeparator3
            x: 317
            y: 17
            width: 13
            height: 77
        }

        ToolSeparator {
            id: toolSeparator4
            x: 445
            y: 21
            width: 13
            height: 73
        }
    }

    Item{
        id: leftSide
        x: 0
        y: 324
        width: 400
        height: 261

        Item{
            id: position_grup
            x: 15
            y: 5
            width: 232
            height: 86
            Rectangle {
                id: position_bg
                x: 5
                y: 5
                width: 209
                height: 72
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }

            Text {
                id: position
                x: 15
                y: 15
                color: "#3056b8"
                text: qsTr("POSITION")
                font.pixelSize: 12
            }
            Text {
                id: latidude_position
                x: 15
                y: 35
                text: qsTr("Latitude    :")
                font.pixelSize: 12
            }
            Text {
                id: longitude_position
                x: 15
                y: 55
                text: qsTr("Longitude :")
                font.pixelSize: 12
            }
            Text {
                id: latidude_position_value
                x: 103
                y: 36
                text: qsTr("-6.892382")
                font.pixelSize: 12
            }
            Text {
                id: longitude_position_value
                x: 92
                y: 55
                text: qsTr("107.608352")
                font.pixelSize: 12
            }

        }

        Item{
            id: velocity_grup
            x: 228
            y: 5
            width: 82
            height: 86
            Rectangle {
                id: velocity_bg
                x: 5
                y: 5
                width: 73
                height: 72
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }

            Text {
                id: velocity
                x: 14
                y: 12
                color: "#3056b8"
                text: qsTr("VELOCITY")
                font.pixelSize: 12
            }
            Text {
                id: velocity_value
                x: 23
                y: 26
                color: "#3056b8"
                text: qsTr("1.2")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 26
            }
            Text {
                id: knot
                x: 29
                y: 58
                text: qsTr("knot")
                font.pixelSize: 12
            }
        }

        Item{
            id: heading_grup
            x: 305
            y: 5
            width: 87
            height: 74
            Rectangle {
                id: heading_bg
                x: 5
                y: 5
                width: 78
                height: 72
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }

            Text {
                id: heading
                x: 19
                y: 13
                color: "#3056b8"
                text: qsTr("HEADING")
                font.pixelSize: 12
            }
            Text {
                id: heading_value
                x: 17
                y: 35
                color: "#3056b8"
                text: qsTr("313.3")
                font.pixelSize: 22
            }

        }

        Item{
            id: cable_grup
            x: 15
            y: 85
            width: 184
            height: 86
            Rectangle {
                id: rectangle8
                x: 5
                y: 5
                width: 188
                height: 72
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }

            Text {
                id: cable
                x: 15
                y: 15
                color: "#3056b8"
                text: qsTr("CABLE")
                font.pixelSize: 12
            }
            Text {
                id: out
                x: 15
                y: 35
                text: qsTr("Out    :")
                font.pixelSize: 12
            }
            Text {
                id: remaining
                x: 15
                y: 55
                text: qsTr("Remaining :")
                font.pixelSize: 12
            }
            Text {
                id: out_value
                x: 103
                y: 36
                text: qsTr("234 m")
                font.pixelSize: 12
            }
            Text {
                id: remaining_value
                x: 92
                y: 55
                text: qsTr("3661 m")
                font.pixelSize: 12
            }

        }

        Item{
            id: distance_grup
            x: 209
            y: 85
            width: 188
            height: 86
            Rectangle {
                id: distance_bg
                x: 5
                y: 5
                width: 175
                height: 72
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }

            Text {
                id: distance
                x: 15
                y: 15
                color: "#3056b8"
                text: qsTr("REMAINING DISTANCE")
                font.pixelSize: 12
            }
            Text {
                id: nextWaypoint
                x: 15
                y: 35
                text: qsTr("Next Waypoint :")
                font.pixelSize: 12
            }
            Text {
                id: desination
                x: 15
                y: 55
                text: qsTr("Destination :")
                font.pixelSize: 12
            }
            Text {
                id: nextWaypoint_value
                x: 103
                y: 36
                text: qsTr("123 m")
                font.pixelSize: 12
            }
            Text {
                id: desination_value
                x: 92
                y: 55
                text: qsTr("3000 m")
                font.pixelSize: 12
            }

        }

        Item{
            id: nextWaypoint_grup
            x: 15
            y: 166
            width: 232
            height: 86
            Rectangle {
                id: nextWaypoint_bg
                x: 5
                y: 5
                width: 209
                height: 72
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }

            Text {
                id: nextWaypointt
                x: 15
                y: 15
                color: "#3056b8"
                text: qsTr("Next Waypoint")
                font.pixelSize: 12
            }
            Text {
                id: latidude_nextWaypoint
                x: 15
                y: 35
                text: qsTr("Latitude    :")
                font.pixelSize: 12
            }
            Text {
                id: longitude_nextWaypoint
                x: 15
                y: 55
                text: qsTr("Longitude :")
                font.pixelSize: 12
            }
            Text {
                id: latidude_nextWaypoint_value
                x: 103
                y: 36
                text: qsTr("-8.892382")
                font.pixelSize: 12
            }
            Text {
                id: longitude_nextWaypoint_value
                x: 92
                y: 55
                text: qsTr("106.608352")
                font.pixelSize: 12
            }

        }

        Item{
            id: windSpeed_grup
            x: 235
            y: 166
            width: 144
            height: 72
            Rectangle {
                id: windSpeed_bg
                x: 5
                y: 5
                width: 134
                height: 72
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }

            Text {
                id: windSpeed
                x: 15
                y: 15
                color: "#3056b8"
                text: qsTr("WIND SPEED")
                font.pixelSize: 12
            }
            Text {
                id: windSpeed_value
                x: 15
                y: 36
                text: qsTr("0.1 m/s")
                font.pixelSize: 12
            }




        }

        /*
        Item{
            id:seaforce
            Text {
                id: text24
                x: 234
                y: 611
                color: "#3056b8"
                text: qsTr("SEA FORCE")
                font.pixelSize: 12
            }

            Text {
                id: text25
                x: 234
                y: 635
                text: qsTr("0.1 m/s")
                font.pixelSize: 12
            }
            Rectangle {
                id: rectangle4
                x: 175
                y: 189
                width: 75
                height: 58
                color: "#fdfdfd"
                border.color: "#dbdbdb"
            }
        }
        */


    }

    Text {
        id: copyright
        x: 1259
        y: 657
        height: 8
        color: "#313131"
        text: qsTr("© 2018 - Syergie")
        font.letterSpacing: 0.3
        verticalAlignment: Text.AlignBottom
        font.pixelSize: 7
    }


    Item {
        id: mapGroup
        x: 397
        y: 100
        width: 940
        height: 558

        Map{
            id: map
            x: -387
            y: -28
            width: 920
            height: 506
            color: "#f9f9f9"
            anchors.top: parent.top
            anchors.topMargin: 36
            anchors.left: parent.left
            anchors.leftMargin: 8
            maximumZoomLevel: 29.4
            copyrightsVisible: true
            antialiasing: true
            maximumTilt: 89.3
            plugin: mapPlugin
            center: QtPositioning.coordinate(-6.89, 107.61) // ITB
            zoomLevel: 16

            Rectangle {
                id:trackingMap
                //layer untuk nampilin tracking
                //data dari array tracking-show
                x: 95
                y: 38
                width: 699
                height: 430
                color: "#63ceab"
                opacity: 0.2
                visible: control2.checked
            }
        }
    }


    //plugin open Street Map
    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter { name: "osm.useragent"; value: "My great Qt OSM application" }
        PluginParameter { name: "osm.mapping.host"; value: "http://osm.tile.server.address/" }
        PluginParameter { name: "osm.mapping.copyright"; value: "All mine" }
        PluginParameter { name: "osm.routing.host"; value: "http://osrm.server.address/viaroute" }
        PluginParameter { name: "osm.geocoding.host"; value: "http://geocoding.server.address" }
        PluginParameter { name: "osm.mapping.providersrepository.address"; value: "satellite"}
    }


    Dialog {
        id: dialog
        x: 21
        y: 90
        width: 110
        height: 171
        title: "TitleDialog"
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: console.log("Ok clicked")
        onRejected: console.log("Cancel clicked")
    }

    Pane {
        id: pane
        x: 490
        y: 252
        width: 338
        height: 200
        //background: color "#dddddd"
        visible: addbutton.checked


        TextField {
            id: textField
            x: 97
            y: 50
            width: 200
            height: 23
            text: qsTr("-6.89")
            placeholderText: "Longitude"
        }

        TextField {
            id: textField1
            x: 97
            y: 90
            width: 200
            height: 23
            text: qsTr("107.61")
        }

        Text {
            id: text11
            x: 8
            y: 54
            text: qsTr("Latitude")
            renderType: Text.NativeRendering
            font.pixelSize: 12
        }

        Text {
            id: text12
            x: 8
            y: 95
            text: qsTr("Longitude")
            font.pixelSize: 12
        }

        Text {
            id: text13
            x: 8
            y: 9
            width: 157
            height: 16
            text: qsTr("Add Waypoint")
            font.family: "Verdana"
            font.pixelSize: 13
        }

        Button {
            id: add
            x: 216
            y: 143
            width: 81
            height: 27
            text: "add"
            visible: {
                startbutton.checked = false;
                startbutton.checkable = true;
            }
            onClicked: visibleChanged(false)

            Text {
                id: text19
                x: -447
                y: 247
                width: 87
                height: 20
                color: "#313131"
                text: qsTr("mission on")
                font.pixelSize: 16
                visible: add.checked

            }
        }

    }

/*
    Rectangle {
        id: button
        x: 92
        y: 218
        property bool checked: false
        property alias text : buttonText.text
        Accessible.name: text
        Accessible.description: "This button does " + text
        Accessible.role: Accessible.Button
        Accessible.onPressAction: {
            button.clicked()
        }

        signal clicked

        width: buttonText.width + 20
        height: 30
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0;
                color: button.focus ? "red" : "blue" }
        }

        radius: 5
        antialiasing: true

        Text {
            id: buttonText
            text: parent.description
            anchors.centerIn: parent
            font.pixelSize: parent.height * .5
            style: Text.Sunken
            color: "white"
            styleColor: "black"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: parent.clicked()
        }

        Keys.onSpacePressed: clicked()
    }*/


/*
    Button {
        anchors.centerIn: parent
        property string firstfield: "a"
        property string secondfield: "sometext"
        iconSource: ""
        text: firstfield + " " + secondfield
        style: ButtonStyle {
            background: Rectangle {
                id: bg
                border.width: 1
                border.color: palette.mid
                radius: 3
                gradient: Gradient {
                    GradientStop { position: 0.0; color: control.pressed ? palette.button : palette.light }
                    GradientStop { position: 0.5; color: palette.midlight }
                    GradientStop { position: 1.0; color: control.pressed ? palette.light : palette.button }
                }
            }
            label: RowLayout {
                id: row
                spacing: 5
                Image { source: control.iconSource }
                Label {text: control.firstfield; font.family: "Symbol"; font.pixelSize: 18; color: palette.buttonText}
                Label {text: control.secondfield; color: palette.buttonText}

            }
        }
    }

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

}*/

}
