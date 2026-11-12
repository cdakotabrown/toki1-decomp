/* @CompileFlags: -cckr */
/* @SymbolSize: 900 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-24 */
/* tt_slot_disp -- TT overlay, Address: 0x8014C83C, Size: 900 bytes (225 insns) */
#include "types.h"

extern u8 *tt_disp_wk;
extern u8 *tt_slot_buf;
extern u8 *tt_scale_buf;
extern u8 *tt_db_buf;

extern void tt_sprite_ot_add_16x16();
extern void tt_sprite_ot_add_var();
extern void anim_sprite_disp();

typedef struct {
    u8 pad[0x8008];
    u32 tbl[1];
} Ot;

void tt_slot_disp()
{
    u8 *base;
    u8 *s;
    u8 *e;
    int i;
    u8 *p;
    u8 *q;

    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    s = tt_scale_buf;
    e = tt_slot_buf;
    for (i = 0; i != 64; i++, e += 120) {
        if (e[0] == 0) {
            continue;
        }
        if (*(u16 *)(e + 4) == 113) {
            anim_sprite_disp(e + 20, e + 56, *(u16 *)(e + 16));
            continue;
        }
        if (*(u16 *)(e + 2) == 71) {
            p = *(u8 **)(base + 4);
            *(u8 **)(base + 4) = p + 48;
            *(s16 *)(s + 4) = *(s16 *)(e + 20);
            *(s16 *)(s + 6) = *(s16 *)(e + 22);
            *(s16 *)(p + 8) = *(s16 *)(e + 20) - 8;
            *(s16 *)(p + 10) = *(s16 *)(e + 22) - 8;
            p[41] = 0;
            p[42] = 1;
            *(s16 *)(p + 16) = 16;
            *(s16 *)(p + 18) = 16;
            p[40] = e[60];
            p[12] = e[92];
            p[13] = e[93];
            *(u16 *)(p + 22) = *(u16 *)(e + 88);
            *(u16 *)(p + 14) = *(u16 *)(e + 90);
            p[44] = 0;
            q = p;
            if (*(u16 *)(e + 6) == 2) {
                p[43] = 0;
                tt_sprite_ot_add_var(&((Ot *)base)->tbl[*(u16 *)(e + 16)], q, s);
            } else {
                q[43] = 1;
                *(s16 *)(s + 2) = *(s16 *)(s + 0) = ((unsigned)(*(u16 *)(e + 110) - 1024) >> 1) + 3072;
                if (*(s16 *)(s + 0) >= 4097) {
                    q[43] = 0;
                }
                tt_sprite_ot_add_var(base + 32804, q, s);
            }
            continue;
        }
        p = *(u8 **)(base + 4);
        *(u8 **)(base + 4) = p + 48;
        *(s16 *)(s + 4) = *(s16 *)(e + 20);
        *(s16 *)(s + 6) = *(s16 *)(e + 22);
        q = p;
        if (66 == *(u16 *)(e + 2)) {
            *(s16 *)(p + 8) = *(s16 *)(e + 20) - 8;
            *(s16 *)(p + 10) = *(s16 *)(e + 22) - 16;
        } else {
            *(s16 *)(q + 8) = *(s16 *)(e + 20) - 8;
            *(s16 *)(q + 10) = *(s16 *)(e + 22) - 8;
        }
        q[12] = e[92];
        q[13] = e[93];
        q[40] = e[60];
        q[41] = 0;
        q[42] = 1;
        *(u16 *)(q + 22) = *(u16 *)(e + 88);
        *(u16 *)(q + 14) = *(u16 *)(e + 90);
        q[44] = 0;
        q[43] = 0;
        tt_sprite_ot_add_16x16(&((Ot *)base)->tbl[*(u16 *)(e + 16)], q, s);
        if (66 == *(u16 *)(e + 2)) {
            q = *(u8 **)(base + 4);
            *(u8 **)(base + 4) = q + 48;
            *(s16 *)(s + 4) = *(s16 *)(e + 20);
            *(s16 *)(s + 6) = *(s16 *)(e + 22);
            *(s16 *)(q + 8) = *(s16 *)(e + 20) - 8;
            *(s16 *)(q + 10) = *(s16 *)(e + 22);
            q[12] = 208; q[13] = 16;
            q[40] = e[60];
            q[41] = 0;
            q[42] = 1;
            *(u16 *)(q + 22) = *(u16 *)(e + 88);
            *(u16 *)(q + 14) = *(u16 *)(e + 90);
            q[44] = 0;
            q[43] = 0;
            tt_sprite_ot_add_16x16(&((Ot *)base)->tbl[*(u16 *)(e + 16)], q, s);
        }
    }
}
