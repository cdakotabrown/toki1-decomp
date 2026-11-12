/* @SymbolSize: 208 */
/* @CompileFlags: -cckr */
/* tt_ent_spawn -- TT overlay, Address: 0x8014742C, 208 bytes (52 insns) */
#include "types.h"

extern s32 tt_ent_buf;

extern void *get_tt_ent_free();
extern void *tt_ent_set();
extern void tt_obj_make();

void tt_ent_spawn(s16 arg0, u16 arg1, u8 arg2) {
    u8 *p;
    s32 base;

    base = tt_ent_buf;
    p = (u8 *)get_tt_ent_free(base, base + 624);
    if (p == 0) {
        return;
    }
    *(s32 *)(p + 32) = (s32)arg0 << 16;
    *(s32 *)(p + 36) = 0xFFF00000;
    *(s32 *)(p + 40) = 0;
    *(s32 *)(p + 44) = ((s32)arg2 << 14) + 0x10000;
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    *(s16 *)(p + 22) = *(s16 *)(p + 38);
    *(s16 *)(p + 82) = 4096;
    tt_ent_set(p, arg1);
    if (arg1 == 48) {
        tt_obj_make(p);
    }
    *(s16 *)(p + 4) = 64;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 6) = 0;
    *(u8 *)(p + 15) = 0;
    *(u8 *)(p + 14) = 0;
    *(u8 *)(p + 13) = 0;
}
