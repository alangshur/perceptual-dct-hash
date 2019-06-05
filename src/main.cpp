#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include "hash/phash.h"
#include "pimg/pimage.h"
using namespace std;

int main(int args, char* argv[]) {
    if (args != 3) return 0;
    string filename1 = argv[1];
    string filename2 = argv[2];
    try {
        clock_t start = clock();

        PureImage pimg1(filename1, true); 
        // PureImage pimg2(filename2, true); 

        // // compare hashes
        // ImagePerceptualHash::compareHashes(pimg1.getPHash(), pimg2.getPHash());

        // double duration = (clock() - start) / double(CLOCKS_PER_SEC);
        // cout << endl << "Processing time: " << to_string(duration) << endl << flush;
    }
    catch (const char* e) { cout << e << endl; }
    return 0;
}