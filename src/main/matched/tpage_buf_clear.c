/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void _sys_default_tpage_set(void);

typedef struct {
    u8 padding[0x1228];
    s32 arr[64];
} DataStruct;

extern DataStruct screen_w;

void tpage_buf_clear(void) {
    s32 i;
    DataStruct *ptr;

    ptr = &screen_w;
    for (i = 0; i < 64; i++) {
        if ((i & 0xF) < 5) {
            ptr->arr[i] = 0;
        } else {
            ptr->arr[i] = -1;
        }
    }
    _sys_default_tpage_set();
}
