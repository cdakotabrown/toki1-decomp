/* Function: debug_hex_show */
/* Address: 0x8004B50C */
/* Size: 412 bytes (103 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 prim_alloc(s32, s32);
extern void SetSprt8(s32);
extern void SetSemiTrans(s32, s32);
extern s16 GetClut(s32, s32);
extern void AddPrim(u8 *, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);

extern u8 _debug_number_show_tbl0;
extern u8 _debug_number_show_str0;
extern u8 _debug_number_show_tbl1[];
extern s16 screen_w[];
extern s32 activeBuff;
extern u8 ot_buff0[];

void debug_hex_show(u32 arg0, s32 arg1, s32 arg2) {
    s32 obj;
    u32 idx;
    s32 i;
    u8 *p;

    i = 0;

    if (_debug_number_show_tbl0 < 4U) {
        return;
    }

    idx = arg0;

    if (_debug_number_show_str0 == 0) {
        return;
    }

    do {
        obj = prim_alloc(16, activeBuff);
        SetSprt8(obj);
        SetSemiTrans(obj, 0);

        *(u8 *)(obj + 4) = 128;
        *(u8 *)(obj + 5) = 128;
        *(u8 *)(obj + 6) = 128;

        *(s16 *)(obj + 8) = screen_w[activeBuff + 10] + arg1 - i * 6;
        *(s16 *)(obj + 10) = screen_w[activeBuff + 12] + arg2;

        p = &_debug_number_show_tbl1[(idx & 0xF) * 2];
        *(u8 *)(obj + 12) = p[0];
        *(u8 *)(obj + 13) = p[1];

        *(s16 *)(obj + 14) = GetClut(256, 511);

        AddPrim(&ot_buff0[(activeBuff << 10) + 4], obj);

        if (idx < 16U) {
            dtd_on_tpage(704, 256, 1, 0, 0);
            return;
        }

        idx = idx >> 4;
        i++;
    } while (1);
}
