#ifndef BMP_H
#define BMP_H

#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <stdlib.h>
#include "pimg/bmp.h"
#include "pimg/grid.h"
using namespace std;

typedef struct {
    int16_t signature; 
    uint32_t fileSize;
    uint32_t dataOffset;
} BMPHeader;

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

class BMPImage {
    public:
        BMPImage(const string& filename, const bool expediteLoad = true);
        ~BMPImage(void);
        void loadBMPImage(void); 
        size_t getBMPImageSize(void) const { return header.fileSize; }
        size_t getBMPImageWidth(void) const { return infoHeader.width; }
        size_t getBMPImageHeight(void) const { return infoHeader.height; }
        BMPHeader getBMPHeader(void) const { return header; }
        BMPInfoHeader getBMPInfoHeader(void) const { return infoHeader; }
        PixelGrid& getBMPPixelGrid(void) { return *imageGrid; }
        void printBMPPixelGrid(void) const { (*imageGrid).printPixelGrid(); }

    private:

        // state conditions
        bool loadedFlag;
        bool expediteLoad;

        // image file object
        FILE* file;
        string bmpFileName;

        // image parameters
        BMPHeader header;
        BMPInfoHeader infoHeader;

        // image pixel grid
        unique_ptr<PixelGrid> imageGrid;
};

#endif