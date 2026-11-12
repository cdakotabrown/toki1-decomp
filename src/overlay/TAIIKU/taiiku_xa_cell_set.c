/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x8013384C */
/* Size: 220 bytes (55 insns)   BYTE-PERFECT, zero PostProc */
#include "types.h"

typedef struct {
    s32 e[12][4];
} Blk192;

extern Blk192 taiiku_res_xa_beg;
extern Blk192 taiiku_res_xa_end;

extern void device_cmd_send(s32 a0);
extern void set_XAsector(s32 a0, s32 a1, s32 a2);

void taiiku_xa_cell_set(s32 a0_in, u8 a1_in)
{
    s32 padTop[2];
    Blk192 buf1;
    Blk192 buf2;
    s32 padBot[2];
    s32 a2_val;

    buf1 = taiiku_res_xa_beg;
    buf2 = taiiku_res_xa_end;

    if (a0_in == 11) {
        a2_val = 0;
    } else {
        a2_val = a0_in;
    }

    set_XAsector(buf1.e[a0_in][a1_in], buf2.e[a0_in][a1_in], a2_val);
    device_cmd_send(768);
}
