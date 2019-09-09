import QtQuick 2.0

Item {
    id: container
    property int message_type: 1
    property alias message_text: msg.text
    property bool stateVisible: false
    states: [
            State { when: stateVisible;
                PropertyChanges {   target: rectangle; opacity: 1.0    }
            },
            State { when: !stateVisible;
                PropertyChanges {   target: rectangle; opacity: 0.0    }
            }
        ]
        transitions: Transition {
            NumberAnimation { property: "opacity"; duration: 500}
        }

    Timer{
        id: timer_msg
        interval: 1000
        running: false
        repeat: false
        onTriggered: {
              container.stateVisible  = false
        }
    }
    Rectangle{
        id: rectangle

        opacity: 0.0
        color: 'black'
        anchors.fill: parent
        visible: true
        Text {
            id: msg
            color: 'white'
            text: "this is an error sample message"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    function show_message(text_msg, type_msg){
        message_text = text_msg
        message_type = type_msg
        container.stateVisible  = true
        if(message_type!=2){
            timer_msg.running = true
        }
    }
}
