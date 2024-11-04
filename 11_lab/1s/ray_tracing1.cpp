#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <limits>

using namespace std;
using namespace cv;

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

// Proyección en 2D
void projectTo2D(Mesh& mesh, float scale, float offsetX, float offsetY) {
    for (auto& vertex : mesh.vertices) {
        vertex.x2D = scale * vertex.x + offsetX;
        vertex.y2D = scale * vertex.y + offsetY;
    }
}

// Dibujar la malla proyectada
void drawMesh(const Mesh& mesh, const Size& imageSize) {
    Mat image = Mat::zeros(imageSize, CV_8UC3);

    // Encontrar la profundidad mínima y máxima
    float minDepth = numeric_limits<float>::max();
    float maxDepth = numeric_limits<float>::min();
    for (const auto& vertex : mesh.vertices) {
        if (vertex.z < minDepth) minDepth = vertex.z;
        if (vertex.z > maxDepth) maxDepth = vertex.z;
    }

    // Dibujar cada cara de la malla
    for (const auto& face : mesh.faces) {
        const Vertex& v1 = mesh.vertices[face.v1];
        const Vertex& v2 = mesh.vertices[face.v2];
        const Vertex& v3 = mesh.vertices[face.v3];

        Point p1(v1.x2D, v1.y2D);
        Point p2(v2.x2D, v2.y2D);
        Point p3(v3.x2D, v3.y2D);

        // Calcular el color basado en la profundidad (z)
        float depth = (v1.z + v2.z + v3.z) / 3.0f;
        int colorValue = static_cast<int>(255 * (1 - (depth - minDepth) / (maxDepth - minDepth))); // Escalar a [0, 255]
        Scalar color(colorValue, colorValue, colorValue);

        // Dibujar el triángulo
        vector<Point> points = {p1, p2, p3};
        fillConvexPoly(image, points, color);
    }

    imshow("2D Projection of Mesh", image);
    waitKey(0);
}

int main() {
    Mesh mesh;
    if (!loadOFF("Tetraedro.off", mesh)) {
        return 1;
    }

    float scale = 100.0f;
    float offsetX = 400.0f;
    float offsetY = 400.0f;
    projectTo2D(mesh, scale, offsetX, offsetY);

    Size imageSize(800, 800);
    drawMesh(mesh, imageSize);

    return 0;
}