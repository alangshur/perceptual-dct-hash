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
 * Parameters: filename string
 * Functionality: 
 */
BMPImage::BMPImage(const string& filename) {
    FILE* f = fopen(filename.c_str(), "rb");
    if (f == nullptr) throw "BMPImage Error: Failed to open file.";

    // read 14-byte BMP header
    size_t bytesRead = 0;
    uint8_t headerBuf[BMP_HEADER_SIZE];
    do { bytesRead += fread(headerBuf + bytesRead, sizeof(uint8_t), 
        BMP_HEADER_SIZE - bytesRead, f); }
    while (bytesRead < BMP_HEADER_SIZE);
    header.signature = *((int16_t*) headerBuf);
    header.fileSize = *((uint32_t*) &headerBuf[2]);
    header.dataOffset = *((uint32_t*) &headerBuf[10]);

    // read info header size field
    bytesRead = 0;
    uint8_t infoHeaderSizeFieldBuf[sizeof(uint32_t)];
    do { bytesRead += fread(infoHeaderSizeFieldBuf + bytesRead, sizeof(uint8_t), 
        sizeof(uint32_t) - bytesRead, f); }
    while (bytesRead < sizeof(uint32_t));
    infoHeader.infoHeaderSize = *((uint32_t*) infoHeaderSizeFieldBuf);

    // read variable-size info header
    bytesRead = 0;
    uint8_t infoHeaderBuf[infoHeader.infoHeaderSize - sizeof(uint32_t)];
    do { bytesRead += fread(infoHeaderBuf + bytesRead, sizeof(uint8_t), 
        infoHeader.infoHeaderSize - sizeof(uint32_t) - bytesRead, f); }
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

    // read info header color parameters
    infoHeader.colorMasks.redChannelMask = *((uint32_t*) &infoHeaderBuf[36]);
    infoHeader.colorMasks.greenChannelMask = *((uint32_t*) &infoHeaderBuf[40]);
    infoHeader.colorMasks.blueChannelMask = *((uint32_t*) &infoHeaderBuf[44]);
    infoHeader.colorMasks.alphaChannelMask = *((uint32_t*) &infoHeaderBuf[48]);
    infoHeader.colorParams.colorSpaceType = *((uint32_t*) &infoHeaderBuf[52]);
    colorSpaceEndpoints.reset(new BMPColorSpaceEndpoints(COLOR_ENDPOINTS_SIZE));
    for (size_t i = 0; i < COLOR_ENDPOINTS_SIZE; i++)
        (*colorSpaceEndpoints)[i] = infoHeaderBuf[56 + i];
    infoHeader.colorParams.redChannelGamma = *((uint32_t*) &infoHeaderBuf[92]);
    infoHeader.colorParams.greenChannelGamma = *((uint32_t*) &infoHeaderBuf[96]);
    infoHeader.colorParams.blueChannelGamma = *((uint32_t*) &infoHeaderBuf[100]);
    infoHeader.colorParams.renderIntent = *((uint32_t*) &infoHeaderBuf[104]);
    infoHeader.colorParams.ICCProfileData = *((uint32_t*) &infoHeaderBuf[108]);
    infoHeader.colorParams.ICCProfilesize = *((uint32_t*) &infoHeaderBuf[112]);

    // read color table if applicable
    if (infoHeader.colorCount > 0) { 
        bytesRead = 0;
        uint8_t colorTableBuf[sizeof(uint32_t) * infoHeader.colorCount];
        do { bytesRead += fread(infoHeaderBuf + bytesRead, sizeof(uint8_t), 
            (sizeof(uint32_t) * infoHeader.colorCount) - bytesRead, f); }
        while (bytesRead < (sizeof(uint32_t) * infoHeader.colorCount));
        colorTable.reset(new BMPColorTable(infoHeader.colorCount));
        for (size_t i = 0; i < infoHeader.colorCount; i++)
            (*colorTable)[i] = *((uint32_t*) &colorTableBuf[sizeof(uint32_t) * i]);
    }

    // get bit mask offsets
    uint32_t redOffset = 0, greenOffset = 0, blueOffset = 0, alphaOffset = 0;
    while (!((infoHeader.colorMasks.redChannelMask >> redOffset) & 0x1)) redOffset++;
    while (!((infoHeader.colorMasks.greenChannelMask >> greenOffset) & 0x1)) greenOffset++;
    while (!((infoHeader.colorMasks.blueChannelMask >> blueOffset) & 0x1)) blueOffset++;
    while (!((infoHeader.colorMasks.alphaChannelMask >> alphaOffset) & 0x1)) alphaOffset++;

    // read pixel image data
    pixelArray.reset(new BMPPixelArray(infoHeader.width * infoHeader.height));
    fseek(f, header.dataOffset, SEEK_SET);
    const size_t rowSize = ceil(((double) (infoHeader.bitsPerPixel * infoHeader.width)) / 32.0) * 4;
    for (size_t i = 0; i < infoHeader.height; i++) {
        size_t rowBytesRead = 0;
        for (size_t j = 0; j < infoHeader.width; j++) {
            bytesRead = 0;
            uint8_t pixelBuf[4];
            do { bytesRead += fread(pixelBuf + bytesRead, sizeof(uint8_t), 
                (infoHeader.bitsPerPixel / 8) - bytesRead, f); }
            while (bytesRead < (infoHeader.bitsPerPixel / 8));
            rowBytesRead += bytesRead;
            uint32_t pixelBits = *((uint32_t*) pixelBuf);            
            get<0>((*pixelArray)[(i * infoHeader.width) + j]) = (pixelBits & infoHeader.colorMasks.redChannelMask) >> redOffset;
            get<1>((*pixelArray)[(i * infoHeader.width) + j]) = (pixelBits & infoHeader.colorMasks.greenChannelMask) >> greenOffset;
            get<2>((*pixelArray)[(i * infoHeader.width) + j]) = (pixelBits & infoHeader.colorMasks.blueChannelMask) >> blueOffset;
            get<3>((*pixelArray)[(i * infoHeader.width) + j]) = (pixelBits & infoHeader.colorMasks.alphaChannelMask) >> alphaOffset;
        }
        fseek(f, rowSize - rowBytesRead, SEEK_CUR);
    }
    fclose(f);
}

/*
 * Title: BMPImage destructor
 * Parameters: N/A
 * Functionality: 
 */
BMPImage::~BMPImage(void) {
    colorSpaceEndpoints.reset(nullptr);
    colorTable.reset(nullptr);
    pixelArray.reset(nullptr);
}