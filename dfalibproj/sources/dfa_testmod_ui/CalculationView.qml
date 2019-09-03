import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4

Item {
    id: element


    Connections{
        target: Wrapper
        onTextReceivedtoCalc:{
//            console.log(text)
        }
        onUpdateCalcProgress:{
            progressBar.value = val
            if(val==100){
                button.text = "Calculate Strength"
            }
        }
        onClearCalcUI:{
            progressBar.value = 0
        }
    }

    TextArea {
        id: textArea
        height: 180
        text: qsTr("")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: label.bottom
        anchors.topMargin: 10
    }

    Button {
        id: button
        width: 150
        text: qsTr("Calculate Strength")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: textArea.bottom
        anchors.topMargin: 10
        onClicked: {
            if(button.text == "Calculate Strength"){
                Wrapper.doParseString(textArea.text)
                progressBar.value = 0
                button.text = "Stop"
            }else if(button.text == "Stop"){
                Wrapper.stopParse()
                button.text = "Calculate Strength"
            }
        }
    }

        TableView {
            id: tableView
            model: Wrapper.m_result_model_calc
            sortIndicatorVisible: true
            onSortIndicatorColumnChanged: model.sort(sortIndicatorColumn, sortIndicatorOrder)
            onSortIndicatorOrderChanged:  model.sort(sortIndicatorColumn, sortIndicatorOrder)
            anchors.bottom: button1.top
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: button.bottom
            anchors.topMargin: 10
            TableViewColumn{ role: "string"; title: "String";width: 240;}
            TableViewColumn{ role: "gqd"; title: "GQD"; width: 50; }
            TableViewColumn{ role: "imt"; title: "IMT"; width: 50; }
            TableViewColumn{ role: "hrp"; title: "HRP"; width: 50;}
            TableViewColumn{ role: "trp"; title: "TRP"; width: 50; }

        }

        Button {
            id: button1
            y: 361
            width: 150
            text: qsTr("Export")
            anchors.bottom: progressBar.top
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            onClicked: {
                Wrapper.saveToCSV(1)
            }
        }

        Label {
            id: label
            text: qsTr("Strings:")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
        }

        ProgressBar {
            id: progressBar
            y: 445
            height: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
        }

    }


    /*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2;anchors_width:150}D{i:3;anchors_x:10;anchors_y:251}
D{i:9;anchors_x:550;anchors_y:361}D{i:10;anchors_x:73;anchors_y:8}D{i:11;anchors_height:25;anchors_x:10;anchors_y:445}
}
 ##^##*/
