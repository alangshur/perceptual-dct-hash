#include <cstdio>
#include <cmath>
#include <iostream>
#include "phash.h"
using namespace std;

#define REDUCE_SCALE_FACTOR 10
#define REDUCE_SCALE_FACTOR_SQUARED 100

/*
 * Title: ImagePerceptualHash method
 * Parameters (0): N/A
 * Functionality: Computes and sets the general image perceptual hash.
 */
void ImagePerceptualHash::executeHash(void) {
    if (computedFlag) throw "ImagePerceptualHash Error: Hash already computed.";

    // determine reduction scale
    const uint32_t horizontalReductionScale = floor(grid.getGridWidth() / REDUCE_SCALE_FACTOR);
    const uint32_t horizontalOverflow =  grid.getGridWidth() % REDUCE_SCALE_FACTOR;
    const uint32_t verticalReductionScale = floor(grid.getGridHeight() / REDUCE_SCALE_FACTOR);
    const uint32_t verticalOverflow =  grid.getGridHeight() % REDUCE_SCALE_FACTOR;

    // initialize reduction grid
    PixelGrid reducedGrid({verticalReductionScale + (verticalOverflow ? 1 : 0), 
        horizontalReductionScale + (horizontalOverflow ? 1 : 0)});

    // build reduced grid
    for (uint32_t row = 0; row < verticalReductionScale; row++) {
        for (uint32_t col = 0; col < horizontalReductionScale; col++) {

            // take RGB pixel sums for block
            size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
            for (uint32_t i = 1; i <= REDUCE_SCALE_FACTOR; i++) {
                for (uint32_t j = 1; j <= REDUCE_SCALE_FACTOR; j++) {
                    GridPixel& pixel = grid.getPixel({(row * REDUCE_SCALE_FACTOR) + i, 
                        (col * REDUCE_SCALE_FACTOR) + j});
                    blockSumRed += pixel.red;
                    blockSumGreen += pixel.green;
                    blockSumBlue += pixel.blue;
                }
            }

            // calculate block mean
            uint8_t blockRedMean = blockSumRed / REDUCE_SCALE_FACTOR_SQUARED;
            uint8_t blockGreenMean = blockSumGreen / REDUCE_SCALE_FACTOR_SQUARED;
            uint8_t blockBlueMean = blockSumBlue / REDUCE_SCALE_FACTOR_SQUARED;
            reducedGrid.setPixel({row + 1, col + 1}, {blockRedMean, blockGreenMean, 
                blockBlueMean});
        }
    } 

    // build overflow column
    if (horizontalOverflow) {
        for (uint32_t row = 0; row < verticalReductionScale; row++) {

            // take RGB pixel sums for overflow block
            size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
            for (uint32_t i = 1; i <= REDUCE_SCALE_FACTOR; i++) {
                for (uint32_t j = 1; j <= horizontalOverflow; j++) {
                    GridPixel& pixel = grid.getPixel({(row * REDUCE_SCALE_FACTOR) + i, 
                        (horizontalReductionScale * REDUCE_SCALE_FACTOR) + j});
                    blockSumRed += pixel.red;
                    blockSumGreen += pixel.green;
                    blockSumBlue += pixel.blue;
                }
            }

            // calculate block mean
            uint32_t overflowDivisor = REDUCE_SCALE_FACTOR * horizontalOverflow;
            uint8_t blockRedMean = blockSumRed / overflowDivisor;
            uint8_t blockGreenMean = blockSumGreen / overflowDivisor;
            uint8_t blockBlueMean = blockSumBlue / overflowDivisor;
            reducedGrid.setPixel({row + 1, horizontalReductionScale + 1}, {blockRedMean, 
                blockGreenMean, blockBlueMean});
        }
    }

    // build overflow row
    if (verticalOverflow) {
        for (uint32_t col = 0; col < horizontalReductionScale; col++) {

            // take RGB pixel sums for overflow block
            size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
            for (uint32_t i = 1; i <= verticalOverflow; i++) {
                for (uint32_t j = 1; j <= REDUCE_SCALE_FACTOR; j++) {
                    GridPixel& pixel = grid.getPixel({(verticalReductionScale * REDUCE_SCALE_FACTOR) + i, 
                        (col * REDUCE_SCALE_FACTOR) + j});
                    blockSumRed += pixel.red;
                    blockSumGreen += pixel.green;
                    blockSumBlue += pixel.blue;
                }
            }

            // calculate block mean
            uint32_t overflowDivisor = verticalOverflow * REDUCE_SCALE_FACTOR;
            uint8_t blockRedMean = blockSumRed / overflowDivisor;
            uint8_t blockGreenMean = blockSumGreen / overflowDivisor;
            uint8_t blockBlueMean = blockSumBlue / overflowDivisor;
            reducedGrid.setPixel({verticalReductionScale + 1, col + 1}, {blockRedMean, 
                blockGreenMean, blockBlueMean});
        }
    }

    // build overflow row/col
    if (horizontalOverflow && verticalOverflow) {
        size_t blockSumRed = 0, blockSumGreen = 0, blockSumBlue = 0;
        for (uint32_t i = 1; i <= verticalOverflow; i++) {
            for (uint32_t j = 1; j <= horizontalOverflow; j++) {
                GridPixel& pixel = grid.getPixel({(verticalReductionScale * REDUCE_SCALE_FACTOR) + i, 
                    (horizontalReductionScale * REDUCE_SCALE_FACTOR) + j});
                blockSumRed += pixel.red;
                blockSumGreen += pixel.green;
                blockSumBlue += pixel.blue;
            }
        }

        // calculate block mean
        uint32_t overflowDivisor = verticalOverflow * horizontalOverflow;
        uint8_t blockRedMean = blockSumRed / overflowDivisor;
        uint8_t blockGreenMean = blockSumGreen / overflowDivisor;
        uint8_t blockBlueMean = blockSumBlue / overflowDivisor;
        reducedGrid.setPixel({verticalReductionScale + 1, horizontalReductionScale + 1}, {blockRedMean, 
            blockGreenMean, blockBlueMean});
    }

    reducedGrid.printPixelGrid();
    computedFlag = true;
}

/*
 * Title: TokenPerceptualHash method
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
 * Title: TokenPerceptualHash method
 * Parameters (0): N/A
 * Functionality: Computes normal token hash throughout entire grid
 */
void TokenPerceptualHash::executeTokenHash(void) {}

/*
 * Title: TokenPerceptualHash method
 * Parameters (0): N/A
 * Functionality: Computes token hash based on specific region in grid
 */
void TokenPerceptualHash::executeTokenRegionHash(void) {}
