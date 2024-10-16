#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Estructura para representar un vértice
struct Vertice {
    float x, y, z;
};

// Función para escribir la esfera en formato OFF
void escribirOFF(const std::vector<Vertice>& vertices, const std::vector<std::vector<int>>& caras, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);

    archivo << "OFF\n";
    archivo << vertices.size() << " " << caras.size() << " 0\n"; // Números de vértices, caras y aristas
    
    // Escribir los vértices
    for (const auto& vertice : vertices) {
        archivo << vertice.x << " " << vertice.y << " " << vertice.z << "\n";
    }
    
    // Escribir las caras
    for (const auto& cara : caras) {
        archivo << cara.size(); // El número de vértices por cara (en este caso, triángulos = 3 vértices)
        for (int indice : cara) {
            archivo << " " << indice;
        }
        archivo << "\n";
    }

    archivo.close();
}

// Función para generar los vértices y caras de una esfera
void generarEsfera(float radio, int resolucion, std::vector<Vertice>& vertices, std::vector<std::vector<int>>& caras) {
    // Generar los vértices
    for (int i = 0; i <= resolucion; ++i) {
        float phi = M_PI * i / resolucion;
        for (int j = 0; j <= 2 * resolucion; ++j) {
            float theta = 2 * M_PI * j / (2 * resolucion);
            float x = radio * sin(phi) * cos(theta);
            float y = radio * sin(phi) * sin(theta);
            float z = radio * cos(phi);
            vertices.push_back({x, y, z});
        }
    }

    // Generar las caras (triángulos)
    for (int i = 0; i < resolucion; ++i) {
        for (int j = 0; j < 2 * resolucion; ++j) {
            int p1 = i * (2 * resolucion + 1) + j;
            int p2 = p1 + 2 * resolucion + 1;

            // Triángulo 1
            caras.push_back({p1, p2, p1 + 1});
            // Triángulo 2
            caras.push_back({p1 + 1, p2, p2 + 1});
        }
    }
}

int main() {
    float radio = 1.0f; // Radio de la esfera
    int resolucion = 20; // A mayor resolución, más detallada la esfera
    std::vector<Vertice> vertices;
    std::vector<std::vector<int>> caras;

    generarEsfera(radio, resolucion, vertices, caras);

    escribirOFF(vertices, caras, "esfera.off");

    std::cout << "Archivo esfera.off generado exitosamente." << std::endl;

    return 0;
}
