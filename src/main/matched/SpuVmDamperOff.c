/* Address: 0x80095030, Size: 12 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s16 _svm_damper;

void SpuVmDamperOff(void) {
    _svm_damper = 0;
}
