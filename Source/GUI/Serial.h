#ifndef SERIAL_H
#define SERIAL_H

//** Written by Mark Mahony & Nick Oudyk
// Serial code uses mostly Microsoft provided code.

//TODO: Identify correct COM port to use, using name. 
//See http://stackoverflow.com/questions/304986/how-do-i-get-the-friendly-name-of-a-com-port-in-windows.
#include <stdio.h>
#include <Windows.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdebug.h>

#define COMPORT L"COM8"
#define PANRATE 2
#define PICYSIZE 3072
#define PICXSIZE 1308
#define WINXSIZE PICXSIZE-480
#define WINYSIZE PICYSIZE-640
#define CNTR_INIT_X (PICXSIZE/2)-240
#define CNTR_INIT_Y (PICYSIZE/2)-320
#define SLOW_THRESH 7
#define MID_THRESH 18
#define FAST_THRESH 25

class Serial
{
private:
	double xangle = 0, yangle = 0, zangle = 0;
    int windowX = CNTR_INIT_X, windowY = CNTR_INIT_Y;
    int winxsize = WINXSIZE;
	int winysize = WINYSIZE;
	HANDLE serialHandle;
    void parseBuffer(char * buffer);
	void updateWindow();


public:
    Serial(){}
    ~Serial();
    int getData(HANDLE port);
	HANDLE openSerial(LPCWSTR comPort);
    int getx();
    int gety();
    int getxangle();
    int getyangle();
    int getzangle();
};


#endif
