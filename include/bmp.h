#ifndef BMP_H
#define BMP_H

#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <stdlib.h>
#include "bmp.h"
using namespace std;

// BMP header struct type definition
typedef struct {
    int16_t signature; 
    uint32_t fileSize;
    uint32_t dataOffset;
} BMPHeader;

// BMP info header struct type definition
typedef struct {
    uint32_t infoHeaderSize; 
    uint32_t width;
    uint32_t height;
    uint16_t planeCount;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t colorCount;
    uint32_t importantColorCount;
} BMPInfoHeader;

// BMP pixel array type definition
typedef vector<tuple<uint8_t, uint8_t, uint8_t>> BMPPixelArray;

class BMPImage {
    public:
        BMPImage(const string& filename);
        ~BMPImage(void);
        void loadBMPImage(void); 
        BMPHeader& getBMPHeader(void) { return header; }
        BMPInfoHeader& getBMPInfoHeader(void) { return infoHeader; }
        BMPPixelArray& getBMPPixelArray(void) { return *pixelArray; }
        BMPImage(const BMPImage&) = delete;
        BMPImage& operator=(const BMPImage&) = delete;

    private:

        // image file object
        FILE* file;

        // image parameters
        BMPHeader header;
        BMPInfoHeader infoHeader;

        // image pixel data container
        unique_ptr<BMPPixelArray> pixelArray;
};

#endif