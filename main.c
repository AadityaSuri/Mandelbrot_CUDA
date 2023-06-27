#include <stdio.h>
#include "utils/bitmap.h"

int main ()
{
    int height = 1550;
    int width = 1800;
    unsigned char image[height][width][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "bitmapImage.bmp";

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            double x = 0;
            double y = 0;

            int iteration = 0;
            int max_iteration = 1000;
            
            while (x*x + y*y <= 4 && iteration < max_iteration) {
                double xtemp = x*x - y*y + (j - 2.0*height/3.0) / (height/3.0);
                y = 2*x*y + (i - height/2.0) / (height/2.0);
                x = xtemp;
                iteration++;
            }

            int r = 0; 
            int g = 0;
            int b = 0;

            if (iteration < max_iteration) {
                int i = iteration % 16;
                switch (i)
                {
                case 0:
                    r = 60;
                    g = 30;
                    b = 15;
                    break;

                case 1:
                    r = 25;
                    g = 7;
                    b = 26;
                    break;
                
                case 2:
                    r = 9;
                    g = 1;
                    b = 47;
                    break;

                case 3:
                    r = 4;
                    g = 4;
                    b = 73;
                    break;
                
                case 4:
                    r = 0;
                    g = 7;
                    b = 100;
                    break;

                case 5:
                    r = 12;
                    g = 44;
                    b = 138;
                    break;
                
                case 6:
                    r = 24;
                    g = 82;
                    b = 177;
                    break;

                case 7:
                    r = 57;
                    g = 125;
                    b = 209;
                    break;

                case 8:
                    r = 134;
                    g = 181;
                    b = 229;
                    break;

                case 9:
                    r = 211;
                    g = 236;
                    b = 248;
                    break;
                
                case 10:
                    r = 241;
                    g = 233;
                    b = 191;
                    break;
                
                case 11:
                    r = 248;
                    g = 201;
                    b = 95;
                    break;

                case 12:
                    r = 255;
                    g = 170;
                    b = 0;
                    break; 
                
                case 13:
                    r = 204;
                    g = 128;
                    b = 0;
                    break;
                
                case 14:
                    r = 153;
                    g = 87;
                    b = 0;
                    break;
                
                case 15:
                    r = 106;
                    g = 52;
                    b = 3;
                    break;  
                
                default:
                    break;
                }
                

            }

            writePixel((unsigned char*) image, height, width, i, j, r, g, b);
        }
    }

    

    generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    printf("Image generated!!");
}