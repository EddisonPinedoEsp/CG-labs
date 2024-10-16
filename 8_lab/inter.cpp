#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Estructura para representar un vértice
struct Vertice {
    float x, y, z;
    
    // Normalizar un vértice al radio dado (proyectar sobre la esfera)
    void normalizar(float radio) {
        float magnitud = std::sqrt(x * x + y * y + z * z);
        x = (x / magnitud) * radio;
        y = (y / magnitud) * radio;
        z = (z / magnitud) * radio;
    }
};

// Función para generar el punto medio de dos vértices y proyectarlo a la esfera
Vertice puntoMedio(const Vertice& v1, const Vertice& v2, float radio) {
    Vertice medio = {(v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2};
    medio.normalizar(radio);
    return medio;
}

// Función para escribir la esfera subdividida en formato PLY
void escribirPLY(const std::vector<Vertice>& vertices, const std::vector<std::vector<int>>& caras, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);

    archivo << "ply\nformat ascii 1.0\n";
    archivo << "element vertex " << vertices.size() << "\n";
    archivo << "property float x\nproperty float y\nproperty float z\n";
    archivo << "element face " << caras.size() << "\n";
    archivo << "property list uchar int vertex_indices\n";
    archivo << "end_header\n";
    
    // Escribir los vértices
    for (const auto& vertice : vertices) {
        archivo << vertice.x << " " << vertice.y << " " << vertice.z << "\n";
    }
    
    // Escribir las caras
    for (const auto& cara : caras) {
        archivo << "3 " << cara[0] << " " << cara[1] << " " << cara[2] << "\n";
    }

    archivo.close();
}

// Función para subdividir las caras de un tetraedro
void subdividir(std::vector<Vertice>& vertices, std::vector<std::vector<int>>& caras, float radio, int niveles) {
    // Para cada nivel de subdivisión
    for (int nivel = 0; nivel < niveles; ++nivel) {
        std::vector<std::vector<int>> nuevasCaras;

        // Subdividir cada cara en 4 nuevas caras
        for (const auto& cara : caras) {
            int v1 = cara[0], v2 = cara[1], v3 = cara[2];
            
            // Obtener los puntos medios
            Vertice m1 = puntoMedio(vertices[v1], vertices[v2], radio);
            Vertice m2 = puntoMedio(vertices[v2], vertices[v3], radio);
            Vertice m3 = puntoMedio(vertices[v3], vertices[v1], radio);
            
            // Agregar los nuevos vértices
            int i_m1 = vertices.size();
            vertices.push_back(m1);
            int i_m2 = vertices.size();
            vertices.push_back(m2);
            int i_m3 = vertices.size();
            vertices.push_back(m3);
            
            // Crear las nuevas 4 caras
            nuevasCaras.push_back({v1, i_m1, i_m3});
            nuevasCaras.push_back({v2, i_m2, i_m1});
            nuevasCaras.push_back({v3, i_m3, i_m2});
            nuevasCaras.push_back({i_m1, i_m2, i_m3});
        }

        caras = nuevasCaras; // Actualizar las caras con las nuevas subdivididas
    }
}

// Función para inicializar un tetraedro inscrito en una esfera
void generarTetraedro(float radio, std::vector<Vertice>& vertices, std::vector<std::vector<int>>& caras) {
    float sqrt2 = std::sqrt(2.0f);
    
    // Definir los vértices del tetraedro
    vertices = {
        {1, 1, 1},
        {-1, -1, 1},
        {-1, 1, -1},
        {1, -1, -1}
    };

    // Normalizar los vértices al radio de la esfera
    for (auto& v : vertices) {
        v.normalizar(radio);
    }

    // Definir las caras del tetraedro (como triángulos)
    caras = {
        {0, 1, 2},
        {0, 3, 1},
        {0, 2, 3},
        {1, 3, 2}
    };
}

int main() {
    float radio = 1.0f; // Radio de la esfera
    int niveles = 1; // Nivel de subdivisión
    std::vector<Vertice> vertices;
    std::vector<std::vector<int>> caras;

    generarTetraedro(radio, vertices, caras);
    subdividir(vertices, caras, radio, niveles);

    escribirPLY(vertices, caras, "esfera_subdividida.ply");

    std::cout << "Archivo esfera_subdividida.ply generado exitosamente." << std::endl;

    return 0;
}
