
//SPM impl

#include "main.h"

void unwrapKey(const unsigned char key_basis[16], const unsigned char data_id[20], unsigned char dst[16]) {
    unsigned char buf[16];
    decrypt_main(key_basis, buf);
    bind_key(buf, data_id, dst);
}
