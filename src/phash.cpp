#include <cstdio>
#include <cmath>
#include <iostream>
#include "phash.h"
using namespace std;

#define NORMALIZATION_DIMENSION 32

/*
 * Title: ImagePerceptualHash public method
 * Parameters (0): N/A
 * Functionality: Computes and sets the general image perceptual hash.
 */
void ImagePerceptualHash::executeHash(void) {
    if (computedFlag) throw "ImagePerceptualHash Error: Hash already computed.";

    // iteratively normalize grid RGB 
    PixelGrid normalizedGrid({NORMALIZATION_DIMENSION, NORMALIZATION_DIMENSION});
    normalizeGridRGB(grid, normalizedGrid);
    
    computedFlag = true;
}

/*
 * Title: ImagePerceptualHash private method
 * Parameters (0): N/A
 * Functionality: Reduces supplied image into target grid by normalizing RGB pixel clusters.
 */
void ImagePerceptualHash::normalizeGridRGB(const PixelGrid& pixelGrid, PixelGrid& normalizedGrid) const {
    uint32_t horizontalScaleSize, verticalScaleSize, horizontalOverflow = 0, verticalOverflow = 0;

    // initialize horizontal grid parsing parameters
    if (!(pixelGrid.getGridWidth() % NORMALIZATION_DIMENSION)) {
        horizontalScaleSize = pixelGrid.getGridWidth() / NORMALIZATION_DIMENSION;
        horizontalOverflow = NORMALIZATION_DIMENSION;
    } 
    else {
        horizontalScaleSize = pixelGrid.getGridWidth() / (NORMALIZATION_DIMENSION - 1);
        horizontalOverflow = pixelGrid.getGridWidth() % (NORMALIZATION_DIMENSION - 1);
    }

    // initialize vertical grid parsing parameters
    if (!(pixelGrid.getGridHeight() % NORMALIZATION_DIMENSION)) {
        verticalScaleSize = pixelGrid.getGridHeight() / NORMALIZATION_DIMENSION;
        verticalOverflow = NORMALIZATION_DIMENSION;
    }
    else {
        verticalScaleSize = pixelGrid.getGridHeight() / (NORMALIZATION_DIMENSION - 1);
        verticalOverflow = pixelGrid.getGridHeight() % (NORMALIZATION_DIMENSION - 1);
    }

    // build reduced grid
    for (uint32_t row = 0; row < (NORMALIZATION_DIMENSION - 1); row++) {
        for (uint32_t col = 0; col < (NORMALIZATION_DIMENSION - 1); col++) {

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

            // calculate block mean
            uint32_t overflowDivisor = verticalScaleSize * horizontalScaleSize;
            uint8_t blockRedMean = blockSumRed / overflowDivisor;
            uint8_t blockGreenMean = blockSumGreen / overflowDivisor;
            uint8_t blockBlueMean = blockSumBlue / overflowDivisor;
            normalizedGrid.setPixel({row + 1, col + 1}, {blockRedMean, blockGreenMean, 
                blockBlueMean});
        }
    } 

    // build overflow column
    const uint32_t overflowColumn = horizontalScaleSize * (NORMALIZATION_DIMENSION - 1);
    for (uint32_t row = 0; row < (NORMALIZATION_DIMENSION - 1); row++) {

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

        // calculate block mean
        uint32_t overflowDivisor = verticalScaleSize * horizontalOverflow;
        uint8_t blockRedMean = blockSumRed / overflowDivisor;
        uint8_t blockGreenMean = blockSumGreen / overflowDivisor;
        uint8_t blockBlueMean = blockSumBlue / overflowDivisor;
        normalizedGrid.setPixel({row + 1, NORMALIZATION_DIMENSION}, {blockRedMean, 
            blockGreenMean, blockBlueMean});
    }

    // build overflow row
    const uint32_t overflowRow = verticalScaleSize * (NORMALIZATION_DIMENSION - 1);
    for (uint32_t col = 0; col < (NORMALIZATION_DIMENSION - 1); col++) {

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

        // calculate block mean
        uint32_t overflowDivisor = verticalOverflow * horizontalScaleSize;
        uint8_t blockRedMean = blockSumRed / overflowDivisor;
        uint8_t blockGreenMean = blockSumGreen / overflowDivisor;
        uint8_t blockBlueMean = blockSumBlue / overflowDivisor;
        normalizedGrid.setPixel({NORMALIZATION_DIMENSION, col + 1}, {blockRedMean, 
            blockGreenMean, blockBlueMean});
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
    uint32_t overflowDivisor = verticalOverflow * horizontalOverflow;
    uint8_t blockRedMean = blockSumRed / overflowDivisor;
    uint8_t blockGreenMean = blockSumGreen / overflowDivisor;
    uint8_t blockBlueMean = blockSumBlue / overflowDivisor;
    normalizedGrid.setPixel({NORMALIZATION_DIMENSION, NORMALIZATION_DIMENSION}, {blockRedMean, 
        blockGreenMean, blockBlueMean});
}

/*
 * Title: TokenPerceptualHash public method
 * Parameters (0): N/A
 * Functionality: Computes and sets the specific token perceptual hash.
 */
void TokenPerceptualHash::executeHash(void) {
    if (computedFlag) throw "TokenPerceptualHash Error: Hash already computed.";
    if (regionHash) executeTokenRegionHash();
    else executeTokenHash();
    computedFlag = true;
}

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
