#ifndef IHASH_H
#define IHASH_H

#include "pimg/grid.h"

class PerceptualHash {
    protected:
        PerceptualHash(const PixelGrid& grid) : computedFlag(false), grid(grid) {}
        virtual void executeHash(void) = 0;
        virtual void printHashBits(void) const = 0;

        // state condition
        bool computedFlag; 
 
        // original grid 
        const PixelGrid& grid;
};

#endif