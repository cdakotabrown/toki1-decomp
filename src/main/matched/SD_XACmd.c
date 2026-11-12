/* Address: 0x8007A73C */
/* Size: 444 bytes (111 instructions) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:t7,2:t8,3:t9,4:t1,5:t2,6:t4,7:t5,9:t7,10:t8,12:t1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x78:rt:a2:a1 0x84:rs:a2:a1 */
#include "types.h"

typedef struct {
    s32 field0;
    s32 field1;
    s32 field2;
    s32 field3;
} ColorData;

typedef struct {
    s32 flags;
    s32 flags2;
    s32 arg;
    s32 unk0C;
    s32 pos0;
    s32 pos1;
    u8  unk18[0x35];
    u8  hi0;
    u8  unk4E;
    u8  hi1;
} SaveLoadState;

extern SaveLoadState gSDInfo;

extern ColorData *get_XAsector(void);
extern void serial_vol_base_set(s32, s32);
extern void SD_SerialClear(void);
extern void SD_SerialVolSend(s32);
extern void serial_cmd(s32);
extern void serial_fade_set(s32);

void SD_XACmd(s32 arg0) {
    ColorData *ret;
    s32 a0;
    s32 v1;
    s32 sel;
    u32 t9val;
    u32 t2val;
    s32 flags;

    sel = arg0 & 0xF000;
    switch (sel) {
    case 0x0:
        ret = get_XAsector();
        a0 = ret->field0;
        v1 = ret->field1;
        gSDInfo.pos0 = ret->field2;
        gSDInfo.pos1 = ret->field3;
        t9val = gSDInfo.pos0;
        gSDInfo.hi0 = (u8)(t9val >> 24);
        t2val = gSDInfo.pos1;
        gSDInfo.hi1 = (u8)(t2val >> 24);
        gSDInfo.pos0 = t9val & 0xFFFFFF;
        gSDInfo.pos1 = t2val & 0xFFFFFF;
        serial_vol_base_set(a0, 0xFFFFFF);
        if (a0 == 0xF) {
            a0 = 8;
        } else {
            a0 = 0x20;
        }
        flags = gSDInfo.flags;
        gSDInfo.pos0 = (gSDInfo.pos0 * a0) + gSDInfo.hi0 + v1 - 0x10;
        gSDInfo.pos1 = (gSDInfo.pos1 * a0) + gSDInfo.hi0 + v1 + 0x10;
        if (flags & 0x400) {
            serial_fade_set(0xB4);
        } else if (!(gSDInfo.flags2 & 0x200) && !(flags & 0x4000)) {
            SD_SerialClear();
            SD_SerialVolSend(gSDInfo.arg);
        }
        break;
    case 0x1000:
        serial_cmd(0x74);
        SD_SerialClear();
        break;
    case 0x2000:
        serial_cmd(0xE4);
        break;
    case 0x4000:
        serial_cmd(0xF4);
        break;
    }
}
