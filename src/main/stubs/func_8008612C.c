/* Address: 0x8008612C */
/* Size: 760 bytes (190 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void func_80059370(void);
extern void scroll_poly_disp(void);
extern void func_800B29A0(void);
extern void func_800B2A74(void);
extern void func_800B2BD0(void);
extern void func_800B2C98(void);
extern void func_800B2CD4(void);
extern void func_800B2DD8(void);

extern s32 screen_bri;

#if 0
void scroll_poly_disp(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 sp50;
    u32 sp60;
    s32 sp68;
    s32 var_s2;
    s32 var_s4;
    s32 var_s5;
    s8 temp_s8;
    s8 temp_t9;
    s8 temp_v1;
    void* temp_v0;

    sp50 = arg1 * 4;
    sp68 = arg0 * 2;
    var_s2 = 0;
    var_s4 = 0;
    var_s5 = 0;
    temp_s8 = arg2 + arg3;
    sp60 = (u32) arg4;
    do {
        temp_v0 = (void* (*)(s32, s32))0x49370(0x28, *(s32* )0x80127180);
        ((void (*)(void*))0xA2DD8)(temp_v0);
        ((void (*)(void*, s32))0xA2C98)(temp_v0, 0);
        ((void (*)(void*, s32))0xA2CD4)(temp_v0, 0);
        temp_v0->unk4 = (u8) *(u8* )0x800EE79A;
        temp_v0->unk5 = (u8) *(void* )0x800EE79A;
        temp_v0->unk6 = (u8) *(void* )0x800EE79A;
        temp_t9 = (sp68 + ((var_s4 % 2) << 6)) & 0x7F;
        temp_v1 = temp_t9 + 0x40;
        temp_v0->unk14 = temp_v1;
        temp_v0->unk24 = temp_v1;
        temp_v0->unkC = temp_t9;
        temp_v0->unk1C = temp_t9;
        temp_v0->unk25 = temp_s8;
        temp_v0->unk1D = temp_s8;
        temp_v0->unk15 = (s8) arg2;
        temp_v0->unkD = (s8) arg2;
        temp_v0->unk8 = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk14 + var_s2) - 0xA0);
        temp_v0->unkA = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk18 + arg2) - 0x78);
        temp_v0->unk10 = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk14 + var_s2) - 0x60);
        temp_v0->unk12 = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk18 + arg2) - 0x78);
        temp_v0->unk18 = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk14 + var_s2) - 0xA0);
        temp_v0->unk1A = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk18 + arg2 + arg3) - 0x78);
        temp_v0->unk20 = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk14 + var_s2) - 0x60);
        temp_v0->unk22 = (s16) ((((*(void* )0x80127180 * 2) + 0x800EE760)->unk18 + arg2 + arg3) - 0x78);
        temp_v0->unk16 = ((s16 (*)(s32, s32, s32, s32))0xA29A0)(1, 0, ((u32) (((u32) (var_s5 + arg0) >> 6) << 6) % sp60) + 0x140, 0);
        temp_v0->unkE = ((s16 (*)(s32, s32))0xA2A74)(0, 0x1F0);
        ((void (*)(s32, void*))0xA2BD0)((*(void* )0x80127180 << 0xA) + sp50 + 0x800F1180, temp_v0);
        var_s4 += 1;
        var_s5 += 0x20;
        var_s2 += 0x40;
    } while (var_s4 != 5);
}
#endif

void scroll_poly_disp(void) {

}
