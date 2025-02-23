import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4


Item {

    function clear(){
        textField_password.text = ""
        textField_username.text = ""
        textField_password.enabled =  true
        textField_username.enabled = true
        button_login.enabled = true
//        button_login.text = "Login"
    }
    function validate_and_send(){
        if(     textField_password.text.length > 1 &&
                textField_username.text.length > 1){
            AuthManager.doLogin(textField_username.text, textField_password.text)
            textField_password.enabled =  false
            textField_username.enabled = false
            button_login.enabled = false
//            button_login.text = "Connecting..."
            messageComponent.show_message("connecting...",2)
        }else{
            messageComponent.show_message("invalid data provided",0)
        }
    }

    Rectangle {
        id: rectangle
        color: "#ffffff"
        anchors.fill: parent

        Label {
            id: label
            x: 145
            y: 158
            text: qsTr("Username")
            anchors.horizontalCenterOffset: -140
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: textField_username.top
            anchors.bottomMargin: 10
        }

        TextField {
            id: textField_username
            x: 145
            y: 181
            width: 350
            height: 30
            text: qsTr("")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: "username"
            KeyNavigation.tab: textField_password
        }

        TextField {
            id: textField_password
            x: 145
            width: 350
            height: 30
            text: qsTr("")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: label1.bottom
            anchors.topMargin: 10
            echoMode: TextInput.Password
            placeholderText: "password"
            KeyNavigation.tab: textField_username
        }

        Label {
            id: label1
            x: 145
            text: qsTr("Password")
            anchors.horizontalCenterOffset: -140
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: textField_username.bottom
            anchors.topMargin: 10
        }

        Button {
            id: button_login
            x: 415
            height: 30
            text: qsTr("Login")
            anchors.right: textField_password.right
            anchors.rightMargin: 0
            anchors.top: textField_password.bottom
            anchors.topMargin: 10
            onClicked: {
               validate_and_send()
            }
        }
    }




}











/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:3;anchors_width:350;anchors_x:145;anchors_y:181}
D{i:4;anchors_y:334}D{i:5;anchors_y:232}D{i:6;anchors_y:310}D{i:1;anchors_height:200;anchors_width:200;anchors_x:212;anchors_y:140}
}
 ##^##*/
