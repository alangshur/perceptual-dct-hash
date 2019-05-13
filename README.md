### Software Requirements/Libraries:
- C++17 
- openCV (opencv4)
- pkg-config (note: you may have to manually set PKG_CONFIG_PATH)
- Homebrew (recommended)

# To Do
- Clean current modules (style guidelines)
  - Split perceptual hash modules
  - Add configuration options for full perceptual hash (faster options)
  - Unify reference-passing guidelines and privacy modifiers
  - Build smarter interfaces and documentation
- Wrap all libraries using CMake for maximum compatibility
- Build organizational graph of all modules and future modules
  - All past and future modules
  - Organization and library dependencies
- Build graphics module:
  - Reverse grid printing (tie graphics module into grid)
  - Graphical feature highlighting
- Build discrete cosine transfer perceptual hash
    - Links:
        - https://users.cs.cf.ac.uk/Dave.Marshall/Multimedia/node231.html
        - http://hackerfactor.com/blog/index.php%3F/archives/432-Looks-Like-It.html
    - Separate full and partial phash modules 
    - Redundancy technologies to maximize effectivness
- Implement pure image algorithm (module)
  - Feature and keypoint extraction
    - SIFT keypoints
    - Naive pixel values
  - Add ML algorithm for comparing 32x32 grids (double feedback)
  - Redundancy technologies to maximize effectivness
- Key point scrambbling algorithm (module)
  - Intelligent scrambbling of keypoint results
  - Use fixed hash values based on region hashes
  - Test results and tweak parameters for best success rates
- Implement database module
- Implement web client

### Planned system specs
- Bucket-based image hashing
  - Based on specified DOF
  - 64-bit hash with hidden seed (RSA?)
  - Hash is inimitable and irreversible
  - Unique reading pattern for each hash
- Public/private key:
  - Private key provides ownership/modification permissions
  - Public key allows for context checking (and original photo)
  - Network connected: extra layer (and why not?)
- Relevant DOFs:
  - Cropping (!!)
  - Color/image slider modifications
  - General changes in image content (e.g. photoshop)
- Validation results:
  - No mapping
  - Mapping (with metadata)
  - Tamper error (this should *at least* always be returned if an initial mapping is applied)
  
### Methodology
- Public/private key approach:
    - O (Original): Original image fed into image validation
    - P (Private): Private key generated during initial image validation
    - M (Mapped): Mapped image acts as public key
    - H (Heuristic): Heuristic to tighten irreversability of image validation (SHA, time, user credentials, NDR image metadata)
    - O + P + M = H (image validation abstraction)
- Mapped detection: 
    - Spread of "tags" -- small bit patterns in pixels around image that can be detected by algorithm and correspond to private key
    - Hidden algorithm to determine private key from tag pattern
    - Images indexed in database by private key -- does detected public key entry match the public key on file?
    - How to encode tags in image? 
        - Requirements: cannot (at all) modify the image appearance
        - Somehow get from image (that is potentially extremely modified) to the tag
        - Potential solution:
            - Given a single image mapping, use propietary information (no parameters) to discern X focal points
                - This is tricky: we need to assume that people can determine what pixels are changed but they can't be able to replicate the algorithm that determines the focal points
                - How to find focal points? Heuristic hash tree algorithm:
                    - Start by normalizing pixel values in order to ignore slider changes
                    - Choose random pixel (based on heuristics)
                    - If point passes first tree hash bucket, move to second (else, restart)
                    - Make small jump based on second-level node (if jump is out of bounds, restart)
                    - Repeat from root of tree N more times (where N is a number that doesn't require too much processing time and the result returns around 20 focal points)
                    - If after N repetitions, the final pixel is valid AND it's focal region is in bounds AND doesn't interfere with a previously chosen focal region, this will become a focal point (if not, restart) (N ~ 22)
                - Tree structure:
                    - First level (root): pixel hash (binary buckets) with hidden heuristic
                    - Edge from first to second level: based on hash of direct neighbor pixels (8 x 255R x 255G x 255B buckets) with hidden heuristic, move to second-level node corresponding to bucket
                    - Second level: execute small jump from pixel given heuristically by hash bucket and first-level pixel value
            - Each focal point corresponds to a particular 64-bit hash based on neighboring pixel values (based on the location of focal point, it should "point at" a very specific group of tweaked neighboring pixels)
            - X should be high enough that a single cropping (of a meaningful size) should contains several focal points
            - Once we have a small cutout of the image, we can determine the same original focal points and their hash
            - The neighboring pixel group of a focal point can be run through a propetary algorithm to reveal the private key
            - The focal points should be chosen in a way such that any modification not in the DOFs will still reveal the same focal points and the same private hash
                - Algorithm normalizes RGB values (R, G, and B individually for each pixel) throughout image which prevents sliders from changing selected focal points
                - Focal points don't necessarily need to select a single set group of neighbors (could use each pixel in a YxY block)
        - Defense against photoshop (manual modification of pixel values) and cropping:
            - Most difficult to the defend against
            - Only feasible solution (that doesn't require high processing times): increase X (number of focal points) and make smart focal point decisions
            - Example: a 1000x1000 image can fit around 50 15x15 spaced focal point regions (each contains 225 pixels that can be slightly tweaked to create private key hash)
        - How to encode private key around a focal point?
            - Assume we have 15x15 focal point regions: this means we have 225 pixels in each region and 675 variables (one for each RGB)
            - Given 675 variables (that can be slightly tweaked), how can you encode a 64-bit private key?
            - Alternative: use 10x10 focal regions to build a 128-bit hash that will point to a bucket in a massive database that points to the card for the original image
                - This alternative allows us to not modify the original image
                - All the focal regions (based on their unique hashes) for a specific image will point to the same original photo card
                - Focal regions hashes should avoid all modifications not invoked by the degree of freedom (DOF)
                - Probabilistic model that prevents collisions of focal region hashes
                - Goal: reduce the size of the focal regions so that we can incorporate far more -- this will boost the effectivness of the utility in avoiding photoshop/cropping changes 

### Protection and corruption
- How do you prevent people from cracking the algorithm?
    - Variability: Users can detect the specific changes in a single instance, but will never predict the changes made by the algorithm
    - User incentive: 
        - The core user is the only one with access to the original and the mapped
        - Others won't be able to verify the changes (no access to the original) 
 
# Comparing images and image conversions
- Perceptual hash: ignores color and develop series of weights for Hamming distance
- Avoiding lossy compression due to image conversion: the images should be broken down into a universal format before processing regardless of whether slight modifications (from lossy copmression conversion) to the individual pixel values have taken place
    - Repeatedly converting to JPEG converges in amount of compression saved
    - Potential idea: run JPEG conversion repeatedly to an impractical image quality (that's worse then what anyone will ever use) so that if the actual image is converted to JPEG, it will still be run through this convergence algorithm
    - Do different lossy compression algorithms compress differently? Employ a system of feedback between lossy compression convergence of different compression algorithms
    - Main idea: we should always end up with roughly the same low-quality image before we run the focal points mapping
    - Using perceptual image hashing also helps with this: we can calculate Hamming viability of the individual focal regions 
    - Problem: using slightly modified pixel values in jumps for tree algorithm -- can't trust that the converged low-quality pre-mapping image will be exactly the same each time (small differences in pixel values)
    - Solution to above problem: use bucket-based perceptual hashing around focal points to make steps in tree algorithms (slight differences in cluster of pixels will still allow us to move to the correct place)

# Perceptual Hash Algorithm (Survive color histogram and gamma adjustments)
- Reduce pixel size (depending on original size)
- Convert to greyscale or normalize pixel values (convert to 8-bit pixel intensity)
- Compute the discrete cosine transform (Fourier-like transformation)
- Reduce the DCT (signal energy lies at low frequency in upper left of domain 8 x 8 grid)
- Compute the average value (not including the outlier first frequency term)
- For each remaining frequency (presumably 64 of them), add a 1 bit to the integer if that frequency is greater than the mean (and a 0 otherwise)

# Perceptual Hash Alternatives
- Mean vs DCT
- Mean used for image perceptual hashes (more strict about modifications)
- DCT used for strict token perceptual hashes
