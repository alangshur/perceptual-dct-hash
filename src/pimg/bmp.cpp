#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <random>
#include <regex>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "pimg/bmp.h"
using namespace std;

#define BMP_HEADER_SIZE 14
#define COLOR_ENDPOINTS_SIZE 36
#define BITMAP_INFO_HEADER_SIZE 40

static const char alphaNumLib[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'Y', 'X', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

/*
 * Opens supplied filename and saves file session.
 */
BMPImage::BMPImage(const string& filename, const bool expediteLoad) : loadedFlag(false), 
    expediteLoad(expediteLoad), file(nullptr), imageGrid(nullptr) {

    // validate filename
    string validFilename = filename;
    const size_t pos = filename.find_last_of('/');
    if (pos != string::npos) validFilename = filename.substr(pos + 1);
    regex r("^.*[.](png|jpeg|jpg|bmp|tiff)$");
    if (!regex_match(validFilename, r)) 
        throw "BMPImage Error: Invalid image or filename format.";

    // generate random identifier string
    string identifier;
    minstd_rand0 rv(chrono::system_clock::now().time_since_epoch().count());
    for (uint8_t i = 0; i < 16; i++) identifier += alphaNumLib[rv() % 30];

    // convert to bmp
    cv::Mat imageBMP;
    const string basename = validFilename.substr(0, validFilename.find('.'));
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (image.data == NULL) throw "BMPImage Error: Failed to convert file.";
    image.convertTo(imageBMP, CV_8UC3);
    cv::imwrite(basename + "-" + identifier + ".bmp", imageBMP);
    bmpFileName = basename + "-" + identifier + ".bmp";

    // open file
    file = fopen(bmpFileName.c_str(), "rb");
    if (file == nullptr) throw "BMPImage Error: Failed to open file.";
}

/*
 * Parses BMP file and individually loads header, info header, and pixel data.
 */
void BMPImage::loadBMPImage(void) {
    if (loadedFlag) throw "BMPImage Error: Image already loaded.";
    if (fseek(file, 0, SEEK_SET)) throw "BMPImage Error: Failed to seek pixel data.";

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
    if (infoHeader.infoHeaderSize != BITMAP_INFO_HEADER_SIZE)
        throw "BMPImage Error: Invalid header data.";

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
    if (expediteLoad) goto EXPEDITE_READ_DATA;
    infoHeader.compression = *((uint32_t*) &infoHeaderBuf[12]);
    infoHeader.imageSize = *((uint32_t*) &infoHeaderBuf[16]);
    infoHeader.horizontalResolution = *((uint32_t*) &infoHeaderBuf[20]);
    infoHeader.verticalResolution = *((uint32_t*) &infoHeaderBuf[24]);
    infoHeader.colorCount = *((uint32_t*) &infoHeaderBuf[28]);
    infoHeader.importantColorCount = *((uint32_t*) &infoHeaderBuf[32]);

    // read pixel image data
    EXPEDITE_READ_DATA: const size_t bytesPerPixel = infoHeader.bitsPerPixel / 8;
    if (fseek(file, header.dataOffset, SEEK_SET)) throw "BMPImage Error: Failed to seek pixel data."; 
    imageGrid.reset(new PixelGrid({infoHeader.height, infoHeader.width}));
    const size_t rowSize = ceil(((double) (infoHeader.bitsPerPixel * infoHeader.width)) / 32.0) * 4;
    for (ssize_t i = (infoHeader.height - 1); i >= 0; i--) {
        bytesRead = 0;
        uint8_t pixelBuf[rowSize];
        do { bytesRead += fread(pixelBuf + bytesRead, sizeof(uint8_t), rowSize, file); }
        while (bytesRead < rowSize);
        for (size_t j = 0; j < infoHeader.width; j++) {
            (*imageGrid).setPixel({(uint32_t) (i + 1), (uint32_t) (j + 1)}, {pixelBuf[(j * bytesPerPixel) + 2], 
                pixelBuf[(j * bytesPerPixel) + 1], pixelBuf[j * bytesPerPixel]});
        }
    }

    // set image to loaded
    loadedFlag = true;
}

/*
 * Closes file and frees dynamic memory.
 */
BMPImage::~BMPImage(void) {
    fclose(file);
    if (remove(bmpFileName.c_str())) cerr << "BMPImage Error: Failed to clean myself." << endl; 
    imageGrid.reset(nullptr);
}