#ifndef HYPRMPV_HYPRMPVCONFIG_H
#define HYPRMPV_HYPRMPVCONFIG_H

#include <QObject>
#include <QString>

namespace HyprMpv
{
    extern std::string media_url;

    namespace Config
    {
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
