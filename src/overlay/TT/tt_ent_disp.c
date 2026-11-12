/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=2,3 */
/* @SymbolSize: 644 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* @PostProc: patch_insn_reg_field 0x7c:rd:s5:s3 0x80:rt:s3:s4 0x84:rt:s4:s5 0x88:rt:s7:s6 0x8c:rt:s6:s7 0xbc:rt:a1:v1 0xc0:rd:v0:a0 0xc4:rs:a1:v1 0xcc:rs:a1:v1 0xcc:rt:s6:s7 0xd0:rs:a1:v1 0xd0:rt:s7:s6 0xe0:rs:a1:v1 0xe8:rs:a1:v1 0xe8:rt:s3:s4 0xf0:rs:a1:v1 0xf4:rs:a1:v1 0xf8:rs:a1:v1 0xf8:rt:s4:s5 0xfc:rs:v0:a0 0x100:rs:a1:v1 0x100:rt:a0:a2 0x10c:rt:a0:a2 0x118:rt:a0:a2 0x11c:rs:a1:v1 0x120:rs:v0:a0 0x120:rt:v1:a1 0x124:rs:a1:v1 0x128:rt:v1:a1 0x134:rt:v1:a1 0x13c:rs:v0:a0 0x148:rd:v0:a0 0x14c:rt:a1:v1 0x154:rs:a1:v1 0x15c:rs:a1:v1 0x15c:rt:s6:s7 0x160:rs:a1:v1 0x160:rt:s7:s6 0x168:rs:a1:v1 0x168:rt:a0:a2 0x170:rs:a1:v1 0x178:rs:a1:v1 0x178:rt:s3:s4 0x180:rs:a1:v1 0x184:rs:a1:v1 0x184:rt:s3:s4 0x188:rs:a1:v1 0x188:rt:s4:s5 0x18c:rs:v0:a0 0x198:rt:a0:a2 0x1a4:rt:a0:a2 0x1a8:rs:a1:v1 0x1ac:rs:v0:a0 0x1ac:rt:v1:a1 0x1b0:rs:a1:v1 0x1b4:rt:v1:a1 0x1c0:rt:v1:a1 0x1c4:rs:v0:a0 0x244:rt:s4:s5 0x254:rs:s5:s3 0x254:rt:s5:s3 0x25c:rs:s5:s3 */
/* Address: 0x80147634, Size: 644 bytes (161 instructions) */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=2,3 */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

extern u8 *tt_disp_wk;
extern u8 *tt_ent_buf;
extern u8 *tt_scale_buf;
extern u8 *tt_db_buf;

extern void tt_sprite_ot_add_var(void *, void *, void *);

void tt_ent_disp(void)
{
    u8 *base;
    u8 *rec;
    u8 *cam;
    u8 *p;
    s32 i;

    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    cam = tt_scale_buf;
    rec = tt_ent_buf;
    i = 0;
    do {
        if (rec[0] != 0) {
            if (*(s16 *)(rec + 82) < 2816) {
                p = *(u8 **)(base + 4);
                *(u8 **)(base + 4) = p + 16;
                p[7] = 117;
                p[3] = 3;
                *(s16 *)(p + 8) = (s16)(*(s16 *)(rec + 20) - 4);
                *(s16 *)(p + 10) = (s16)(*(s16 *)(rec + 22) - 3);
                p[13] = 80;
                p[12] = 88;
                *(s16 *)(p + 14) = 15499;
                ((PTag *)p)->addr = ((PTag *)(base + 0x8000))[7].addr;
                ((PTag *)(base + 0x8000))[7].addr = (u32)p;
            } else if (*(s16 *)(rec + 82) >= 3841) {
                p = *(u8 **)(base + 4);
                *(u8 **)(base + 4) = p + 16;
                p[7] = 117;
                p[3] = 3;
                *(s16 *)(p + 8) = (s16)(*(s16 *)(rec + 20) - 3);
                *(s16 *)(p + 10) = (s16)(*(s16 *)(rec + 22) - 3);
                p[13] = 80;
                p[12] = 80;
                *(s16 *)(p + 14) = 15499;
                ((PTag *)p)->addr = ((PTag *)(base + 0x8000))[7].addr;
                ((PTag *)(base + 0x8000))[7].addr = (u32)p;
            }
            p = *(u8 **)(base + 4);
            *(u8 **)(base + 4) = p + 48;
            *(s16 *)(cam + 4) = *(s16 *)(rec + 20);
            *(s16 *)(cam + 6) = *(s16 *)(rec + 22);
            *(s16 *)(cam + 0) = *(s16 *)(cam + 2) = *(s16 *)(rec + 82);
            *(s16 *)(p + 8) = (s16)(*(s16 *)(cam + 4) - 31);
            *(s16 *)(p + 10) = (s16)(*(s16 *)(cam + 6) - 15);
            *(s16 *)(p + 16) = 64;
            *(s16 *)(p + 18) = 32;
            p[13] = 96;
            p[12] = 0;
            p[40] = 0;
            p[41] = 0;
            p[42] = 1;
            *(s16 *)(p + 22) = 11;
            *(s16 *)(p + 14) = 15499;
            p[44] = 0;
            p[43] = 1;
            tt_sprite_ot_add_var(base + 0x801C, p, cam);
        }
        i++;
        rec += 104;
    } while (i != 6);
}
