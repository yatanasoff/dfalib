import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: "DAFNA"

    Component.onCompleted: {
       AuthManager.startLogin()
    }

    Connections {
        target: AuthManager
        onValidCredentials :{
            login_view.visible= false;
            messageComponent.show_message("Logged in",1)
        }
        onNotValidCredentials:{
            login_view.clear()
            login_view.visible =  true;
            if(notification_enabled)
                messageComponent.show_message("Not valid credentials",0)
        }

    }

    LoginView{
        id: login_view
        anchors.fill: parent;
        visible: true
        z:1
    }

    TabBar {
        id: tabBar
        height: 40
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        TabButton {
            text: "Options"
        }
        TabButton {
            text: "Calculations"
        }
    }

    StackLayout {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: tabBar.bottom
        anchors.topMargin: 0
        currentIndex: tabBar.currentIndex
        OptionsView {
            id: options_view
        }
        CalculationView {
            id:calculation_view
        }
    }

    MessageComponent {
            id: messageComponent
            y: 412
            z:1
            height: 30
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 38
        }

}
