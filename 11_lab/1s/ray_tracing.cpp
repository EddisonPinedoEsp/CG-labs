
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;


// Implementar ray tracing apartir de un archivo .OFF

// la recta generada por ese rayo

// ax + by + cz + d = 0
// si tenemos la recta

// x = alfa * dx
// y = alfa * dy
// z = alfa * dz

// sustituyendo en la ecuacion del plano

// a * alfa * dx + b * alfa * dy + c * alfa * dz + d = 0

// ( a * dx + b * dy + c * dz ) * alfa + d = 0

// alfa = -d / ( a * dx + b * dy + c * dz )

// con el alfa calculado, podemos calcular el punto de interseccion


struct Cuadrilatero {
    int vertices[4];
};

void lista_cuadrilateros(vector<Cuadrilatero>& cuadrilateros, const string& archivo) {
    ifstream file(archivo);
    string line;
    bool readFaces = false;

    while (getline(file, line)) {
        if (line == "OFF") {
            continue;
        }
        if (line[0] == '3') {
            readFaces = true;
            continue;
        }
        if (readFaces) {
            Cuadrilatero cuadrilatero;
            stringstream ss(line);
            int numVertices;
            ss >> numVertices;
            if (numVertices == 4) {
                for (int i = 0; i < 4; ++i) {
                    ss >> cuadrilatero.vertices[i];
                }
                cuadrilateros.push_back(cuadrilatero);
            }
        }
    }
    file.close();
}


int main() {
    vector<Cuadrilatero> cuadrilateros;

    lista_cuadrilateros(cuadrilateros, "esfera.off");

    for (const auto& cuadrilatero : cuadrilateros) {
        for (int i = 0; i < 4; ++i) {
            cout << cuadrilatero.vertices[i] << " ";
        }
        cout << endl;
    }


    cout << "Fin" << endl;
    return 0;
}









