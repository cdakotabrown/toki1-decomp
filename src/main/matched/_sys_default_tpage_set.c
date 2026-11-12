/* Address: 0x80048728 */
/* Size: 300 bytes (75 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0[4692];
    s32 a[10];
    s32 b[10];
    u8 pad4772[48];
    s32 c[10];
    s32 d[10];
} Tbl;

extern Tbl screen_w;
extern s32 _sys_default_tpage_set_tbl2, _sys_default_tpage_set_tbl3, _sys_default_tpage_set_tbl4, _sys_default_tpage_set_tbl5;
extern s32 _sys_default_tpage_set_tbl6, _sys_default_tpage_set_tbl7, _sys_default_tpage_set_tbl8, _sys_default_tpage_set_tbl9;
extern s32 _sys_default_tpage_set_tbl0, _sys_default_tpage_set_tbl1;
extern s32 tpage_buf[];

void _sys_default_tpage_set()
{
    s32 i;

    for (i = 0; i < 5; i++) {
        screen_w.d[i] = screen_w.b[i] = screen_w.c[i] = screen_w.a[i] = 119;
    }
    tpage_buf[60] = _sys_default_tpage_set_tbl2;
    tpage_buf[61] = _sys_default_tpage_set_tbl3;
    tpage_buf[62] = _sys_default_tpage_set_tbl4;
    tpage_buf[63] = _sys_default_tpage_set_tbl5;
    tpage_buf[31] = _sys_default_tpage_set_tbl6;
    tpage_buf[30] = _sys_default_tpage_set_tbl7;
    tpage_buf[29] = _sys_default_tpage_set_tbl8;
    tpage_buf[28] = _sys_default_tpage_set_tbl9;
    tpage_buf[27] = _sys_default_tpage_set_tbl0;
    tpage_buf[59] = _sys_default_tpage_set_tbl1;
}
