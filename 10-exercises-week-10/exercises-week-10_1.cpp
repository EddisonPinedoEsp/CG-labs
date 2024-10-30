#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

// Marching squares adaptative algorithm
// Formato EPS

/*
const int N_SAMPLES = 100;
const double PRECISION_X = 0.01;
const double PRECISION_Y = 0.01;
const double R = 3;
const double X_FROM = -4;
const double X_TO = 4;
const double Y_FROM = -5;
const double Y_TO = 5;
*/
const int N_SAMPLES = 100;
const double PRECISION_X = 0.5;
const double PRECISION_Y = 0.5;
const double R = 3;
const double X_FROM = -4;
const double X_TO = 4;
const double Y_FROM = -4;
const double Y_TO = 4;

const double SCALE_FACTOR = 100.0;

ofstream epsFile;

double fun(double x, double y) {
    // Circle 
    return x * x + y * y - R * R;
}

void dibujar_rectangulo(double x0, double y0, double x1, double y1, const string &color) {

    double x0s = (x0 - X_FROM) * SCALE_FACTOR;
    double y0s = (y0 - Y_FROM) * SCALE_FACTOR;
    double x1s = (x1 - X_FROM) * SCALE_FACTOR;
    double y1s = (y1 - Y_FROM) * SCALE_FACTOR;

    if (color == "red") {
        epsFile << "1 0 0 setrgbcolor\n"; // Rojo
    } else if (color == "blue") {
        epsFile << "0 0 1 setrgbcolor\n"; // Azul
    } else {
        epsFile << "0 1 0 setrgbcolor\n"; // Verde
    }

    epsFile << x0s << " " << y0s << " moveto\n";
    epsFile << x1s << " " << y0s << " lineto\n";
    epsFile << x1s << " " << y1s << " lineto\n";
    epsFile << x0s << " " << y1s << " lineto\n";
    epsFile << "closepath stroke\n";
}


void marchingSquares(double x0, double y0, double x1, double y1) {

    // Evaluar f en N puntos del rectangulo aleatoriamente

    int nNegativos = 0;
    int nPositivos = 0;
    int nCeros = 0;

    const string COLOR_FUERA = "red";
    const string COLOR_DENTRO = "blue";
    const string COLOR_BORDE = "green";

    for (int i = 0; i < N_SAMPLES; ++i) {

        double x = x0 + static_cast<double>(rand()) / RAND_MAX * (x1 - x0);
        double y = y0 + static_cast<double>(rand()) / RAND_MAX * (y1 - y0);
        double v = fun(x, y);
        if (v < 0) {
            ++nNegativos;
        } else if (v > 0) {
            ++nPositivos;
        } else {
            ++nCeros;
        }
    }

    if (nPositivos == N_SAMPLES) {
        // fuera
        dibujar_rectangulo(x0, y0, x1, y1, COLOR_FUERA);
    } else if (nNegativos == N_SAMPLES) {
        // fuera
        dibujar_rectangulo(x0, y0, x1, y1, COLOR_DENTRO);

    } else {
        if (x1 - x0 < PRECISION_X && y1 - y0 < PRECISION_Y) {
            // dibujar rectangulo
            dibujar_rectangulo(x0, y0, x1, y1, COLOR_BORDE);
        } else {
            double xMid = (x0 + x1) / 2;
            double yMid = (y0 + y1) / 2;
            marchingSquares(x0, y0, xMid, yMid);
            marchingSquares(xMid, y0, x1, yMid);
            marchingSquares(x0, yMid, xMid, y1);
            marchingSquares(xMid, yMid, x1, y1);
        }
    }
}


int main() {
    // Abrimos el archivo EPS para escribir
    epsFile.open("marching_squares_scaled.eps");

    // Encabezado EPS y ajuste de escala
    epsFile << "%!PS-Adobe-3.0 EPSF-3.0\n";
    epsFile << "%%BoundingBox: 0 0 " << (X_TO - X_FROM) * SCALE_FACTOR << " " << (Y_TO - Y_FROM) * SCALE_FACTOR << "\n";
    
    // Llamamos al algoritmo de Marching Squares para el rectángulo completo
    marchingSquares(X_FROM, Y_FROM, X_TO, Y_TO);
    
    // Cerramos el archivo EPS
    epsFile << "showpage\n";
    epsFile.close();

    cout << "Gráfico EPS escalado generado: marching_squares_scaled.eps" << endl;

    return 0;
}