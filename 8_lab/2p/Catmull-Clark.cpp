
// Implementar catmull clark

#include <iostream>
#include <vector>
#include <cmath>

class Vertex {
public:
    float x, y, z;

    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

    Vertex operator+(const Vertex& other) const {
        return Vertex(x + other.x, y + other.y, z + other.z);
    }

    Vertex operator-(const Vertex& other) const {
        return Vertex(x - other.x, y - other.y, z - other.z);
    }

    Vertex operator*(float scalar) const {
        return Vertex(x * scalar, y * scalar, z * scalar);
    }

    Vertex operator/(float scalar) const {
        return Vertex(x / scalar, y / scalar, z / scalar);
    }

    float dot(const Vertex& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vertex cross(const Vertex& other) const {
        return Vertex(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vertex normalize() const {
        return *this / length();
    }
};


class Face {
public:
    std::vector<int> vertices;

    Face(int v0, int v1, int v2) : vertices({v0, v1, v2}) {}
};

class catmullClark {
public:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    catmullClark(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) : vertices(vertices), faces(faces) {}

    void subdivide() {
        std::vector<Vertex> newVertices;
        std::vector<Face> newFaces;

        // Create edge midpoints
        std::map<std::pair<int, int>, int> edgeMidpointIndex;
        for (const auto& face : faces) {
            int v0 = face.vertices[0];
            int v1 = face.vertices[1];
            int v2 = face.vertices[2];

            // Create midpoints for edges
            int a, b, c;

            auto edge1 = std::make_pair(std::min(v0, v1), std::max(v0, v1));
            if (edgeMidpointIndex.count(edge1) == 0) {
                Vertex m = midpoint(vertices[v0], vertices[v1]);
                a = newVertices.size();
                newVertices.push_back(m);
                edgeMidpointIndex[edge1] = a;
            } else {
                a = edgeMidpointIndex[edge1];
            }

            auto edge2 = std::make_pair(std::min(v1, v2), std::max(v1, v2));
            if (edgeMidpointIndex.count(edge2) == 0) {
                Vertex m = midpoint(vertices[v1], vertices[v2]);
                b = newVertices.size();
                newVertices.push_back(m);
                edgeMidpointIndex[edge2] = b;
            } else {
                b = edgeMidpointIndex[edge2];
            }

            auto edge3 = std::make_pair(std::min(v0, v2), std::max(v0, v2));
            if (edgeMidpointIndex.count(edge3) == 0) {
                Vertex m = midpoint(vertices[v0], vertices[v2]);
                c = newVertices.size();
                newVertices.push_back(m);
                edgeMidpointIndex[edge3] = c;
            } else {
                c = edgeMidpointIndex[edge3];
            }

            // Create new faces
            newFaces.push_back({v0, a, c});
            newFaces.push_back({v1, b, a});
            newFaces.push_back({v2, c, b});
            newFaces.push_back({v0, b, a});
        }
    }


