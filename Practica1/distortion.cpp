#include <cv.h>
#include <highgui.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imageproc/imageproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

double k1;
double k2;

double radius2(int x_pos, int y_pos, int x_centro, int y_centro) {
    double x = x_pos - x_centro;
    double y = y_pos - y_centro;

    return (x*x + y*y);
}

int main( int argc, char** argv )
{
	VideoCapture cap(0); // open the default camera

    if(!cap.isOpened()){
    	// check if we succeeded
        return -1;
    }  

    cout<<" Distorsion "<< endl;
	cout<<" ---------- "<< endl;

	cout<<"* Introduzca el valor de k1: ";
	cin >> k1;

	cout<<"* Introduzca el valor de k2: "; 
	cin >> k2;

    /// Global variables
 	Mat src, dst, abajo, giro, todo;
 	Mat map_x, map_y;

    cap >> src;

    /// Create dst, map_x and map_y with the same size as src:
  	dst.create( src.size(), src.type() );
 	map_x.create( src.size(), CV_32FC1 );
  	map_y.create( src.size(), CV_32FC1 );

    int nl = src.rows; // number of lines
    int nc = src.cols; // number of cols

    int x_central = nc/2;
    int y_central = nl/2;

    cout << nl << endl;
    cout << nc << endl;
    cout << x_central << endl;
    cout << y_central << endl;

    double rsquared;
    double x_corr;
    double y_corr;

    for(int y=0; y <= nl; y++){
    	for(int x=0; x <= nc; x++){
     		rsquared = radius2(x,y,x_central,y_central);
     		cout << x << " - " << y << " - " << rsquared << endl;
     		x_corr = x/(1 + k1*rsquared + k2*rsquared*rsquared);
     		y_corr = y/(1 + k1*rsquared + k2*rsquared*rsquared);
     		map_x.at<float>(y,x) = x_corr;
     		map_y.at<float>(y,x) = y_corr;
 	  	}
    }

    remap(src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0));

    namedWindow("Original", 1);
	imshow("Original", src );

	namedWindow("Distorsionada", 1);
	imshow("Distorsionada", dst );


	// for(int y=0; y < nl; y++){
 //    	for(int x=0; x < nc; x++){
 //    		map_x.at<float>(y,x) = x;
 //    		map_y.at<float>(y,x) = nl - y;
 //    	}
 //    }

 //    remap(src, abajo, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0));

 //    namedWindow("Boca abajo", 1);
	// imshow("Boca abajo", abajo );

	// for(int y=0; y < nl; y++){
 //    	for(int x=0; x < nc; x++){
 //    		map_x.at<float>(y,x) = nc - x;
 //    		map_y.at<float>(y,x) = y;
 //    	}
 //    }

 //    remap(src, giro, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0));

 //    namedWindow("Girado", 1);
	// imshow("Girado", giro );

	// for(int y=0; y < nl; y++){
 //    	for(int x=0; x < nc; x++){
 //    		map_x.at<float>(y,x) = nc - x;
 //    		map_y.at<float>(y,x) = nl - y;
 //    	}
 //    }

 //    remap(src, todo, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0));

 //    namedWindow("Del revés", 1);
	// imshow("Del revés", todo );

	waitKey(0); //wait for key press

    destroyAllWindows(); //destroy all open windows
}
