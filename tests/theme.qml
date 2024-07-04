import QtQuick
import QtQuick.Controls
import QtMultimedia

import HyprMpv

ApplicationWindow
{
    id: window
    visible: true
    title: "Hyprland Video Player"

    HyprMpvConfig
    {
        id: config
    }

    MediaPlayer
    {
        id: mediaPlayer
        source: Qt.resolvedUrl(config.get_media_url())
        videoOutput: videoOutput
        audioOutput: AudioOutput
        {
            id: audio
            volume: root.volume
        }
    }

    Item
    {
        property bool ready: false

        id: root
        anchors.fill: parent
        focus: true
        opacity: ready ? 1 : 0

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 1500
            }
        }

        property var volume: 1;
        property var volume_step: 0.05

        function toggle_player()
        {
            switch(mediaPlayer.playbackState)
            {
                case MediaPlayer.PlayingState:  mediaPlayer.pause(); break;
                case MediaPlayer.PausedState:   mediaPlayer.play(); break;
                case MediaPlayer.StoppedState:  mediaPlayer.play(); break;
            }
        }

        function show_slider ()
        {
            slider_hide_timer.restart()
            progressSlider.hide = false
        }

        VideoOutput
        {
            id: videoOutput
            anchors.fill: parent
        }

        Shortcut
        {
            sequence: "Ctrl+S"

            onActivated:
            {
                console.log("ok")
            }
        }

        Shortcut
        {
            sequence: "Space"

            onActivated: root.toggle_player()
        }

        TapHandler
        {
            acceptedButtons: Qt.LeftButton
            onTapped: {
                root.toggle_player()
                root.show_slider()
            }
        }



        Timer
        {
            id: slider_hide_timer
            interval: 3000
            running: true
            repeat: false
            onTriggered: function () {
                progressSlider.hide = true
            }
        }

        Slider
        {
            property bool hide: false
            property bool ready: false

            id: progressSlider
            width: parent.width - 20
            height: 15
            y: parent.height - height - 10
            x: 10
            opacity: hide ? 0 : 1

            Behavior on opacity
            {
                NumberAnimation
                {
                    duration: 300
                }
            }

            enabled: mediaPlayer.seekable
            value: mediaPlayer.duration > 0 ? mediaPlayer.position / mediaPlayer.duration : 0

            background: Rectangle
            {
                implicitHeight: 8
                color: "white"
                radius: 10
            }

            Rectangle
            {
                width: progressSlider.visualPosition * parent.width
                height: parent.height
                color: "#1D8BF8"
                radius: 10
            }

            handle: Item {}

            onMoved: function ()
            {
                if (mediaPlayer.playbackState === MediaPlayer.PlayingState)
                {
                    mediaPlayer.pause()
                }

                root.show_slider()
            }

            onPressedChanged: function (pressed)
            {
                if (pressed)
                {
                    mediaPlayer.stop()

                    root.show_slider()
                }
                else
                {
                    mediaPlayer.position = mediaPlayer.duration * progressSlider.position

                    mediaPlayer.play ()
                }
            }
        }

        MouseArea
        {
            propagateComposedEvents: true
            z: -1
            anchors.fill: parent
            onWheel: function (wheel)
            {
                if (wheel.angleDelta.y > 0)
                {
                    root.volume += root.volume_step;
                }
                else
                {
                    root.volume -= root.volume_step;
                }

                if (root.volume > 1)
                {
                    root.volume = 1
                }
                else if (root.volume < 0)
                {
                    root.volume = 0
                }

                wheel.accepted = true
            }
        }
        Component.onCompleted: {
            root.ready = true

            mediaPlayer.position = mediaPlayer.duration * progressSlider.position
            mediaPlayer.play()
        }
    }
}