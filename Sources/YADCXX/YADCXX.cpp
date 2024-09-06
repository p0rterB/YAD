
//SPM impl

#include "main.h"

int unwrapKey(unsigned char *key_basis, unsigned char *data_id, unsigned char *dst) {
    return processData(key_basis, data_id, dst);
}
