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
        audioOutput: AudioOutput
        {
            id: audio
            muted: false
            volume: 20
        }
        videoOutput: videoOutput
    }

    Item
    {
        id: root
        anchors.fill: parent
        focus: true

        VideoOutput
        {
            id: videoOutput
            anchors.fill: parent
            visible: true
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

            onActivated:
            {
                switch(mediaPlayer.playbackState)
                {
                    case MediaPlayer.PlayingState:  mediaPlayer.pause(); break;
                    case MediaPlayer.PausedState:   mediaPlayer.play(); break;
                    case MediaPlayer.StoppedState:  mediaPlayer.play(); break;
                }
            }
        }

        Slider
        {
            id: progressSlider
            width: parent.width
            anchors.bottom: parent.bottom
            enabled: mediaPlayer.seekable
            value: mediaPlayer.duration > 0 ? mediaPlayer.position / mediaPlayer.duration : 0

            background: Rectangle {
                implicitHeight: 8
                color: "white"
                radius: 3
            }

            Rectangle {
                width: progressSlider.visualPosition * parent.width
                height: parent.height
                color: "#1D8BF8"
                radius: 3
            }

            handle: Item {}

            onMoved: function () {
                mediaPlayer.pause ()

                mediaPlayer.position = mediaPlayer.duration * progressSlider.position

                mediaPlayer.play ()
            }
        }

        Component.onCompleted:
        {
            mediaPlayer.play()
        }
    }
}