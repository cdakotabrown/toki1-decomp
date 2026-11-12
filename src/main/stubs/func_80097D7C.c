/* Address: 0x80097D7C */
/* Size: 840 bytes (210 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void _SpuMallocSeparateTo3(void);
extern void func_800A7770(void);

extern s32 _spu_rev_reserve_wa;
extern s32 _spu_rev_offsetaddr;
extern s32 _spu_AllocBlockNum;
extern s32 _spu_AllocLastNum;
extern s32 spu_mem_mode;

#if 0
s32 _SpuMallocSeparateTo3(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_a3;
    s32 temp_t4;
    s32 temp_t9;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_a0;
    s32 var_a1;
    s32 var_t0;
    s32 var_t2;
    s32 var_t6;
    s32 var_v0;
    s32 var_v1;
    s32 var_v1_2;
    void* temp_t1;
    void* temp_v0;
    void* temp_v0_2;
    void* temp_v0_3;
    void* temp_v0_4;
    void* var_v0_2;

    temp_t4 = temp_t1->unk0;
    temp_a3 = temp_t1->unk4;
    if (*(s32* )0x800E3258 == 0) {
        var_v0 = 0;
    } else {
        var_v0 = (0x10000 - *(s32* )0x800E3260) << *(s32* )0x800EBD30;
    }
    temp_v1 = temp_t4 & 0x40000000;
    if (temp_v1 != 0) {
        var_t2 = temp_t4 & 0x0FFFFFFF;
        var_a0 = arg1 - var_t2;
        var_t0 = temp_a3 - var_a0;
        var_a1 = var_t0 - var_v0;
    } else {
        var_t2 = temp_t4 & 0x0FFFFFFF;
        var_a0 = arg1 - var_t2;
        var_t0 = temp_a3 - var_a0;
        var_a1 = var_t0;
    }
    if (var_a1 < arg2) {
        return -1;
    }
    if (temp_v1 != 0) {
        if (var_a0 > 0) {
            temp_v1_2 = *(s32* )0x800E3688;
            if ((*(s32* )0x800E3684 - 2) < temp_v1_2) {
                return -1;
            }
            temp_v0->unk0 = (s32) (temp_v0->unk0 & 0x0FFFFFFF);
            temp_v0->unk0 = (s32) (temp_v0->unk0 | 0x80000000);
            temp_v0->unk4 = var_a0;
            var_v1 = temp_v1_2 + 1;
            var_v0_2->unk4 = arg2;
            var_v0_2->unk0 = arg1;
            goto block_18;
        }
        var_v1 = *(void* )0x800E3688;
        if (var_v1 >= *(void* )0x800E3684) {
            return -1;
        }
        var_v0_2->unk0 = (s32) (var_v0_2->unk0 & 0x0FFFFFFF);
        var_v0_2->unk4 = arg2;
block_18:
        var_v1_2 = var_v1 + 1;
        var_v0_2->unk8 = (s32) ((var_t2 + var_a0 + arg2) | 0x40000000);
        var_v0_2->unkC = (s32) (var_t0 - arg2);
        goto block_37;
    }
    if (var_a0 > 0) {
        if (var_a1 == arg2) {
            temp_v1_3 = *(void* )0x800E3688;
            var_t6 = temp_v1_3 * 8;
            if ((*(void* )0x800E3684 - 2) < temp_v1_3) {
                return -1;
            }
            goto block_26;
        }
        temp_v1_4 = *(void* )0x800E3688;
        var_t6 = temp_v1_4 * 8;
        if (temp_v1_4 >= *(void* )0x800E3684) {
            return -1;
        }
block_26:
        temp_t1->unk4 = var_a0;
        temp_v0_2->unk8 = (s32) temp_v0_2->unk0;
        temp_v0_2->unkC = (s32) temp_v0_2->unk4;
        temp_v0_2->unk4 = arg2;
        temp_v0_3 = temp_v0_2 + 8;
        var_v1_2 = *(void* )0x800E3688 + 1;
        temp_v0_3->unk-8 = arg1;
        if (var_a1 < arg2) {

        } else {
            temp_v0_3->unk8 = (s32) temp_v0_3->unk0;
            temp_v0_3->unkC = (s32) temp_v0_3->unk4;
            temp_v0_3->unk0 = (s32) ((var_t2 + var_a0 + arg2) | 0x80000000);
            temp_v0_3->unk4 = (s32) (var_a1 - arg2);
            var_v1_2 += 1;
        }
        goto block_37;
    }
    if (arg2 >= var_a1) {
        goto block_35;
    }
    if (*(void* )0x800E3688 >= *(void* )0x800E3684) {
        return -1;
    }
block_35:
    temp_t1->unk0 = (s32) (temp_t4 & 0x0FFFFFFF);
    temp_t1->unk4 = arg2;
    if (arg2 < var_a1) {
        temp_v0_4->unk8 = (s32) temp_v0_4->unk0;
        temp_t9 = temp_v0_4->unk4;
        temp_v0_4->unk0 = (s32) ((var_t2 + arg2) | 0x80000000);
        temp_v0_4->unk4 = (s32) (var_a1 - arg2);
        var_v1_2 = *(void* )0x800E3688 + 1;
        temp_v0_4->unkC = temp_t9;
block_37:
        *(void* )0x800E3688 = var_v1_2;
    }
    ((void (*)(s32, s32, s32))0x97770)(var_a0, var_a1, temp_a3);
    return arg1;
}
#endif

void _SpuMallocSeparateTo3(void) {

}
