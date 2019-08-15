import QtQuick 2.0
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.3



Item {

    Connections{
        target: Wrapper
        onTextReceived:{
            textArea_result.append(text)
            textArea_result.cursorPosition  =  0
        }
        onUpdateProgress:{
            progressBar.value = val;
            if(val>99){
                tableView.visible =  true
            }
        }
    }

    id: element
    Label {
        id: label_len
        width: 100
        height: 30
        text: qsTr("Length:")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }

    SpinBox {
        id: spinBox_len
        y: 40
        height: 30
        //        editable: true
        //        to: 100
        value: 1
        anchors.left: label_len.right
        anchors.leftMargin: 0
        anchors.verticalCenter: label_len.verticalCenter
    }

    CheckBox {
        id: checkBox_gqd
        y: 76
        height: 30
        text: qsTr("")
        anchors.left: label_gqd.right
        anchors.leftMargin: 0
        anchors.verticalCenter: label_gqd.verticalCenter
    }

    Label {
        id: label_gqd
        width: 100
        height: 30
        text: qsTr("GQD:")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: label_len.bottom
        anchors.topMargin: 10
    }

    Label {
        id: label_imt
        width: 100
        height: 30
        text: qsTr("IMT:")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: label_gqd.bottom
        anchors.topMargin: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }

    Label {
        id: label_hrp
        width: 100
        height: 30
        text: qsTr("HRP:")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: label_imt.bottom
        anchors.topMargin: 10
    }

    Label {
        id: label_tpr
        width: 100
        height: 30
        text: qsTr("TRP:")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: label_hrp.bottom
        anchors.topMargin: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }

    CheckBox {
        id: checkBox_imt
        y: 114
        height: 30
        text: qsTr("")
        anchors.left: label_imt.right
        anchors.leftMargin: 0
        anchors.verticalCenter: label_imt.verticalCenter
    }

    CheckBox {
        id: checkBox_hrp
        y: 143
        height: 30
        text: qsTr("")
        anchors.left: label_hrp.right
        anchors.leftMargin: 0
        anchors.verticalCenter: label_hrp.verticalCenter
    }

    CheckBox {
        id: checkBox_trp
        y: 178
        height: 30
        text: qsTr("")
        anchors.left: label_tpr.right
        anchors.leftMargin: 0
        anchors.verticalCenter: label_tpr.verticalCenter
    }

    Button {
        id: button_calculate
        height: 30
        text: qsTr("Calculate")
        anchors.right: spinBox_len.right
        anchors.rightMargin: 0
        anchors.left: label_tpr.left
        anchors.leftMargin: 0
        anchors.top: label_tpr.bottom
        anchors.topMargin: 10
        onClicked: {
            tableView.visible = false;
            progressBar.value = 0
            textArea_result.text = ""
            Wrapper.doDFA(
                        checkBox_gqd.checked,
                        checkBox_imt.checked,
                        checkBox_trp.checked,
                        checkBox_hrp.checked,
                        spinBox_len.value)
        }
    }

    TextArea {
        id: textArea_result
        text: qsTr("")
        anchors.bottom: tableView.top
        anchors.bottomMargin: 10
        anchors.top: spinBox_len.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: spinBox_len.right
        anchors.leftMargin: 10
    }

    ProgressBar {
        id: progressBar
        y: 449
        value: 0
        maximumValue: 100
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
    }

    TableView {
        id: tableView
        anchors.top: button_calculate.top
        anchors.topMargin: 0
        visible: false;
        anchors.left: button_calculate.right
        anchors.leftMargin: 10
        model: Wrapper.m_result_model
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: progressBar.top
        anchors.bottomMargin: 10
        TableViewColumn{ role: "string"; title: "String";width: 200;}
        TableViewColumn{ role: "gqd"; title: "GQD"; width: 50; }
        TableViewColumn{ role: "imt"; title: "IMT"; width: 50; }
        TableViewColumn{ role: "hrp"; title: "HRP"; width: 50;}
        TableViewColumn{ role: "trp"; title: "TRP"; width: 50; }
    }

}















/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:30;anchors_width:100;anchors_x:83;anchors_y:51}
D{i:2;anchors_x:155}D{i:3;anchors_x:255}D{i:4;anchors_x:89;anchors_y:89}D{i:5;anchors_x:89;anchors_y:120}
D{i:6;anchors_x:87;anchors_y:149}D{i:7;anchors_x:89;anchors_y:184}D{i:8;anchors_x:241;anchors_y:114}
D{i:9;anchors_x:241}D{i:10;anchors_x:241}D{i:11;anchors_x:95;anchors_y:269}D{i:12;anchors_x:327;anchors_y:60}
D{i:15;anchors_height:271;anchors_y:170}
}
 ##^##*/
