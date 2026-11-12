/* Address: 0x8008F054 */
/* Size: 832 bytes (208 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void SsUtKeyOnV(void);
extern void func_8009C9A8(void);
extern void func_8009CE94(void);
extern void func_8009D08C(void);
extern void func_8009D658(void);
extern void func_800A1080(void);

#if 0
s16 SsUtKeyOnV(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7) {
    s16 temp_a2;
    s16 temp_a3;
    void* temp_v0;
    void* temp_v0_2;
    void* temp_v1;

    temp_a3 = arg3;
    temp_a2 = arg2;
        return -1;
    }
    *(s32* )0x8013140C = 1;
    if ((arg0 < 0) || (arg0 >= 0x18)) {
        *(void* )0x8013140C = 0;
        return -1;
    }
    arg0 = arg0;
    arg2 = temp_a2;
    arg3 = temp_a3;
    if ((s32 (*)(s16, s16, s16, s16))0x91080(arg1, temp_a2, temp_a2, temp_a3) != 0) {
        *(void* )0x8013140C = 0;
        return -1;
    }
    *(s16* )0x80131606 = 0x20;
    *(s8* )0x801315F2 = (s8) arg4;
    *(s8* )0x801315F3 = (s8) arg5;
    *(s8* )0x801315FC = (s8) arg3;
    if (arg6 == arg7) {
        *(s8* )0x801315F5 = 0x40;
        *(s8* )0x801315F4 = (s8) arg6;
    } else if (arg7 < arg6) {
        *(void* )0x801315F5 = (s8) ((s32) (arg7 << 6) / arg6);
        *(void* )0x801315F4 = (s8) arg6;
    } else {
        *(void* )0x801315F5 = (s8) (0x7F - (s8) ((s32) (arg6 << 6) / arg7));
        *(void* )0x801315F4 = (s8) arg7;
    }
    temp_v1 = *(s32* )0x8013161C + (arg2 * 0x10);
    *(u8* )0x801315FA = temp_v1->unk1;
    *(u8* )0x801315FB = temp_v1->unk4;
    *(u8* )0x801315F0 = temp_v1->unk0;
    temp_v0 = *(s32* )0x80131620 + ((s16) ((*(s8* )0x801315F7 * 0x10) + (s8) arg3) << 5);
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
    temp_v0_2 = (arg0 * 0x18) + 0x800EB8E8;
    *(s16* )0x8013160A = arg0;
    temp_v0_2->unkA = 0x20;
    temp_v0_2->unk12 = arg1;
    temp_v0_2->unkE = arg2;
    temp_v0_2->unkC = (s16) *(void* )0x801315F7;
    temp_v0_2->unk2 = 0;
    temp_v0_2->unk8 = arg4;
    temp_v0_2->unk17 = 1;
    temp_v0_2->unk10 = (s16) (s8) arg3;
    temp_v0_2->unk0 = (s16) *(void* )0x80131608;
    ((void (*)(s16))0x8CE94)(arg4);
    if (*(void* )0x80131608 == 0xFF) {
        ((void (*)(u8))0x8D08C)((u8) arg0);
    } else {
        ((void (*)(s32, s32))0x8C9A8)(1, ((s32 (*)(u16, u16))0x8D658)((u16) arg4, (u16) arg5));
    }
    *(void* )0x8013140C = 0;
    return arg0;
}
#endif

void SsUtKeyOnV(void) {

}
