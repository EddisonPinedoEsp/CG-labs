#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const double SCALE_FACTOR = 100.0;
ofstream epsFile;

// Función para interpolar entre dos puntos
void interpolate_and_draw(double v1, double v2, double x1, double y1, double x2, double y2) {
    if (abs(v1 - v2) > 1e-10) {  // Evitar división por cero
        double t = v1 / (v1 - v2);
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        
        // Escalar y escribir al archivo
        epsFile << x * SCALE_FACTOR << " " 
                << y * SCALE_FACTOR << " lineto\n";
    }
}

void marchingSquares(double (*f)(double, double), double x0, double y0, double x1, double y1, double precision) {
    // Evaluar la función en las cuatro esquinas
    double f00 = f(x0, y0);  // Esquina inferior izquierda
    double f10 = f(x1, y0);  // Esquina inferior derecha
    double f01 = f(x0, y1);  // Esquina superior izquierda
    double f11 = f(x1, y1);  // Esquina superior derecha

    // Calcular el índice del caso
    int caseIndex = 0;
    if (f00 >= 0) caseIndex |= 1;
    if (f10 >= 0) caseIndex |= 2;
    if (f01 >= 0) caseIndex |= 4;
    if (f11 >= 0) caseIndex |= 8;

    // Si el cuadrado es lo suficientemente pequeño y cruza la curva
    if ((x1 - x0 <= precision && y1 - y0 <= precision) && (caseIndex != 0 && caseIndex != 15)) {
        
        double xc = (x0 + x1) / 2;
        double yc = (y0 + y1) / 2;

        // Comenzar nuevo segmento
        epsFile << "newpath\n";
        
        // Punto inicial basado en el caso
        switch (caseIndex) {
            case 1: case 5: case 13:
                epsFile << xc * SCALE_FACTOR << " " << y0 * SCALE_FACTOR << " moveto\n";
                break;
            case 2: case 3: case 7:
                epsFile << x1 * SCALE_FACTOR << " " << yc * SCALE_FACTOR << " moveto\n";
                break;
            case 4: case 12: case 14:
                epsFile << xc * SCALE_FACTOR << " " << y1 * SCALE_FACTOR << " moveto\n";
                break;
            case 8: case 10: case 11:
                epsFile << x0 * SCALE_FACTOR << " " << yc * SCALE_FACTOR << " moveto\n";
                break;
            case 6: case 9:
                // Casos ambiguos - usar el punto central
                epsFile << xc * SCALE_FACTOR << " " << yc * SCALE_FACTOR << " moveto\n";
                break;
        }

        // Dibujar los segmentos correspondientes
        switch (caseIndex) {
            case 1:
                interpolate_and_draw(f00, f10, x0, y0, x1, y0);
                interpolate_and_draw(f00, f01, x0, y0, x0, y1);
                break;
            case 2:
                interpolate_and_draw(f00, f10, x0, y0, x1, y0);
                interpolate_and_draw(f10, f11, x1, y0, x1, y1);
                break;
            case 3:
                interpolate_and_draw(f00, f01, x0, y0, x0, y1);
                interpolate_and_draw(f10, f11, x1, y0, x1, y1);
                break;
            case 4:
                interpolate_and_draw(f00, f01, x0, y0, x0, y1);
                interpolate_and_draw(f01, f11, x0, y1, x1, y1);
                break;
            case 5:
                interpolate_and_draw(f00, f10, x0, y0, x1, y0);
                interpolate_and_draw(f01, f11, x0, y1, x1, y1);
                break;
            case 6:
                interpolate_and_draw(f00, f01, x0, y0, x0, y1);
                interpolate_and_draw(f10, f11, x1, y0, x1, y1);
                break;
            case 7:
                interpolate_and_draw(f01, f11, x0, y1, x1, y1);
                break;
            case 8:
                interpolate_and_draw(f01, f11, x0, y1, x1, y1);
                interpolate_and_draw(f10, f11, x1, y0, x1, y1);
                break;
            case 9:
                interpolate_and_draw(f00, f10, x0, y0, x1, y0);
                interpolate_and_draw(f01, f11, x0, y1, x1, y1);
                break;
            case 10:
                interpolate_and_draw(f00, f10, x0, y0, x1, y0);
                interpolate_and_draw(f01, f11, x0, y1, x1, y1);
                break;
            case 11:
                interpolate_and_draw(f01, f11, x0, y1, x1, y1);
                break;
            case 12:
                interpolate_and_draw(f00, f01, x0, y0, x0, y1);
                break;
            case 13:
                interpolate_and_draw(f10, f11, x1, y0, x1, y1);
                break;
            case 14:
                interpolate_and_draw(f00, f10, x0, y0, x1, y0);
                break;
        }
        
        // Finalizar y dibujar el segmento
        epsFile << "stroke\n";
    }
    // Si el cuadrado es demasiado grande, subdividir
    else if (x1 - x0 > precision || y1 - y0 > precision) {
        double xm = (x0 + x1) / 2;
        double ym = (y0 + y1) / 2;
        
        marchingSquares(f, x0, y0, xm, ym, precision);
        marchingSquares(f, xm, y0, x1, ym, precision);
        marchingSquares(f, x0, ym, xm, y1, precision);
        marchingSquares(f, xm, ym, x1, y1, precision);
    }
}

void draw_curve(double (*f)(double, double), const string& output_filename, double xmin, double ymin, double xmax, double ymax, double precision) {
    epsFile.open(output_filename);
    
    // Encabezado EPS
    epsFile << "%!PS-Adobe-3.0 EPSF-3.0\n";
    epsFile << "%%BoundingBox: 0 0 " 
            << (xmax - xmin) * SCALE_FACTOR << " " 
            << (ymax - ymin) * SCALE_FACTOR << "\n";
    
    // Configuración de línea
    epsFile << "0.5 setlinewidth\n";
    epsFile << "0 0 0 setrgbcolor\n";
    
    // Trasladar y escalar para manejar coordenadas negativas
    epsFile << SCALE_FACTOR * (-xmin) << " " 
            << SCALE_FACTOR * (-ymin) << " translate\n";
    
    marchingSquares(f, xmin, ymin, xmax, ymax, precision);
    
    epsFile << "showpage\n";
    epsFile.close();
    
    cout << "Archivo EPS generado: " << output_filename << endl;
}

// Función ejemplo (círculo de radio 3)
double fun(double x, double y) {
    return x * x + y * y - 9;  // r = 3, por lo tanto r^2 = 9
}

int main() {
    // Prueba con un círculo
    draw_curve(fun, "ci.eps", -4, -4, 4, 4, 0.01);

    return 0;
}