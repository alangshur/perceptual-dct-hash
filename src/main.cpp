#include <iostream>
#include <string>
#include <cstdio>
#include "bmp.h"
#include "phash.h"
using namespace std;

int main(int args, char* argv[]) {
    if (args != 2) return 0;
    string filename = argv[1];
    try {
        BMPImage image(filename); 
        image.loadBMPImage();
        ImagePerceptualHash hash(image.getBMPPixelGrid());
        hash.executeHash();
    }
    catch (const char* e) { cout << e << endl; }
    return 0;
}