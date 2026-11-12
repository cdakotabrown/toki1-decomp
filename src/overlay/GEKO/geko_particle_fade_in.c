/* @SymbolStart: 0x80139D04 */
/* @SymbolSize: 200 */
/* @PostProc: patch_insn_reg_field 0x24:rt:a1:a2 0x28:rt:a0:v1 0x2c:rs:a0:v1 0x2c:rt:a0:v1 0x30:rs:a1:a2 0x30:rt:a1:a2 0x34:rt:v1:a0 0x38:rt:a2:a3 0x3c:rs:v1:a0 0x3c:rt:a2:a3 0x40:rs:v1:a0 0x40:rt:v1:a0 0x44:rt:v1:a0 0x48:rd:v1:a0 0x4c:rs:v1:a0 0x54:rs:a1:a2 0x60:rt:a0:v1 0x74:rt:a0:a1 0x78:rt:a1:a2 0x7c:rs:a0:a1 0x7c:rt:a0:a1 0x80:rs:a1:a2 0x80:rt:a1:a2 0x84:rt:a2:a3 0x88:rt:v1:a0 0x8c:rs:a0:a1 0x98:rs:v1:a0 0x98:rt:a2:a3 0xa0:rs:a1:a2 0xa4:rt:a0:a1 0xa8:rt:a0:a1 0xac:rt:a0:a1 0xb0:rs:v1:a0 0xb0:rt:v1:a0 0xb4:rt:v1:a0 0xb8:rd:v1:a0 0xbc:rs:v1:a0 0xc4:rs:a0:a1 */
/* geko_particle_fade_in -- GEKO overlay, Address: 0x80139D0C, Size: 192 bytes (47 insns) */
#include "types.h"

extern void date_fade_in(void);

typedef struct {
    s32 field_00;
    u8  pad_04[16];
    u8  field_14;
    u8  field_15;
    u8  field_16;
    u8  pad_17[13];
} GekoElem;

extern GekoElem Sprites[];
extern u8 fade_bri;
extern s32 step_counter;

void geko_particle_fade_in(void)
{
    s32 t6 = step_counter;

    s32 i;
    u8 val;

    if (t6 == 0) {
        for (i = 4; i < 19; i = (s16)(i + 1)) {
            Sprites[i].field_00 &= 0x7FFFFFFF;
        }
    }
    date_fade_in();

    val = fade_bri;
    i = 4;
    do {
        if ((u32)val < 97U) {
            Sprites[i].field_16 = val;
            Sprites[i].field_15 = val;
            Sprites[i].field_14 = val;
        }
        i = (s16)(i + 1);
    } while (i < 19);
}
