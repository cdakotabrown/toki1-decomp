/* Address: 0x8004DF04 */
/* Size: 804 bytes (201 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map 104:96 */
/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:16 IDO52_UGEN_TEMP_REG=30:t8,31:t9,32:t7,40:t5,41:t6 IDO52_AS1_TIE_REJECT=239 */
#include "types.h"

typedef struct {
    s32 ptr0;
    s32 field_04;
    u8 field_08;
    u8 pad[3];
    u8 field_0C;
    u8 field_0D;
    u8 pad2[2];
    s32 field_10;
} SpriteInfoDF;

extern SpriteInfoDF mod_w[];
extern s32 activeBuff;
extern u8 ot_buff0[];

extern s32 get_brightness();
extern s32 prim_alloc();
extern void SetSprt8();
extern void SetSemiTrans();
extern s16 GetClut();
extern void AddPrim();
extern s32 search_tpage_multi();
extern void goto_tpage();

void mod_set_gpu(a0, a1, a2, a3, arg4, arg5, arg6)
s32 a0; s32 a1; s32 a2; s32 a3; s32 arg4; s32 arg5; s32 arg6;
{
    s32 retval;
    s32 x;
    u32 row;
    s32 obj;
    u32 col;
    u16 tpage;
    u32 w;
    u32 b1, b2, b3;
    u32 *src;
    s32 colorShift;
    s32 y;
    u32 newTpage;

    retval = get_brightness();

    switch (mod_w[a0].field_08) {
    case 0:
        colorShift = 1;
        break;
    case 1:
        colorShift = 2;
        break;
    case 2:
        colorShift = 2;
        break;
    case 3:
        colorShift = 1;
        break;
    case 4:
        colorShift = 1;
        break;
    case 5:
        colorShift = 1;
        break;
    default:
        colorShift = 1;
        break;
    }

    tpage = 0xFFFF;
    src = (u32 *)mod_w[a0].field_04 + mod_w[a0].field_0C * (mod_w[a0].field_0D * a1);

    for (row = 0; row < mod_w[a0].field_0D; row++) {
        for (col = 0, y = (row << 3) + a3 + 360; col < mod_w[a0].field_0C; col++) {
            b2 = (*src & 0xFF0000) >> 16;
            b3 = (*src & 0xFF000000) >> 24;
            b1 = (*src & 0xFF00) >> 8;

            obj = prim_alloc(16, activeBuff);
            SetSprt8(obj);
            SetSemiTrans(obj, 0);
            *(s16 *)(obj + 8) = a2 + 160 + col * 8;
            *(s16 *)(obj + 10) = y - activeBuff * 240;
            *(u8 *)(obj + 13) = ((b2 & 0xF0) >> 4) << 3;
            *(u8 *)(obj + 12) = (b2 & 0xF) << 3;
            *(u8 *)(obj + 6) = retval;
            *(u8 *)(obj + 5) = retval;
            *(u8 *)(obj + 4) = retval;

            newTpage = search_tpage_multi(*(s32 *)(mod_w[a0].ptr0 + b3 * 4), colorShift);
            if (tpage != newTpage) {
                newTpage = (u16)newTpage;
                if (tpage != 0xFFFF) {
                    goto_tpage((tpage & 0xF) << 6, (tpage & 0x10) << 4, arg6 + 1, 0, 0);
                }
                tpage = newTpage;
            }

            *(s16 *)(obj + 14) = GetClut(arg4 + (b1 << 4), arg5);
            AddPrim(&ot_buff0[(activeBuff << 10) + (arg6 << 2) + 4], obj);

            src++;
        }
    }

    goto_tpage((tpage & 0xF) << 6, (tpage & 0x10) << 4, arg6 + 1, 0, 0);
}
