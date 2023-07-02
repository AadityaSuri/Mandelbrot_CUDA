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


int main ()
{
    int height = 1200;
    int width = 1500;
    unsigned char image[height][width][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "bitmapImage.bmp";

    // drawMandelbrotSet((unsigned char*) image, height, width, 0, 0);

    double padding = 0.025; // specify your padding value
    double x_min = -0.8 - padding; // min value for x
    double x_max = -0.6 + padding;    // max value for x
    double y_min = -0.25 - padding;   // min value for y
    double y_max = -0.5 + padding;    // max value for y

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            double x = 0;
            double y = 0;

            int iteration = 0;
            int max_iteration = 1000;

            // scale x and y values based on the min and max values
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

            if ((x_scaled == 0 && y_scaled != 0) || (x_scaled != 0 && y_scaled == 0) || (x_scaled == 0 && y_scaled == 0)) {
                r = 255;
                g = 255;
                b = 255;
            }

            writePixel((unsigned char*) image, height, width, i, j, r, g, b);
        }
                
    }

    generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    printf("Image generated!!");
}