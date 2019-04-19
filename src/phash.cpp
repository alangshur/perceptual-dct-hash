#include <cstdio>
#include <cmath>
#include <iostream>
#include <bitset>
#include <string.h>
#include "phash.h"
using namespace std;

// initialize static member variables
IPHSErrorWeights ImagePerceptualHash::errorWeights = {
    0.1, 0.1, 0.1, 0.3, 0.2, 0.1, 0.1
};

/*
 * Title: ImagePerceptualHash constructor
 * Parameters (3): grid hash target (PixelGrid), error weights (IPHSErrorWeights),
 *   normalization dimensions (uint32_t)
 * Functionality: Initializes error weights and dynamic grid memory.
 */
ImagePerceptualHash::ImagePerceptualHash(const PixelGrid& grid, 
    const uint32_t normalizationSize) : 
    PerceptualHash(grid), normalizationDimension(normalizationSize), 
    hashColorLength(pow(normalizationDimension, 2) / HASH_SEGMENT_SIZE) {

    // allocate dynamic memory
    result.redData.reset(new vector<uint64_t>(hashColorLength));
    result.greenData.reset(new vector<uint64_t>(hashColorLength));
    result.blueData.reset(new vector<uint64_t>(hashColorLength));
    result.luminanceData.reset(new vector<uint64_t>(hashColorLength));
    result.grayscaleData.reset(new vector<uint64_t>(hashColorLength));
    result.combinedData1.reset(new vector<uint64_t>(hashColorLength));
    result.combinedData2.reset(new vector<uint64_t>(hashColorLength));
}

/*
 * Title: ImagePerceptualHash public method
 * Parameters (5): first hash (ImagePerceptualHash), second hash (ImagePerceptualHash), 
 *   error tolerance (uint32_t), verbose setting (const bool), normalization dimension (uint32_t)
 * Functionality: Calculates and analyzes error between two perceptual image hashes.
 */
IPHSErrorDiagnosis ImagePerceptualHash::compareHashes(ImagePerceptualHash& hs1, ImagePerceptualHash& hs2,
    const uint32_t errorDegree, const bool verbose, const uint32_t normalizationDimension) {
    uint8_t hashColorLength = pow(normalizationDimension, 2) / HASH_SEGMENT_SIZE;
    uint32_t redError = 0, greenError = 0, blueError = 0, luminanceError = 0, 
        grayscaleError = 0, combinedError1 = 0, combinedError2 = 0;

    // count bit errors
    for (uint8_t i = 0; i < hashColorLength; i++) {
        for (uint8_t j = 0; j < HASH_SEGMENT_SIZE; j++) {

            // get red error
            bool set1 = ((*(hs1.getHash().redData))[i] & (0x1 << j)) != 0;
            bool set2 = ((*(hs2.getHash().redData))[i] & (0x1 << j)) != 0;
            if (set1 != set2) redError++;

            // get green error
            set1 = ((*(hs1.getHash().greenData))[i] & (0x1 << j)) != 0;
            set2 = ((*(hs2.getHash().greenData))[i] & (0x1 << j)) != 0;
            if (set1 != set2) greenError++;

            // get blue error
            set1 = ((*(hs1.getHash().blueData))[i] & (0x1 << j)) != 0;
            set2 = ((*(hs2.getHash().blueData))[i] & (0x1 << j)) != 0;
            if (set1 != set2) blueError++;

            // get luminance error
            set1 = ((*(hs1.getHash().luminanceData))[i] & (0x1 << j)) != 0;
            set2 = ((*(hs2.getHash().luminanceData))[i] & (0x1 << j)) != 0;
            if (set1 != set2) luminanceError++;

            // get grayscale error
            set1 = ((*(hs1.getHash().grayscaleData))[i] & (0x1 << j)) != 0;
            set2 = ((*(hs2.getHash().grayscaleData))[i] & (0x1 << j)) != 0;
            if (set1 != set2) grayscaleError++;

            // get combined 1 error
            set1 = ((*(hs1.getHash().combinedData1))[i] & (0x1 << j)) != 0;
            set2 = ((*(hs2.getHash().combinedData1))[i] & (0x1 << j)) != 0;
            if (set1 != set2) combinedError1++;

            // get combined 2 error
            set1 = ((*(hs1.getHash().combinedData2))[i] & (0x1 << j)) != 0;
            set2 = ((*(hs2.getHash().combinedData2))[i] & (0x1 << j)) != 0;
            if (set1 != set2) combinedError2++;
        }
    }

    // compute error percentages
    IPHSErrorDiagnosis errorDiagnosis;
    errorDiagnosis.redErrorRat = ((double) redError) / ((double) HASH_SEGMENT_SIZE * hashColorLength);
    errorDiagnosis.greenErrorRat = ((double) greenError) / ((double) HASH_SEGMENT_SIZE * hashColorLength);
    errorDiagnosis.blueErrorRat = ((double) blueError) / ((double) HASH_SEGMENT_SIZE * hashColorLength);
    errorDiagnosis.luminanceErrorRat = ((double) luminanceError) / ((double) HASH_SEGMENT_SIZE * hashColorLength);
    errorDiagnosis.grayscaleErrorRat = ((double) grayscaleError) / ((double) HASH_SEGMENT_SIZE * hashColorLength);
    errorDiagnosis.combined1ErrorRat = ((double) combinedError1) / ((double) HASH_SEGMENT_SIZE * hashColorLength);
    errorDiagnosis.combined2ErrorRat = ((double) combinedError2) / ((double) HASH_SEGMENT_SIZE * hashColorLength);

    // calculate final error with weights
    errorDiagnosis.resultantError = (errorDiagnosis.redErrorRat * errorWeights.redErrorWeight) + 
        (errorDiagnosis.greenErrorRat * errorWeights.greenErrorWeight) + 
        (errorDiagnosis.blueErrorRat * errorWeights.blueErrorWeight) + 
        (errorDiagnosis.luminanceErrorRat * errorWeights.luminanceErrorWeight) + 
        (errorDiagnosis.grayscaleErrorRat * errorWeights.grayscaleErrorWeight) + 
        (errorDiagnosis.combined1ErrorRat * errorWeights.combined1ErrorWeight) +
        (errorDiagnosis.combined2ErrorRat * errorWeights.combined2ErrorWeight);

    // print verbose output
    if (verbose) {  
        cout << "Red Error: " << to_string(errorDiagnosis.redErrorRat) << endl;
        cout << "Green Error: " << to_string(errorDiagnosis.greenErrorRat) << endl;
        cout << "Blue Error: " << to_string(errorDiagnosis.blueErrorRat) << endl;
        cout << "Luminance Error: " << to_string(errorDiagnosis.luminanceErrorRat) << endl;
        cout << "Grayscale Error: " << to_string(errorDiagnosis.grayscaleErrorRat) << endl;
        cout << "Combined Error 1: " << to_string(errorDiagnosis.combined1ErrorRat) << endl;
        cout << "Combined Error 2: " << to_string(errorDiagnosis.combined2ErrorRat) << endl;
        cout << "Resultant Error: " << to_string(errorDiagnosis.resultantError) << endl << flush;
    }
    
    return errorDiagnosis;
}

/*
 * Title: ImagePerceptualHash public method
 * Parameters (0): N/A
 * Functionality: Computes and sets the general image perceptual hash.
 */
void ImagePerceptualHash::executeHash(void) {
    if (computedFlag) throw "ImagePerceptualHash Error: Hash already computed.";

    // zero hash memory
    memset(&((*(result.redData)).front()), 0, sizeof(uint64_t) * (*(result.redData)).size());
    memset(&((*(result.greenData)).front()), 0, sizeof(uint64_t) * (*(result.greenData)).size());
    memset(&((*(result.blueData)).front()), 0, sizeof(uint64_t) * (*(result.blueData)).size());
    memset(&((*(result.luminanceData)).front()), 0, sizeof(uint64_t) * (*(result.luminanceData)).size());
    memset(&((*(result.grayscaleData)).front()), 0, sizeof(uint64_t) * (*(result.grayscaleData)).size());
    memset(&((*(result.combinedData1)).front()), 0, sizeof(uint64_t) * (*(result.combinedData1)).size());
    memset(&((*(result.combinedData2)).front()), 0, sizeof(uint64_t) * (*(result.combinedData2)).size());

    // iteratively normalize grid RGB 
    PixelGrid normalizedGrid({normalizationDimension, normalizationDimension});
    GridPixel meanRGBValues = normalizeGridRGB(grid, normalizedGrid);

    // compute RGB hash values
    computeRGBHash(normalizedGrid, meanRGBValues);
    computedFlag = true;
}

/*
 * Title: ImagePerceptualHash public method
 * Parameters (0): N/A
 * Functionality: Prints bit image of RGB perceptual image hash.
 */
void ImagePerceptualHash::printHashBits(void) const {
    if (!computedFlag) throw "ImagePerceptualHash Error: Hash not yet computed.";

    // print red hash bits
    cout << endl << "Red Hash:" << endl << flush;
    for (uint8_t i = 0; i < hashColorLength; i++) {
        bitset<64> bucket((*(result.redData))[i]);
        cout << bucket << flush;
    }

    // print green hash bits
    cout << endl << endl << "Green Hash:" << endl << flush;
    for (uint8_t i = 0; i < hashColorLength; i++) {
        bitset<64> bucket((*(result.greenData))[i]);
        cout << bucket << flush;
    }

    // print blue hash bits
    cout << endl << endl << "Blue Hash:" << endl << flush;
    for (uint8_t i = 0; i < hashColorLength; i++) {
        bitset<64> bucket((*(result.blueData))[i]);
        cout << bucket << flush;
    }

    // print luminance hash bits
    cout << endl << endl << "Luminance Hash:" << endl << flush;
    for (uint8_t i = 0; i < hashColorLength; i++) {
        bitset<64> bucket((*(result.luminanceData))[i]);
        cout << bucket << flush;
    }

    // print grayscale hash bits
    cout << endl << endl << "Grayscale Hash:" << endl << flush;
    for (uint8_t i = 0; i < hashColorLength; i++) {
        bitset<64> bucket((*(result.grayscaleData))[i]);
        cout << bucket << flush;
    }

    // print combined hash 1 bits
    cout << endl << endl << "Combined Hash 1:" << endl << flush;
    for (uint8_t i = 0; i < hashColorLength; i++) {
        bitset<64> bucket((*(result.combinedData1))[i]);
        cout << bucket << flush;
    }

    // print combined hash 2 bits
    cout << endl << endl << "Combined Hash 2:" << endl << flush;
    for (uint8_t i = 0; i < hashColorLength; i++) {
        bitset<64> bucket((*(result.combinedData2))[i]);
        cout << bucket << flush;
    }

    cout << endl << flush;
}

/*
 * Title: ImagePerceptualHash private method
 * Parameters (0): N/A
 * Functionality: Reduces supplied image into target grid by normalizing RGB pixel clusters.
 */
GridPixel ImagePerceptualHash::normalizeGridRGB(const PixelGrid& pixelGrid, 
    PixelGrid& normalizedGrid) const {
    uint32_t horizontalScaleSize, verticalScaleSize, horizontalOverflow = 0, verticalOverflow = 0;
    size_t runningRedSum = 0, runningGreenSum = 0, runningBlueSum = 0;

    // initialize horizontal grid parsing parameters
    if (!(pixelGrid.getGridWidth() % normalizationDimension)) {
        horizontalScaleSize = pixelGrid.getGridWidth() / normalizationDimension;
        horizontalOverflow = normalizationDimension;
    } 
    else {
        horizontalScaleSize = pixelGrid.getGridWidth() / (normalizationDimension - 1);
        horizontalOverflow = pixelGrid.getGridWidth() % (normalizationDimension - 1);
    }

    // initialize vertical grid parsing parameters
    if (!(pixelGrid.getGridHeight() % normalizationDimension)) {
        verticalScaleSize = pixelGrid.getGridHeight() / normalizationDimension;
        verticalOverflow = normalizationDimension;
    }
    else {
        verticalScaleSize = pixelGrid.getGridHeight() / (normalizationDimension - 1);
        verticalOverflow = pixelGrid.getGridHeight() % (normalizationDimension - 1);
    }

    // build reduced grid
    for (uint32_t row = 0; row < (normalizationDimension - 1); row++) {
        for (uint32_t col = 0; col < (normalizationDimension - 1); col++) {

            // take RGB pixel sums for block
            size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
            for (uint32_t i = 1; i <= verticalScaleSize; i++) {
                for (uint32_t j = 1; j <= horizontalScaleSize; j++) {
                    GridPixel& pixel = pixelGrid.getPixel({(row * verticalScaleSize) + i, 
                        (col * horizontalScaleSize) + j});
                    blockSumRed += pixel.red;
                    blockSumGreen += pixel.green;
                    blockSumBlue += pixel.blue;
                }
            }

            // update running sums
            runningRedSum += blockSumRed;
            runningGreenSum += blockSumGreen;
            runningBlueSum += blockSumBlue;

            // calculate block mean
            uint32_t blockDivisor = verticalScaleSize * horizontalScaleSize;
            normalizedGrid.setPixel({row + 1, col + 1}, {uint8_t(blockSumRed / blockDivisor), 
                uint8_t(blockSumGreen / blockDivisor), uint8_t(blockSumBlue / blockDivisor)});
        }
    } 

    // build overflow column
    const uint32_t overflowColumn = horizontalScaleSize * (normalizationDimension - 1);
    for (uint32_t row = 0; row < (normalizationDimension - 1); row++) {

        // take RGB pixel sums for overflow block
        size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
        for (uint32_t i = 1; i <= verticalScaleSize; i++) {
            for (uint32_t j = 1; j <= horizontalOverflow; j++) {
                GridPixel& pixel = pixelGrid.getPixel({(row * verticalScaleSize) + i, 
                    overflowColumn + j});
                blockSumRed += pixel.red;
                blockSumGreen += pixel.green;
                blockSumBlue += pixel.blue;
            }
        }

        // update running sums
        runningRedSum += blockSumRed;
        runningGreenSum += blockSumGreen;
        runningBlueSum += blockSumBlue;

        // calculate block mean
        uint32_t overflowDivisor = verticalScaleSize * horizontalOverflow;
        normalizedGrid.setPixel({row + 1, normalizationDimension}, {uint8_t(blockSumRed / overflowDivisor), 
            uint8_t(blockSumGreen / overflowDivisor), uint8_t(blockSumBlue / overflowDivisor)});
    }

    // build overflow row
    const uint32_t overflowRow = verticalScaleSize * (normalizationDimension - 1);
    for (uint32_t col = 0; col < (normalizationDimension - 1); col++) {

        // take RGB pixel sums for overflow block
        size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
        for (uint32_t i = 1; i <= verticalOverflow; i++) {
            for (uint32_t j = 1; j <= horizontalScaleSize; j++) {
                GridPixel& pixel = pixelGrid.getPixel({overflowRow + i, 
                    (col * horizontalScaleSize) + j});
                blockSumRed += pixel.red;
                blockSumGreen += pixel.green;
                blockSumBlue += pixel.blue;
            }
        }

        // update running sums
        runningRedSum += blockSumRed;
        runningGreenSum += blockSumGreen;
        runningBlueSum += blockSumBlue;

        // calculate block mean
        uint32_t overflowDivisor = verticalOverflow * horizontalScaleSize;
        normalizedGrid.setPixel({normalizationDimension, col + 1}, {uint8_t(blockSumRed / overflowDivisor), 
            uint8_t(blockSumGreen / overflowDivisor), uint8_t(blockSumBlue / overflowDivisor)});
    }

    // build overflow row/col
    size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
    for (uint32_t i = 1; i <= verticalOverflow; i++) {
        for (uint32_t j = 1; j <= horizontalOverflow; j++) {
            GridPixel& pixel = pixelGrid.getPixel({overflowRow + i, overflowColumn + j});
            blockSumRed += pixel.red;
            blockSumGreen += pixel.green;
            blockSumBlue += pixel.blue;
        }
    }

    // wrap-up overflow normalization
    runningRedSum += blockSumRed;
    runningGreenSum += blockSumGreen;
    runningBlueSum += blockSumBlue;
    uint32_t overflowDivisor = verticalOverflow * horizontalOverflow;
    normalizedGrid.setPixel({normalizationDimension, normalizationDimension}, {uint8_t(blockSumRed / overflowDivisor), 
        uint8_t(blockSumGreen / overflowDivisor), uint8_t(blockSumBlue / overflowDivisor)});
    uint32_t imageDivisor = pixelGrid.getGridHeight() * pixelGrid.getGridWidth();
    return {uint8_t(runningRedSum / imageDivisor), uint8_t(runningGreenSum / imageDivisor), 
        uint8_t(runningBlueSum / imageDivisor)};
}

/*
 * Title: ImagePerceptualHash private method
 * Parameters (0): N/A
 * Functionality: Breaks down normalized image into hash using mean RGB key.
 */
void ImagePerceptualHash::computeRGBHash(const PixelGrid& normalizedGrid, const GridPixel& mean) {
    const uint32_t luminance = 0.2126 * uint32_t(mean.red) + 0.7152 * uint32_t(mean.green) + 
        0.0722 * uint32_t(mean.blue);
    const uint8_t grayscaleMean = uint8_t((uint32_t(mean.red) + uint32_t(mean.green) + 
        uint32_t(mean.blue)) / 3);
    
    // iterate through normalized grid
    for (uint32_t i = 1; i <= normalizationDimension; i++) {
        for (uint32_t j = 1; j <= normalizationDimension; j++) {
            uint32_t position = (((i - 1) * normalizationDimension) + (j - 1));
            uint32_t bucket = position / 64;
            uint32_t iterator = position % 64;

            // compute RGB hash
            const GridPixel& pixel = normalizedGrid.getPixel({i, j});
            if (pixel.red >= mean.red) (*(result.redData))[bucket] |= 0x1 << iterator;
            if (pixel.green >= mean.green) (*(result.greenData))[bucket] |= 0x1 << iterator;
            if (pixel.blue >= mean.blue) (*(result.blueData))[bucket] |= 0x1 << iterator;

            // compute luminance hash
            const uint32_t pixelLuminance = 0.2126 * uint32_t(pixel.red) + 0.7152 * uint32_t(pixel.green) + 
                0.0722 * uint32_t(pixel.blue);
            if (pixelLuminance >= luminance) (*(result.luminanceData))[bucket] |= 0x1 << iterator;

            // compute grayscale hash
            const uint8_t pixelMean = uint8_t((uint32_t(pixel.red) + uint32_t(pixel.green) + 
                uint32_t(pixel.blue)) / 3);
            if (pixelMean >= grayscaleMean) (*(result.grayscaleData))[bucket] |= 0x1 << iterator;

            // compute combined hash 1
            uint8_t majorityBool = uint8_t(pixel.red >= mean.red) + 
                uint8_t(pixel.green >= mean.green) + uint8_t(pixel.blue >= mean.blue);
            if ((majorityBool == 0) || (majorityBool == 2)) 
                (*(result.combinedData1))[bucket] |= 0x1 << iterator;

            // compute combined hash 2
            if ((majorityBool == 1) || (majorityBool == 3)) 
                (*(result.combinedData2))[bucket] |= 0x1 << iterator;
        }
    }    
} 

/*
 * Title: ImagePerceptualHash destructor
 * Parameters (0): N/A
 * Functionality: Frees dynamic memory.
 */
ImagePerceptualHash::~ImagePerceptualHash(void) {
    result.redData.reset(nullptr);
    result.greenData.reset(nullptr);
    result.blueData.reset(nullptr);
    result.luminanceData.reset(nullptr);
    result.grayscaleData.reset(nullptr);
    result.combinedData1.reset(nullptr);
    result.combinedData2.reset(nullptr);
}

/*
 * Title: TokenPerceptualHash public method
 * Parameters (5): first hash (TokenPerceptualHash), second hash (TokenPerceptualHash), 
 *   error tolerance (uint32_t), verbose setting (bool) normalization dimension (uint32_t)
 * Functionality: Calculates and analyzes error between two perceptual token hashes.
 */
bool TokenPerceptualHash::compareHashes(TokenPerceptualHash& hs1, TokenPerceptualHash& hs2,
    const uint32_t errorDegree, const bool verbose, const uint32_t normalizationSize) { 
    return false; 
}

/*
 * Title: TokenPerceptualHash public method
 * Parameters (0): N/A
 * Functionality: Computes and sets the specific token perceptual hash.
 */
void TokenPerceptualHash::executeHash(void) {
    if (computedFlag) throw "TokenPerceptualHash Error: Hash already computed.";
    if (regionHashFlag) executeTokenRegionHash();
    else executeTokenHash();
    computedFlag = true;
}

/*
 * Title: TokenPerceptualHash public method
 * Parameters (0): N/A
 * Functionality: Prints bit image of single-data perceptual token hash.
 */
void TokenPerceptualHash::printHashBits(void) const {}

/*
 * Title: TokenPerceptualHash private method
 * Parameters (0): N/A
 * Functionality: Computes normal token hash throughout entire grid
 */
void TokenPerceptualHash::executeTokenHash(void) {}

/*
 * Title: TokenPerceptualHash private method
 * Parameters (0): N/A
 * Functionality: Computes token hash based on specific region in grid
 */
void TokenPerceptualHash::executeTokenRegionHash(void) {}
