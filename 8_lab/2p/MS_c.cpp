#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Estructura para almacenar un rectángulo con su color
struct Rectangulo {
    double x0, y0, x1, y1;
    string color;
};

vector<Rectangulo> rectangulos;

// Función que dibuja un rectángulo almacenándolo para el archivo EPS
void dibujar_rectangulo(double x0, double y0, double x1, double y1, const string &color) {
    rectangulos.push_back({x0, y0, x1, y1, color});
}

// Función de evaluación
double fun(double x, double y) {
    return x * x + y * y - 9;  // Un círculo como ejemplo de función implícita
}

// Algoritmo adaptativo de Marching Squares
void marchingSquares(double x0, double y0, double x1, double y1) {
    const int N_SAMPLES = 10;
    const double PRECISION_X = 0.01;
    const double PRECISION_Y = 0.01;

    int nNegativos = 0;
    int nPositivos = 0;
    int nCeros = 0;

    // Define los colores para el rectángulo
    const string COLOR_FUERA = "red";
    const string COLOR_DENTRO = "blue";
    const string COLOR_BORDE = "black";

    for (int i = 0; i < N_SAMPLES; ++i) {
        // Coordenas aleatorias dentro del rectángulo
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
        // dentro
        dibujar_rectangulo(x0, y0, x1, y1, COLOR_DENTRO);
    } else {
        if (x1 - x0 < PRECISION_X && y1 - y0 < PRECISION_Y) {
            // borde
            dibujar_rectangulo(x0, y0, x1, y1, COLOR_BORDE);
        } else {
            // Dividir en 4 cuadrantes y procesar recursivamente
            double xMid = (x0 + x1) / 2;
            double yMid = (y0 + y1) / 2;
            marchingSquares(x0, y0, xMid, yMid);
            marchingSquares(xMid, y0, x1, yMid);
            marchingSquares(x0, yMid, xMid, y1);
            marchingSquares(xMid, yMid, x1, y1);
        }
    }
}

// Función para escribir todos los rectángulos en un archivo EPS
void exportToEPS(const string &filename) {
    ofstream epsFile(filename);

    // Encabezado EPS
    epsFile << "%!PS-Adobe-3.0 EPSF-3.0\n";
    epsFile << "%%BoundingBox: 0 0 500 500\n";
    epsFile << "1 setlinewidth\n";

    // Dibujar cada rectángulo acumulado en el vector
    for (const auto& rect : rectangulos) {
        // Establecer el color según el rectángulo
        if (rect.color == "red") {
            epsFile << "1 0 0 setrgbcolor\n";  // Rojo
        } else if (rect.color == "blue") {
            epsFile << "0 0 1 setrgbcolor\n";  // Azul
        } else if (rect.color == "black") {
            epsFile << "0 0 0 setrgbcolor\n";  // Negro
        }

        // Dibujar el rectángulo (con escala para que se vea en EPS)
        epsFile << "newpath\n";
        epsFile << rect.x0 * 100 << " " << rect.y0 * 100 << " moveto\n";
        epsFile << rect.x1 * 100 << " " << rect.y0 * 100 << " lineto\n";
        epsFile << rect.x1 * 100 << " " << rect.y1 * 100 << " lineto\n";
        epsFile << rect.x0 * 100 << " " << rect.y1 * 100 << " lineto\n";
        epsFile << "closepath\nfill\n";  // Rellenar el rectángulo con el color
    }

    // Finalizar el archivo EPS
    epsFile << "showpage\n";
    epsFile.close();
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // Semilla para números aleatorios

    // Ejecutar Marching Squares en un rango (-1.5, -1.5) a (1.5, 1.5)
    marchingSquares(-1.5, -1.5, 1.5, 1.5);

    // Exportar a archivo EPS
    exportToEPS("marching_squares_rectangles.eps");

    cout << "Archivo EPS generado exitosamente.\n";
    return 0;
}
