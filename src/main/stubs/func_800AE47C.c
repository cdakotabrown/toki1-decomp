/* Address: 0x800AE47C */
/* Size: 720 bytes (180 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void GsTMDdivTF4LFG(void);
extern void func_800B5114(void);
extern void func_800B5634(void);

#if 0
s32 GsTMDdivTF4LFG(void* arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, void* arg6, void* arg7) {
    s32 sp70;
    s32 sp74;
    s32 sp78;
    s32 sp7C;
    s32 spB4;
    s32 spB8;
    s32 spBC;
    u16 spC0;
    u32 spC4;
    u32 temp_v0;
    void* temp_s4;
    void* temp_s5;
    void* temp_s6;
    void* temp_s7;
    void* temp_t2;
    void* temp_t3;
    void* temp_t6;
    void* temp_t7;
    void* var_s0;

    spB4 = 0x2C808080;
    temp_s4 = arg7 + 0x18;
    temp_s5 = arg7 + 0x30;
    temp_s6 = arg7 + 0x48;
    temp_s7 = arg7 + 0x60;
    arg7->unkF0 = temp_s4;
    arg7->unkF4 = temp_s5;
    arg7->unkF8 = temp_s6;
    arg7->unkFC = temp_s7;
    var_s0 = arg0;
    spC4 = 0;
    if (arg4 != 0) {
        sp7C = temp_s4 + 0x10;
        sp78 = temp_s5 + 0x10;
        sp70 = temp_s7 + 0x10;
        sp74 = temp_s6 + 0x10;
        do {
            temp_t2 = arg1 + (var_s0->unk16 * 8);
            temp_s4->unk3 = (s32) temp_t2->unk3;
            temp_s4->unk7 = (s32) temp_t2->unk7;
            temp_t6 = arg1 + (var_s0->unk18 * 8);
            temp_s5->unk3 = (s32) temp_t6->unk3;
            temp_s5->unk7 = (s32) temp_t6->unk7;
            temp_t3 = arg1 + (var_s0->unk1A * 8);
            temp_s6->unk3 = (s32) temp_t3->unk3;
            temp_s6->unk7 = (s32) temp_t3->unk7;
            temp_t7 = arg1 + (var_s0->unk1C * 8);
            temp_s7->unk3 = (s32) temp_t7->unk3;
            temp_s7->unk7 = (s32) temp_t7->unk7;
            if ((s32 (*)(void*, void*, void*, void*, s32, s32, s32, s32, s32*, u16*, void*))0xA5634(temp_s4, temp_s5, temp_s6, temp_s7, sp7C, sp78, sp74, sp70, &spBC, &spC0, &spB8) > 0) {
                ((void (*)(void*, void*, void*, void*))0xA5114)(temp_s4 + 0x14, temp_s5 + 0x14, temp_s6 + 0x14, temp_s7 + 0x14);
                ((void (*)(s32, s32*, s32, void*))0xB1790)(arg2 + (var_s0->unk14 * 8), &spB4, spBC, arg7 + 0x10);
                arg7->unk14 = (s32) (arg6->unk4 + (((u32) (spC0 - arg6->unk8) >> arg5) * 4));
                arg7->unk13 = (s8) ((*(s32* )0x80131D80 * 2) | var_s0->unk3);
                arg7->unkC = (u16) var_s0->unk6;
                arg7->unkE = (u16) var_s0->unkA;
                temp_s4->unk8 = (s32) var_s0->unk4;
                temp_s5->unk8 = (s32) var_s0->unk8;
                temp_s6->unk8 = (s32) var_s0->unkC;
                temp_s7->unk8 = (s32) var_s0->unk10;
                arg3 = ((s32 (*)(s32, void*, s32, void*))0xB2F88)(arg3, arg7, 0, arg7 + 0x78);
            }
            temp_v0 = spC4 + 1;
            var_s0 += 0x20;
            spC4 = temp_v0;
        } while (temp_v0 < arg4);
    }
    return arg3;
}
#endif

void GsTMDdivTF4LFG(void) {

}
