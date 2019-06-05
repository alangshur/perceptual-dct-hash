#ifndef PIMAGE_H
#define PIMAGE_H

#include <string>
#include <memory>
#include "pimg/bmp.h"
#include "pimg/grid.h"
#include "hash/phash.h"
using namespace std;

class PureImage {
    public:
        PureImage(const string& filename, bool verbose = false);
        ~PureImage();
        PixelGrid& getPixelGrid() { return image->getBMPPixelGrid(); }
        ImagePerceptualHash& getPHash() { return imagePHash; }

    private:
        const string& filename; 
        unique_ptr<BMPImage> image; 
        ImagePerceptualHash imagePHash; 
};

#endif