import QtQuick 2.0

Item {
    id: container
    property int message_type: 1
    property alias message_text: msg.text
    Timer{
        id: timer_msg
        interval: 3000
        running: false
        repeat: false
        onTriggered: {
            rectangle.visible = false
        }
    }
    Rectangle{
        id: rectangle
        color: message_type == 0 ? '#F87060' :  '#09BC8A'
        anchors.fill: parent
        visible: false
        Text {
            id: msg
            color: '#FFFCF9'
            text: "this is an error sample message"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    function show_message(text_msg, type_msg){
        message_text = text_msg
        message_type = type_msg
        rectangle.visible = true
        timer_msg.running = true
    }
}
