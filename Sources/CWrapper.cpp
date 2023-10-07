//
//  CWrapper.cpp
//  YAD
//
//  Created by Developer on 07.10.2023.
//

#include "main.h"
// extern "C" will cause the C++ compiler
// (remember, this is still C++ code!) to
// compile the function in such a way that
// it can be called from C
// (and Swift).
//int processData(unsigned char *key_basis, unsigned char *data_id, unsigned char *dst)
extern "C" int unwrapKey(unsigned char *key_basis, unsigned char *data_id, unsigned char *dst)
{
    return processData(key_basis, data_id, dst);
}
