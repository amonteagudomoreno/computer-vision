#include <cv.h>
#include <highgui.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imageproc/imageproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat barrel_pincusion_dist(Mat img, double Cx,double Cy,double kx,double ky)
{
    Mat mapx = cvCreateImage( cvGetSize(img), IPL_DEPTH_32F, 1 );
    Mat mapy = cvCreateImage( cvGetSize(img), IPL_DEPTH_32F, 1 );

    int w= img.width;
    int h= img.height;

    float* pbuf = (float*)mapx.imageData;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {         
            float u= Cx+(x-Cx)*(1+kx*((x-Cx)*(x-Cx)+(y-Cy)*(y-Cy)));
            *pbuf = u;
            ++pbuf;
        }
    }

    pbuf = (float*)mapy.imageData;
    for (int y = 0;y < h; y++)
    {
        for (int x = 0; x < w; x++) 
        {
            *pbuf = Cy+(y-Cy)*(1+ky*((x-Cx)*(x-Cx)+(y-Cy)*(y-Cy)));
            ++pbuf;
        }
    }

    /*float* pbuf = (float*)mapx->imageData;
    for (int y = 0; y < h; y++)
    {
        int ty= y-Cy;
        for (int x = 0; x < w; x++)
        {
            int tx= x-Cx;
            int rt= tx*tx+ty*ty;

            *pbuf = (float)(tx*(1+kx*rt)+Cx);
            ++pbuf;
        }
    }

    pbuf = (float*)mapy->imageData;
    for (int y = 0;y < h; y++)
    {
        int ty= y-Cy;
        for (int x = 0; x < w; x++) 
        {
            int tx= x-Cx;
            int rt= tx*tx+ty*ty;

            *pbuf = (float)(ty*(1+ky*rt)+Cy);
            ++pbuf;
        }
    }*/

    Mat temp = cvCloneImage(img);
    cvRemap( temp, img, mapx, mapy ); 
    cvReleaseImage(&temp);
    cvReleaseImage(&mapx);
    cvReleaseImage(&mapy);

    return img;
}

int main( int argc, char** argv )
{
	VideoCapture cap(0); // open the default camera

    if(!cap.isOpened()){
    	// check if we succeeded
        return -1;
    }  
   
    Mat dst;
	Mat image;

	cap >> image;


	dst = barrel_pincusion_dist(image, 1.0, 1.0, 1.0 ,1.0);

	namedWindow("Barril", 1);
	imshow("Barril", dst);

}

