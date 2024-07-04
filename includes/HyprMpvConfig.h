#ifndef HYPRMPV_HYPRMPVCONFIG_H
#define HYPRMPV_HYPRMPVCONFIG_H

#include <QObject>
#include <QString>

#define HYPR_MPV_MEDIA_BACKEND_GSTREAMER    0
#define HYPR_MPV_MEDIA_BACKEND_FFMPEG       1

namespace HyprMpv
{
    namespace Config
    {
        extern std::string media_url;
        extern int use_vulkan;
        extern int media_backend;
        extern int custom_qml;

        bool resolve_url (std::string &url);
        int setup (int argc, char *argv[]);
    }

    class ConfigObject : public QObject
    {
        Q_OBJECT
    public:
        Q_INVOKABLE QString get_media_url ();
    };
}

#endif //HYPRMPV_HYPRMPVCONFIG_H
