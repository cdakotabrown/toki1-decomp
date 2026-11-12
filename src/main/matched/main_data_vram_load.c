/* Address: 0x80059E48 (true start: the `lw _sys_default_tpage_set_tbl2` head is hoisted */
/* Size: 536 bytes (134 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0[4648];
    s32 t[64];
} Save;

extern s32 _sys_default_tpage_set_tbl2[11];
extern s32 _sys_default_tpage_set_tbl0[2];
extern Save screen_w;

void main_data_vram_load()
{
    s32 i;
    s32 d;

    d = 0x80000;

    if (!(_sys_default_tpage_set_tbl2[0] & 0x80000)) {
        for (i = 0; i < 11; i++) {
            _sys_default_tpage_set_tbl2[i] += d;
        }
        for (i = 0; i < 2; i++) {
            _sys_default_tpage_set_tbl0[i] += d;
        }
    }
    load_tpage_buf_lock(31, _sys_default_tpage_set_tbl2[7], 2);
    load_tpage_buf_lock(30, _sys_default_tpage_set_tbl2[8], 2);
    load_tpage_buf_lock(29, _sys_default_tpage_set_tbl2[9], 2);
    load_tpage_buf_lock(28, _sys_default_tpage_set_tbl2[10], 2);
    load_tpage_buf_lock(27, _sys_default_tpage_set_tbl0[0], 2);
    load_tpage_buf_lock(59, _sys_default_tpage_set_tbl0[1], 2);
    load_tpage_buf_lock(60, _sys_default_tpage_set_tbl2[0], 2);
    load_tpage_buf_lock(61, _sys_default_tpage_set_tbl2[1], 2);
    load_tpage_buf_lock(62, _sys_default_tpage_set_tbl2[2], 2);
    load_tpage_buf_lock(63, _sys_default_tpage_set_tbl2[3], 2);
    for (i = 0; i < 11; i++) {
        _sys_default_tpage_set_tbl2[i] -= d;
    }
    for (i = 0; i < 2; i++) {
        _sys_default_tpage_set_tbl0[i] -= d;
    }
    for (i = 27; i < 32; i++) {
        screen_w.t[i] -= 0x80000;
    }
    i = 59;
    do {
        screen_w.t[i] -= 0x80000;
    } while (++i < 64);
}
