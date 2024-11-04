#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

struct Vertex {
    float x, y, z;
    float x2D, y2D;
};

struct Face {
    int v1, v2, v3;
    float depth;
};

struct Mesh {
    vector<Vertex> vertices;
    vector<Face> faces;
};

bool loadOFF(const string& filename, Mesh& mesh) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return false;
    }

    string line;
    file >> line;
    if (line != "OFF") {
        cerr << "Error: Formato incorrecto, se esperaba 'OFF'" << endl;
        return false;
    }

    int numVertices, numFaces, numEdges;
    file >> numVertices >> numFaces >> numEdges;

    mesh.vertices.resize(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        file >> mesh.vertices[i].x >> mesh.vertices[i].y >> mesh.vertices[i].z;
    }

    mesh.faces.resize(numFaces);
    for (int i = 0; i < numFaces; ++i) {
        int n, v1, v2, v3;
        file >> n >> v1 >> v2 >> v3;
        if (n != 3) {
            cerr << "Error: Solo se admiten caras triangulares" << endl;
            return false;
        }
        mesh.faces[i] = {v1, v2, v3};
    }

    file.close();
    return true;
}



int main() {
    Mesh mesh;
    if (!loadOFF("esfera.off", mesh)) {
        return 1;
    }

    return 0;
}
