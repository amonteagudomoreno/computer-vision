#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/** MATH FUNCTIONS **/

//Function for average
double average( vector<double>& v ){
  double return_value = 0.0;
  int n = v.size();
 
  for ( int i=0; i < n; i++ )
  {
      return_value += v[i];
  }
 
  return ( return_value / n);
}


//Function for variance
double variance( vector<double>& v , double mean ){
  double sum = 0.0;
  double temp = 0.0;

  int n = v.size();
 
  for ( int j =0; j < n; j++ )
  {
      temp = pow((v[j] - mean),2);
      sum += temp;
  }
 
  return sum/n;
}

//Function for standard deviation
double stdev( vector<double>& v ){
	return sqrt(variance(v, average(v)));
}

/******** END FUNCTIONS **********/

#endif