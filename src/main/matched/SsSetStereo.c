/* Address: 0x80090CB0, Size: 12 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s16 _svm_stereo_mono;

void SsSetStereo(void) {
    _svm_stereo_mono = 0;
}
