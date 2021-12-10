#include "math_functions.hpp"
#include "aprender.hpp"

FileStorage fs("objetos.yml", FileStorage::READ);

int tamanyo;

const char * nombre = "Contorno";

int main(int argc, char ** argv){

	if (argc != 2){
		cout << "Uso: ./reconocer [imagen_a_reconocer]" << endl;
		return 0;
	}

	cout << "[+] Reconociendo objetos en " << argv[1] << endl;
	reconocer(argv[1]);


	for(;;){
		char c = (char)waitKey( 0 );
		if( c == 27 ){ 
		  break;  
		}
	}
	destroyAllWindows();

}


void reconocer( string imagen ){
	src_gray = imread( imagen, CV_LOAD_IMAGE_GRAYSCALE ); // Load an image pass in argv as first parameter

	Mat dst = Mat::zeros(src_gray.size(), CV_8UC3);

	if (src_gray.empty()){
		cout << "ERROR de lectura en la imagen" << imagen << endl;
	}

	threshold( src_gray, otsu, 0, MAX_BINARY_VALUE, THRESH_BINARY_INV | THRESH_OTSU ); // OTSU NO RECONOCE AGUJEROS OSCURILLOS 

	//adaptiveThreshold( src_gray, adaptive, 
	//      MAX_BINARY_VALUE, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 85, 30); // RECONOCE PEOR

	vector<vector<Point> > contours;

	findContours(otsu, contours, hierarchy, 
	    CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0) );

	
	cout << "[+] Clasificando contornos detectados." << endl;
	for (int i = 0; i < contours.size(); ++i){
		
		tamanyo = clasificar(contours[i]);
		if (tamanyo > 500){
			Scalar color( rand()&255, rand()&255, rand()&255 );
			drawContours( dst, contours, i, color, CV_FILLED, 8, hierarchy, 0, Point() );
			Scalar border( 0, 0, 255 );
			drawContours( dst, contours, i, border, 1, 8, hierarchy, 0, Point() );
			
		}
	}
	namedWindow("Contornos", CV_WINDOW_AUTOSIZE);
	imshow("Contornos" , dst);
}

int clasificar( vector<Point> cont ){
  
	int exitos = 0;
	string objeto = "No reconocido";

	double areaM, areaV, perimM, perimV, m1M, m1V, m2M, m2V, m3M, m3V;

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

	cout << "Area: " << area << endl;
	cout << "Perimetro: " << perimeter << endl;
	cout << "MU1: " << mu1 << endl;
	cout << "MU2: " << mu2 << endl;
	cout << "MU3: " << mu3 << endl;


	if (area > 500){
		for (int i = 0; i < 5; i++){
			fs [ objetos[i] ];
			fs [ "Area-Media" ] >> areaM;
			fs [ "Area-Varianza" ] >> areaV;
			fs [ "Perimetro-Media" ] >> perimM;
			fs [ "Perimetro-Varianza" ] >> perimV;
			fs [ "MI1-Media" ] >> m1M;
			fs [ "MI1-Varianza" ] >> m1V;
			fs [ "MI2-Media" ] >> m2M;
			fs [ "MI2-Varianza" ] >> m2V;
			fs [ "MI3-Media" ] >> m3M;
			fs [ "MI3-Varianza" ] >> m3V;
		
			//Aplicamos el test de chi-cuadrado

			double chi = (area - areaM)/areaV + (perimeter - perimM) / perimV + (mu1 - m1M)/m1V + 
			  (mu2 - m2M) / m2V + (mu3 - m3M) / m3V;

			cout << objetos[i] << endl;
			cout << (area - areaM)/areaV << endl;
			cout << (perimeter - perimM) / perimV << endl;
			cout << (mu1 - m1M)/m1V << endl;
			cout << (mu2 - m2M) / m2V << endl;
			cout << (mu3 - m3M) / m3V << endl;

			if(chi <  11,0705){
				
			} /* if */
		} /* for */
	}
	return area;
 }