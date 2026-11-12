/* Address: 0x80090CA0, Size: 16 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s16 _svm_stereo_mono;
void SsSetMono(void) {
    _svm_stereo_mono = 1;
}
