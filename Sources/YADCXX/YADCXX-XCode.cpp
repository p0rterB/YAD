
//CocoaPods impl

#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif

// extern "C" will cause the C++ compiler (remember, this is still C++ code!) to
// compile the function in such a way that it can be called from C (and Swift).

#include "main.h"

FOUNDATION_EXPORT void unwrapKey(const unsigned char key_basis[16], const unsigned char data_id[20], unsigned char dst[16]) {
    unsigned char buf[16];
    decrypt_main(key_basis, buf);
    bind_key(buf, data_id, dst);
}
