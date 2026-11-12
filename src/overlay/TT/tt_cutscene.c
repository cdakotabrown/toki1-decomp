/* @SymbolSize: 600 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1ac */
/* @PostProc: patch_insn_reg_field 0x6c:rt:v0:v1 0x70:rt:a0:v0 0x74:rt:v1:a0 0x84:rs:v0:v1 0x88:rs:v0:v1 0x8c:rs:v0:v1 0x90:rs:v0:v1 0x94:rs:v0:v1 0x98:rs:a0:v0 0xa0:rs:v0:v1 0xa0:rt:v0:v1 0xa4:rs:a0:v0 0xa4:rt:a0:v0 0xa8:rs:v1:a0 0xa8:rt:v1:a0 0xac:rs:v1:a0 0xb0:rs:v1:a0 0xb8:rs:v1:a0 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x8013B1C4, Size: 600 bytes (150 insns) */
#include "types.h"

extern u8 tt_puff_seq_tbl[];
extern u8 tt_anim_tbl[];
extern s16 tt_cutscene_scr[];

extern u8 *tt_boss_wk;

extern void tt_cmd_once(s32 id);
extern void tt_shape_clear(void);
extern void tt_seq_step(u8 *p, s32 n);

void tt_cutscene(void)
{
    u8 *s;
    u8 *q;
    u8 *p;
    u8 *b;
    u8 *h;
    u8 *e;
    u8 *tbl;
    s16 *tab;
    s32 i;
    s32 n;
    s32 u;

    s = tt_boss_wk;
    switch (*(u16 *)(s + 8)) {
    case 0:
        tt_shape_clear();
        *(s16 *)(s + 868) = 0;
        *(s16 *)(s + 980) = 0;
        tbl = tt_anim_tbl;
        i = 16;
        p = s + 320;
        b = s + 16;
        h = s + 32;
        do {
            *(s32 *)(p + 56) = (s32)tbl;
            p[60] = 0;
            p[61] = 4;
            p[66] = 0;
            p[72] = 0;
            b[784] = 0;
            i += 1;
            p += 20;
            b += 1;
            h += 2;
            *(s16 *)(h + 678) = 8;
            *(s16 *)(h + 582) = 0;
            *(s16 *)(h + 630) = 0;
        } while (i != 19);
        *(s16 *)(s + 8) = 64;
        break;
    case 64:
        q = tt_boss_wk;
        tab = tt_cutscene_scr;
        n = tab[*(u16 *)(q + 980)];
        if (n != -1) {
            if (n == *(u16 *)(q + 868)) {
                tt_cmd_once(1281);
                u = tab[*(u16 *)(s + 980) + 4];
                s[u + 784] = 1;
                s[u * 20 + 64] = 128;
                e = (u8 *)((s32)s + u * 2);
                *(s16 *)(e + 584) = tab[*(u16 *)(s + 980) + 1];
                *(s16 *)(e + 632) = tab[*(u16 *)(s + 980) + 2];
                *(s16 *)(e + 680) = tab[*(u16 *)(s + 980) + 3];
                *(s16 *)(s + 980) = (s16)(*(u16 *)(s + 980) + 5);
            }
        }
        tbl = tt_puff_seq_tbl;
        i = 16;
        do {
            tt_seq_step(tbl, i);
            i += 1;
        } while (i < 19);

        if (*(u16 *)(s + 868) == 133) {
            i = 0;
            p = s;
            do {
                i += 4;
                p[785] = 0;
                p[786] = 0;
                p[787] = 0;
                p += 4;
                p[780] = 0;
            } while (i != 16);
        }
        if (*(u16 *)(s + 868) == 156) {
            i = 0;
            p = s;
            do {
                i += 4;
                p[785] = 0;
                p[786] = 0;
                p[787] = 0;
                p += 4;
                p[780] = 0;
            } while (i != 24);
            *(s16 *)(s + 8) = 0;
            *(s16 *)(s + 4) = 160;
        }
        break;
    default:
        break;
    }
    *(s16 *)(s + 868) = (s16)(*(u16 *)(s + 868) + 1);
}
