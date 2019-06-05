#include "hash/dcthash.h"
using namespace std;

/*
 * Calculates and analyzes error between two perceptual token hashes.
 */
bool TokenPerceptualHash::compareHashes(TokenPerceptualHash& hs1, TokenPerceptualHash& hs2,
    const uint32_t errorDegree, const bool verbose, const uint32_t normalizationSize) { 
    return false; 
}

/*
 * Computes and sets the specific token perceptual hash.
 */
void TokenPerceptualHash::executeHash(void) {
    if (computedFlag) throw "TokenPerceptualHash Error: Hash already computed.";
    if (regionHashFlag) executeTokenRegionHash();
    else executeTokenHash();
    computedFlag = true;
}

/*
 * Prints bit image of single-data perceptual token hash.
 */
void TokenPerceptualHash::printHashBits(void) const {}

/*
 * Computes normal token hash throughout entire grid
 */
void TokenPerceptualHash::executeTokenHash(void) {}

/*
 * Computes token hash based on specific region in grid
 */
void TokenPerceptualHash::executeTokenRegionHash(void) {}