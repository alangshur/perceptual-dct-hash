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
    protected:
        PerceptualHash(const PixelGrid& grid) : computedFlag(false), grid(grid) {}
        ~PerceptualHash(void) {}
        virtual void executeHash(void) = 0;
        virtual void printHashBits(void) const = 0;

        // state condition
        bool computedFlag; 
 
        // original grid 
        const PixelGrid& grid;
};

class ImagePerceptualHash : PerceptualHash {
    public:
        ImagePerceptualHash(const PixelGrid& grid, 
            const uint32_t normalizationSize = DEFAULT_NORMALIZATION_DIMENSION);
        ~ImagePerceptualHash(void);
        static bool compareHashes(ImagePerceptualHash& hs1, ImagePerceptualHash& hs2, 
            const uint32_t errorDegree, const uint32_t normalizationSize = DEFAULT_NORMALIZATION_DIMENSION);
        void executeHash(void);
        void printHashBits(void) const;
        IPHS& getHash(void) { if (computedFlag) return result; 
            else throw "ImagePerceptualHash Error: Hash not yet computed."; }

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
            regionHashFlag(false), region({}) {}
        TokenPerceptualHash(const PixelGrid& grid, const GridDimensions region) :
            PerceptualHash(grid), result({}), regionHashFlag(true), region(region) {}
        static bool compareHashes(TokenPerceptualHash& hs1, TokenPerceptualHash& hs2, 
            const uint32_t errorDegree, const uint32_t normalizationSize = DEFAULT_NORMALIZATION_DIMENSION);
        void executeHash(void);
        void printHashBits(void) const;
        TPHS& getHash(void) { if (computedFlag) return result; 
            else throw "TokenPerceptualHash Error: Hash not yet computed."; }

    private:
        void executeTokenHash(void);
        void executeTokenRegionHash(void);

        // hash result
        TPHS result;

        // hash region parameters
        const bool regionHashFlag;
        const GridDimensions region;
};

#endif