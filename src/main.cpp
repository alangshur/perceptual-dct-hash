#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include "bmp.h"
#include "phash.h"
using namespace std;

int main(int args, char* argv[]) {
    if (args != 3) return 0;
    string filename1 = argv[1];
    string filename2 = argv[2];
    try {
        clock_t start = clock();

        // get first image 
        BMPImage image1(filename1); 
        image1.loadBMPImage();

        // get second image
        BMPImage image2(filename2); 
        image2.loadBMPImage();
        
        // get first hash
        ImagePerceptualHash hs1(image1.getBMPPixelGrid());
        hs1.executeHash();

        // get second hash
        ImagePerceptualHash hs2(image2.getBMPPixelGrid());
        hs2.executeHash();

        // compare hashes
        ImagePerceptualHash::compareHashes(hs1, hs2, 0);

        double duration = (clock() - start) / double(CLOCKS_PER_SEC);
        cout << endl <<  "Processing time: " << to_string(duration) << endl << flush;
    }
    catch (const char* e) { cout << e << endl; }
    return 0;
}