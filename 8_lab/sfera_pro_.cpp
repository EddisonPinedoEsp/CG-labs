#include<iostream>
#include<vector>

using namespace std;
typedef vector<double> Point;
typedef vector<int> Face;


struct mesh_OFF {
    int nEdges;
    
    mesh_OFF();

    vector<Point> points;
    vector<Face> faces;
    int add_point(Point const& point);
    int add_face(Face const& face);
};

mesh_OFF::mesh_OFF() : nEdges(0) {}

int mesh_OFF::add_point(Point const& point) {
    points.push_back(point);
    return points.size() - 1;
}

int mesh_OFF::add_face(Face const& face) {
    faces.push_back(face);
    nEdges += face.size();
    return faces.size() - 1;
}

ostream& operator<<(ostream& oss, Point const& point) {
    for (size_t i = 0; i < point.size(); ++i) {
        if (i > 0) oss << " ";
        oss << point[i];
    }
    return oss;
}

ostream& operator<<(ostream& oss, Face const& face) {
    oss << face.size();
    for (size_t i = 0; i < face.size(); ++i) {
        oss << " " << face[i];
    }
    return oss;
}

ostream& operator<<(ostream& oss, mesh_OFF const& mesh) {
    oss << "OFF\n";
    oss << mesh.points.size() << " " << mesh.faces.size() << " " << mesh.nEdges << "\n";
    for (size_t i = 0; i < mesh.points.size(); ++i) {
        oss << mesh.points[i] << "\n";
    }
    for (size_t i = 0; i < mesh.faces.size(); ++i) {
        oss << mesh.faces[i] << "\n";
    }
    return oss;
}

int main() {
    mesh_OFF mesh;
    mesh.add_point({-1, -1, -1});
    mesh.add_point({1, -1, -1});
    mesh.add_point({1, 1, -1});
    mesh.add_point({-1, 1, -1});
    mesh.add_point({-1, -1, 1});
    mesh.add_point({1, -1, 1});
    mesh.add_point({1, 1, 1});
    mesh.add_point({-1, 1, 1});
    mesh.add_face({0, 3, 2, 1});
    mesh.add_face({7, 4, 5, 6});
    mesh.add_face({0, 1, 5, 4});
    mesh.add_face({1, 2, 6, 5});
    mesh.add_face({2, 3, 7, 6});
    mesh.add_face({3, 0, 4, 7});

    cout << mesh;

    return 0;
}

