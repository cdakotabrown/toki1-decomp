/* Address: 0x800AEEC0 */
/* Size: 824 bytes (206 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void GsTMDdivTG4L(void);
extern void func_800B5114(void);
extern void func_800B5634(void);

#if 0
s32 GsTMDdivTG4L(void* arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, void* arg6, void* arg7) {
    void* sp8C;
    s32 sp90;
    s32 sp94;
    s32 sp98;
    void* sp9C;
    void* spA0;
    s32 spCC;
    s32 spD0;
    s32 spD4;
    u16 spD8;
    u32 spDC;
    u32 temp_v0;
    void* temp_s6;
    void* temp_s7;
    void* temp_s8;
    void* temp_t4;
    void* temp_t5;
    void* temp_t6;
    void* temp_t8;
    void* temp_v1;
    void* var_s0;

    spCC = 0x3C808080;
    temp_s6 = arg7 + 0x18;
    temp_s7 = arg7 + 0x30;
    temp_s8 = arg7 + 0x48;
    temp_v1 = arg7 + 0x60;
    arg7->unkF0 = temp_s6;
    arg7->unkF4 = temp_s7;
    arg7->unkF8 = temp_s8;
    arg7->unkFC = temp_v1;
    var_s0 = arg0;
    spDC = 0;
    if (arg4 != 0) {
        spA0 = temp_v1;
        sp98 = temp_s6 + 0x10;
        sp94 = temp_s7 + 0x10;
        sp8C = temp_v1 + 0x10;
        sp90 = temp_s8 + 0x10;
        do {
            temp_t4 = arg1 + (var_s0->unk16 * 8);
            temp_s6->unk3 = (s32) temp_t4->unk3;
            temp_s6->unk7 = (s32) temp_t4->unk7;
            temp_t8 = arg1 + (var_s0->unk1A * 8);
            temp_s7->unk3 = (s32) temp_t8->unk3;
            temp_s7->unk7 = (s32) temp_t8->unk7;
            temp_t6 = arg1 + (var_s0->unk1E * 8);
            temp_s8->unk3 = (s32) temp_t6->unk3;
            temp_s8->unk7 = (s32) temp_t6->unk7;
            temp_t5 = arg1 + (var_s0->unk22 * 8);
            spA0->unk3 = (s32) temp_t5->unk3;
            spA0->unk7 = (s32) temp_t5->unk7;
            if ((s32 (*)(void*, void*, void*, void*, s32, s32, s32, void*, void*, u16*, void*))0xA5634(temp_s6, temp_s7, temp_s8, spA0, sp98, sp94, sp90, sp8C, &spD4, &spD8, &spD0) > 0) {
                sp9C = arg7 + 0x78;
                ((void (*)(void*, void*, void*, void*))0xA5114)(temp_s6 + 0x14, temp_s7 + 0x14, temp_s8 + 0x14, spA0 + 0x14);
                ((void (*)(s32, s32, s32, s32*, void*, void*, void*))0xB181C)(arg2 + (var_s0->unk14 * 8), arg2 + (var_s0->unk18 * 8), arg2 + (var_s0->unk1C * 8), &spCC, temp_s6 + 0xC, temp_s7 + 0xC, temp_s8 + 0xC);
                ((void (*)(s32, s32*, void*))0xB17FC)(arg2 + (var_s0->unk20 * 8), &spCC, spA0 + 0xC);
                arg7->unk14 = (s32) (arg6->unk4 + (((u32) (spD8 - arg6->unk8) >> arg5) * 4));
                arg7->unk13 = (s8) (((*(s32* )0x80131D80 * 2) | var_s0->unk3) & 0xFE);
                arg7->unkC = (u16) var_s0->unk6;
                arg7->unkE = (u16) var_s0->unkA;
                temp_s6->unk8 = (s32) var_s0->unk4;
                temp_s7->unk8 = (s32) var_s0->unk8;
                temp_s8->unk8 = (s32) var_s0->unkC;
                spA0->unk8 = (s32) var_s0->unk10;
                arg3 = ((s32 (*)(s32, void*, s32, void*))0xB34A8)(arg3, arg7, 0, sp9C);
            }
            temp_v0 = spDC + 1;
            var_s0 += 0x24;
            spDC = temp_v0;
        } while (temp_v0 < arg4);
    }
    return arg3;
}
#endif

void GsTMDdivTG4L(void) {

}
