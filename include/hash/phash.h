#ifndef PHASH_H
#define PHASH_H

#include <vector>
#include <memory>
#include "pimg/grid.h"
#include "hash/ihash.h"
using namespace std;

#define HASH_SEGMENT_SIZE 64
#define DEFAULT_NORMALIZATION_DIMENSION 32

typedef struct { 
    unique_ptr<vector<uint64_t>> redData;
    unique_ptr<vector<uint64_t>> greenData;
    unique_ptr<vector<uint64_t>> blueData;
    unique_ptr<vector<uint64_t>> luminanceData;
    unique_ptr<vector<uint64_t>> grayscaleData;
    unique_ptr<vector<uint64_t>> combinedData1;
    unique_ptr<vector<uint64_t>> combinedData2;
} IPHS;

typedef struct {
    float redErrorRat;
    float greenErrorRat;
    float blueErrorRat;
    float luminanceErrorRat;
    float grayscaleErrorRat;
    float combined1ErrorRat;
    float combined2ErrorRat;
} IPHSErrorDiagnosis;

class ImagePerceptualHash : PerceptualHash {
    public:
        ImagePerceptualHash(const PixelGrid& grid, 
            const uint32_t normalizationSize = DEFAULT_NORMALIZATION_DIMENSION);
        ~ImagePerceptualHash(void);
        static IPHSErrorDiagnosis compareHashes(ImagePerceptualHash& hs1, ImagePerceptualHash& hs2, 
            const bool verbose = true, const uint32_t normalizationSize = DEFAULT_NORMALIZATION_DIMENSION);
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

#endif