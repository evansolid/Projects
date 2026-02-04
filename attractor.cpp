#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

const int WIDTH  = 800;
const int HEIGHT = 600;

const double SCALE = 2.0;

const int ITERATIONS = 1000000;

// clifford attractor formula (four parameters)
void clifford(double &x, double &y, double a, double b, double c, double d) {
    double newX = sin(a*y)+c*cos(a*x);
    double newY = sin(b*x)+d*cos(b*y);
    x = newX;
    y = newY;
}

// plot a point into a box in the histogram
void plotPoint(std::vector<int>& histogram, double x, double y, int width, int height, double scale) {
    double ratio = static_cast<double>(width) / height;
    int px = (int)((x + scale*ratio) / (2*scale)*height);
    int py = (int)((y + scale) / (2*scale)*height);
    if (px >= 0 && px < width && py >= 0 && py < height) {
        histogram[py * width + px]++;
    }
}

// turn the histogram into an image, using ppm format because its easy
void savePPM(std::vector<int>& histogram) {
    std::ofstream out("attractor.ppm");
    out << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
    int maxValue = *std::max_element(histogram.begin(), histogram.end());
    for (int i = 0; i < WIDTH * HEIGHT; i++){
        int value = (int)(255 * (log(1 + histogram[i]) / log(1 + maxValue)));
        out << value << " " << value << " " << value << "\n";
    }
    out.close();
}

int main() {
    // creating the histogram
    std::vector<int> histogram(WIDTH * HEIGHT, 0);

    // attractor parameters
    double a = -1.4;
    double b = 1.6;
    double c = 1.0;
    double d = 0.7;

    // starting point
    double x = 0.1;
    double y = 0.1;

    std::cout << "Starting to generate attractor\n";

    // generate attractor
    for (int i = 0; i < ITERATIONS; i++) {
        clifford(x,y,a,b,c,d);
        plotPoint(histogram, x, y, WIDTH, HEIGHT, SCALE);
    }
    
    std::cout << "Done, saving PPM file\n";

    // generate and save ppm
    savePPM(histogram);

    std::cout << "Done!\n";

    return 0;
}