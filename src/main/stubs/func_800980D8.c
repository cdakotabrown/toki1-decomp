/* Address: 0x800980D8 */
/* Size: 816 bytes (204 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void _gcSPU(void);
extern void _SpuMallocSeparateTo3(void);
extern void func_800980D8(void);
extern void func_800A7770(void);
extern void func_800A7D74(void);
extern u8 _spu_rev_offsetaddr;
extern u8 _spu_AllocBlockNum;
extern u8 _spu_AllocLastNum;
extern u8 _SpuMemList;

extern void func_800980D8(void);
extern void func_800A7770(void);
extern void func_800A7D74(void);

extern s32 _spu_rev_offsetaddr;
extern s32 _spu_AllocBlockNum;
extern s32 _spu_AllocLastNum;
extern s32 _SpuMemList;

#if 0
s32 func_800980D8(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp1C;
    s32 sp24;
    s32 sp28;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a1;
    s32 temp_t1;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_t0;
    s32 var_t4;
    s32 var_t6;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    u32 temp_a3;
    void* temp_v1_2;
    void* var_v0_4;

        var_t4 = 0;
    } else {
        var_t4 = (0x10000 - *(s32*)&_spu_rev_offsetaddr) << arg2;
    }
    temp_a0 = ~temp_v1;
    var_v0_2 = 0;
    if (arg1 & temp_a0) {
        var_v0_2 = temp_v1;
    }
    temp_t1 = ((s32) (arg1 + var_v0_2) >> arg2) << arg2;
    if (arg0 & temp_a0) {
        var_v0_3 = temp_v1;
    } else {
        var_v0_3 = 0;
    }
    temp_a1 = ((s32) (arg0 + var_v0_3) >> arg2) << arg2;
    if (temp_a1 < 0x1010) {
        return -1;
    }
    temp_v0 = temp_a1 + temp_t1;
    if (((0x10000 << arg2) - var_t4) < temp_v0) {
        return -1;
    }
    temp_v1_2 = *(void**)&_SpuMemList;
    if (temp_v1_2->unk0 & 0x40000000) {
        temp_a0_2 = ((temp_v1_2->unk4 - temp_a1) - temp_t1) + 0x1010;
        if (temp_a1 >= 0x1011) {
            if ((*(s32*)&_spu_AllocBlockNum - 2) < *(s32*)&_spu_AllocLastNum) {
                return -1;
            }
            temp_v1_2->unk8 = temp_a1;
            temp_v1_2->unkC = temp_t1;
            temp_v1_2->unk10 = (s32) (temp_v0 | 0x40000000);
            temp_v1_2->unk0 = 0x80001010;
            temp_v1_2->unk4 = (s32) (temp_a1 - 0x1010);
            temp_v1_2->unk14 = temp_a0_2;
            *(void*)&_spu_AllocLastNum = 2;
            goto block_23;
        }
        if (*(void*)&_spu_AllocLastNum >= *(void*)&_spu_AllocBlockNum) {
            return -1;
        }
        temp_v1_2->unk0 = temp_a1;
        temp_v1_2->unk4 = temp_t1;
        temp_v1_2->unk8 = (s32) (temp_v0 | 0x40000000);
        temp_v1_2->unkC = temp_a0_2;
        *(void*)&_spu_AllocLastNum = 1;
block_23:
        sp28 = temp_a1;
        _gcSPU(temp_a0_2, temp_a1, 0x40000000, arg0);
        return temp_a1;
    }
    sp28 = temp_a1;
    arg1 = temp_t1;
    sp1C = var_t4;
    _gcSPU(temp_a0, temp_a1, 0x40000000, arg0);
    var_t0 = 0;
    var_v0_4 = *(void*)&_SpuMemList;
loop_25:
    temp_v1_3 = var_v0_4->unk0;
    if (temp_v1_3 & 0x80000000) {
        temp_a3 = temp_v1_3 & 0x0FFFFFFF;
        if ((u32) temp_a1 >= temp_a3) {
            temp_v1_4 = var_v0_4->unk4;
            var_t6 = temp_v1_4 - temp_a1;
            if ((u32) temp_a1 >= (u32) (temp_a3 + temp_v1_4)) {
                var_t6 = temp_v1_4 - temp_a1;
                if (!(var_v0_4->unk8 & 0x40000000)) {
                    var_t0 += 1;
                    var_v0_4 += 8;
                    goto loop_25;
                }
            }
            if ((var_t6 + temp_a3) < arg1) {
                var_t0 += 1;
                var_v0_4 += 8;
                goto loop_25;
            }
            var_v0 = _SpuMallocSeparateTo3(var_t0, temp_a1, arg1, temp_a3);
            goto block_46;
        }
        if (var_v0_4->unk4 < arg1) {
            var_t0 += 1;
            var_v0_4 += 8;
            goto loop_25;
        }
        var_v0 = _SpuMallocSeparateTo3(var_t0, temp_a3, arg1, temp_a3);
        goto block_46;
    }
    if (!(temp_v1_3 & 0x40000000)) {
        var_t0 += 1;
        var_v0_4 += 8;
        goto loop_25;
    }
    if ((var_v0_4->unk4 - var_t4) < arg1) {
        return -1;
    }
    var_v0 = _SpuMallocSeparateTo3(var_t0, temp_v1_3 & 0x0FFFFFFF, arg1);
block_46:
    sp24 = var_v0;
    _gcSPU();
    return var_v0;
}
#endif

void func_800980D8(void) {

}
