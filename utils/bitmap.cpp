#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

constexpr int BYTES_PER_PIXEL = 3;
constexpr int FILE_HEADER_SIZE = 14;
constexpr int INFO_HEADER_SIZE = 40;

void generateBitmapImage(const std::vector<std::vector<std::vector<uint8_t>>>& image, int height, int width, const std::string& imageFileName);
std::vector<uint8_t> createBitmapFileHeader(int height, int stride);
std::vector<uint8_t> createBitmapInfoHeader(int height, int width);

int main()
{
    int height = 361;
    int width = 867;
    std::vector<std::vector<std::vector<uint8_t>>> image(height, std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(BYTES_PER_PIXEL)));
    std::string imageFileName = "bitmapImage.bmp";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j][2] = static_cast<uint8_t>(i * 255 / (height / 4));             // red
            image[i][j][1] = static_cast<uint8_t>(j * 255 / (width / 5));              // green
            image[i][j][0] = static_cast<uint8_t>((i + j) * 255 / ((height + width) / 10)); // blue
        }
    }

    generateBitmapImage(image, height, width, imageFileName);
    std::cout << "Image generated!!" << std::endl;

    return 0;
}

void generateBitmapImage(const std::vector<std::vector<std::vector<uint8_t>>>& image, int height, int width, const std::string& imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    std::vector<uint8_t> padding(3, 0);
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    std::ofstream imageFile(imageFileName, std::ios::binary);

    std::vector<uint8_t> fileHeader = createBitmapFileHeader(height, stride);
    imageFile.write(reinterpret_cast<const char*>(fileHeader.data()), FILE_HEADER_SIZE);

    std::vector<uint8_t> infoHeader = createBitmapInfoHeader(height, width);
    imageFile.write(reinterpret_cast<const char*>(infoHeader.data()), INFO_HEADER_SIZE);

    for (int i = 0; i < height; i++) {
        imageFile.write(reinterpret_cast<const char*>(image[i].data()), BYTES_PER_PIXEL * width);
        imageFile.write(reinterpret_cast<const char*>(padding.data()), paddingSize);
    }

    imageFile.close();
}

std::vector<uint8_t> createBitmapFileHeader(int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    std::vector<uint8_t> fileHeader(FILE_HEADER_SIZE);
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    fileHeader[2] = static_cast<uint8_t>(fileSize);
    fileHeader[3] = static_cast<uint8_t>(fileSize >> 8);
    fileHeader[4] = static_cast<uint8_t>(fileSize >> 16);
    fileHeader[5] = static_cast<uint8_t>(fileSize >> 24);
    fileHeader[10] = static_cast<uint8_t>(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

std::vector<uint8_t> createBitmapInfoHeader(int height, int width)
{
    std::vector<uint8_t> infoHeader(INFO_HEADER_SIZE);
    infoHeader[0] = static_cast<uint8_t>(INFO_HEADER_SIZE);
    infoHeader[4] = static_cast<uint8_t>(width);
    infoHeader[5] = static_cast<uint8_t>(width >> 8);
    infoHeader[6] = static_cast<uint8_t>(width >> 16);
    infoHeader[7] = static_cast<uint8_t>(width >> 24);
    infoHeader[8] = static_cast<uint8_t>(height);
    infoHeader[9] = static_cast<uint8_t>(height >> 8);
    infoHeader[10] = static_cast<uint8_t>(height >> 16);
    infoHeader[11] = static_cast<uint8_t>(height >> 24);
    infoHeader[12] = static_cast<uint8_t>(1);
    infoHeader[14] = static_cast<uint8_t>(BYTES_PER_PIXEL * 8);

    return infoHeader;
}
