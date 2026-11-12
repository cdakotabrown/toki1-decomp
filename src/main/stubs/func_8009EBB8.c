/* Address: 0x8009EBB8 */
/* Size: 996 bytes (249 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void GsSetRefView(void);
extern void func_800AE130(void);
extern void func_800AE860(void);
extern void func_800AF354(void);
extern void func_800AF5C4(void);
extern void func_800B49A0(void);
extern void func_800B4E60(void);
extern void func_800B5F88(void);

#if 0
s32 GsSetRefView(void* arg0) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 temp_a1;
    s32 temp_lo;
    s32 temp_s0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_v0_8;
    s32 var_s0;
    s32 var_s1;
    s32 var_v0;

    (void* )0x80131CB0->unk8 = (s32) (void* )0x80131720->unk8;
    (void* )0x80131CB0->unk4 = (s32) (void* )0x80131720->unk4;
    (void* )0x80131CB0->unk0 = (s32) (void* )0x80131720->unk0;
    (void* )0x80131CB0->unk14 = (s32) (void* )0x80131720->unk14;
    (void* )0x80131CB0->unk10 = (s32) (void* )0x80131720->unk10;
    (void* )0x80131CB0->unkC = (s32) (void* )0x80131720->unkC;
    (void* )0x80131CB0->unk1C = (s32) (void* )0x80131720->unk1C;
    (void* )0x80131CB0->unk18 = (s32) (void* )0x80131720->unk18;
    ((void (*)(s32, s32))0x9E130)(0x80131CB0, -arg0->unk18);
    temp_v0 = arg0->unkC - arg0->unk0;
    temp_v0_2 = arg0->unk10 - arg0->unk4;
    temp_v0_3 = arg0->unk14 - arg0->unk8;
    temp_a1 = (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
    temp_v0_4 = ((s32 (*)(s32, s32))0xA4E60)(temp_a1 + (temp_v0_3 * temp_v0_3), temp_a1);
    var_v0 = 1;
    if (temp_v0_4 != 0) {
        temp_v0_5 = arg0->unkC - arg0->unk0;
        temp_lo = temp_v0_5 * temp_v0_5;
        temp_v0_6 = arg0->unk14 - arg0->unk8;
        temp_s0 = -((s32) ((arg0->unk4 - arg0->unk10) << 0xC) / temp_v0_4);
        temp_v0_7 = ((s32 (*)(s32, s32))0xA4E60)(temp_lo + (temp_v0_6 * temp_v0_6), temp_lo);
        ((void (*)(s32*, s16, s16, s32))0x9E860)(&sp10, (s16) temp_s0, (s16) ((s32) (temp_v0_7 << 0xC) / temp_v0_4), 0x78);
        ((void (*)(s32, s32*))0xA49A0)(0x80131CB0, &sp10);
        if (temp_v0_7 != 0) {
            ((void (*)(s32*, s16, s16, s32))0x9E860)(&sp10, (s16) (((s32) ((arg0->unkC - arg0->unk0) << 0xC) / temp_v0_7) * -1), (s16) ((s32) ((arg0->unk14 - arg0->unk8) << 0xC) / temp_v0_7), 0x79);
            ((void (*)(s32, s32*))0xA49A0)(0x80131CB0, &sp10);
        }
        sp30 = -arg0->unk0;
        sp34 = -arg0->unk4;
        sp38 = -arg0->unk8;
        ((void (*)(s32, s32*, void*))0xA5F88)(0x80131CB0, &sp30, (void* )0x80131CB0 + 0x14);
        temp_v0_8 = ((s32 (*)(s32))0x9F5C4)(arg0->unk1C);
        var_v0 = 0;
        if (temp_v0_8 != -1) {
            sp10 = (void* )0x8012DCD0->unk0;
            sp14 = (void* )0x8012DCD0->unk4;
            sp18 = (void* )0x8012DCD0->unk8;
            sp24 = (void* )0x8012DCD0->unk14;
            sp20 = (void* )0x8012DCD0->unk10;
            sp1C = (void* )0x8012DCD0->unkC;
            sp2C = (void* )0x8012DCD0->unk1C;
            sp28 = (void* )0x8012DCD0->unk18;
            var_s1 = 0;
            if (temp_v0_8 >= 0) {
                var_s0 = 0x801317A0;
                do {
                    ((void (*)(s32*, s32))0x9F354)(&sp10, var_s0);
                    var_s1 += 1;
                    var_s0 += 0x28;
                } while (temp_v0_8 >= var_s1);
            }
            ((void (*)(s32, s32*, s32*))0xA5F88)(0x80131CB0, &sp24, &sp30);
            *(void* )0x80131CC4 = (s32) (*(s32* )0x80131CC4 + sp30);
            *(void* )0x80131CC8 = (s32) (*(s32* )0x80131CC8 + sp34);
            *(void* )0x80131CCC = (s32) (*(s32* )0x80131CCC + sp38);
            ((void (*)(s32, s32*, s32, s32))0xA49A0)(0x80131CB0, &sp10, sp34, sp38);
            var_v0 = 0;
        }
    }
    return var_v0;
}
#endif

void GsSetRefView(void) {

}
