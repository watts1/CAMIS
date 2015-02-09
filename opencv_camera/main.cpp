#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

#include "wtypes.h"

#include <iostream>
#include <Windows.h>

using namespace cv;

#define MAX_THREADS 3

struct thread_data
{
	int t_id;
	thread_data(int id) : t_id(id) {}
};

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}

DWORD WINAPI thread_func(LPVOID lpParameter)
{
	thread_data *td = (thread_data*)lpParameter;
 	int c;
	int horizontal = 0;
	int vertical = 0;
	int id = td->t_id;
	GetDesktopResolution(horizontal, vertical);

	VideoCapture cap(id); // open the video camera no. 0

	if (!cap.isOpened())  // if not success, exit program
	{
		std::cout << "Cannot open the video cam" << std::endl;
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
	std::cout << "id: " << id << " Frame size : " << dWidth << " x " << dHeight << std::endl;
	std::cout << "monitor size: " << horizontal << "x by " << vertical <<  "y" << std::endl;
	if (id == 0){
		namedWindow("MyVideo0",CV_WINDOW_NORMAL); //create a window called "MyVideo"
		resizeWindow("MyVideo0", 720, 540);
		moveWindow("MyVideo0", 0,0);
	} else if (id == 1) {
		namedWindow("MyVideo1",CV_WINDOW_NORMAL); //create a window called "MyVideo"
		resizeWindow("MyVideo1", 720, 540);
		moveWindow("MyVideo1", 0,540);
	} else if (id == 2) {
		namedWindow("MyVideo2",CV_WINDOW_NORMAL); //create a window called "MyVideo"
		resizeWindow("MyVideo2", 720, 540);
		moveWindow("MyVideo2", 960,540);
	}
	const string name = "MyVideo" + std::to_string(static_cast<long long>(id));

	std::wstring stemp = std::wstring(name.begin(), name.end());
	LPCWSTR sw = stemp.c_str();
	HWND win_handle = FindWindow(0, sw);
	if (!win_handle)
	{
		printf("Failed FindWindow\n");
	}

	SetWindowLong(win_handle, GWL_STYLE, GetWindowLong(win_handle, GWL_EXSTYLE) | WS_EX_TOPMOST);
	ShowWindow(win_handle, SW_SHOW);

	Mat frame;
	for(;;) {
		cap >> frame; // get a new frame from camera
		//imshow("MyVideo", frame);
		imshow(name, frame);
		c = waitKey(1000/24); // wait 10 ms or for key stroke
		if(c == 27)
			break; // if ESC, break and quit
	}

	return 0;
}


int main(int argc,char *argv[])
{
	HANDLE  hThreadArray[MAX_THREADS]; 

	for (int i=0; i < MAX_THREADS; i++)
	{
		hThreadArray[i] = CreateThread(NULL, 0, thread_func, new thread_data(i) , 0, 0);
		if (hThreadArray[i] == NULL) 
		{
			//ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	}
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

    // Close all thread handles and free memory allocations.
    for(int i=0; i<MAX_THREADS; i++)
    {
        CloseHandle(hThreadArray[i]);
    }

	return 0;
}

