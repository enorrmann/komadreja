#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "audio/AudioEngine.h"
#include "models/PadModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Initialize Audio Engine
    AudioEngine audioEngine;
    if (!audioEngine.init()) {
        qWarning() << "Could not initialize audio engine.";
    }

    // Load sample for all pads
    // (Commented out so pads start empty for recording testing)
    // QString samplePath = ":/assets/samples/kick.wav";
    // for (int i = 0; i < 64; ++i) {
    //     audioEngine.loadSample(i, samplePath);
    // }

    // Initialize Models
    PadModel padModel;

    QQmlApplicationEngine engine;
    
    // Register contexts
    engine.rootContext()->setContextProperty("audioEngine", &audioEngine);
    engine.rootContext()->setContextProperty("padModel", &padModel);

    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/Komadreja/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
