import QtQuick 2.9
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4



Item {

    Connections{
        target: Wrapper
        onTextReceived:{
            textArea_result.append(text)
            textArea_result.cursorPosition  =  0
        }
        onStatusReceived:{
            label_status.text = text
        }
        onUpdateProgress:{
            progressBar.value = val;
            if(val==100){
                button_calculate.text = "Calculate"
                button_gen_grammar.enabled = true
                tableView.sortIndicatorVisible = true
                spinBox_len.enabled = true
                checkBox_gqd.enabled = true
                checkBox_hrp.enabled = true
                checkBox_imt.enabled = true
                checkBox_trp.enabled = true
            }
        }
        onClearUI:{
            textArea_result.text = ""
            progressBar.value = 0
        }
    }

    function validate_calc(){
        if(button.text == "Calculate"){
            button_calculate.enabled  = !(
                        checkBox_gqd.checked == false &&
                        checkBox_imt.checked == false &&
                        checkBox_hrp.checked == false &&
                        checkBox_trp.checked ==  false
                        )
        }
    }

    id: element
    Label {
        id: label_len
        x: 480
        width: 100
        height: 25
        text: qsTr("Length:")
        anchors.right: parent.right
        anchors.rightMargin: 60
        anchors.top: checkBox_hrp.bottom
        anchors.topMargin: 0
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }

    SpinBox {
        id: spinBox_len
        x: 255
        width: 150
        height: 25
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: label_len.bottom
        anchors.topMargin: 0
        //        editable: true
        //        to: 100
        value: 0
    }

    CheckBox {
        id: checkBox_gqd
        x: 425
        width: 151
        height: 25
        text: qsTr("GQD")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: checkBox_trp.bottom
        anchors.topMargin: 0
        onClicked: {
            validate_calc()
        }

    }

    CheckBox {
        id: checkBox_imt
        x: 495
        width: 150
        height: 25
        text: qsTr("IMT")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: label_result.bottom
        anchors.topMargin: 10
        onClicked: {
            validate_calc()
        }
    }

    CheckBox {
        id: checkBox_hrp
        x: 556
        width: 150
        height: 25
        text: qsTr("HRP")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: checkBox_gqd.bottom
        anchors.topMargin: 0
        onClicked: {
            validate_calc()
        }
    }

    CheckBox {
        id: checkBox_trp
        x: 456
        width: 150
        height: 25
        text: qsTr("TRP")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: checkBox_imt.bottom
        anchors.topMargin: 0
        checked: true
        onClicked: {
            validate_calc()
        }
    }

    Button {
        id: button_calculate
        x: 480
        width: 150
        height: 25
        text: "Calculate"
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: spinBox_len.bottom
        anchors.topMargin: 5
        onClicked: {
            if(button_calculate.text=="Calculate"){
                progressBar.value = 0
                textArea_result.text = ""
                Wrapper.doDFA(
                            checkBox_gqd.checked,
                            checkBox_imt.checked,
                            checkBox_trp.checked,
                            checkBox_hrp.checked,
                            spinBox_len.value)
                checkBox_gqd.enabled = false
                checkBox_hrp.enabled = false
                checkBox_imt.enabled = false
                checkBox_trp.enabled = false
                button_calculate.text = "Stop"
                button_gen_grammar.enabled = false
                spinBox_len.enabled = false
            }else{
                Wrapper.stopDFA()
                button_calculate.text = "Calculate"
                button_gen_grammar.enabled =true
                spinBox_len.enabled = true
                checkBox_gqd.enabled = true
                checkBox_hrp.enabled = true
                checkBox_imt.enabled = true
                checkBox_trp.enabled = true
            }
        }
    }

    TextArea {
        id: textArea_result
        text: qsTr("")
        anchors.bottom: tableView.top
        anchors.bottomMargin: 6
        anchors.right: checkBox_imt.left
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: label_result.bottom
        anchors.topMargin: 10
    }

    ProgressBar {
        id: progressBar
        height: 25
        anchors.top: tableView.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        value: 0
        maximumValue: 100
        anchors.left: button.right
        anchors.leftMargin: 10

        Label {
            id: label_status
            color: "#2b2629"
            text: ""
            anchors.rightMargin: 10
            lineHeight: 0.6
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            anchors.fill: parent
        }
    }

    TableView {
        id: tableView
        model: Wrapper.m_result_model
        sortIndicatorVisible: true
        onSortIndicatorColumnChanged: model.sort(sortIndicatorColumn, sortIndicatorOrder)
        onSortIndicatorOrderChanged: model.sort(sortIndicatorColumn, sortIndicatorOrder)
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: button.top
        anchors.bottomMargin: 10
        anchors.top: button_gen_grammar.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        TableViewColumn{ role: "string"; title: "String";width: 240;}
        TableViewColumn{ role: "gqd"; title: "GQD"; width: 50; }
        TableViewColumn{ role: "imt"; title: "IMT"; width: 50; }
        TableViewColumn{ role: "hrp"; title: "HRP"; width: 50;}
        TableViewColumn{ role: "trp"; title: "TRP"; width: 50; }
    }

    Label {
        id: label_result
        text: qsTr("Result:")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    Button {
        id: button
        y: 447
        width: 150
        text: qsTr("Export")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        onClicked: {
            Wrapper.saveToCSV(0)
        }
    }

    Button {
        id: button_gen_grammar
        x: 480
        width: 150
        height: 25
        text: qsTr("Generate Grammar")
        anchors.top: button_calculate.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: {
            Wrapper.clearModel()
            textArea_result.text = ""
            progressBar.value = 0
            Wrapper.generateGrammar(
                        checkBox_gqd.checked,
                        checkBox_imt.checked,
                        checkBox_trp.checked,
                        checkBox_hrp.checked,
                        spinBox_len.value)
        }
    }


}













/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:30;anchors_width:100;anchors_x:83;anchors_y:51}
D{i:2;anchors_height:30;anchors_width:100;anchors_x:155;anchors_y:182}D{i:3;anchors_height:25;anchors_width:150;anchors_x:255;anchors_y:40}
D{i:4;anchors_height:30;anchors_width:151;anchors_x:10;anchors_y:115}D{i:5;anchors_height:30;anchors_x:241;anchors_y:40}
D{i:6;anchors_y:140}D{i:7;anchors_y:90}D{i:8;anchors_y:268}D{i:10;anchors_height:25;anchors_y:416}
D{i:12;anchors_height:176;anchors_width:437;anchors_y:246}D{i:19;anchors_x:22}D{i:20;anchors_y:223}
}
 ##^##*/
