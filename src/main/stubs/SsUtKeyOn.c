/* Address: 0x8008EB8C */
/* Size: 936 bytes (234 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void SsUtKeyOn(void);
extern void func_8009C800(void);
extern void func_8009C9A8(void);
extern void func_8009CE94(void);
extern void func_8009D08C(void);
extern void func_8009D658(void);
extern void func_8009D788(void);
extern void func_800A1080(void);

#if 0
s16 SsUtKeyOn(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
    s16 sp30;
    s16 sp34;
    s16 temp_a0;
    s16 temp_a2;
    s16 temp_v0_2;
    void* temp_v0;
    void* temp_v0_3;
    void* temp_v0_4;
    void* temp_v1;

    temp_a2 = arg2;
    if (*(s32* )0x8013140C == 1) {
        return -1;
    }
    *(void* )0x8013140C = 1;
    arg2 = temp_a2;
    if ((s32 (*)(s16, s16, s16))0x91080(arg0, arg1, temp_a2) != 0) {
        *(void* )0x8013140C = 0;
        return -1;
    }
    *(s16* )0x80131606 = 0x20;
    *(s8* )0x801315F2 = (s8) arg3;
    *(s8* )0x801315F3 = (s8) arg4;
    *(s8* )0x801315FC = (s8) arg2;
    if (arg5 == arg6) {
        *(s8* )0x801315F5 = 0x40;
        *(s8* )0x801315F4 = (s8) arg5;
    } else if (arg6 < arg5) {
        *(void* )0x801315F5 = (s8) ((s32) (arg6 << 6) / arg5);
        *(void* )0x801315F4 = (s8) arg5;
    } else {
        *(void* )0x801315F5 = (s8) (0x7F - (s8) ((s32) (arg5 << 6) / arg6));
        *(void* )0x801315F4 = (s8) arg6;
    }
    temp_v1 = *(s32* )0x8013161C + (arg1 * 0x10);
    *(u8* )0x801315FA = temp_v1->unk1;
    *(u8* )0x801315FB = temp_v1->unk4;
    *(u8* )0x801315F0 = temp_v1->unk0;
    temp_a0 = (*(s8* )0x801315F7 * 0x10) + (s8) arg2;
    sp30 = temp_a0;
    temp_v0 = *(s32* )0x80131620 + (temp_a0 << 5);
    *(u8* )0x801315FF = temp_v0->unk0;
    *(s16* )0x80131608 = temp_v0->unk16;
    *(u8* )0x801315FD = temp_v0->unk2;
    *(u8* )0x801315FE = temp_v0->unk3;
    *(u8* )0x80131600 = temp_v0->unk4;
    *(u8* )0x80131601 = temp_v0->unk5;
    *(u8* )0x80131604 = temp_v0->unk1;
    *(u8* )0x80131602 = temp_v0->unk6;
    *(u8* )0x80131603 = temp_v0->unk7;
    if (*(void* )0x80131608 == 0) {
        *(void* )0x8013140C = 0;
        return -1;
    }
    temp_v0_2 = ((s16 (*)(s16))0x8C800)(*(void* )0x80131608);
    if (*(s8* )0x801315E4 == temp_v0_2) {
        *(void* )0x8013140C = 0;
        return -1;
    }
    *(s16* )0x8013160A = temp_v0_2;
    temp_v0_3 = (temp_v0_2 * 0x18) + 0x800EB8E8;
    temp_v0_3->unkA = 0x20;
    temp_v0_3->unk12 = arg0;
    temp_v0_3->unkC = (s16) *(void* )0x801315F7;
    temp_v0_3->unkE = arg1;
    temp_v0_3->unk0 = (s16) *(void* )0x80131608;
    temp_v0_3->unk10 = (s16) *(void* )0x801315FC;
    temp_v0_3->unk2 = 0;
    temp_v0_3->unk17 = 1;
    temp_v0_3->unk8 = arg3;
    sp34 = temp_v0_2;
    ((void (*)())0x8CE94)();
    if (*(void* )0x80131608 == 0xFF) {
        ((void (*)(u8))0x8D08C)((u8) sp34);
    } else {
        ((void (*)(s32, s32))0x8C9A8)(1, ((s32 (*)(u16, u16))0x8D658)((u16) arg3, (u16) arg4));
    }
    if ((s8) *(void* )0x80131604 & 1) {
        temp_v0_4 = *(void* )0x80131620 + (sp30 << 5);
        ((void (*)(s16, u8, u8))0x8D788)(sp34, temp_v0_4->unk8, temp_v0_4->unk9);
    }
    *(void* )0x8013140C = 0;
    return sp34;
}
#endif

void SsUtKeyOn(void) {

}
