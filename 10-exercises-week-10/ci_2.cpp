#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <functional>

std::vector<std::vector<float>> field;

void initializeField(std::function<float(float, float)> f, int cols, int rows, float xmin, float ymin, float precision) {
    field.resize(cols, std::vector<float>(rows, 0.0f));

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            float x = xmin + i * precision;
            float y = ymin + j * precision;
            field[i][j] = f(x, y);  
        }
    }
}

int getState(float a, float b, float c, float d) {
    return (a > 0) * 8 + (b > 0) * 4 + (c > 0) * 2 + (d > 0) * 1;
}

void drawLine(std::ofstream &file, float x1, float y1, float x2, float y2) {
    file << x1 << " " << y1 << " moveto\n";
    file << x2 << " " << y2 << " lineto\n";
    file << "stroke\n";
}

void marchSquares(std::ofstream &file, int cols, int rows, float xmin, float ymin, float precision) {
    for (int i = 0; i < cols - 1; i++) {
        for (int j = 0; j < rows - 1; j++) {
            float x = xmin + i * precision;
            float y = ymin + j * precision;

            float a = field[i][j];
            float b = field[i + 1][j];
            float c = field[i + 1][j + 1];
            float d = field[i][j + 1];

            int state = getState(a, b, c, d);

            float half = precision / 2.0;
            switch (state) {
                case 1:
                    drawLine(file, x, y + half, x + half, y + precision);
                    break;
                case 2:
                    drawLine(file, x + half, y + precision, x + precision, y + half);
                    break;
                case 3:
                    drawLine(file, x, y + half, x + precision, y + half);
                    break;
                case 4:
                    drawLine(file, x + half, y, x + precision, y + half);
                    break;
                case 5:
                    drawLine(file, x + half, y, x, y + half);
                    drawLine(file, x + half, y + precision, x + precision, y + half);
                    break;
                case 6:
                    drawLine(file, x + half, y, x + half, y + precision);
                    break;
                case 7:
                    drawLine(file, x, y + half, x + half, y);
                    break;
                case 8:
                    drawLine(file, x, y + half, x + half, y);
                    break;
                case 9:
                    drawLine(file, x + half, y, x + half, y + precision);
                    break;
                case 10:
                    drawLine(file, x + half, y, x + precision, y + half);
                    drawLine(file, x, y + half, x + half, y + precision);
                    break;
                case 11:
                    drawLine(file, x + half, y, x + precision, y + half);
                    break;
                case 12:
                    drawLine(file, x, y + half, x + precision, y + half);
                    break;
                case 13:
                    drawLine(file, x + half, y + precision, x + precision, y + half);
                    break;
                case 14:
                    drawLine(file, x, y + half, x + half, y + precision);
                    break;
            }
        }
    }
}

void draw_curve(std::function<float(float, float)> f, const std::string &output_filename,
                float xmin, float ymin, float xmax, float ymax, float precision) {
                    
    if ((xmax - xmin) < precision && (ymax - ymin) < precision) {
        return;
    }

    int cols = static_cast<int>((xmax - xmin) / precision);
    int rows = static_cast<int>((ymax - ymin) / precision);

    initializeField(f, cols, rows, xmin, ymin, precision);

    std::ofstream file(output_filename);
    file << "%!PS-Adobe-3.0 EPSF-3.0\n";
    file << "%%BoundingBox: 0 0 " << (xmax - xmin) << " " << (ymax - ymin) << "\n";
    file << "0 setlinewidth\n";

    marchSquares(file, cols, rows, xmin, ymin, precision);

    file << "showpage\n";
    file.close();

    std::cout << "Archivo EPS '" << output_filename << "' creado con éxito.\n";
}

int main() {
    float xmin = 0.0, ymin = 0.0, xmax = 800.0, ymax = 800.0, RADIUS = 200.0;
    float precision = 5.0;

    // Función implícita de un círculo
    auto circleFunction = [xmax, ymax, RADIUS](float x, float y) {
        return std::pow(x - (xmax / 2), 2) + std::pow(y - (ymax / 2), 2) - std::pow(RADIUS, 2);
    };

    draw_curve(circleFunction, "ci_2.eps", xmin, ymin, xmax, ymax, precision);

    return 0;
}