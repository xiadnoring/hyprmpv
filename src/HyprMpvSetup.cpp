#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "HyprMpvSetup.h"
#include "HyprMpvConfig.h"

int HyprMpv::setup (int argc, char *argv[])
{
    // setup configs
    if (HyprMpv::Config::setup (argc, argv) < 0)
    {
        // exit
        return 0;
    }

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // register custom objects
    qmlRegisterType <ConfigObject> ("HyprMpv", 1, 0, "HyprMpvConfig");

    const QUrl url(u"/home/Timur/CLionProjects/hyprmpv/tests/theme.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}