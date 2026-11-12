/* @CompileFlags: -Wc,-pic0 */
/* 4. `@RodataBase: 0x80134A2C` for the switch's jr table (`lw $t6,18988($at)` */
/* 6. ⛔ DELETED `@PostProc: swap_registers v1:v0` -- it was the last 3 words. */
/* 7. ⛔ DELETED `@PostProc: scheduler_normalizer`.  It had been hiding a REAL */
/* @SymbolSize: 644 */
/* @RodataBase: 0x80134A2C */
/* @PostProc: remap_stack_slots --map '28:40,32:44,36:48,40:52,44:56,52:64,64:76,72:84' */
/* omimai_bustup_read - OMIMAI overlay, Address: 0x80134628, Size: 644 bytes (161 insns) */
#include "types.h"

typedef struct {
    s32 words[11];
} Data44;

typedef struct {
    s32 words[2];
} Data8;

extern u8 now_month;
extern u16 basyo_pack;
extern s32 sched_icon;
extern u8 now_girl;

extern Data44 omimai_bustup_read_tbl0;
extern Data8 omimai_bustup_read_tbl1;
extern Data8 omimai_bustup_read_tbl2;
extern Data8 omimai_bustup_read_tbl3;

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);
extern void girl_message_load(void);

void omimai_bustup_read(void)
{
    u8 v0;
    s32 t3;
    u32 t1;
    u32 padIdx;
    Data44 localArr;
    Data8 localB;
    Data8 localC;
    Data8 localD;

    v0 = now_month;
    localArr = omimai_bustup_read_tbl0;
    localB = omimai_bustup_read_tbl1;
    localC = omimai_bustup_read_tbl2;
    localD = omimai_bustup_read_tbl3;

    if ((u32)v0 < 6U || (u32)v0 >= 10U) {

        localArr.words[0] = 21578;
        localArr.words[3] = 21578;
        localArr.words[5] = 21578;
        localB.words[0] = 23873;
        localC.words[1] = 21038;
        localD.words[1] = 22703;
    }
    t3 = sched_icon;
    if ((t3 & 0xF) != 4) {
        goto no_match;
    }
    padIdx = now_girl;
    switch (padIdx) {
        case 0:
            t1 = (u32)basyo_pack >> 12;
            bustup_cd_read(localArr.words[t1]);
            break;
        case 1:
            t1 = (u32)basyo_pack >> 12;
            bustup_cd_read(localB.words[t1]);
            break;
        case 2:
            t1 = (u32)basyo_pack >> 12;
            bustup_cd_read(localC.words[t1 - 2]);
            break;
        case 3:
            t1 = (u32)basyo_pack >> 12;
            bustup_cd_read(localD.words[t1 - 4]);
            break;
        case 4:
            bustup_cd_read(26348);
            break;
        case 5:
            bustup_cd_read(24548);
            break;
        case 6:
            bustup_cd_read(25178);
            break;
        case 10:
            bustup_cd_read(26978);
            break;
        case 9:
            bustup_cd_read(25808);
            break;
        default:
            if ((u32)v0 >= 6U && (u32)v0 < 10U) {
                bustup_cd_read(22523);
            } else {
                bustup_cd_read(22568);
            }
            break;
    }
    sub_sub_step_up();
    return;
no_match:
    girl_message_load();
}
