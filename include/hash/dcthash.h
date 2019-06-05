#ifndef DCTHASH_H
#define DCTHASH_H

#include <vector>
#include <memory>
#include "pimg/grid.h"
#include "hash/phash.h"
#include "hash/ihash.h"
using namespace std;

typedef struct { uint64_t data; } TPHS;

class TokenPerceptualHash : PerceptualHash {
    public:
        TokenPerceptualHash(const PixelGrid& grid) : PerceptualHash(grid), result({}), 
            regionHashFlag(false), region({}) {}
        TokenPerceptualHash(const PixelGrid& grid, const GridDimensions region) :
            PerceptualHash(grid), result({}), regionHashFlag(true), region(region) {}
        static bool compareHashes(TokenPerceptualHash& hs1, TokenPerceptualHash& hs2, 
            const uint32_t errorDegree, const bool verbose = true,
            const uint32_t normalizationSize = DEFAULT_NORMALIZATION_DIMENSION);
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