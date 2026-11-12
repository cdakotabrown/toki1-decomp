/* Address: 0x80092FBC */
/* Size: 980 bytes (245 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void SetControlChange(void);
extern void func_8009EA64(void);
extern void func_8009FAD4(void);
extern void func_800A3390(void);
extern void func_800A347C(void);
extern void func_800A3568(void);
extern void func_800A36C8(void);
extern void func_800A3780(void);
extern void func_800A3890(void);
extern void func_800A3998(void);
extern void func_800A3A10(void);
extern void func_800A3A88(void);
extern void func_800A4A1C(void);
extern void func_800A5020(void);
extern void func_800A5030(void);

#if 0
void SetControlChange(s16 arg0, s16 arg1, s32 arg2) {
    void* sp1C;
    s16 sp28;
    u8 sp2C;
    u32 temp_a2;
    u8 temp_a3;
    u8 temp_v1;
    u8* temp_v0;
    void* temp_t0;

    temp_v0 = temp_t0->unk4;
    temp_a3 = temp_t0->unk12;
    temp_v1 = *temp_v0;
    temp_a2 = arg2 & 0xFF;
    temp_t0->unk4 = (u8* ) (temp_v0 + 1);
    switch (temp_a2) {
    case 0x0:
        temp_t0->unk4A = (s16) temp_v1;
        sp1C = temp_t0;
        return;
    case 0x1:
        return;
    case 0x5:
        return;
    case 0x6:
        return;
    case 0x7:
        (temp_t0 + (temp_a3 * 2))->unk4C = (s16) temp_v1;
        sp1C = temp_t0;
        return;
    case 0xA:
        (temp_t0 + temp_a3)->unk17 = temp_v1;
        sp1C = temp_t0;
        return;
    case 0xB:
        sp1C = temp_t0;
        sp28 = (s16) temp_a3;
        sp2C = temp_v1;
        (temp_t0 + (temp_a3 * 2))->unk4C = (s16) ((u32) ((s16 (*)(s16, u8, u32, u8))0x8EA64(temp_t0->unk4A, (temp_t0 + temp_a3)->unk2C, temp_a2, temp_a3) * temp_v1) / 127U);
        temp_t0->unk88 = ((s32 (*)(s16, s16))0x94A1C)(arg0, arg1);
        return;
    case 0x40:
        if (temp_v1 < 0x40U) {
            sp1C = temp_t0;
            ((void (*)(u32, u8))0x95030)(temp_a2, temp_a3);
        } else {
            sp1C = temp_t0;
            ((void (*)(u32, u8))0x95020)(temp_a2, temp_a3);
        }
        sp1C = temp_t0;
        temp_t0->unk88 = ((s32 (*)(s16, s16))0x94A1C)(arg0, arg1);
        return;
    case 0x41:
        return;
    case 0x5B:
        sp1C = temp_t0;
        ((void (*)(s16, s16, u32, u8))0x8FAD4)((s16) temp_v1, (s16) temp_v1, temp_a2, temp_a3);
        temp_t0->unk88 = ((s32 (*)(s16, s16))0x94A1C)(arg0, arg1);
        return;
    case 0x62:
        return;
    case 0x63:
        return;
    case 0x64:
        return;
    case 0x65:
        return;
    case 0x79:
        return;
    default:
        sp1C = temp_t0;
        return;
    }
}
#endif

void SetControlChange(void) {

}
