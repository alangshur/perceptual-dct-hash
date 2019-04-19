#ifndef PHASH_H
#define PHASH_H

#include <vector>
#include <memory>
#include "grid.h"
using namespace std;

#define HASH_SEGMENT_SIZE 64
#define DEFAULT_NORMALIZATION_DIMENSION 32

// IPHS struct type definition
typedef struct { 
    unique_ptr<vector<uint64_t>> redData;
    unique_ptr<vector<uint64_t>> greenData;
    unique_ptr<vector<uint64_t>> blueData;
    unique_ptr<vector<uint64_t>> luminanceData;
    unique_ptr<vector<uint64_t>> grayscaleData;
    unique_ptr<vector<uint64_t>> combinedData1;
    unique_ptr<vector<uint64_t>> combinedData2;
} IPHS;

// image hash error weights struct type definition
typedef struct {
    float redErrorWeight;
    float greenErrorWeight;
    float blueErrorWeight;
    float luminanceErrorWeight;
    float grayscaleErrorWeight;
    float combined1ErrorWeight;
    float combined2ErrorWeight;
} IPHSErrorWeights;

// image hash error diagnosis
typedef struct {
    float redErrorRat;
    float greenErrorRat;
    float blueErrorRat;
    float luminanceErrorRat;
    float grayscaleErrorRat;
    float combined1ErrorRat;
    float combined2ErrorRat;
    float resultantError;
} IPHSErrorDiagnosis;

// token hash string struct type definition
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
        static IPHSErrorDiagnosis compareHashes(ImagePerceptualHash& hs1, ImagePerceptualHash& hs2, 
            const uint32_t errorDegree, const bool verbose = true,
            const uint32_t normalizationSize = DEFAULT_NORMALIZATION_DIMENSION);
        void executeHash(void);
        void printHashBits(void) const;
        IPHS& getHash(void) { if (computedFlag) return result; 
            else throw "ImagePerceptualHash Error: Hash not yet computed."; }

        // hash error parameters
        static IPHSErrorWeights errorWeights;

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