#include <cv.h>
#include <highgui.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imageproc/imageproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

bool skinRGB(int R, int G, int B) {
    bool e1 = (R>95) && (G>40) && (B>20) && ((max(R,max(G,B)) - min(R, min(G,B)))>15) && (abs(R-G)>15) && (R>G) && (R>B);
    bool e2 = (R>220) && (G>210) && (B>170) && (abs(R-G)<=15) && (R>B) && (G>B);
    return (e1||e2);
}

bool skinHSV(float H, float S, float V) {
    return (H<25) || (H > 230);
}

Mat GetSkin(Mat const &src) {
    // allocate the result matrix
    Mat dst = src.clone();

    Vec3b cwhite = Vec3b::all(255);
    Vec3b cblack = Vec3b::all(0);

    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);

    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {

            Vec3b pix_bgr = src.ptr<Vec3b>(i)[j];
            int B = pix_bgr.val[0];
            int G = pix_bgr.val[1];
            int R = pix_bgr.val[2];

            // apply rgb rule
            bool a = skinRGB(R,G,B);

            Vec3b cred(B,0,0); 

            Vec3f pix_hsv = hsv.ptr<Vec3f>(i)[j];
            float H = pix_hsv.val[0];
            float S = pix_hsv.val[1];
            float V = pix_hsv.val[2];
            // apply hsv rule
            bool b = skinHSV(H,S,V);

            // si detecta piel se pone
            if(a && b){
                dst.ptr<Vec3b>(i)[j] = cred;
            }
        }
    }
    return dst;
}


int main(int argc, const char *argv[]) {
   
	VideoCapture cap(0); // open the default camera

    if(!cap.isOpened()){
    	// check if we succeeded
        return -1;
    }  

    Mat image;

    namedWindow("skin", 1);

    while (1) {
        Mat image;

        bool bSuccess = cap.read(image);
        Mat skin = GetSkin(image);

        if (!bSuccess) {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        imshow("skin", skin);

        if (waitKey(30) == 27) {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }
    return 0;
}