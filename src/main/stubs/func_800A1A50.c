/* Address: 0x800A1A50 */
/* Size: 712 bytes (178 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void _drs(void);
extern void set_alarm(void);
extern void func_800B2880(void);
extern void func_800B28B0(void);
extern u8 libgpu_gp0_reg_ptr;
extern u8 GPU_status;
extern u8 libgpu_dma2_madr_ptr;
extern u8 libgpu_dma2_bcr_ptr;
extern u8 libgpu_dma2_chcr_ptr;
extern u8 D_800EBD64;
extern u8 D_800EBD66;

#if 0
s32 _drs(void* arg0, void* arg1) {
    s32 sp3C;
    s16 sp40;
    s16 sp42;
    s16 sp44;
    s16 sp46;
    s16 temp_v0;
    s16 temp_v1;
    s16 var_v0;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_v0_2;
    s32 temp_v1_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_v0_2;
    s32* temp_a1;
    void* var_s2;

    var_s2 = arg1;
    set_alarm();
    sp40 = arg0->unk0;
    sp42 = arg0->unk2;
    sp44 = arg0->unk4;
    sp46 = arg0->unk6;
    if (sp44 < 0) {
        sp44 = 0;
    } else {
        temp_v0 = *(s16*)&D_800EBD64;
        var_v1 = sp44;
        if (temp_v0 < sp44) {
            var_v1 = temp_v0;
        }
        sp44 = var_v1;
    }
    if (sp46 < 0) {
        sp46 = 0;
    } else {
        temp_v1 = *(s16*)&D_800EBD66;
        var_v0 = sp46;
        if (temp_v1 < sp46) {
            var_v0 = temp_v1;
        }
        sp46 = var_v0;
    }
    temp_t7 = (sp44 * sp46) + 1;
    var_at = temp_t7;
    if (temp_t7 < 0) {
        var_at += 1;
    }
    temp_t8 = var_at >> 1;
    sp3C = temp_t8;
    if (temp_t8 <= 0) {
        return -1;
    }
    if (!(**(s32**)&GPU_status & 0x04000000)) {
loop_19:
        if ((s32 (*)())0xA28B0() != 0) {
            return -1;
        }
        if (**(void*)&GPU_status & 0x04000000) {
            goto block_22;
        }
        goto loop_19;
    }
block_22:
    temp_a1 = *(s32**)&libgpu_gp0_reg_ptr;
    **(void*)&GPU_status = 0x04000000;
    *temp_a1 = 0x01000000;
    *temp_a1 = 0xC0000000;
    *temp_a1 = (s32) sp40;
    *temp_a1 = (s32) sp44;
    if (!(**(void*)&GPU_status & 0x08000000)) {
loop_23:
        if ((s32 (*)())0xA28B0() != 0) {
            return -1;
        }
        if (**(void*)&GPU_status & 0x08000000) {
            goto block_27;
        }
        goto loop_23;
    }
block_27:
    temp_a0 = sp3C % 16;
    var_v0_2 = temp_a0 - 1;
    if (temp_a0 != 0) {
        do {
            temp_v1_2 = var_v0_2;
            var_v0_2 -= 1;
            var_s2 += 4;
            var_s2->unk-4 = (s32) **(void*)&libgpu_gp0_reg_ptr;
        } while (temp_v1_2 != 0);
    }
    var_at_2 = sp3C;
    if (sp3C < 0) {
        var_at_2 += 0xF;
    }
    temp_v0_2 = var_at_2 >> 4;
    if (temp_v0_2 != 0) {
        **(void*)&GPU_status = 0x04000003;
        **(void***)&libgpu_dma2_madr_ptr = var_s2;
        **(s32**)&libgpu_dma2_bcr_ptr = (temp_v0_2 << 0x10) | 0x10;
        **(s32**)&libgpu_dma2_chcr_ptr = 0x01000200;
    }
    return 0;
}
#endif

void _drs(void) {

}
