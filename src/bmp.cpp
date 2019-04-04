#include <iostream>
#include <stdio.h>
#include <cmath>
#include <bitset>
#include "bmp.h"
using namespace std;

#define BMP_HEADER_SIZE 14
#define COLOR_ENDPOINTS_SIZE 36

/*
 * Title: BMPImage constructor
 * Parameters (1): filename (std::string)
 * Functionality: Opens supplied filename and saves file session.
 */
BMPImage::BMPImage(const string& filename) {
    file = fopen(filename.c_str(), "rb");
    if (file == nullptr) throw "BMPImage Error: Failed to open file.";
}

/*
 * Title: BMPImage method
 * Parameters (0): N/A
 * Functionality:
 */
void BMPImage::loadBMPImage(void) {

    // read 14-byte BMP header
    size_t bytesRead = 0;
    uint8_t headerBuf[BMP_HEADER_SIZE];
    do { bytesRead += fread(headerBuf + bytesRead, sizeof(uint8_t), 
        BMP_HEADER_SIZE - bytesRead, file); }
    while (bytesRead < BMP_HEADER_SIZE);
    header.signature = *((int16_t*) headerBuf);
    header.fileSize = *((uint32_t*) &headerBuf[2]);
    header.dataOffset = *((uint32_t*) &headerBuf[10]);

    // read info header size field
    bytesRead = 0;
    uint8_t infoHeaderSizeFieldBuf[sizeof(uint32_t)];
    do { bytesRead += fread(infoHeaderSizeFieldBuf + bytesRead, sizeof(uint8_t), 
        sizeof(uint32_t) - bytesRead, file); }
    while (bytesRead < sizeof(uint32_t));
    infoHeader.infoHeaderSize = *((uint32_t*) infoHeaderSizeFieldBuf);

    // read variable-size info header
    bytesRead = 0;
    uint8_t infoHeaderBuf[infoHeader.infoHeaderSize - sizeof(uint32_t)];
    do { bytesRead += fread(infoHeaderBuf + bytesRead, sizeof(uint8_t), 
        infoHeader.infoHeaderSize - sizeof(uint32_t) - bytesRead, file); }
    while (bytesRead < (infoHeader.infoHeaderSize - sizeof(uint32_t)));
    infoHeader.width = *((uint32_t*) &infoHeaderBuf);
    infoHeader.height = *((uint32_t*) &infoHeaderBuf[4]);
    infoHeader.planeCount = *((uint16_t*) &infoHeaderBuf[8]);
    infoHeader.bitsPerPixel = *((uint16_t*) &infoHeaderBuf[10]);
    infoHeader.compression = *((uint32_t*) &infoHeaderBuf[12]);
    infoHeader.imageSize = *((uint32_t*) &infoHeaderBuf[16]);
    infoHeader.horizontalResolution = *((uint32_t*) &infoHeaderBuf[20]);
    infoHeader.verticalResolution = *((uint32_t*) &infoHeaderBuf[24]);
    infoHeader.colorCount = *((uint32_t*) &infoHeaderBuf[28]);
    infoHeader.importantColorCount = *((uint32_t*) &infoHeaderBuf[32]);

    // read pixel image data
    const size_t bytesPerPixel = infoHeader.bitsPerPixel / 8;
    if (fseek(file, header.dataOffset, SEEK_SET)) throw "BMPImage Error: Failed to seek pixel data."; 
    pixelArray.reset(new BMPPixelArray(infoHeader.width * infoHeader.height));
    const size_t rowSize = ceil(((double) (infoHeader.bitsPerPixel * infoHeader.width)) / 32.0) * 4;
    for (size_t i = 0; i < infoHeader.height; i++) {
        bytesRead = 0;
        uint8_t pixelBuf[rowSize];
        do { bytesRead += fread(pixelBuf + bytesRead, sizeof(uint8_t), rowSize, file); }
        while (bytesRead < rowSize);
        for (size_t j = 0; j < infoHeader.width; j++) {
            get<2>((*pixelArray)[(i * infoHeader.width) + j]) = pixelBuf[j * bytesPerPixel];
            get<1>((*pixelArray)[(i * infoHeader.width) + j]) = pixelBuf[(j * bytesPerPixel) + 1];
            get<0>((*pixelArray)[(i * infoHeader.width) + j]) = pixelBuf[(j * bytesPerPixel) + 2];
        }
    }

    cout << ftell(file) << endl;
}
 
/*
 * Title: BMPImage destructor
 * Parameters (0): N/A
 * Functionality: Closes file and frees dynamic memory.
 */
BMPImage::~BMPImage(void) {
    fclose(file);
    pixelArray.reset(nullptr);
}