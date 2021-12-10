#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

/**
 * @function main
 */
int main( int argc, char** argv )
{

  // Fuente: http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
  Mat src, dst;

  /// Load image
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened()){
    // check if we succeeded
    return -1;
  }  
    
   
  cap >> src; // get a new frame from camera

  if( !src.data ){ 
    return -1; 
  }


  /// Display
  namedWindow("Original", CV_WINDOW_AUTOSIZE );
  imshow("Original", src );

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( src, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  // Draw the histograms for B, G and R
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  namedWindow("Histograma", CV_WINDOW_AUTOSIZE );
  imshow("Histograma", histImage );

  // ECUALIZACION DE IMAGEN
  Mat equalized_b, equalized_g, equalized_r;

  // ECUALIZAMOS POR COLORES
  equalizeHist( bgr_planes[0], equalized_b );
  equalizeHist( bgr_planes[1], equalized_g );
  equalizeHist( bgr_planes[2], equalized_r );

  vector<Mat> rgb;
  Mat equalized;

  rgb.push_back(equalized_b);
  rgb.push_back(equalized_g);
  rgb.push_back(equalized_r);

  merge(rgb, equalized);

  /// Compute the histograms:
  calcHist( &equalized_b, 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &equalized_g, 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &equalized_r, 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  Mat histImageEcualizada( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  normalize(b_hist, b_hist, 0, histImageEcualizada.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImageEcualizada.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImageEcualizada.rows, NORM_MINMAX, -1, Mat() );

  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImageEcualizada, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImageEcualizada, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImageEcualizada, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  namedWindow("Imagen ecualizada", 1);
  imshow("Imagen ecualizada", equalized);

  /// Display
  namedWindow("Histograma ecualizado", CV_WINDOW_AUTOSIZE );
  imshow("Histograma ecualizado", histImageEcualizada );

  waitKey(0); //wait for key press

  destroyAllWindows(); //destroy all open windows

  return 0;
}