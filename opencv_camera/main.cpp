#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
void main(int argc,char *argv[])
{
    int c;
    IplImage* color_img;
	CvCapture* cv_cap;
	IplImage* color_img2;
	CvCapture* cv_cap2;

	/*
	for (int i = 0; i < 3; i++){
		cv_cap = cvCaptureFromCAM(i);
		if (cv_cap == NULL){
			std::cout << "no camera!" << std::endl;
		} else {
			break;
		}
	}
	*/
	cv_cap = cvCaptureFromCAM(0);
	cv_cap2 = cvCaptureFromCAM(1);
    cvNamedWindow("Video",0); // create window
	cvNamedWindow("Video2",0); // create window
    for(;;) {
        color_img = cvQueryFrame(cv_cap); // get frame
		color_img2 = cvQueryFrame(cv_cap2); // get frame
		//if(color_img != 0){
        if((color_img != 0) && (color_img2 != 0)){
            cvShowImage("Video", color_img); // show frame
			cvShowImage("Video2", color_img2); // show frame
		}
        c = cvWaitKey(10); // wait 10 ms or for key stroke
        if(c == 27)
            break; // if ESC, break and quit
    }
    /* clean up */
    cvReleaseCapture( &cv_cap );
	cvReleaseCapture( &cv_cap2 );
    cvDestroyWindow("Video");
	cvDestroyWindow("Video2");
}



/*
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], IMREAD_COLOR); // Read the file

    if(! image.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", image ); // Show our image inside it.

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
*/