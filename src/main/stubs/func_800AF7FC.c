/* Address: 0x800AF7FC */
/* Size: 716 bytes (179 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void GsTMDdivTNG4(void);
extern void func_800B5114(void);
extern void func_800B5634(void);

#if 0
s32 GsTMDdivTNG4(void* arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4, void* arg5, void* arg6) {
    s32 sp6C;
    s32 sp70;
    s32 sp74;
    s32 sp78;
    s32 spB0;
    s32 spB4;
    u16 spB8;
    u32 var_s8;
    void* temp_s3;
    void* temp_s4;
    void* temp_s5;
    void* temp_s6;
    void* temp_t2;
    void* temp_t5;
    void* temp_t6;
    void* temp_t9;
    void* var_s0;

    temp_s3 = arg6 + 0x18;
    temp_s4 = arg6 + 0x30;
    temp_s5 = arg6 + 0x48;
    temp_s6 = arg6 + 0x60;
    arg6->unkF0 = temp_s3;
    arg6->unkF4 = temp_s4;
    arg6->unkF8 = temp_s5;
    arg6->unkFC = temp_s6;
    var_s0 = arg0;
    var_s8 = 0;
    if (arg3 != 0) {
        sp78 = temp_s3 + 0x10;
        sp74 = temp_s4 + 0x10;
        sp6C = temp_s6 + 0x10;
        sp70 = temp_s5 + 0x10;
        do {
            temp_t9 = arg1 + (var_s0->unk24 * 8);
            temp_s3->unk3 = (s32) temp_t9->unk3;
            temp_s3->unk7 = (s32) temp_t9->unk7;
            temp_t5 = arg1 + (var_s0->unk26 * 8);
            temp_s4->unk3 = (s32) temp_t5->unk3;
            temp_s4->unk7 = (s32) temp_t5->unk7;
            temp_t2 = arg1 + (var_s0->unk28 * 8);
            temp_s5->unk3 = (s32) temp_t2->unk3;
            temp_s5->unk7 = (s32) temp_t2->unk7;
            temp_t6 = arg1 + (var_s0->unk2A * 8);
            temp_s6->unk3 = (s32) temp_t6->unk3;
            temp_s6->unk7 = (s32) temp_t6->unk7;
            if ((s32 (*)(void*, void*, void*, void*, s32, s32, s32, s32, void*, u16*, void*))0xA5634(temp_s3, temp_s4, temp_s5, temp_s6, sp78, sp74, sp70, sp6C, &spB4, &spB8, &spB0) > 0) {
                ((void (*)(void*, void*, void*, void*))0xA5114)(temp_s3 + 0x14, temp_s4 + 0x14, temp_s5 + 0x14, temp_s6 + 0x14);
                arg6->unk14 = (s32) (arg5->unk4 + (((u32) (spB8 - arg5->unk8) >> arg4) * 4));
                arg6->unk13 = (s8) ((*(s32* )0x80131D80 * 2) | (var_s0->unk3 & 0xFE));
                arg6->unkC = (u16) var_s0->unk6;
                arg6->unkE = (u16) var_s0->unkA;
                temp_s3->unkC = (s32) var_s0->unk14;
                temp_s4->unkC = (s32) var_s0->unk18;
                temp_s5->unkC = (s32) var_s0->unk1C;
                temp_s6->unkC = (s32) var_s0->unk20;
                temp_s3->unk8 = (s32) var_s0->unk4;
                temp_s4->unk8 = (s32) var_s0->unk8;
                temp_s5->unk8 = (s32) var_s0->unkC;
                temp_s6->unk8 = (s32) var_s0->unk10;
                arg2 = ((s32 (*)(s32, void*, s32, void*))0xB34A8)(arg2, arg6, 0, arg6 + 0x78);
            }
            var_s8 += 1;
            var_s0 += 0x2C;
        } while (var_s8 < arg3);
    }
    return arg2;
}
#endif

void GsTMDdivTNG4(void) {

}
