#include <iostream>
#include <string>
#include <bitset>
#include "bmp.h"
using namespace std;

int main(int args, char* argv[]) {
    if (args != 2) return 0;
    const string filename = argv[1];
    try {
        BMPImage image(filename); 
        BMPPixelArray& a = image.getBMPPixelArray();
        BMPInfoHeader& h = image.getBMPInfoHeader();
        for (size_t i = 0; i < (h.height * h.width); i++) {
            cout << "(" << to_string(get<0>(a[i])) << ", " << to_string(get<1>(a[i])) 
                << ", " << to_string(get<2>(a[i])) << ", " << to_string(get<3>(a[i])) 
                << ")" << endl;
        } 
    }
    catch (const char* e) { cout << e << endl; }
    return 0;
}