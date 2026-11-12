/* Address: 0x80095020, Size: 16 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s16 _svm_damper;
void SpuVmDamperOn(void) {
    _svm_damper = 2;
}
