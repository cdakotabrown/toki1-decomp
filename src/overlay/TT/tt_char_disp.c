/* @CompileEnv: IDO52_FORCE_WEB_COLOR=20:t1,18:v1,0:t0,51:t3,63:a0,68:v0,80:t2,10:a2,107:v1,43:a1,46:a0 IDO52_UGEN_TEMP_REG=21:t9,14:t8,13:t7,22:t4,23:t5 */
/* @PostProc: remap_stack_slots --from 52 --to 48 */
/* @CompileFlags: -cckr */
/* Address: 0x801487C0 (cross-prologue head absorbed), Size: 420 bytes (105 insns) */
/* @SymbolStart: 0x801487C0 */
/* @SymbolSize: 420 */
#include "types.h"

struct PTag {
    unsigned int addr : 24;
    unsigned int len : 8;
};

extern char *tt_disp_wk;
extern char *tt_db_buf;

void tt_char_disp(x, y, kind)
short x;
short y;
unsigned char kind;
{
    char *base;
    struct PTag *ot;
    unsigned char *p;
    unsigned char *q;

    base = tt_db_buf + *(int *)(tt_disp_wk + 8) * 32952;
    if (kind == 32) {
        return;
    }
    kind -= 32;
    ot = (struct PTag *)(base + 0x8000);
    p = *(unsigned char **)(base + 4);
    *(unsigned char **)(base + 4) = p + 20;
    p[3] = 4;
    *(unsigned short *)(p + 16) = 6;
    *(short *)(p + 18) = 8;
    p[4] = 128;
    p[5] = 128;
    p[6] = 128;
    p[7] = 101;
    p[12] = (int)(kind & 0xF) * 6; p[13] = (kind >> 4) * 8 + 128;
    *(short *)(p + 14) = 32764;
    *(short *)(p + 10) = y;
    *(short *)(p + 8) = x;
    ((struct PTag *)p)->addr = ot[2].addr;
    ot[2].addr = (unsigned int)p;
    q = *(unsigned char **)(base + 4);
    *(unsigned char **)(base + 4) = q + 12;
    SetDrawMode(q, 0, 0, 31, 0);
    ((struct PTag *)q)->addr = ot[2].addr;
    ot[2].addr = (unsigned int)q;
}
