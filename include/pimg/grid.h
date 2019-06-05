#ifndef GRID_H
#define GRID_H

#include <cstdio>
#include <memory>
#include <vector>
#include <stdlib.h>
using namespace std;

typedef struct {
    size_t height;
    size_t width;
} GridDimensions;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} GridPixel;

typedef struct {
    uint32_t row; // 1-indexed
    uint32_t column; // 1-indexed
} GridIndex;

class PixelGrid {
    public:
        PixelGrid(const GridDimensions& d);
        ~PixelGrid(void);
        GridPixel& getPixel(const GridIndex& i) const;
        size_t getGridHeight(void) const { return dimensions.height; }
        size_t getGridWidth(void) const { return dimensions.width; }
        void setPixel(const GridIndex& i, const GridPixel& p);
        void printPixelGrid(void) const;

    private:
    
        // dynamic pixel data container
        unique_ptr<vector<GridPixel>> pixelArray;

        // pixel grid parameters
        GridDimensions dimensions;
        const size_t gridSize;
};

#endif