#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include "bmp.h"
#include "phash.h"
using namespace std;

int main(int args, char* argv[]) {
    if (args != 2) return 0;
    string filename = argv[1];
    try {
        clock_t start = clock();
        BMPImage image(filename); 
        image.loadBMPImage();
        ImagePerceptualHash hash(image.getBMPPixelGrid());
        hash.executeHash();
        double duration = (clock() - start) / double(CLOCKS_PER_SEC);
        cout << "Processing time: " << to_string(duration) << endl << flush;
    }
    catch (const char* e) { cout << e << endl; }
    return 0;
}