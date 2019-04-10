#include "phash.h"
using namespace std;

/*
 * Title: ImagePerceptualHash method
 * Parameters (0): N/A
 * Functionality: Computes and sets the general image perceptual hash.
 */
void ImagePerceptualHash::executeHash(void) {
    if (computedFlag) throw "ImagePerceptualHash Error: Hash already computed.";

    // TODO: Implement method

    computedFlag = true;
}

/*
 * Title: TokenPerceptualHash method
 * Parameters (0): N/A
 * Functionality: Computes and sets the specific token perceptual hash.
 */
void TokenPerceptualHash::executeHash(void) {
    if (computedFlag) throw "TokenPerceptualHash Error: Hash already computed.";

    // TODO: Implement method

    computedFlag = true;
}
