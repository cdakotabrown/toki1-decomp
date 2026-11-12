/* Address: 0x800AD79C */
/* Size: 776 bytes (194 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void GsTMDdivG4LFG(void);
extern void func_800B5114(void);
extern void func_800B5634(void);

#if 0
s32 GsTMDdivG4LFG(void* arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, void* arg6, void* arg7) {
    void* sp84;
    void* sp88;
    void* sp8C;
    s32 sp90;
    void* sp9C;
    void* spA0;
    void* spA4;
    s32 spC8;
    s32 spCC;
    u16 spD0;
    u32 spD4;
    u32 temp_v0;
    void* temp_s0;
    void* temp_s8;
    void* temp_t0;
    void* temp_t1;
    void* temp_t6;
    void* temp_t6_2;
    void* temp_t9;
    void* temp_t9_2;
    void* temp_v1;
    void* var_s1;

    temp_s8 = arg7 + 0x18;
    temp_v1 = arg7 + 0x30;
    temp_t0 = arg7 + 0x48;
    temp_t1 = arg7 + 0x60;
    arg7->unkF0 = temp_s8;
    arg7->unkF4 = temp_v1;
    arg7->unkF8 = temp_t0;
    arg7->unkFC = temp_t1;
    var_s1 = arg0;
    spD4 = 0;
    if (arg4 != 0) {
        spA4 = temp_v1;
        spA0 = temp_t0;
        sp9C = temp_t1;
        sp90 = temp_s8 + 0x10;
        sp8C = temp_v1 + 0x10;
        sp84 = temp_t1 + 0x10;
        sp88 = temp_t0 + 0x10;
        do {
            temp_t9 = arg1 + (var_s1->unkA * 8);
            temp_s8->unk3 = (s32) temp_t9->unk3;
            temp_s8->unk7 = (s32) temp_t9->unk7;
            temp_t6 = arg1 + (var_s1->unkE * 8);
            spA4->unk3 = (s32) temp_t6->unk3;
            spA4->unk7 = (s32) temp_t6->unk7;
            temp_t9_2 = arg1 + (var_s1->unk12 * 8);
            spA0->unk3 = (s32) temp_t9_2->unk3;
            spA0->unk7 = (s32) temp_t9_2->unk7;
            temp_t6_2 = arg1 + (var_s1->unk16 * 8);
            sp9C->unk3 = (s32) temp_t6_2->unk3;
            sp9C->unk7 = (s32) temp_t6_2->unk7;
            if ((s32 (*)(void*, void*, void*, void*, s32, void*, void*, void*, s32*, u16*, void*))0xA5634(temp_s8, spA4, spA0, sp9C, sp90, sp8C, sp88, sp84, &spCC, &spD0, &spC8) > 0) {
                ((void (*)(void*, void*, void*, void*))0xA5114)(temp_s8 + 0x14, spA4 + 0x14, spA0 + 0x14, sp9C + 0x14);
                temp_s0 = var_s1 + 4;
                ((void (*)(s32, s32, s32, void*, s32, void*, void*, void*))0xB17B4)(arg2 + (var_s1->unk8 * 8), arg2 + (var_s1->unkC * 8), arg2 + (var_s1->unk10 * 8), temp_s0, spCC, temp_s8 + 0xC, spA4 + 0xC, spA0 + 0xC);
                ((void (*)(s32, void*, s32, void*))0xB1790)(arg2 + (var_s1->unk14 * 8), temp_s0, spCC, sp9C + 0xC);
                arg7->unk14 = (s32) (arg6->unk4 + (((u32) (spD0 - arg6->unk8) >> arg5) * 4));
                arg7->unk13 = (s8) ((*(s32* )0x80131D80 * 2) | var_s1->unk3);
                arg3 = ((s32 (*)(s32, void*, s32, void*))0xB2A38)(arg3, arg7, 0, arg7 + 0x78);
            }
            temp_v0 = spD4 + 1;
            var_s1 += 0x18;
            spD4 = temp_v0;
        } while (temp_v0 < arg4);
    }
    return arg3;
}
#endif

void GsTMDdivG4LFG(void) {

}
