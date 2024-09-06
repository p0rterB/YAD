
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

FOUNDATION_EXPORT int unwrapKey(unsigned char *key_basis, unsigned char *data_id, unsigned char *dst) {
    return processData(key_basis, data_id, dst);
}
