#include <cv.h>
#include <highgui.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imageproc/imageproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

int main( int argc, char** argv )
{
	VideoCapture cap(0); // open the default camera

    if(!cap.isOpened()){
    	// check if we succeeded
        return -1;
    }  
   
   
    Mat image, dst; // get a new frame from camera
	Mat new_image = Mat::zeros( image.size(), image.type() );

	cap >> image;

	cout<<" Mejora de contraste "<<endl;
	cout<<"-------------------------"<<endl;

	cout<<"* Introduzca valor de alfa [1.0 - 3.0]: ";
	cin>>alpha;

	cout<<"* Introduzca valor de beta: "; 
	cin>>beta;


	image.convertTo(new_image, -1, alpha, beta);

	namedWindow("Original", 1);
	imshow("Original", image );

	namedWindow("Contraste", 1);
	imshow("Contraste", new_image );

	waitKey(0); //wait for key press

    destroyAllWindows(); //destroy all open windows

  return 0;
}