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

// BMP header struct definition
typedef struct {
    int16_t signature; 
    uint32_t fileSize;
    uint32_t dataOffset;
} BMPHeader;

// BMP info header color masks struct definition
typedef struct {
    uint32_t redChannelMask;
    uint32_t greenChannelMask;
    uint32_t blueChannelMask;
    uint32_t alphaChannelMask;
} BMPColorMasks;

// BMP info header color parameters struct definition
typedef struct {
    uint32_t colorSpaceType;
    uint32_t redChannelGamma;
    uint32_t greenChannelGamma;
    uint32_t blueChannelGamma;
    uint32_t renderIntent;
    uint32_t ICCProfileData;
    uint32_t ICCProfilesize;
} BMPColorParams;

// BMP info header struct definition
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
    BMPColorMasks colorMasks;
    BMPColorParams colorParams;
} BMPInfoHeader;

// BMP pixel array type definition
typedef vector<tuple<uint8_t, uint8_t, uint8_t, uint8_t>> BMPPixelArray;

class BMPImage {
    public:
        BMPImage(const string& filename);
        ~BMPImage(void);
        BMPHeader& getBMPHeader(void) { return header; }
        BMPInfoHeader& getBMPInfoHeader(void) { return infoHeader; }
        BMPPixelArray& getBMPPixelArray(void) { return *pixelArray; }

    private:

        // additional BMP type definitions
        typedef vector<uint8_t> BMPColorSpaceEndpoints;
        typedef vector<uint32_t> BMPColorTable;

        // image parameters
        BMPHeader header;
        BMPInfoHeader infoHeader;

        // image data containers
        unique_ptr<BMPColorSpaceEndpoints> colorSpaceEndpoints;
        unique_ptr<BMPColorTable> colorTable;
        unique_ptr<BMPPixelArray> pixelArray;
};

#endif