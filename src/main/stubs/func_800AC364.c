/* Address: 0x800AC364 */
/* Size: 908 bytes (227 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void GsTMDdivTNF3(void);
extern void func_800B5100(void);
extern void func_800B52D8(void);
extern void func_800B550C(void);

#if 0
s32 GsTMDdivTNF3(void* arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4, void* arg5, void* arg6) {
    s32 sp80;
    s32 sp84;
    s32 sp88;
    s32 spB8;
    s32 spBC;
    u16 spC0;
    u32 spC4;
    s32 var_v0;
    void* temp_s6;
    void* temp_s7;
    void* temp_s8;
    void* temp_t0;
    void* temp_t2;
    void* temp_t6;
    void* var_s0;

    temp_s6 = arg6 + 0x18;
    temp_s7 = arg6 + 0x30;
    temp_s8 = arg6 + 0x48;
    arg6->unkA8 = temp_s6;
    arg6->unkAC = temp_s7;
    arg6->unkB0 = temp_s8;
    var_s0 = arg0;
    spC4 = 0;
    if (arg3 != 0) {
        sp88 = temp_s6 + 0x10;
        sp80 = temp_s8 + 0x10;
        sp84 = temp_s7 + 0x10;
loop_2:
        temp_t2 = arg1 + (var_s0->unk14 * 8);
        temp_s6->unk3 = (s32) temp_t2->unk3;
        temp_s6->unk7 = (s32) temp_t2->unk7;
        temp_t6 = arg1 + (var_s0->unk16 * 8);
        temp_s7->unk3 = (s32) temp_t6->unk3;
        temp_s7->unk7 = (s32) temp_t6->unk7;
        temp_t0 = arg1 + (var_s0->unk18 * 8);
        temp_s8->unk3 = (s32) temp_t0->unk3;
        temp_s8->unk7 = (s32) temp_t0->unk7;
        if (!(var_s0->unk2 & 2)) {
            if ((s32 (*)(void*, void*, void*, s32, s32, s32, void*, u16*, void*))0xA550C(temp_s6, temp_s7, temp_s8, sp88, sp84, sp80, &spBC, &spC0, &spB8) > 0) {
                ((void (*)(void*, void*, void*))0xA5100)(temp_s6 + 0x14, temp_s7 + 0x14, temp_s8 + 0x14);
                arg6->unk14 = (s32) (arg5->unk4 + (((u32) (spC0 - arg5->unk8) >> arg4) * 4));
                arg6->unk10 = (s32) var_s0->unk10;
                arg6->unk13 = (s8) ((*(s32* )0x80131D80 * 2) | (var_s0->unk3 & 0xFE));
                arg6->unkC = (u16) var_s0->unk6;
                arg6->unkE = (u16) var_s0->unkA;
                temp_s6->unk8 = (s32) var_s0->unk4;
                temp_s7->unk8 = (s32) var_s0->unk8;
                temp_s8->unk8 = (s32) var_s0->unkC;
                var_v0 = ((s32 (*)(s32, void*, s32, void*))0xB2198)(arg2, arg6, 0, arg6 + 0x60);
                goto block_6;
            }
        } else {
            spC0 = ((u16 (*)(void*, void*, void*, s32, s32, s32, void*, void*))0xA52D8)(temp_s6, temp_s7, temp_s8, sp88, sp84, sp80, &spBC, &spB8);
            ((void (*)(void*, void*, void*))0xA5100)(temp_s6 + 0x14, temp_s7 + 0x14, temp_s8 + 0x14);
            arg6->unk14 = (s32) (arg5->unk4 + (((u32) (spC0 - arg5->unk8) >> arg4) * 4));
            arg6->unk10 = (s32) var_s0->unk10;
            arg6->unk13 = (s8) ((*(void* )0x80131D80 * 2) | (var_s0->unk3 & 0xFE));
            arg6->unkC = (u16) var_s0->unk6;
            arg6->unkE = (u16) var_s0->unkA;
            temp_s6->unk8 = (s32) var_s0->unk4;
            temp_s7->unk8 = (s32) var_s0->unk8;
            temp_s8->unk8 = (s32) var_s0->unkC;
            var_v0 = ((s32 (*)(s32, void*, s32, void*))0xB2198)(arg2, arg6, 0, arg6 + 0x60);
block_6:
            arg2 = var_v0;
        }
        spC4 += 1;
        var_s0 += 0x1C;
        if (spC4 < arg3) {
            goto loop_2;
        }
    }
    return arg2;
}
#endif

void GsTMDdivTNF3(void) {

}
