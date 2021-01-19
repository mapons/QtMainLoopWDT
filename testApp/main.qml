import QtQuick 2.12
import QtQuick.Window 2.12


import QtQuick.Controls 2.12
Window {
    id:playerWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("QT Main Loop block TEST")
    Page{
        id:page
        anchors.fill: parent
        Column{
            spacing:10
            anchors.centerIn: parent
            /*BusyIndicator { //uses his own thread
                running: true
                anchors.horizontalCenter: parent.horizontalCenter
            }*/
            Rectangle {
                width: 100; height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                SequentialAnimation on color {
                    loops: -1
                    ColorAnimation { to: "gray"; duration: 1000 }
                    ColorAnimation { to: "red"; duration: 1000 }
                }
                SequentialAnimation on width {
                    loops: -1
                    NumberAnimation { to: 50; duration: 1000 }
                    NumberAnimation { to: 100; duration: 1000 }
                }
            }
            Label{
                anchors.horizontalCenter: parent.horizontalCenter
                text:timer.counter.toString()
                Timer{
                    id:timer
                    property int counter:0
                    interval: 100
                    running: true
                    repeat: true
                    onTriggered: counter++
                }
            }
            Label{
                text:qsTr("Press button to start a infinite loop")
            }
            Button{
                anchors.horizontalCenter: parent.horizontalCenter
                text:"START QML INFINITE LOOP"
                onClicked: {
                    for(;;){} //inifinite loop
                }
            }
            Button{
                anchors.horizontalCenter: parent.horizontalCenter
                text:"BLOCK UI 3500ms"
                onClicked: {
                    Utils.temporalBlockMainLoop(3500)
                }
            }
            Button{
                anchors.horizontalCenter: parent.horizontalCenter
                text:"START C++ INFINITE LOOP"
                onClicked: {
                    Utils.infiniteLoop()
                }
            }
            Button{
                anchors.horizontalCenter: parent.horizontalCenter
                text:"C++ MUTEX DOUBLE LOCK"
                onClicked: {
                    Utils.mutexDoubleLock()
                }
            }
        }
    }
}
