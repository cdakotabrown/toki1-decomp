/* Size: 84 bytes */
/* @SymbolSize: 84 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];


void taiiku_y_step(void) {
    class_w[100].frame = class_w[100].frame - 4;
    class_w[101].frame = class_w[101].frame - 3;
    class_w[102].frame = class_w[102].frame - 2;
    class_w[103].frame = class_w[103].frame - 1;
}
