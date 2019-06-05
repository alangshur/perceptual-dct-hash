#include <iostream>
#include "pimg/pimage.h"
using namespace std;

/*
 * Initialize pure image object with supplied filename.
 */
PureImage::PureImage(const string& filename, bool verbose) : 
    filename(filename) {

    // load pure image contents
    image.reset(new BMPImage(filename));
    imagePHash = ImagePerceptualHash(image->getBMPPixelGrid());
    image->loadBMPImage();
    imagePHash.executeHash();
    if (verbose) 
        cout << "Finished loading pure image." << endl << flush;
}

/*
 * Free dynamically-allocated memory.
 */
PureImage::~PureImage() {
    image.reset(nullptr);
}