
#include<iostream>
using namespace std;
// Implementar la función 'marchingSquares' para dibujar una curva implícita, usando marchin squares y la subdivisión adaptativa.
// f(x,y) = 0, es la curva implícita que queremos dibujar.
// xmin,ymin,xmax,ymax son las coordenadas del rectángulo que clasificaremos como completamente interior, completamente exterior o que contiene borde; Y subdividiremos o trazaremos una línea basada en los casos de Marching Squares.

// La precisión es la condición de parada de la subdivisión. Cuando ambos xmax - xmin < precision y ymax - ymin < precision pre dejamos de subdividir
// output_filename: Nombre de archivo de salida que se debe generar. Tiene que ser un le en formato EPS.

// Prototipo de la función

struct fun {
    double x;
    double y;

    fun(double x, double y) : x(x), y(y) {}

    double valor() {
        return x * x + y * y - 1;
    }
};

void draw_curve(fun f, string output_filename, double xmin, double ymin, double xmax, double ymax, double precision);






