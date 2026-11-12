/* @PostProc: scheduler_normalizer */
/* name_ent_fall_obj_init - NAME_ENT overlay, Address: 0x80145408, Size: 280 bytes (70 insns) */
#include "types.h"

typedef struct {
    u8 pad0[1];
    u8 field_01;
    u8 field_02;
    u8 field_03;
    u8 pad1[1];
    u8 field_05;
    u8 pad2a[1];
    u8 field_07;
    u8 pad2b[4];
    s32 field_0C;
    s32 field_10;
    u8 pad3[2];
    s16 field_16;
    s16 field_18;
    u8 pad4[12];
    s16 field_26;
    u8 pad26b[2];
    s16 field_2A;
    u8 pad5[8];
    s32 field_34;
    s32 field_38;
    u8 pad6[8];
} ObjEntry;

extern void class_clear_n();
extern s32 rand();

extern ObjEntry class_w[];

void name_ent_fall_obj_init(void)
{
    s32 i;
    ObjEntry *ptr;
    s32 rnd;

    ptr = class_w;
    for (i = 0; i < 8; i++) {
        class_clear_n(i + 96);
        ptr[96].field_01 = 10;
        ptr[96].field_02 = 1;
        ptr[96].field_38 = 0x41000000;
        ptr[96].field_03 = 164;
        ptr[96].field_0C = 0x80184228;
        ptr[96].field_10 = 0x80184258;
        ptr[96].field_34 = 0x80184224;
        ptr[96].field_16 = 0;
        rnd = rand();
        ptr[96].field_18 = rnd & 0x7;
        ptr[96].field_05 = 128;
        rnd = rand();
        ptr[96].field_26 = (rnd % 440) - 160;
        rnd = rand();
        ptr[96].field_2A = -120 - (rnd % 120);
        rnd = rand();
        ptr++;
        ptr[95].field_07 = (u8)rnd;
    }
}
