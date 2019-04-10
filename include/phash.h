#ifndef PHASH_H
#define PHASH_H

#include "grid.h"
using namespace std;
 
// perceptual hash string type definition
typedef uint64_t PHS;

class PerceptualHash {
    public:
        PHS getHashResult(void) const { return result; }

    protected:
        PerceptualHash(const PixelGrid& grid) : computedFlag(false), 
            grid(grid), result(0) {}
        ~PerceptualHash(void);
        virtual void executeHash(void);

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
        ~ImagePerceptualHash(void);
        void executeHash(void);
};

class TokenPerceptualHash : PerceptualHash {
    public:
        TokenPerceptualHash(const PixelGrid& grid) : PerceptualHash(grid) {}
        ~TokenPerceptualHash(void);
        void executeHash(void);
};

#endif