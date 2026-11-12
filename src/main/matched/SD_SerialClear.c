/* Function: SD_SerialClear */
/* Address: 0x8007A8F8 */
/* Size: 188 bytes (47 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers a0 v0 --bidirectional */
/* @PostProc: swap_registers_liveness --address 0x8007A8F8 */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: scheduler_normalizer */
#include "types.h"

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 pad_2C[8];
    u8  field_4C;
    u8  field_4D;
    u8  field_4E;
    u8  field_4F;
} MemCardState;

extern MemCardState gSDInfo;
extern s16 gSDCDLevelInfo[];

void SD_SerialClear(void) {
    MemCardState *p = &gSDInfo;
    s32 temp_t6;
    u8 temp_t7;
    s32 temp_v0;
    u8 temp_t8;
    s32 temp_v1;
    s32 temp_a0;

    gSDCDLevelInfo[0] = 0;
    gSDCDLevelInfo[1] = 0;

    temp_t6 = p->field_14;
    temp_a0 = p->field_04;
    temp_v1 = p->field_00;
    temp_v0 = p->field_10;

    p->field_0C = temp_t6;

    temp_t7 = p->field_4D;
    p->field_4C = temp_t7;

    temp_t8 = p->field_4F;
    p->field_4E = temp_t8;

    p->field_10 = 0;
    p->field_14 = 0;
    p->field_4D = 0;
    p->field_4F = 0;

    p->field_18 = temp_v0;

    temp_v1 &= ~0x400;
    temp_v1 &= ~0x4000;
    temp_a0 &= ~0x200;
    temp_a0 &= ~0x100;

    p->field_08 = temp_v0;
    p->field_00 = temp_v1;
    p->field_04 = temp_a0;
}
