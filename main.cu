#include <stdio.h>
#include "utils/bitmap.h"
#include <math.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <chrono>

extern "C" {
    void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName);
    void writePixel(unsigned char* image, int height, int width, int i, int j, int r, int g, int b);
}

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

__constant__ int d_colorMap[16][3];

__global__ void mandelbrotKernel(unsigned char* image, int height, int width, 
                                 double x_min, double x_max, double y_min, double y_max, 
                                 int max_iteration) {
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (i < height && j < width) {
        double x = 0;
        double y = 0;
        int iteration = 0;
        double x_scaled = x_min + j * (x_max - x_min) / (width - 1.0);
        double y_scaled = y_min + i * (y_max - y_min) / (height - 1.0);
        
        while (x*x + y*y <= 4 && iteration < max_iteration) {
            double xtemp = x*x - y*y + x_scaled;
            y = 2*x*y + y_scaled;
            x = xtemp;
            iteration++;
        }
        
        int r = 0, g = 0, b = 0;
        if (iteration < max_iteration) {
            int colorIndex = iteration % 16;
            r = d_colorMap[colorIndex][0];
            g = d_colorMap[colorIndex][1];
            b = d_colorMap[colorIndex][2];
        }
        
        int index = (i * width + j) * BYTES_PER_PIXEL;
        image[index + 2] = (unsigned char) r;  // Red
        image[index + 1] = (unsigned char) g;  // Green
        image[index] = (unsigned char) b;      // Blue
    }
}

void drawMandelbrot_cuda(unsigned char *image, int height, int width, double centerX, double centerY, 
                         double zoomFactor, char *imageFileName) {
    double padding = 0.025;
    double xSpan = 1.0 / zoomFactor;
    double ySpan = 1.0 / zoomFactor;
    double x_min = centerX - (xSpan / 2.0) - padding;
    double x_max = centerX + (xSpan / 2.0) + padding;
    double y_min = centerY - (ySpan / 2.0) - padding;
    double y_max = centerY + (ySpan / 2.0) + padding;
    int max_iteration = 1000;

    // Allocate device memory
    unsigned char *d_image;
    cudaMalloc(&d_image, height * width * BYTES_PER_PIXEL * sizeof(unsigned char));

    // Copy colorMap to device constant memory
    cudaMemcpyToSymbol(d_colorMap, colorMap, sizeof(int) * 16 * 3);

    // Set up grid and block dimensions
    dim3 blockSize(16, 16);
    dim3 gridSize((width + blockSize.x - 1) / blockSize.x, 
                  (height + blockSize.y - 1) / blockSize.y);

    // Launch kernel
    mandelbrotKernel<<<gridSize, blockSize>>>(d_image, height, width, x_min, x_max, y_min, y_max, max_iteration);

    // Copy result back to host
    cudaMemcpy(image, d_image, height * width * BYTES_PER_PIXEL * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_image);

    // Generate bitmap image
    generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    // printf("Image generated using CUDA: %s\n", imageFileName);

    // Check for any CUDA errors
    cudaError_t error = cudaGetLastError();
    if (error != cudaSuccess) {
        printf("CUDA error: %s\n", cudaGetErrorString(error));
    }
}


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
    // printf("Image generated!!");
}

int main() {
    int height = 1200;
    int width = 1500;
    unsigned char image[height][width][BYTES_PER_PIXEL];

    // **********************************************************************
    // ************************ LINEAR MANDELBROT **************************
    // **********************************************************************

    auto start_cpu = std::chrono::high_resolution_clock::now();

    char* imageFileName = (char*) "output/linear/rand.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.7, -0.375, 3.0, imageFileName);

    char* seahorseFileName = (char*) "output/linear/seahorseValley.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.747, 0.1, 1/0.005, seahorseFileName);

    // Elephant Valley
    char* elephantFileName = (char*) "output/linear/elephantValley.bmp";
    drawMandelbrot((unsigned char*) image, height, width, 0.275, 0.0, 1/0.01, elephantFileName);

    // Triple Spiral Valley
    char* tripleSpiralFileName = (char*) "output/linear/tripleSpiralValley.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.088, 0.654, 1/0.005, tripleSpiralFileName);

    // Mini Mandelbrot
    char* miniMandelbrotFileName = (char*) "output/linear/miniMandelbrot.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -1.768, 0.001, 1/0.001, miniMandelbrotFileName);

    // full Mandelbrot
    char* fullMandelbrotFileName = (char*) "output/linear/fullMandelbrot.bmp";
    drawMandelbrot((unsigned char*) image, height, width, -0.75, 0.0, 0.35, fullMandelbrotFileName);

    auto end_cpu = std::chrono::high_resolution_clock::now();
    auto duration_cpu = std::chrono::duration_cast<std::chrono::milliseconds>(end_cpu - start_cpu).count();
    printf("CPU time: %d ms\n", duration_cpu);

    // **********************************************************************
    // ************************ CUDA MANDELBROT *****************************
    // **********************************************************************

    auto start_cuda = std::chrono::high_resolution_clock::now();

    char* imageFileName_cuda = (char*) "output/cuda/rand.bmp";
    drawMandelbrot_cuda((unsigned char*) image, height, width, -0.7, -0.375, 3.0, imageFileName_cuda);

    char* seahorseFileName_cuda = (char*) "output/cuda/seahorseValley.bmp";
    drawMandelbrot_cuda((unsigned char*) image, height, width, -0.747, 0.1, 1/0.005, seahorseFileName_cuda);

    // Elephant Valley
    char* elephantFileName_cuda = (char*) "output/cuda/elephantValley.bmp";
    drawMandelbrot_cuda((unsigned char*) image, height, width, 0.275, 0.0, 1/0.01, elephantFileName_cuda);

    // Triple Spiral Valley
    char* tripleSpiralFileName_cuda = (char*) "output/cuda/tripleSpiralValley.bmp";
    drawMandelbrot_cuda((unsigned char*) image, height, width, -0.088, 0.654, 1/0.005, tripleSpiralFileName_cuda);

    // Mini Mandelbrot
    char* miniMandelbrotFileName_cuda = (char*) "output/cuda/miniMandelbrot.bmp";
    drawMandelbrot_cuda((unsigned char*) image, height, width, -1.768, 0.001, 1/0.001, miniMandelbrotFileName_cuda);

    // full Mandelbrot
    char* fullMandelbrotFileName_cuda = (char*) "output/cuda/fullMandelbrot.bmp";
    drawMandelbrot_cuda((unsigned char*) image, height, width, -0.75, 0.0, 0.35, fullMandelbrotFileName_cuda);

    auto end_cuda = std::chrono::high_resolution_clock::now();
    auto duration_cuda = std::chrono::duration_cast<std::chrono::milliseconds>(end_cuda - start_cuda).count();
    printf("CUDA time: %d ms\n", duration_cuda);

    return 0;
}

