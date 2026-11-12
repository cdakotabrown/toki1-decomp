/* @SymbolSize: 544 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rt:v0:v1 0x14:rs:v0:v1 0x1c:rs:v0:v1 */
/* Address: 0x801339c4 */
/* Size: 544 bytes (136 insns) */
#include "types.h"

typedef struct {
    u8  pad0[36];
    s32 field36;
    s32 field40;
    u8  pad1[24];
} DispEnt;

extern DispEnt class_w[];
extern s32 pad_data;

void taiiku_pad_accel(s32 a0) {
    switch (pad_data & 0xF000) {
    case 0x3000:
        class_w[a0].field40 += -0x1E000;
        class_w[a0].field36 += 0x22000;
        break;
    case 0x9000:
        class_w[a0].field40 += -0x1E000;
        class_w[a0].field36 += -0x22000;
        break;
    case 0x6000:
        class_w[a0].field40 += 0x1E000;
        class_w[a0].field36 += 0x22000;
        break;
    case 0xC000:
        class_w[a0].field40 += 0x1E000;
        class_w[a0].field36 += -0x22000;
        break;
    case 0x1000:
        class_w[a0].field40 += -0x20000;
        break;
    case 0x4000:
        class_w[a0].field40 += 0x20000;
        break;
    case 0x8000:
        class_w[a0].field36 += -0x24000;
        break;
    case 0x2000:
        class_w[a0].field36 += 0x24000;
        break;
    }
}
