/* Address: 0x8004E54C (true start 0x8004E534: the element-address head */
/* Size: 572 bytes (143 instructions); window 596 bytes (149) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8004E534 */
/* @SymbolSize: 596 */
/* @SymbolStart: 0x8004E534 / @SymbolSize: 620 and let the C's first */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=19:t9 IDO52_FORCE_WEB_RANGE_SPLIT=67,72 IDO52_SPLIT_BLOCKS=67#1:4,72#1:4 IDO52_FORCE_WEB_SPILL=67.1,72.1 IDO52_FORCE_WEB_NOSPILL=67.0:0.1,72.0:0.1 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t4:t3 0x90:rt:a0:t5 0x94:rt:t2:t6 0xa0:rt:a0:t5 0xa4:rt:t2:t6 0xdc:rd:t3:t2 0xe0:rt:t3:t2 0xe4:rs:t3:t2 0xe8:rt:t4:t3 0xec:rd:t5:t4 0xf8:rs:t5:t4 0x114:rs:t5:t4 0x138:rd:t6:t7 0x13c:rt:t6:t7 0x13c:rd:t7:t8 0x140:rt:t7:t8 0x144:rt:t8:t9 0x148:rt:t9:t2 0x14c:rs:t8:t9 0x158:rs:t9:t2 0x1f8:rt:t9:t2 0x208:rs:t9:t2 */
#include "types.h"

typedef struct {
    s16 field_00;
    s16 field_02;
    s16 field_04;
    s16 field_06;
    s32 field_08;
    u8 pad0C[2];
    u8 field_0E;
    u8 field_0F;
    u8 field_10;
    u8 field_11;
    u8 field_12;
    u8 field_13;
    u8 field_14;
    u8 field_15;
    u16 field_16;
    u16 field_18;
    u8 pad1A[2];
    s32 field_1C;
    u8 pad20[8];
} TileData;

extern TileData srn_w[];
extern u8 D_80129C3C;

extern void mod_set_gpu(s32, s32, s32, s32, s32, s32, s32);

s32 srn_set(idx)
{
    TileData *data;
    s32 divR, divC, colStep, rowStep, mul, yFact, cols, rows;
    s16 baseX, baseY;
    s32 totalRows;
    s32 *p;
    s32 count, row, col, ypos;
    s32 sprRef1, sprRef2;
    s32 colsPerRow, tile, xPos;

    data = &srn_w[idx];
    divR = data->field_0F;
    rowStep = data->field_11;
    divC = data->field_0E;
    colStep = data->field_10;
    mul = data->field_12;
    yFact = data->field_13;
    cols = data->field_14;
    rows = data->field_15;
    baseX = data->field_02;
    baseY = data->field_06;
    p = (s32 *)data->field_08;
    if (divR == 0 || divC == 0) {
        return -1;
    }
    sprRef1 = data->field_16;
    sprRef2 = data->field_18;
    count = data->field_1C;
    colsPerRow = (cols * colStep) / divC;
    totalRows = (rows * rowStep) / divR;
    for (row = 0; row < totalRows; row++) {
        if ((ypos = baseY + row * rowStep) + divR * yFact >= -119) {
            for (col = 0; col < colsPerRow; col++) {
                if (ypos >= 121) {
                    return 0;
                }
                if ((xPos = colStep * col + baseX) + mul * divC >= -159) {
                    tile = *p;
                    tile = (u16)tile;
                    if (tile != 0) {
                        mod_set_gpu(0, tile << 0, xPos, ypos, sprRef1, sprRef2, D_80129C3C);
                    }
                }
                count--;
                p++;
                if (count == 0) {
                    return 0;
                }
            }
        }
    }
}
