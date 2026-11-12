/* Address: 0x800A0DF8 */
/* Size: 736 bytes (184 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void SetDrawEnv(void);
extern void func_800B10D8(void);
extern void func_800B117C(void);
extern void func_800B1258(void);
extern void func_800B1334(void);
extern void func_800B139C(void);

extern s32 D_800EBD64;
extern s32 D_800EBD66;

#if 0
void SetDrawEnv(void* arg0, void* arg1) {
    s32* sp2C;
    s16 sp34;
    s16 sp36;
    s16 sp38;
    s16 sp3A;
    s16 temp_a0;
    s16 temp_a0_2;
    s16 var_v0;
    s16 var_v0_2;
    s32 var_s1;
    s32* temp_v1;
    s32* temp_v1_2;
    s32* temp_v1_3;
    s32* temp_v1_4;
    s32* temp_v1_5;
    u32 temp_v0;

    arg0->unk4 = ((s32 (*)(s16, s16))0xA117C)(arg1->unk0, arg1->unk2);
    temp_v1 = arg0 + (2 * 4);
    *temp_v1 = ((s32 (*)(s32, s32))0xA1258)((arg1->unk4 + arg1->unk0) - 1, (arg1->unk2 + arg1->unk6) - 1);
    temp_v1_2 = temp_v1 + 4;
    sp2C = temp_v1_2;
    *temp_v1_2 = ((s32 (*)(s16, s16))0xA1334)(arg1->unk8, arg1->unkA);
    temp_v1_3 = temp_v1_2 + 4;
    sp2C = temp_v1_3;
    *temp_v1_3 = ((s32 (*)(u8, u8, u16))0xA10D8)(arg1->unk17, arg1->unk16, arg1->unk14);
    temp_v1_4 = temp_v1_3 + 4;
    sp2C = temp_v1_4;
    temp_v1_4->unk0 = ((s32 (*)(void*))0xA139C)(arg1 + 0xC);
    temp_v1_4->unk4 = 0xE6000000;
    temp_v1_5 = temp_v1_4 + 4 + 4;
    var_s1 = 7;
    if (arg1->unk18 != 0) {
        sp34 = arg1->unk0;
        sp36 = arg1->unk2;
        sp38 = arg1->unk4;
        sp3A = arg1->unk6;
        if (sp38 < 0) {
            sp38 = 0;
        } else {
            temp_a0 = *(s16* )0x800EBD64;
            var_v0 = sp38;
            if (temp_a0 < sp38) {
                var_v0 = temp_a0;
            }
            sp38 = var_v0;
        }
        if (sp3A < 0) {
            sp3A = 0;
        } else {
            temp_a0_2 = *(s16* )0x800EBD66;
            var_v0_2 = sp3A;
            if (temp_a0_2 < sp3A) {
                var_v0_2 = temp_a0_2;
            }
            sp3A = var_v0_2;
        }
        if (!(sp34 & 0x3F)) {
            if (sp38 & 0x3F) {
                goto block_18;
            }
            temp_v1_5->unk0 = (arg1->unk1B << 0x10) | 0x02000000 | (arg1->unk1A << 8) | arg1->unk19;
            temp_v1_5->unk4 = (s32) sp34;
            var_s1 = 0xA;
            (temp_v1_5 + 4 + 4 + 4)->unk-4 = (s32) sp38;
        } else {
block_18:
            sp34 -= arg1->unk8;
            sp36 -= arg1->unkA;
            temp_v1_5->unk0 = (arg1->unk1B << 0x10) | 0x60000000 | (arg1->unk1A << 8) | arg1->unk19;
            temp_v1_5->unk4 = (s32) sp34;
            (temp_v1_5 + 4)->unk4 = (s32) sp38;
            sp34 += arg1->unk8;
            var_s1 = 0xA;
            sp36 += arg1->unkA;
        }
    }
    temp_v0 = arg0->unk0;
    arg0->unk0 = (u32) (((((var_s1 - 1) & 0xFF) ^ (temp_v0 >> 0x18)) << 0x18) ^ temp_v0);
}
#endif

void SetDrawEnv(void) {

}
