#ifndef PHASH_H
#define PHASH_H

#include "grid.h"
using namespace std;
 
// perceptual hash string struct type definition
typedef struct { uint64_t data[8]; } PHS;

class PerceptualHash {
    public:
        PHS getHashResult(void) const { return result; }

    protected:
        PerceptualHash(const PixelGrid& grid) : computedFlag(false), 
            grid(grid), result({0, 0, 0, 0, 0, 0, 0, 0}) {}
        ~PerceptualHash(void) {}
        virtual void executeHash(void) = 0;

        // state condition
        bool computedFlag; 
 
        // original grid 
        const PixelGrid& grid;

        // hash result
        PHS result;
};

class ImagePerceptualHash : PerceptualHash {
    public:
        ImagePerceptualHash(const PixelGrid& grid) : PerceptualHash(grid) {}
        ~ImagePerceptualHash(void) {}
        void executeHash(void);

    private:
        void normalizeGridRGB(const PixelGrid& pixelGrid, PixelGrid& normalizedGrid) const;
};

class TokenPerceptualHash : PerceptualHash {
    public:
        TokenPerceptualHash(const PixelGrid& grid) : PerceptualHash(grid), 
            regionHash(false), region({0, 0}) {}
        TokenPerceptualHash(const PixelGrid& grid, const GridDimensions region) :
            PerceptualHash(grid), regionHash(true), region(region) {}
        ~TokenPerceptualHash(void) {}
        void executeHash(void);

    private:
        void executeTokenHash(void);
        void executeTokenRegionHash(void);

        const bool regionHash;
        const GridDimensions region;
};

#endif