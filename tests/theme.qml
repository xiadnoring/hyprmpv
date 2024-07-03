import QtQuick
import QtQuick.Controls
import QtMultimedia

import HyprMpv

ApplicationWindow {
    visible: true
    title: "Hyprland Video Player"

    HyprMpvConfig {
        id: config
    }

    MediaPlayer {
        id: mediaPlayer
        source: config.get_media_url()
        audioOutput: AudioOutput {
            id: audio
            muted: false
            volume: 20
        }
        autoPlay: true
        videoOutput: videoOutput
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        visible: true
    }
}