#include <stdio.h>

const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);
void writePixel(unsigned char* image, int height, int width, int i, int j, int r, int g, int b);


int main ()
{
    int height = 500;
    int width = 500;
    unsigned char image[height][width][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "bitmapImage.bmp";

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            double x = 0;
            double y = 0;

            int iteration = 0;
            int max_iteration = 10000;
            
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

void writePixel(unsigned char* image, int height, int width, int i, int j, int r, int g, int b) {
    int offset = (i * width + j) * 3;  // Calculate the offset based on row, column, and pixel size (3 bytes per pixel)

    image[offset + 2] = (unsigned char) r;  // Red
    image[offset + 1] = (unsigned char) g;  // Green
    image[offset] = (unsigned char) b;      // Blue
}


void generateBitmapImage (unsigned char* image, int height, int width, char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;
    for (i = 0; i < height; i++) {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}