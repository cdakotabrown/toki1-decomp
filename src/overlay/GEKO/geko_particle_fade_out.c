/* @PostProc: patch_insn_reg_field 0x34:rt:v1:a0 0x3c:rs:v1:a0 0x3c:rt:v1:a0 0x4c:rs:v1:a0 0x64:rt:v1:a0 0x68:rt:v1:a0 0x6c:rt:v1:a0 0x84:rs:v1:a0 0x94:rs:t9:s3 0x94:rt:s3:t9 */
/* geko_particle_fade_out -- GEKO overlay, Address: 0x80139C20, Size: 228 bytes (57 insns) */
#include "types.h"

extern void date_fade_out(void);
extern void device_cmd_send(s32 a0);

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
extern u8 now_sub_sub_step;

void geko_particle_fade_out(void)
{
    s32 i;
    u8 saved;
    u8 val;

    saved = now_sub_sub_step;
    date_fade_out();
    val = fade_bri;

    for (i = 4; i < 19; i = (s16)(i + 1)) {
        if ((u32)val < 96U) {
            Sprites[i].field_16 = val;
            Sprites[i].field_15 = val;
            Sprites[i].field_14 = val;
        }
    }

    if (saved != now_sub_sub_step) {
        for (i = 4; i < 19; i = (s16)(i + 1)) {
            Sprites[i].field_00 |= 0x80000000;
            device_cmd_send(5382);
        }
    }
}
