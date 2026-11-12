#include "types.h"

extern void SpuVmSetSeqVol(void);
extern void SpuVmGetSeqVol(s32 a0, void *a1, void *a2);

typedef struct Voice {
    u8 _pad[62];
    s16 field_62;
    u8 _pad2[56];
    s16 field_76;
    u8 _pad3[14];
    u32 field_144;
    u8 _pad4[4];
    u8 *field_152;
    u8 _pad5[0];
} Voice;

void func_800926F0(Voice *voice, u8 ***table_ptr, s16 outer_a0, s16 index_b) {
    u8 *entry;
    u32 flags;
    Voice *v;

    SpuVmSetSeqVol();

    entry = (*(u8 **)table_ptr) + (u32)index_b * 168;
    flags = *(u32 *)(entry + 144);
    *(u32 *)(entry + 144) = flags & ~0x20u;

    v = voice;
    if (v->field_152 != 0 && v->field_62 == 0) {

        entry = (*(u8 **)table_ptr) + (u32)outer_a0 * 168;
        flags = *(u32 *)(entry + 144);
        *(u32 *)(entry + 144) = flags & ~0x20u;
    }

    SpuVmGetSeqVol((s32)outer_a0,
                  (u8 *)v + 118,
                  (u8 *)v + 120);
}
