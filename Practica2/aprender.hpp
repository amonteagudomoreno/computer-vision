#ifndef APRENDER_H
#define APRENDER_H


#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>

#define descriptor map<string,vector<double> >
#define features map<string,descriptor>

#define MAX_BINARY_VALUE 255

using namespace cv;
using namespace std;


/** VARIABLES DECLARATION **/
Mat src_gray, otsu, adaptive, contours;

vector<double> vect;

vector<string> ficheros;

features ctas;
descriptor des;

string objetos[5]={"circulo","rectangulo","triangulo","rueda","vagon"};

vector<Vec4i> hierarchy;

/**** END OF DECLARATION ******/

/** FUNCTION DECLARATION **/

void entrenar( string, string );

void reconocer( string );

int clasificar( vector<Point> ); 

string getItem( string );

void escribirCaracteristicas( string );

/**** END OF DECLARATION ******/

#endif



