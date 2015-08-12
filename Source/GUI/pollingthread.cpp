#include "pollingthread.h"

void PollingThread::pollPort(QString comPort){
    // Open Serial port
	Serial * serialThread = new Serial();
	HANDLE serialPort = serialThread->openSerial(comPort.toStdString.c_str());

    int i = 0;

    if (serialPort != NULL){
        //emit sendDebug("Serial Port opened successfully.");
    }
    else {
        //emit sendDebug("Serial Port not opened, ERROR.");
        return;
    }

    // Update position data.
    while (1){
        serialThread->getData(serialPort);
        if (i == 2){
            emit updatePosition(serialThread->getx(), serialThread->gety(),
                                serialThread->getxangle(), serialThread->getyangle(), serialThread->getzangle());
            i=0;
        }
        i++;
    }
}
