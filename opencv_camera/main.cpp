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

	// Timer objects 
	SYSTEMTIME st;
	SYSTEMTIME st2;
//	SYSTEMTIME st3;
//	SYSTEMTIME st4;

	//GetDesktopResolution(horizontal, vertical);
	VideoCapture cap; // open the video camera no. 0
	cap.open(id);
	if (!cap.isOpened())  // if not success, exit program
	{
		std::cout << id << " Cannot open the video cam" << std::endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH,640); //get the width of frames of the video
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,480); //get the height of frames of the video
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
	double fps = cap.get(CV_CAP_PROP_FPS); //get the fps of the webcam
	std::cout << "id: " << id << " Frame size: " << dWidth << " x " << dHeight << std::endl;
	//std::cout << "id: " << id << " FPS: " << fps << std::endl;
	//std::cout << "monitor size: " << horizontal << "x by " << vertical <<  "y" << std::endl;
	if (id == 0){
		namedWindow("MyVideo0",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO); //create a window called "MyVideo"
		moveWindow("MyVideo0", 0+320,540);
		//resizeWindow("MyVideo0", 704, 502);
		resizeWindow("MyVideo0", 666, 500);
	} else if (id == 1) {
		namedWindow("MyVideo1",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO); //create a window called "MyVideo"
		moveWindow("MyVideo1", 686+320,540);
		//resizeWindow("MyVideo1", 704, 502);
		resizeWindow("MyVideo1", 666, 500);
	} else if (id == 2) {
		namedWindow("MyVideo2",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO); //create a window called "MyVideo"
		moveWindow("MyVideo2", 0+320,0);
		//resizeWindow("MyVideo2", 704, 502);
		resizeWindow("MyVideo2", 666, 500);
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

	const string v_name = "View" + std::to_string(static_cast<long long>(id)) + ".avi";
	VideoWriter video(v_name,CV_FOURCC('x','v','i','d'),10, Size(dWidth,dHeight),true);

	int wait;
	Mat frame;
	for(;;) {
		//GetSystemTime(&st3);
		GetSystemTime(&st);
		cap >> frame; // get a new frame from camera
		if (frame.empty()) {
			std::cout << "Unable to retrieve frame from video stream." << std::endl;
			//VideoCapture cap(id); // open the video camera no. 0
			//Sleep(1000);
			//if (!cap.isOpened())  // if not success, exit program
			//{
			//	std::cout << id << " Cannot open the video cam" << std::endl;
			//	return -1;
			//}
			//cap.set(CV_CAP_PROP_FRAME_WIDTH,640); //get the width of frames of the video
			//cap.set(CV_CAP_PROP_FRAME_HEIGHT,480); //get the height of frames of the video
			//cap >> frame;

			continue;
		}

		//GetSystemTime(&st2);
		//if(id == 0)
		//	std::cout << "cap time: " << st2.wMilliseconds - st.wMilliseconds << std::endl;
		
		//GetSystemTime(&st);
		imshow(name, frame);
		//GetSystemTime(&st2);
		//if(id == 0)
		//	std::cout << "imshow time: " << st2.wMilliseconds - st.wMilliseconds << std::endl;

		//GetSystemTime(&st);
		//video.write(frame);
		//GetSystemTime(&st2);
		//if(id == 0)
		///	std::cout << "write time: " << st2.wMilliseconds - st.wMilliseconds << std::endl;
		
		//GetSystemTime(&st);
		GetSystemTime(&st2);

		wait = st2.wMilliseconds - st.wMilliseconds;
		if (wait < 0)
			wait = wait + 1000; 
		//int seconds = st2.wSecond - st.wSecond;

		if (wait > 49)
			wait = 49;
		//if (wait > 97)
		//	wait = 97;

		//std::cout << id <<" wait time is: " << wait << std::endl;
		//std::cout << "actial wait time is: " << 41-wait << std::endl;
		//GetSystemTime(&st);
		c = waitKey(50-wait); // wait 10 ms or for key stroke
		//c = waitKey(97-wait);
		//GetSystemTime(&st2);

		//std::cout << id <<  " wait key took: " << st2.wMilliseconds - st.wMilliseconds << std::endl;
		//c = waitKey(41); // wait 10 ms or for key stroke
		if(c == 32)
			break; // if ESC, break and quit
		//GetSystemTime(&st2);
		//std::cout << id << std::endl;
		//if(id == 0)
			//std::cout << "wait time: " << st2.wMilliseconds - st.wMilliseconds << std::endl;
		//GetSystemTime(&st4);
		//std::cout << id << " loop time is: " << st4.wMilliseconds - st3.wMilliseconds << std::endl;
	}
	return 0;
}


int main(int argc,char *argv[])
{
	Mat bg(1920, 1280, CV_8UC3, Scalar(0,0,0)); // Black background image

	namedWindow("BG",CV_WINDOW_NORMAL); //create background window
	moveWindow("BG", 0,0);

	HWND win_handle = FindWindow(0, L"BG");
	if (!win_handle)
	{
		printf("Failed FindWindow\n");
	}

	SetWindowLong(win_handle, GWL_STYLE, GetWindowLong(win_handle, GWL_EXSTYLE));
	SetWindowPos(win_handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	ShowWindow(win_handle, SW_SHOW);

	HANDLE  hThreadArray[MAX_THREADS]; 

	for (int i=0; i < MAX_THREADS; i++)
	{
		hThreadArray[i] = CreateThread(NULL, 0, thread_func, new thread_data(i) , 0, 0);
		//Sleep(1000);
		if (hThreadArray[i] == NULL) 
		{
			//ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	}
	 
	Sleep(2000); // A wait is required to prevent artifacts
	imshow("BG", bg);
	waitKey(1);

	WaitForMultipleObjects(MAX_THREADS, hThreadArray, FALSE, INFINITE);

    // Close all thread handles and free memory allocations.
    for(int i=0; i<MAX_THREADS; i++)
    {
        int  p = CloseHandle(hThreadArray[i]);
		if (p == 0) {
			std::cout << "Failed to close thread" << std::endl;
		}
		//std::cout << i << " thread closed" << std::endl;
	}

	std::cout  << "EXITING" << std::endl;

	return 0;
}

