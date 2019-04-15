#ifndef PHASH_H
#define PHASH_H

#include <vector>
#include <memory>
#include "grid.h"
using namespace std;

#define HASH_SEGMENT_SIZE 64
#define DEFAULT_NORMALIZATION_DIMENSION 32

// image perceptual hash string struct type definition
typedef struct { 
    unique_ptr<vector<uint64_t>> redData;
    unique_ptr<vector<uint64_t>> greenData;
    unique_ptr<vector<uint64_t>> blueData;
    unique_ptr<vector<uint64_t>> combinedData;
} IPHS;

// token perceptual hash string struct type definition
typedef struct { uint64_t data; } TPHS;

class PerceptualHash {
    public:
        bool compareHashes(const IPHS& hs1, const IPHS& hs2) { return false; }
        bool compareHashes(const TPHS& hs1, const TPHS& hs2) { return false; }

    protected:
        PerceptualHash(const PixelGrid& grid) : computedFlag(false), grid(grid) {}
        ~PerceptualHash(void) {}
        virtual void executeHash(void) = 0;
        virtual void printHashBits(void) = 0;

        // state condition
        bool computedFlag; 
 
        // original grid 
        const PixelGrid& grid;
};

class ImagePerceptualHash : PerceptualHash {
    public:
        ImagePerceptualHash(const PixelGrid& grid, 
            const uint32_t normalizationDimensions = DEFAULT_NORMALIZATION_DIMENSION);
        ~ImagePerceptualHash(void);
        void executeHash(void);
        void printHashBits(void);

    private:
        GridPixel normalizeGridRGB(const PixelGrid& pixelGrid, PixelGrid& normalizedGrid) const;
        void computeRGBHash(const PixelGrid& normalizedGrid, const GridPixel& meanRGBValues);

        // hash result
        IPHS result; 

        // hash storage parameters
        const uint32_t normalizationDimension;
        const uint32_t hashColorLength;
};

class TokenPerceptualHash : PerceptualHash {
    public:
        TokenPerceptualHash(const PixelGrid& grid) : PerceptualHash(grid), result({}), 
            regionHash(false), region({}) {}
        TokenPerceptualHash(const PixelGrid& grid, const GridDimensions region) :
            PerceptualHash(grid), result({}), regionHash(true), region(region) {}
        void executeHash(void);

    private:
        void executeTokenHash(void);
        void executeTokenRegionHash(void);

        // hash result
        TPHS result;

        // hash region parameters
        const bool regionHash;
        const GridDimensions region;
};

#endif