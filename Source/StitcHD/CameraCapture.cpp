/*
This file is part of StitcHD.

StitcHD is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

StitcHD is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with StitcHD.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CameraCapture.hpp"
#include "Timer.hpp"

#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

HANDLE hSlot;
LPTSTR SlotName = TEXT("\\\\.\\mailslot\\QuadCamPC\\ImageReady");

int CameraCapture::initialize()
{
	if (initialized)
		return 0;

	video.open(id);

	if (!video.isOpened())
	{
		cout << "ERROR: CameraCapture for video " << id << " could not initialize." << endl;
		
		testing = true;

		// create mailslot for QuadCamPC communication
		//MakeSlot(SlotName);

		cout << "For now, using test images..." << endl;
		switch (id)
		{

		case 0:
			frame = imread("../../Test/BL.bmp");
			break;
		case 1:
			frame = imread("../../Test/BR.bmp");
			break;
		case 2:
			frame = imread("../../Test/TL.bmp");
			break;
		case 3:
			frame = imread("../../Test/TR.bmp");
			break;

		/*case 0:
			frame = imread("../../../QuadCamPC/QuadCamPC/avisynth/cam1000000.png");
			break;
		case 1:
			frame = imread("../../../QuadCamPC/QuadCamPC/avisynth/cam2000000.png");
			break;
		case 2:
			frame = imread("../../../QuadCamPC/QuadCamPC/avisynth/cam3000000.png");
			break;
		case 3:
			frame = imread("../../../QuadCamPC/QuadCamPC/avisynth/cam4000000.png");
			break;*/
		default:
			cout << "No test image exists" << endl;
			return -1;
		}

		if (frame.data == NULL){
			cout << "Could not read test image..." << endl;
			return -1;
		}

		/*cout << "For now, using stock video..." << endl;
		switch (id)
		{
		case 0:
			video = VideoCapture("../../video1.mpeg");
			break;
		case 1:
			video = VideoCapture("../../video2.mpeg");
			break;
		default:
			cout << "No stock video exists" << endl;
			return -1;
		}

		if (!video.isOpened())
			return -1;*/
	}
	else
	{
		setSize(width, height);
	}

	initialized = true;
	return 0;
}

void CameraCapture::setSize(int w, int h)
{
	width = w;
	height = h;

	if (video.isOpened())
	{
		video.set(CV_CAP_PROP_FRAME_WIDTH, width);
		video.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	}
}

int CameraCapture::start()
{
	// If you forgot to initialize, I'll do it for you
	if (!initialized)
	{
		if (initialize())
			return -1;
	}
	
	doneEvent = CreateEvent( 
        NULL,               // default security attributes
        true,				// manual-reset?
        false,              // initial state 
        NULL				// object name
        );

    if (doneEvent == NULL) 
    { 
        printf("CreateEvent failed (%d)\n", GetLastError());
        return -1;
    }

	threadHandle = CreateThread(
			NULL,				// default security attributes
			0,					// use default stack size  
			StartThread,		// thread function name
			this,				// argument to thread function 
			0,					// use default creation flags 
			NULL);				// returns the thread identifier

	if (threadHandle == NULL)
	{
		cout << "Could not start CameraCapture " << id << " thread." << endl;
		return -1;
	}
	
	running = true;
	return 0;
}

int CameraCapture::stop()
{
	if (!running)
		return 0;

	running = false;
	DWORD returnCode;
	
	// This forces all other CamCap threads to close too
	SetEvent(startEvent);
	SetEvent(stopEvent);

	do
	{
		Sleep(10);
		GetExitCodeThread(threadHandle, &returnCode);
	}
	while (returnCode == STILL_ACTIVE);

	return 0;
}

int CameraCapture::run()
{
	cout << "Started CameraCapture " << id << " thread." << endl;

	DWORD waitResult;
	do
	{
		// Wait for startEvent
		waitResult = WaitForSingleObject( 
			startEvent,		// event handle
			INFINITE);		// indefinite wait

		switch (waitResult) 
		{
			// Event object was signaled
			case WAIT_OBJECT_0:
				break; 

			// An error occurred
			default: 
				printf("Error in CaptureThread while waiting for startEvent: (%d)\n", GetLastError()); 
				return -1;
		}

		getFrame();

		// Tell the VideoStitcher we're done.
		SetEvent(doneEvent);

		// Wait for stopEvent
		waitResult = WaitForSingleObject( 
			stopEvent,			// event handle
			INFINITE);			// indefinite wait

		switch (waitResult) 
		{
			// Event object was signaled
			case WAIT_OBJECT_0:
				break; 

			// An error occurred
			default: 
				printf("Error in CaptureThread while waiting for stopEvent: (%d)\n", GetLastError()); 
				return -1;
		}

	} while (running);

	cout << "Ending CameraCapture thread " << id << '.' << endl;
	running = false;
	return 0;
}

void CameraCapture::getFrame()
{
	Timer::send(Timer::Camera, id, Timer::CamTimeval::Start);

	/*if (testing)
	{
		ReadSlot();
	}*/

	if (!testing && video.isOpened() && video.grab())
	{
		if (video.retrieve(frame))
		{
			if (frame.rows > 0 && frame.cols > 0 && inverted)
			{
				try
				{
					Point2f src_center(frame.cols/2.0F, frame.rows/2.0F);
					Mat rot_mat = getRotationMatrix2D(src_center, 180, 1.0);
					Mat rotated;
					warpAffine(frame, rotated, rot_mat, frame.size());
					frame = rotated;
				}
				catch (Exception)
				{
					frame = Mat(0,0,0);
				}
			}
		}
	}

	Timer::send(Timer::Camera, id, Timer::CamTimeval::End);
}

BOOL CameraCapture::ReadSlot()
{
	DWORD cbMessage, cMessage, cbRead;
	BOOL fResult;
	LPTSTR lpszBuffer;
	TCHAR achID[80];
	DWORD cAllMessages;
	HANDLE hEvent;
	OVERLAPPED ov;

	cbMessage = cMessage = cbRead = 0;

	hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleSlot"));
	if (NULL == hEvent)
		return FALSE;
	ov.Offset = 0;
	ov.OffsetHigh = 0;
	ov.hEvent = hEvent;

	fResult = GetMailslotInfo(hSlot, // mailslot handle 
		(LPDWORD)NULL,               // no maximum message size 
		&cbMessage,                   // size of next message 
		&cMessage,                    // number of messages 
		(LPDWORD)NULL);              // no read time-out 

	if (!fResult)
	{
		printf("GetMailslotInfo failed with %d.\n", GetLastError());
		return FALSE;
	}

	if (cbMessage == MAILSLOT_NO_MESSAGE)
	{
		printf("Waiting for a message...\n");
		return TRUE;
	}

	cAllMessages = cMessage;

	while (cMessage != 0)  // retrieve all messages
	{
		
		switch (id)
		{
		case 0:
			frame = imread("../../Test/s3.png");
			break;
		case 1:
			frame = imread("../../Test/s2.png");
			break;
		case 2:
			frame = imread("../../Test/s4.png");
			break;
		default:
			cout << "No test image exists" << endl;
			return -1;
		}

		fResult = GetMailslotInfo(hSlot,  // mailslot handle 
			(LPDWORD)NULL,               // no maximum message size 
			&cbMessage,                   // size of next message 
			&cMessage,                    // number of messages 
			(LPDWORD)NULL);              // no read time-out 

		if (!fResult)
		{
			printf("GetMailslotInfo failed (%d)\n", GetLastError());
			return FALSE;
		}
	}
	CloseHandle(hEvent);
	return TRUE;
}

BOOL WINAPI CameraCapture::MakeSlot(LPTSTR lpszSlotName)
{
	hSlot = CreateMailslot(lpszSlotName,
		0,                             // no maximum message size 
		MAILSLOT_WAIT_FOREVER,         // no time-out for operations 
		(LPSECURITY_ATTRIBUTES)NULL); // default security

	if (hSlot == INVALID_HANDLE_VALUE)
	{
		printf("CreateMailslot failed with %d\n", GetLastError());
		return FALSE;
	}
	return TRUE;
}