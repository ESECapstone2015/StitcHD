#ifndef POLLINGTHREAD_H
#define POLLINGTHREAD_H

#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtCore/qtextstream.h>
#include <iostream>
#include "Serial.h"


class PollingThread : public QObject {
	Q_OBJECT
signals:
    Q_SIGNAL void updatePosition(qint64 posx, qint64 posy, qint64 anglex, qint64 angley, qint64 anglez);
    void sendDebug(QString msg);
public slots:
	void pollPort(QString comPort);
};

#endif // POLLINGTHREAD_H

