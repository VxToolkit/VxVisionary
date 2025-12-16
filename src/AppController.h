#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString statusMessage READ statusMessage WRITE setStatusMessage NOTIFY statusMessageChanged)
    QML_ELEMENT

public:
    explicit AppController(QObject* parent = nullptr);

    QString statusMessage() const;

    void setStatusMessage(const QString& message);

public slots:
    void performAction();

    signals:
        void statusMessageChanged();

private:
    QString m_statusMessage;
};