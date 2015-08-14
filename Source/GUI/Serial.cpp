#include "Serial.h"

#include <QtCore/qdebug.h>

Serial::~Serial(){
    CloseHandle(serialHandle);
}

HANDLE Serial::openSerial(LPCWSTR comPort)
{

	//PortChat::Main();
	HANDLE m_hCommPort = ::CreateFileW(COMPORT,
		GENERIC_READ,  // access ( read and write)
		0,                           // (share) 0:cannot share the COM port
		0,                           // security  (None)
		OPEN_EXISTING,               // creation : open_existing
		FILE_ATTRIBUTE_NORMAL,		//FILE_FLAG_OVERLAPPED,        // we want overlapped operation
		0                            // no templates file for COM port...
		);

	if (m_hCommPort != INVALID_HANDLE_VALUE){
		qDebug("Successfully opened %s\n", QString::fromUtf8((const char *)(COMPORT)));
	}
	else {
		qDebug("Error opening %s: error num : %d\n", QString::fromUtf8((const char *)(COMPORT)), GetLastError());
	}

	DCB dcb = { 0 };
	dcb.DCBlength = sizeof(DCB);

	if (!::GetCommState(m_hCommPort, &dcb))
	{
        qDebug("CSerialCommHelper : Failed to Get Comm State Reason: %d\n", GetLastError());
        //getchar();
		return NULL;
	}

    qDebug("Current Settings: \nBaud Rate %d;	Parity %d; Byte Size %d; Stop Bits %d\n", dcb.BaudRate, dcb.Parity, dcb.ByteSize, dcb.StopBits);

	dcb.BaudRate = 115200;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;


	if (!::SetCommState(m_hCommPort, &dcb))
	{
        qDebug("CSerialCommHelper : Failed to Set Comm State Reason: %d\n", GetLastError());
        //getchar();
		return NULL;
	}
    qDebug("Current Settings: \nBaud Rate %d;	Parity %d; Byte Size %d; Stop Bits %d\n", dcb.BaudRate, dcb.Parity, dcb.ByteSize, dcb.StopBits);

	OVERLAPPED ov;
	memset(&ov, 0, sizeof(ov));
	ov.hEvent = CreateEvent(0, true, 0, 0);

	SetCommMask(m_hCommPort, EV_RXCHAR);

    serialHandle = m_hCommPort;

	return m_hCommPort;
}

int Serial::getData(HANDLE port)
{
	DWORD dwEvtMask;
	DWORD bytesRead = 0;
	char buffer[50] = { 0 };

	static char lineBuffer[50] = { 0 };
	static int index = 0;

	int bufIndex = 0;

	WaitCommEvent(port, &dwEvtMask, NULL);
    ReadFile(port, &buffer, 30, &bytesRead, NULL);
	for (int i = 0; i < bytesRead; i++){
		if (buffer[i] == '\n'){ //we have polled a finished line
			//copy digits of the line into the buffer
			memcpy(&lineBuffer[index], &buffer[bufIndex], sizeof(BYTE)*(i + 1 - bufIndex));
            index += (i + 1 - bufIndex);
            parseBuffer(lineBuffer);
			bufIndex = i + 1;
			index = 0;
            //qDebug("X Angle: %.2f\tY Angle: %.2f\t Z Angle: %.2f\n", xangle, yangle, zangle);
            //qDebug("Window X: %d\t Window Y: %d\n", windowX, windowY);
			updateWindow();
		}
	}
	memcpy(&lineBuffer[index], &buffer[bufIndex], sizeof(BYTE)*(bytesRead - bufIndex));
	index += bytesRead - bufIndex;

	return 0;
}

void Serial::parseBuffer(char * buffer){
    try {        
        QByteArray ba;
        const char *string;
        int i = 0;
        //qDebug("Printing buffer to be parsed.");
        //qDebug(buffer);
        QString bufferTxt = buffer;
        QStringList rows = bufferTxt.split(",");

        if (rows.length() == 3) {

            foreach (QString row, rows){
                ba = row.toLatin1();
                string = ba.data();
                //qDebug(string);
                if (i == 0){
                    xangle = atof(string);
                }
                else if (i == 1){
                    yangle = atof(string);
                }
                else if (i == 2){
                    zangle = atof(string);
                }
                i++;
            }
        } else{
            xangle = 0;
            yangle = 0;
            zangle = 0;
            windowX = CNTR_INIT_X;
            windowY = CNTR_INIT_Y;
        }
    }
    catch (...) {
	}
}

void Serial::updateWindow(){
    if (xangle > FAST_THRESH){
        windowX += (int)(4 * PANRATE);
	}
    else if (xangle > MID_THRESH){
        windowX += (int)(2 * PANRATE);
	}
    else if (xangle > SLOW_THRESH){
        windowX += (int)(1 * PANRATE);
	}
    else if (xangle < -FAST_THRESH){
        windowX -= (int)(4 * PANRATE);
	}
    else if (xangle < -MID_THRESH){
        windowX -= (int)(2 * PANRATE);
	}
    else if (xangle < -SLOW_THRESH){
        windowX -= (int)(1 * PANRATE);
	}

    if (yangle > FAST_THRESH){
        windowY -= (int)(4 * PANRATE);
	}
    else if (yangle > MID_THRESH){
        windowY -= (int)(2 * PANRATE);
	}
    else if (yangle > SLOW_THRESH){
        windowY -= (int)(1 * PANRATE);;
	}
    else if (yangle < -FAST_THRESH){
        windowY += (int)(4 * PANRATE);
	}
    else if (yangle < -MID_THRESH){
        windowY += (int)(2 * PANRATE);
	}
    else if (yangle < -SLOW_THRESH){
        windowY += (int)(1 * PANRATE);
	}

    if (windowX > WINXSIZE){
        windowX = WINXSIZE;
	}
	else if (windowX < 0){
		windowX = 0;
	}

    if (windowY > WINYSIZE){
        windowY = WINYSIZE;
	}
	else if (windowY < 0){
		windowY = 0;
	}
}

int Serial::getx()
{
	return windowX;
}

int Serial::gety()
{
	return windowY;
}

int Serial::getxangle()
{
    return xangle;
}

int Serial::getyangle()
{
    return yangle;
}

int Serial::getzangle()
{
    return zangle;
}
