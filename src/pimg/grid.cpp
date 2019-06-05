#include <iostream>
#include <string>
#include <string.h>
#include "pimg/grid.h"
using namespace std;

/*
 * Initializes and zeroes dynamic grid memory.
 */
PixelGrid::PixelGrid(const GridDimensions& d) : dimensions(d), 
    gridSize(d.width * d.height) {
    pixelArray.reset(new vector<GridPixel>(gridSize));
    memset(&(*pixelArray).front(), 0, gridSize * sizeof(GridPixel));
}

/*
 * Retrieves and returns indicated pixel value from grid.
 */
GridPixel& PixelGrid::getPixel(const GridIndex& i) const {
    if ((i.column > dimensions.width) || (i.row > dimensions.height))
        throw "PixelGrid Error: Invalid target index.";
    const size_t pixelOffset = ((i.row - 1) * dimensions.width) + (i.column - 1); 
    return (*pixelArray)[pixelOffset];
}

/*
 * Sets pixel at indiciated location to supplied value.
 */
void PixelGrid::setPixel(const GridIndex& i, const GridPixel& p) {
    if ((i.column > dimensions.width) || (i.row > dimensions.height))
        throw "PixelGrid Error: Invalid target index.";
    const size_t pixelOffset = ((i.row - 1) * dimensions.width) + (i.column - 1); 
    (*pixelArray)[pixelOffset] = p;
}

/*
 * Prints RGB pixel value of entire grid.
 */
void PixelGrid::printPixelGrid(void) const {    
    vector<GridPixel>& g = *pixelArray;
    for (size_t i = 0; i < gridSize; i++) {
        if ((i % dimensions.width) == 0) cout << endl << "Row: " 
            << (i / dimensions.width) + 1 << endl << flush;
        cout << "(" << to_string(g[i].red) << ", " << to_string(g[i].green) 
            << ", " << to_string(g[i].blue) << ")" << endl << flush;
    }
}

/*
 * Frees dynamic memory.
 */
PixelGrid::~PixelGrid(void) {
    pixelArray.reset(nullptr);
}