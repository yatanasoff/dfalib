import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("DAFNA")

    TabBar {
        id: tabBar
        height: 40
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        TabButton{
            text: "Options"
        }
        TabButton{
            text: "Calculations"
        }

    }

    StackLayout{
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: tabBar.bottom
        anchors.topMargin: 0
        currentIndex: tabBar.currentIndex
        OptionsView{
            id: options_view
        }
        CalculationView{
            id:calculation_view
        }
    }

}
