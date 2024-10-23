
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Marching squares adaptative algorithm
// Formato EPS



const int N_SAMPLES = 100;
const double PRECISION_X = 0.01;
const double PRECISION_Y = 0.01;
const double R = 3;
const double X_FROM = -4;
const double X_TO = 4;
const double Y_FROM = -5;
const double Y_TO = 5;

double fun(double x, double y) {
    // Circle 
    return x * x + y * y - R * R;
}

void dibujar_rectangulo(double x0, double y0, double x1, double y1, const string &color) {

}


void marchingSquares(double x0, double y0, double x1, double y1) {

    // Evaluar f en N puntos del rectangulo aleatoriamente

    int nNegativos = 0;
    int nPositivos = 0;
    int nCeros = 0;

    // Define los colores para el rectangulo
    const string COLOR_FUERA = "red";
    const string COLOR_DENTRO = "blue";
    const string COLOR_BORDE = "black";

    for (int i = 0; i < N_SAMPLES; ++i) {
        // coordenas aleatorias
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

    // Graficar
    // graficar();

    return 0;
}