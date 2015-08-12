#include "DisplayStitcHD.h"

void DisplayStitcHD::onUpdatePosition(qint64 posx, qint64 posy, qint64 anglex, qint64 angley, qint64 anglez){


	// May need to be reversed (posx<->posy)
	scroll->verticalScrollBar()->setValue(posy);
	scroll->horizontalScrollBar()->setValue(posx);
	/*
	ui->dot->setPixmap(QPixmap::fromImage(image.copy(posy, posx, posy + 640, posx + 480)));
	ui->Lbl_xangle->setText(QString::number(anglex));
	ui->Lbl_yangle->setText(QString::number(angley));
	ui->Lbl_zangle->setText(QString::number(anglez));
	ui->Lbl_xpos->setText(QString::number(posx));
	ui->Lbl_ypos->setText(QString::number(posy));
	*/
}
// Head Mounted Controller Related.
void DisplayStitcHD::StartPolling(QString comPort){
	//Create worker thread.
	QThread* thread = new QThread;
	PollingThread* workerThread = new PollingThread();

	QSignalMapper * mapper = new QSignalMapper(this);

	// Set up signal mapper to pass argument to new thread.
	connect(mapper,
		SIGNAL(mapped(QString)),
		workerThread,
		SLOT(pollPort(QString)));

	mapper->setMapping(thread, comPort);

	//Connect slots and signals to update dot position.
	// Start polling after thread start.
	connect(thread,
		SIGNAL(started()),
		mapper,
		SLOT(map()));
	// Update position form telemetry data.
	connect(workerThread,
		SIGNAL(updatePosition(qint64, qint64, qint64, qint64, qint64)),
		this,
		SLOT(onUpdatePosition(qint64, qint64, qint64, qint64, qint64)));
	// Send debug message.
	connect(workerThread,
		SIGNAL(sendDebug(QString)),
		this,
		SLOT(debugBox(QString)));
	// For Testing purposes, makes sure signals are firing.
	/*connect(workerThread,
	SIGNAL(sendDebug(QString)),
	qApp,
	SLOT(aboutQt()));*/
	// Start new thread.

	workerThread->moveToThread(thread);

	qDebug("Starting Thread");

	thread->start();
}

void DisplayStitcHD::debugBox(QString msg){
	QMessageBox msgBox;
	msgBox.setText(msg);
	msgBox.exec();
}

// Returns array detailing available com ports, by parsing array. E.g. list[1] = 1 means COM1 exists.
// list[1] = 0, means COM1 does not exist.
char * DisplayStitcHD::listCOM()
{
	char path[10000]; // buffer to store the path of the COMPORTS
	char name[5];
	char list[256];	// Return list.
	DWORD test;
	bool gotPort = 0; // in case the port is not found


	for (int i = 0; i<255; i++) // checking ports from COM0 to COM255
	{
		sprintf(path, "COM%d", i);

		test = QueryDosDevice((LPCWSTR)name, (LPWSTR)path, 5000);

		// Test the return value and error if any
		if (test != 0) //QueryDosDevice returns zero if it didn't find an object
		{
			gotPort = TRUE;
			list[i] = 1;
		}
		else
		{
			list[i] = 0;
		}
	}

	if (gotPort) 
	{
		return list;
	}
	else
	{
		return NULL;
	}
}