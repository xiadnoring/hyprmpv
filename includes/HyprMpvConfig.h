#ifndef HYPRMPV_HYPRMPVCONFIG_H
#define HYPRMPV_HYPRMPVCONFIG_H

#include <QObject>
#include <QString>

namespace HyprMpv
{
    namespace Config
    {
        extern std::string media_url;
        extern int use_vulkan;

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
