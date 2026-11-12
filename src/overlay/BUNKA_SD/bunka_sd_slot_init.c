/* @SymbolSize: 612 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_insn_reg_field 0x80:rt:s3:fp 0x84:rt:fp:s7 0x88:rt:s7:s6 0x8c:rt:s6:s5 0x90:rt:s5:s4 0x94:rt:s4:s3 0xa4:rt:s4:s3 0xa8:rt:s5:s4 0xc4:rt:v1:t9 0xcc:rs:v1:t9 0xcc:rt:v1:t9 0xd0:rt:v0:t0 0xdc:rt:v0:t0 0xe0:rt:s6:s5 0xe8:rt:s7:s6 0xec:rt:fp:s7 0xfc:rt:s3:fp 0x100:rt:v1:t9 0x104:rt:t9:t1 0x108:rs:t9:t1 0x108:rt:t9:t1 0x110:rt:t9:t1 0x128:rt:t0:t2 0x12c:rt:t0:t2 0x130:rt:t2:t4 0x134:rs:t2:t4 0x134:rt:t2:t4 0x13c:rt:t1:t3 0x140:rt:t1:t3 0x144:rs:t2:t4 0x154:rt:t3:t5 0x158:rs:t3:t5 0x158:rt:t3:t5 0x160:rs:t3:t5 0x16c:rt:t4:t6 0x170:rs:t4:t6 0x170:rt:t4:t6 0x178:rs:t4:t6 0x184:rt:t5:t7 0x188:rs:t5:t7 0x188:rt:t5:t7 0x190:rs:t5:t7 0x19c:rt:t6:t8 0x1a0:rs:t6:t8 0x1a0:rt:t6:t8 0x1a8:rs:t6:t8 0x1b4:rt:t7:t9 0x1b8:rs:t7:t9 0x1b8:rt:t7:t9 0x1c0:rs:t7:t9 0x1cc:rt:t8:t0 0x1d0:rs:t8:t0 0x1d0:rt:t8:t0 0x1d8:rs:t8:t0 0x1e4:rt:t9:t1 0x1e8:rs:t9:t1 0x1e8:rt:t9:t1 0x1f0:rs:t9:t1 0x1fc:rt:t0:t2 0x200:rs:t0:t2 0x200:rt:t0:t2 0x208:rs:t0:t2 0x214:rt:t1:t3 0x218:rs:t1:t3 0x218:rt:t1:t3 0x220:rs:t1:t3 0x22c:rt:t2:t4 0x230:rs:t2:t4 0x230:rt:t2:t4 0x238:rs:t2:t4 */
/* bunka_sd_slot_init - BUNKA_SD overlay, Address: 0x80135CD4, Size: 620 bytes (155 insns) */
#include "types.h"

extern void palette_upload_enqueue();
extern void class_clear_n(s32);
extern void load_tpage_buf_lock(s32, s32, s32);

extern s32 bunka_sd_slot_res_3;
extern s32 bunka_sd_slot_res_1;
extern s32 bunka_sd_slot_res_2;
extern s32 *bunka_sd_slot_res;
extern u8 class_w[][68];
extern s32 bunka_sd_kind;
extern s16 cal_cw_anim;
extern u8 cw99_disp;
extern u8 cw100_disp;
extern u8 cw101_anim_flag;
extern s16 cw101_frame;

void bunka_sd_slot_init(void)
{
    s32 i;
    u8 *sp0;
    s32 t9v;
    s32 t0v;
    s32 k6;
    s32 kn120;
    s32 kn160;
    s32 k1;
    s32 k16m;
    s32 k4;
    s32 k128;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_slot_res_3, (u8)(i + 16), 1, 1, i);
    }

    sp0 = class_w[0];
    i = 0;
    k6 = 6;
    kn120 = -120;
    kn160 = -160;
    k1 = 1;
    k16m = 0x01000000;
    k4 = 4;
    k128 = 128;
    do {
        class_clear_n(i + 96);
        sp0[6529] = k4;
        *(s32 *)&sp0[6584] = k16m;
        sp0[6531] = k128;
        *(s32 *)&sp0[6540] = bunka_sd_slot_res_1;
        *(s32 *)&sp0[6544] = bunka_sd_slot_res_2;
        t9v = (s32)bunka_sd_slot_res;
        t0v = i + 3;
        i += 1;
        sp0 += 68;
        *(s16 *)&sp0[6482] = t0v;
        sp0[6466] = k1;
        sp0[6467] = 0;
        *(s16 *)&sp0[6498] = kn160;
        *(s16 *)&sp0[6502] = kn120;
        sp0[6465] = k128;
        sp0[6527] = 0;
        sp0[6464] = 0;
        *(s32 *)&sp0[6512] = t9v;
    } while (i != k6);

    cal_cw_anim = bunka_sd_kind;
    cw99_disp = 0;
    cw100_disp = 0;
    cw101_anim_flag = 64;
    cw101_frame = 139;
    load_tpage_buf_lock(5, bunka_sd_slot_res[0], 1);
    load_tpage_buf_lock(6, bunka_sd_slot_res[1], 1);
    load_tpage_buf_lock(7, bunka_sd_slot_res[2], 1);
    load_tpage_buf_lock(8, bunka_sd_slot_res[3], 1);
    load_tpage_buf_lock(9, bunka_sd_slot_res[4], 1);
    load_tpage_buf_lock(10, bunka_sd_slot_res[5], 1);
    load_tpage_buf_lock(37, bunka_sd_slot_res[6], 1);
    load_tpage_buf_lock(38, bunka_sd_slot_res[7], 1);
    load_tpage_buf_lock(39, bunka_sd_slot_res[8], 1);
    load_tpage_buf_lock(40, bunka_sd_slot_res[9], 1);
    load_tpage_buf_lock(41, bunka_sd_slot_res[10], 1);
}
