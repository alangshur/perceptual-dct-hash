#include <iostream>
#include <string>
#include <bitset>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include "bmp.h"
using namespace std;

int main(int args, char* argv[]) {
    if (args != 2) return 0;
    string filename = argv[1];

    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    cv::Mat image_bmp;
    image.convertTo(image_bmp, CV_8UC3);
    remove("result.bmp");
    cv::imwrite("result.bmp", image_bmp);
    filename = "result.bmp";

    try {
        BMPImage image(filename); 
        image.loadBMPImage();
        BMPPixelArray& a = image.getBMPPixelArray();
        BMPInfoHeader& h = image.getBMPInfoHeader();
        for (size_t i = 0; i < (h.height * h.width); i++) {
            cout << "(" << to_string(get<0>(a[i])) << ", " << to_string(get<1>(a[i])) 
                << ", " << to_string(get<2>(a[i])) << ")" << endl;
        } 
    }
    catch (const char* e) { cout << e << endl; }
    return 0;
}