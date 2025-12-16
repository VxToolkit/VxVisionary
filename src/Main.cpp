#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSplashScreen>
#include <QPixmap>
#include <QQuickWindow>
#include <QTimer>
#include <QDebug>
#include <QDirIterator>

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPixmap pixmap(":/qt/qml/Main/assets/VXVisionarySplash.png");
    QPixmap new_pixmap = pixmap.scaled(600*2, 400*2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QSplashScreen splash(new_pixmap);
    splash.show();
    app.processEvents();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qt/qml/Main/content/Main.qml"_s);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    QTimer::singleShot(2000, [&splash, &engine, &url](){
        splash.finish(nullptr);
        engine.load(url);
    });

    return app.exec();
}