import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4


Window {
    id: mainlayer
    visible: true
    width: 1366
    height: 691
    color: "#000000"
    title: qsTr("Syergie Dynamic Positioning System")

    function updateValue() {
        //Enggine 1
        azimuth1.value = mqttvalue.azimut1()
        rpmEngine1.value = mqttvalue.enginespeed1()
        rpmPropeller1.value = mqttvalue.propelerspeed1()
        rpmPump1.value = mqttvalue.pumpspeed1()
        fuel1.value = mqttvalue.fuel1() * 0.1
        temperature1.value = mqttvalue.temp1()
        //depth1.text = mqttvalue.vdepth1()
        statusIndicator1.active = mqttvalue.engineconect1()

        //Enggine 2
        azimuth2.value = mqttvalue.azimut2()
        rpmEngine2.value = mqttvalue.enginespeed2()
        rpmPropeller2.value = mqttvalue.propelerspeed2()
        fuel2.value = mqttvalue.fuel2() * 0.1
        rpmPump2.value = mqttvalue.pumpspeed2()
        temperature2.value = mqttvalue.temp2()
        //depth2.text = mqttvalue.vdepth2()
        statusIndicator2.active = mqttvalue.engineconect2()

        //Enggine 3
        azimuth3.value = mqttvalue.azimut3()
        rpmEngine3.value = mqttvalue.enginespeed3()
        rpmPropeller3.value = mqttvalue.propelerspeed3()
        rpmPump3.value = mqttvalue.pumpspeed3()
        fuel3.value = mqttvalue.fuel3() * 0.1
        temperature3.value = mqttvalue.temp3()
        //depth3.text = mqttvalue.vdepth3()
        statusIndicator3.active = mqttvalue.engineconect3()

        //Enggine 4
        azimuth4.value = mqttvalue.azimut4()
        rpmEngine4.value = mqttvalue.enginespeed4()
        rpmPropeller4.value = mqttvalue.propelerspeed4()
        rpmPump4.value = mqttvalue.pumpspeed4()
        fuel4.value = mqttvalue.fuel4() * 0.1
        temperature4.value = mqttvalue.temp4()
        //depth4.text = mqttvalue.vdepth4()
        statusIndicator4.active = mqttvalue.engineconect4()

        //WindCondition
        windDirection.value = mqttvalue.directWind()
        windSpeed.value = mqttvalue.speedWind()
        statusIndicator5.active = mqttvalue.windconect()

        //Joystick Control
        steer1.value = mqttvalue.steer1()
        steer2.value = mqttvalue.steer2()
        speed1.value = mqttvalue.speed1()
        speed2.value = mqttvalue.speed2()
        statusIndicator6.active = mqttvalue.joyconect()

        mqttvalue.maincontrol()
    }

    Image {
        id: background
        x: 0
        y: -1
        source: "MockUpFix.png"

        StatusIndicator {
            id: statusIndicator7
            x: 342
            y: 699
            width: 23
            height: 23
            color: "#00ff00"
            anchors.horizontalCenterOffset: -165
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            active: true
        }

        Text {
            id: text1
            y: 704
            color: "#fffbfb"
            text: qsTr("Box Control Connected to Server")
            anchors.left: statusIndicator7.right
            anchors.leftMargin: 10
            anchors.verticalCenter: statusIndicator7.verticalCenter
            font.pixelSize: 12
        }

        StatusIndicator {
            id: statusSteer5
            y: 33
            width: 23
            height: 23
            anchors.verticalCenter: statusIndicator7.verticalCenter
            anchors.left: text1.right
            anchors.leftMargin: 40
            active: true
        }

        Text {
            id: text3
            y: 710
            color: "#fffbfb"
            text: qsTr("Steer Active")
            anchors.left: statusSteer5.right
            font.pixelSize: 12
            anchors.leftMargin: 10
            anchors.verticalCenter: statusSteer5.verticalCenter
        }

    }

    Item {
        id: item1
        x: 0
        y: 0
        width: mainlayer.width * 0.37
        height: mainlayer.height * 0.5

        StatusIndicator {
            id: statusSteer1
            x: 342
            width: 23
            height: 23
            anchors.horizontalCenterOffset: 90
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 36
            active: false
        }

        StatusIndicator {
            id: statusIndicator1
            x: 140
            width: 23
            height: 23
            color: "#00ff00"
            anchors.horizontalCenterOffset: -90
            anchors.top: parent.top
            anchors.topMargin: 36
            anchors.horizontalCenter: parent.horizontalCenter
            active: false
        }

        CircularGauge {
            id: rpmPropeller1
            x: 52
            y: 82
            width: 170
            height: 140
            anchors.verticalCenterOffset: -40
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: -120
            anchors.horizontalCenter: parent.horizontalCenter
            value: 200
            visible: true
            style: PropellerGaugeStyle {

            }
            maximumValue: 350
            Image {
                id: image1
                x: 73
                y: 30
                source: "PropIcon.png"
            }


        }

        CircularGauge {
            id: rpmEngine1
            x: 52
            y: 242
            width: 170
            height: 140
            stepSize: 0
            anchors.horizontalCenterOffset: -120
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -15
            value: 2000
            visible: true
            style: EngineGaugeStyle {
            }
            maximumValue: 3500

            Image {
                id: image2
                x: 73
                y: 33
                source: "EngineIcon.png"
            }
        }

        CircularGauge {
            id: rpmPump1
            x: 295
            y: 241
            width: 170
            height: 140
            anchors.horizontalCenterOffset: 120
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -15
            value: 141
            visible: true
            style: EngineGaugeStyle {
            }
            maximumValue: 3500
        }

        CircularGauge {
            id: azimuth1
            x: 173
            y: 151
            width: 170
            height: 140
            anchors.verticalCenterOffset: 30
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            value: 30
            visible: true
            style: AzimuthStyle {

            }
            maximumValue: 360
            Image {
                id: image
                x: 51
                y: 42
                source: "PropIcon.png"
            }


        }

        Item {
            id: item7
            x: 277
            y: 82
            width: 140
            height: 140
            anchors.verticalCenterOffset: -30
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: 130
            anchors.horizontalCenter: parent.horizontalCenter

            CircularGauge {
                id: fuel1
                x: 45
                value: 0
                maximumValue: 2
                y: 0
                width: 120
                height: 120
                anchors.right: parent.right
                anchors.rightMargin: 0
                visible: true
                style: FuelStyle {
                }
            }

            CircularGauge {
                id: temperature1
                value: 0
                maximumValue: 2
                y: 0
                width: 120
                height: 120
                anchors.left: parent.left
                anchors.leftMargin: 0
                visible: true
                style: TempStyle {
                }
            }

        }
    }

    Item {
        id: item2
        x: 0
        y: mainlayer.height * 0.5
        width: mainlayer.width * 0.37
        height: mainlayer.height * 0.5

        StatusIndicator {
            id: statusSteer2
            x: 342
            width: 23
            height: 23
            anchors.horizontalCenterOffset: 90
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 36
            active: false
        }

        StatusIndicator {
            id: statusIndicator2
            x: 140
            width: 23
            height: 23
            color: "#00ff00"
            anchors.horizontalCenterOffset: -90
            anchors.top: parent.top
            anchors.topMargin: 36
            anchors.horizontalCenter: parent.horizontalCenter
            active: false
        }

        CircularGauge {
            id: rpmPropeller2
            x: 52
            y: 82
            width: 170
            height: 140
            value: 141
            visible: true
            anchors.horizontalCenterOffset: -120
            style: PropellerGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: -40
            anchors.verticalCenter: parent.verticalCenter
            maximumValue: 350

            Image {
                id: image9
                x: 73
                y: 30
                source: "PropIcon.png"
            }
        }

        CircularGauge {
            id: rpmEngine2
            x: 52
            y: 242
            width: 170
            height: 140
            stepSize: 0
            value: 141
            anchors.bottom: parent.bottom
            visible: true
            anchors.horizontalCenterOffset: -120
            style: EngineGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: -15
            maximumValue: 3500

            Image {
                id: image6
                x: 73
                y: 33
                source: "EngineIcon.png"
            }
        }

        CircularGauge {
            id: rpmPump2
            x: 295
            y: 241
            width: 170
            height: 140
            value: 141
            anchors.bottom: parent.bottom
            visible: true
            anchors.horizontalCenterOffset: 120
            style: EngineGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: -15
            maximumValue: 3500
        }

        CircularGauge {
            id: azimuth2
            x: 173
            y: 151
            width: 170
            height: 140
            value: 30
            visible: true
            style: AzimuthStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: 30
            anchors.verticalCenter: parent.verticalCenter
            maximumValue: 360

            Image {
                id: image3
                x: 51
                y: 42
                source: "PropIcon.png"
            }
        }

        Item {
            id: item8
            x: 277
            y: 82
            width: 140
            height: 140
            CircularGauge {
                id: fuel2
                x: 45
                y: 0
                width: 120
                height: 120
                value: 0
                anchors.rightMargin: 0
                visible: true
                style: FuelStyle {
                }
                anchors.right: parent.right
                maximumValue: 2
            }

            CircularGauge {
                id: temperature2
                y: 0
                width: 120
                height: 120
                value: 0
                anchors.left: parent.left
                visible: true
                style: TempStyle {
                }
                anchors.leftMargin: 0
                maximumValue: 2
            }
            anchors.horizontalCenterOffset: 130
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: -30
            anchors.verticalCenter: parent.verticalCenter
        }


    }

    Item {
        id: item3
        x: mainlayer.width * 0.63
        y: 0
        width: mainlayer.width * 0.37
        height: mainlayer.height * 0.5

        StatusIndicator {
            id: statusSteer3
            x: 342
            width: 23
            height: 23
            anchors.horizontalCenterOffset: 90
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 36
            active: false
        }

        StatusIndicator {
            id: statusIndicator3
            x: 140
            width: 23
            height: 23
            color: "#00ff00"
            anchors.horizontalCenterOffset: -90
            anchors.top: parent.top
            anchors.topMargin: 36
            anchors.horizontalCenter: parent.horizontalCenter
            active: false
        }

        CircularGauge {
            id: rpmPropeller3
            x: 52
            y: 82
            width: 170
            height: 140
            value: 141
            visible: true
            anchors.horizontalCenterOffset: -120
            style: PropellerGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: -40
            anchors.verticalCenter: parent.verticalCenter
            maximumValue: 350

            Image {
                id: image10
                x: 73
                y: 30
                source: "PropIcon.png"
            }
        }

        CircularGauge {
            id: rpmEngine3
            x: 52
            y: 242
            width: 170
            height: 140
            stepSize: 0
            value: 141
            anchors.bottom: parent.bottom
            visible: true
            anchors.horizontalCenterOffset: -120
            style: EngineGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: -15
            maximumValue: 3500

            Image {
                id: image7
                x: 73
                y: 33
                source: "EngineIcon.png"
            }
        }

        CircularGauge {
            id: rpmPump3
            x: 295
            y: 241
            width: 170
            height: 140
            value: 141
            anchors.bottom: parent.bottom
            visible: true
            anchors.horizontalCenterOffset: 120
            style: EngineGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: -15
            maximumValue: 3500
        }

        CircularGauge {
            id: azimuth3
            x: 173
            y: 151
            width: 170
            height: 140
            value: 30
            visible: true
            style: AzimuthStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: 30
            anchors.verticalCenter: parent.verticalCenter
            maximumValue: 360

            Image {
                id: image4
                x: 51
                y: 42
                source: "PropIcon.png"
            }
        }

        Item {
            id: item9
            x: 277
            y: 82
            width: 140
            height: 140
            CircularGauge {
                id: fuel3
                x: 45
                y: 0
                width: 120
                height: 120
                value: 0
                anchors.rightMargin: 0
                visible: true
                style: FuelStyle {
                }
                anchors.right: parent.right
                maximumValue: 2
            }

            CircularGauge {
                id: temperature3
                y: 0
                width: 120
                height: 120
                value: 0
                anchors.left: parent.left
                visible: true
                style: TempStyle {
                }
                anchors.leftMargin: 0
                maximumValue: 2
            }
            anchors.horizontalCenterOffset: 130
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: -30
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Item {
        id: item4
        x: mainlayer.width * 0.63
        y: mainlayer.height * 0.5
        width: mainlayer.width * 0.37
        height: mainlayer.height * 0.5

        StatusIndicator {
            id: statusSteer4
            x: 342
            width: 23
            height: 23
            anchors.horizontalCenterOffset: 90
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 36
            active: false
        }

        StatusIndicator {
			id: statusIndicator4
            x: 140
            width: 23
            height: 23
            color: "#00ff00"
            anchors.horizontalCenterOffset: -90
            anchors.top: parent.top
            anchors.topMargin: 36
            anchors.horizontalCenter: parent.horizontalCenter
            active: false
        }

        CircularGauge {
            id: rpmPropeller4
            x: 52
            y: 82
            width: 170
            height: 140
            value: 141
            visible: true
            anchors.horizontalCenterOffset: -120
            style: PropellerGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: -40
            anchors.verticalCenter: parent.verticalCenter
            maximumValue: 350

            Image {
                id: image11
                x: 73
                y: 30
                source: "PropIcon.png"
            }
        }

        CircularGauge {
            id: rpmEngine4
            x: 52
            y: 242
            width: 170
            height: 140
            stepSize: 0
            value: 141
            anchors.bottom: parent.bottom
            visible: true
            anchors.horizontalCenterOffset: -120
            style: EngineGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: -15
            maximumValue: 3500

            Image {
                id: image8
                x: 73
                y: 33
                source: "EngineIcon.png"
            }
        }

        CircularGauge {
            id: rpmPump4
            x: 295
            y: 241
            width: 170
            height: 140
            value: 141
            anchors.bottom: parent.bottom
            visible: true
            anchors.horizontalCenterOffset: 120
            style: EngineGaugeStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: -15
            maximumValue: 3500
        }

        CircularGauge {
            id: azimuth4
            x: 173
            y: 151
            width: 170
            height: 140
            value: 30
            visible: true
            style: AzimuthStyle {
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: 30
            anchors.verticalCenter: parent.verticalCenter
            maximumValue: 360

            Image {
                id: image5
                x: 51
                y: 42
                source: "PropIcon.png"
            }
        }

        Item {
            id: item10
            x: 277
            y: 82
            width: 140
            height: 140
            CircularGauge {
                id: fuel4
                x: 45
                y: 0
                width: 120
                height: 120
                value: 0
                anchors.rightMargin: 0
                visible: true
                style: FuelStyle {
                }
                anchors.right: parent.right
                maximumValue: 2
            }

            CircularGauge {
                id: temperature4
                y: 0
                width: 120
                height: 120
                value: 0
                anchors.left: parent.left
                visible: true
                style: TempStyle {
                }
                anchors.leftMargin: 0
                maximumValue: 2
            }
            anchors.horizontalCenterOffset: 130
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: -30
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Item {
        id: item5
        y: 100
        width: mainlayer.width * 0.26
        height: mainlayer.width * 0.26
        anchors.verticalCenterOffset: -55
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Gauge {
            id: windSpeed
            x: 235
            y: 75
            height: 200
            anchors.verticalCenterOffset: 25
            anchors.right: parent.right
            anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            value: 0
            Text {
                id: text2
                x: 2
                y: -18
                color: "#ffffff"
                text: qsTr("m/s")
                anchors.horizontalCenter: parent.horizontalCenter
                font.bold: true
                font.pixelSize: 14
                font.family: "Verdana"
            }
            maximumValue: 20
        }

        CircularGauge {
            id: windDirection
            y: 151
            width: 170
            height: 170
            anchors.verticalCenterOffset: 25
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 60
            value: 30
            visible: true
            style: AzimuthStyle {
            }
            maximumValue: 360
        }

        StatusIndicator {
            id: statusIndicator5
            x: 342
            width: 23
            height: 23
            color: "#00ff00"
            active: true
            anchors.top: parent.top
            anchors.horizontalCenterOffset: -90
            anchors.topMargin: 58
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Item {
        id: item6
        x: 5
        y: 104
        width: mainlayer.width * 0.25
        height: 200
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 205
        anchors.verticalCenter: parent.verticalCenter

        CircularGauge {
            id: steer1
            x: 46
            y: 50
            width: 100
            height: 100
            anchors.verticalCenterOffset: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: -55
            anchors.horizontalCenter: parent.horizontalCenter
            value: 30
            visible: true
            style: AzimuthStyle {
            }
            maximumValue: 360
        }

        CircularGauge {
            id: steer2
            x: 196
            y: 50
            width: 100
            height: 100
            anchors.verticalCenterOffset: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: 55
            anchors.horizontalCenter: parent.horizontalCenter
            visible: true
            value: 30
            style: AzimuthStyle {
            }
            maximumValue: 360
        }

        Slider {
            id: speed1
            y: 32
            height: 150
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 35
            value: 0
            activeFocusOnPress: true
            stepSize: 1
            tickmarksEnabled: true
            maximumValue: 4
            orientation: Qt.Vertical
        }

        Slider {
            id: speed2
            x: 312
            y: 32
            height: 150
            anchors.right: parent.right
            anchors.rightMargin: 35
            anchors.verticalCenter: parent.verticalCenter
            value: 0
            stepSize: 1
            maximumValue: 4
            orientation: Qt.Vertical
            tickmarksEnabled: true
            activeFocusOnPress: true
        }

        StatusIndicator {
            id: statusIndicator6
            x: 342
            width: 23
            height: 23
            color: "#00ff00"
            active: false
            anchors.top: parent.top
            anchors.horizontalCenterOffset: 0
            anchors.topMargin: 60
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
