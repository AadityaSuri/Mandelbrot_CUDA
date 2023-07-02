#include <stdio.h>
#include "utils/bitmap.h"
#include <math.h>
#include <stdlib.h>

int colorMap[16][3] = {    
    {60, 30, 15},
    {25, 7, 26},
    {9, 1, 47},
    {4, 4, 73},
    {0, 7, 100},
    {12, 44, 138},
    {24, 82, 177},
    {57, 125, 209},
    {134, 181, 229},
    {211, 236, 248},
    {241, 233, 191},
    {248, 201, 95},
    {255, 170, 0},
    {204, 128, 0},
    {153, 87, 0},
    {106, 52, 3}
};

void drawMandelbrot(unsigned char *image, int height, int width, double centerX, double centerY, double zoomFactor, char *imageFileName) {
    double padding = 0.025;

    // calculate the x and y spans
    double xSpan = 1.0 / zoomFactor;
    double ySpan = 1.0 / zoomFactor;

    // calculate min and max x and y based on the center coordinates and zoom factor
    double x_min = centerX - (xSpan / 2.0) - padding;
    double x_max = centerX + (xSpan / 2.0) + padding;
    double y_min = centerY - (ySpan / 2.0) - padding;
    double y_max = centerY + (ySpan / 2.0) + padding;

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            double x = 0;
            double y = 0;

            int iteration = 0;
            int max_iteration = 1000;

            double x_scaled = x_min + j * (x_max - x_min) / (width - 1.0);
            double y_scaled = y_min + i * (y_max - y_min) / (height - 1.0);

            while (x*x + y*y <= 4 && iteration < max_iteration) {
                double xtemp = x*x - y*y + x_scaled;
                y = 2*x*y + y_scaled;
                x = xtemp;
                iteration++;
            }

            int r = 0;
            int g = 0;
            int b = 0;

            if (iteration < max_iteration) {
                int i = iteration % 16;

                r = colorMap[i][0];
                g = colorMap[i][1];
                b = colorMap[i][2];
            }

            writePixel((unsigned char*) image, height, width, i, j, r, g, b);
        }
    }

    generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    printf("Image generated!!");
}

int main() {
    int height = 1200;
    int width = 1500;
    unsigned char image[height][width][BYTES_PER_PIXEL];

    char* imageFileName = (char*) "rand.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.7, -0.375, 3.0, imageFileName);

    char* seahorseFileName = (char*) "seahorseValley.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.747, 0.1, 1/0.005, seahorseFileName);

    // Elephant Valley
    char* elephantFileName = (char*) "elephantValley.bmp";
    drawMandelbrot((unsigned char*) image, height, width, 0.275, 0.0, 1/0.01, elephantFileName);

    // Triple Spiral Valley
    char* tripleSpiralFileName = (char*) "tripleSpiralValley.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.088, 0.654, 1/0.005, tripleSpiralFileName);

    // Mini Mandelbrot
    char* miniMandelbrotFileName = (char*) "miniMandelbrot.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -1.768, 0.001, 1/0.001, miniMandelbrotFileName);

    // full Mandelbrot
    char* fullMandelbrotFileName = (char*) "fullMandelbrot.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.75, 0.0, 0.35, fullMandelbrotFileName);


    return 0;
}

