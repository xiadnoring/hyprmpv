#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVulkanInstance>
#include <QQuickWindow>

#include "HyprMpvUtils.h"
#include "HyprMpvSetup.h"
#include "HyprMpvConfig.h"

static QVulkanInstance *vkInstance;

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
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
    Qt::QueuedConnection);

    engine.load(url);

    QObject *rootObject = engine.rootObjects().at(0);
    auto rootWindow = qobject_cast<QQuickWindow*>(rootObject);

    if (Config::use_vulkan == 1)
    {
        debug ("The Vulkan Instance will be used");

        vkInstance = new QVulkanInstance;
        vkInstance->setLayers({ "VK_LAYER_KHRONOS_validation" });
        vkInstance->setExtensions({ VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME });
        vkInstance->create();
        rootWindow->setVulkanInstance(vkInstance);
    }

    int result = app.exec();

    // clean up
    if (Config::use_vulkan == 1)
    {
        delete vkInstance;
    }

    return result;
}