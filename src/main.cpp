#include <iostream>
#include <string>
#include <bitset>
#include <cstdio>
#include "bmp.h"
using namespace std;

int main(int args, char* argv[]) {
    if (args != 2) return 0;
    string filename = argv[1];

    try {
        BMPImage image(filename); 
        image.loadBMPImage();
        BMPPixelArray& a = image.getBMPPixelArray();
        for (size_t i = 0; i < (image.getBMPImageHeight() * image.getBMPImageWidth()); i++) {
            cout << "(" << to_string(get<0>(a[i])) << ", " << to_string(get<1>(a[i])) 
                << ", " << to_string(get<2>(a[i])) << ")" << endl;
        } 
    }
    catch (const char* e) { cout << e << endl; }
    return 0;
}