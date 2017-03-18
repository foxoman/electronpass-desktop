#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "ActionHandler.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    app.setOrganizationName(QLatin1String("ElectronPass"));
    app.setOrganizationDomain(QLatin1String("electronpass.github.io"));
    app.setApplicationName(QLatin1String("ElectronPass"));
    app.setDesktopFileName(QLatin1String("io.electronpass.desktop"));

    // Set the X11 WML_CLASS so X11 desktops can find the desktop file
    qputenv("RESOURCE_NAME", "io.electronpass.desktop");

    if (QQuickStyle::name().isEmpty())
        QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    ActionHandler actionHandler(argv[0]);

    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("actionHandler", &actionHandler);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
