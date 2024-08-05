#define BYTES_PER_PIXEL 3 /// red, green, & blue
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

#ifdef __cplusplus
extern "C" {
#endif

void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);
void writePixel(unsigned char* image, int height, int width, int i, int j, int r, int g, int b);

#ifdef __cplusplus
}
#endif