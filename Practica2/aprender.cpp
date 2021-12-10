#include <dirent.h>
#include <map>
#include "aprender.hpp"
#include "math_functions.hpp"

using namespace cv;
using namespace std;


FileStorage fs("objetos.yml", FileStorage::WRITE);

const char* binarizada_otsu = "Binarización OTSU";
const char* binarizada_adaptativa = "Binarización Adaptive";
const char* contornos = "Contornos";
const string nombre_dir = "imagenesT2/";
string item;

/** FILES AND MAP INITIALIZATION **/


void inicializarMap(){
  des.insert(make_pair( "Area" , vect ));
  des.insert(make_pair( "Perimetro", vect ));
  des.insert(make_pair( "MI1", vect ));
  des.insert(make_pair( "MI2", vect ));
  des.insert(make_pair( "MI3", vect ));

  ctas.insert(make_pair( "circulo" , des ));
  ctas.insert(make_pair( "triangulo" , des ));
  ctas.insert(make_pair( "rueda" , des ));
  ctas.insert(make_pair( "vagon" , des ));
  ctas.insert(make_pair( "rectangulo" , des ));
}

void iterarDirectorio(string directorio){
  DIR * dir;
  struct dirent * e;
  string element;

  dir = opendir(directorio.c_str());
  if (dir != NULL){
    e = readdir(dir);
    while(e != NULL){
      element = e -> d_name;
      if (strcmp(element.c_str(),".") != 0 && strcmp(element.c_str(),"..") != 0){
        ficheros.push_back(element);
      }
      e = readdir(dir); 
    }

  } else {
    cout << "No se puede leer el directorio" << endl;
  }
  closedir(dir);
}

/*************************************************************/


/**
 * @function main
 */
int main( int argc, char** argv )
{
  inicializarMap();
  iterarDirectorio( nombre_dir );

  cout << "[+] Iniciando entrenamiento..." << endl;

  for(int i=0; i < ficheros.size(); i++){
    item = getItem(ficheros.at(i));
    if(item.compare("no identificable") != 0){
      entrenar(ficheros.at(i),item);
    }
  }

  cout << "[+] Entrenamiento completo." << endl;
  cout << "[+] Mapeando información en objetos.yml." << endl;

  escribirCaracteristicas("circulo");
  escribirCaracteristicas("triangulo");
  escribirCaracteristicas("vagon");
  escribirCaracteristicas("rectangulo");
  escribirCaracteristicas("rueda");

  cout << "[+] Fin de mapping." << endl;

  //reconocer(nombre_dir + "reco1.pgm");

  fs.release();
  
  // for(;;){
  //   char c = (char)waitKey( 0 );
  //   if( c == 27 ){ 
  //     break;  
  //   }
  // }
  // destroyAllWindows();
}

void entrenar(string nombreFichero, string nombreObj){
  //cout << "Reconociendo " << nombreObj << " en " << nombreFichero << endl;

  src_gray = imread( nombre_dir + nombreFichero, CV_LOAD_IMAGE_GRAYSCALE ); // Load an image pass in argv as first parameter

  threshold( src_gray, otsu, 0, MAX_BINARY_VALUE, THRESH_BINARY_INV | THRESH_OTSU ); // OTSU NO RECONOCE AGUJEROS OSCURILLOS 

  //adaptiveThreshold( src_gray, adaptive, 
  //      MAX_BINARY_VALUE, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 85, 30); // RECONOCE PEOR

  vector<vector<Point> > contours;
  
  findContours(otsu, contours, hierarchy, 
        CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0) );

  for (int i=0; i<contours.size(); i++){
    vector<Point> cont = contours[i];
    Moments mom = moments(cont);
    double hu[7];
    HuMoments(mom, hu);

    // AREA
    double area = mom.m00; // Esto también es el momento invariante mu0
    
    // PERMIETRO
    double perimeter = arcLength(cont, true);
    
    // MOMENTOS INVARIANTES
    double mu1 = hu[0];
    double mu2 = hu[1];
    double mu3 = hu[2];

    if (area > 500){ // Descartamos contornos que no interesan para esta práctica           
      // METER EN MAPA
      ctas.at(nombreObj).at("Area").push_back(area);
      ctas.at(nombreObj).at("Perimetro").push_back(perimeter);
      ctas.at(nombreObj).at("MI1").push_back(mu1);
      ctas.at(nombreObj).at("MI2").push_back(mu2);
      ctas.at(nombreObj).at("MI3").push_back(mu3);

    } /* if */
  } /* for */
}

string getItem(string nombreFichero){
  if(nombreFichero.find("circulo") != string::npos ){
    return "circulo";
  } else if (nombreFichero.find("triangulo") != string::npos){
    return "triangulo";
  } else if (nombreFichero.find("vagon") != string::npos){
    return "vagon";
  } else if (nombreFichero.find("rectangulo") != string::npos){
    return "rectangulo";
  } else if (nombreFichero.find("rueda") != string::npos){
    return "rueda";
  } else {
    return "no identificable";
  }
  
}

void escribirCaracteristicas(string nombreObj){

  fs << nombreObj << "[";

  // AREA
  vector<double> v = ctas.at(nombreObj).at("Area");
  double mean = average(v);
  double var = variance(v,mean);
  fs << "{:" << "Area-Media" << mean << "Area-Varianza" << var << "}";

  // PERIMETRO
  v = ctas.at(nombreObj).at("Perimetro");
  mean = average(v);
  var = variance(v,mean);
  fs << "{:" << "Perimetro-Media" << mean << "Perimetro-Varianza" << var << "}";

  // MOMENTO INVARIANTE 1
  v = ctas.at(nombreObj).at("MI1");
  mean = average(v);
  var = variance(v,mean);
  fs << "{:" << "MI1-Media" << mean << "MI1-Varianza" << var << "}";

  // MOMENTO INVARIANTE 2
  v = ctas.at(nombreObj).at("MI2");
  mean = average(v);
  var = variance(v,mean);
  fs << "{:" << "MI2-Media" << mean << "MI2-Varianza" << var << "}";

  // MOMENTO INVARIANTE 3
  v = ctas.at(nombreObj).at("MI3");
  mean = average(v);
  var = variance(v,mean);
  fs << "{:" << "MI3-Media" << mean << "MI3-Varianza" << var << "}";

  fs << "]" ; 

}


