/* Address: 0x8007AA1C */
/* Size: 408 bytes (102 instructions) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=20 IDO52_UGEN_NO_TARGET=12 IDO52_UGEN_TEMP_REG=1:t7,2:t8,3:a0,4:t9,5:t0,6:a0,7:t1,8:t2,9:t3,10:t4,11:t5 IDO52_UGEN_TEMP_REG_INPLACE=3,6 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x94 */
#include "types.h"

typedef struct {
    u32 flags;
    u32 flags2;
    u32 limit;
    u32 unk0C;
    u32 mode;
    u32 unk14;
    u32 cur;
} SaveLoadState;
extern SaveLoadState gSDInfo;

void SsSetSerialAttr(s32 a0, s32 a1, s32 a2);
void cd_command_issue(s32 a0, s32 a1, s32 a2);
s32  cdq_result_word_get(void);
void serial_fade_set(s32 a0);
void func_8007A64C(void);
void SD_SerialVolSend(s32 a0);

void serial_cmd(s32 a0) {
    u32 v;
    s32 result;

    switch ((u8)a0) {
    case 0x74:
        SsSetSerialAttr(0, 0, 0);
        cd_command_issue(9, 0, 0);
        gSDInfo.flags = gSDInfo.flags & ~0x400u & ~0x4000u;
        gSDInfo.flags2 = gSDInfo.flags2 & ~0x200u & ~0x100u;
        break;

    case 0xE4:
        if (gSDInfo.flags2 & 0x200u) {
            gSDInfo.flags = gSDInfo.flags | 0x4000u;
            cd_command_issue(9, 0, 0);
        } else {
            v = gSDInfo.flags;
            if (!(v & 0x400u)) {
                break;
            }
            result = cdq_result_word_get();
            gSDInfo.cur = (u32)result;
            if (gSDInfo.limit >= (u32)result) {
                gSDInfo.cur = gSDInfo.limit;
            }
            gSDInfo.flags = (gSDInfo.flags & ~0x400u) | 0x4000u;
            serial_fade_set(180);
        }
        break;

    case 0xF4:
        v = gSDInfo.flags;
        if (!(v & 0x4000u)) {
            break;
        }
        gSDInfo.flags = v & ~0x4000u;
        if (gSDInfo.mode != 0) {
            func_8007A64C();
        } else {
            SD_SerialVolSend((s32)gSDInfo.cur);
        }
        break;

    default:
        break;
    }
}
