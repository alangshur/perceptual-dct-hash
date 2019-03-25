### Main features
- Validate image (public permission)
- Construct image (private permission)
  - Disallow layered mappings
  - Specify degree of image modification freedom
  
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
  
 
 
