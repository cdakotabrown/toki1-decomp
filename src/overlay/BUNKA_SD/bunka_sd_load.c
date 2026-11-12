/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_insn_reg_field 0xc8:rt:v0:t9 0xd0:rs:v0:t9 0xd0:rt:v0:t9 0x104:rt:v0:t9 0x108:rt:t9:t0 0x10c:rs:t9:t0 0x10c:rt:t9:t0 0x114:rs:t9:t0 0x120:rt:t0:t1 0x124:rs:t0:t1 0x124:rt:t0:t1 0x12c:rs:t0:t1 0x138:rt:t1:t2 0x13c:rs:t1:t2 0x13c:rt:t1:t2 0x144:rs:t1:t2 0x150:rt:t2:t3 0x154:rs:t2:t3 0x154:rt:t2:t3 0x15c:rs:t2:t3 0x168:rt:t3:t4 0x16c:rs:t3:t4 0x16c:rt:t3:t4 0x174:rs:t3:t4 0x180:rt:t4:t5 0x184:rs:t4:t5 0x184:rt:t4:t5 0x18c:rs:t4:t5 0x198:rt:t5:t6 0x19c:rs:t5:t6 0x19c:rt:t5:t6 0x1a4:rs:t5:t6 0x1b0:rt:t6:t7 0x1b4:rs:t6:t7 0x1b4:rt:t6:t7 0x1bc:rs:t6:t7 0x1c8:rt:t7:t8 0x1cc:rs:t7:t8 0x1cc:rt:t7:t8 0x1d4:rs:t7:t8 0x1e0:rt:t8:t9 0x1e4:rs:t8:t9 0x1e4:rt:t8:t9 0x1ec:rs:t8:t9 0x1f8:rt:t9:t0 0x1fc:rs:t9:t0 0x1fc:rt:t9:t0 0x204:rs:t9:t0 0x210:rt:t0:t1 0x214:rs:t0:t1 0x214:rt:t0:t1 0x21c:rs:t0:t1 0x228:rt:t1:t2 0x22c:rs:t1:t2 0x22c:rt:t1:t2 0x234:rs:t1:t2 0x240:rt:t2:t3 0x244:rs:t2:t3 0x244:rt:t2:t3 0x24c:rs:t2:t3 0x258:rt:t3:t4 0x25c:rs:t3:t4 0x25c:rt:t3:t4 0x264:rs:t3:t4 0x270:rt:t4:t5 0x274:rs:t4:t5 0x274:rt:t4:t5 0x27c:rs:t4:t5 0x288:rt:t5:t6 0x28c:rs:t5:t6 0x28c:rt:t5:t6 0x294:rs:t5:t6 0x2a0:rt:t6:t7 0x2a4:rs:t6:t7 0x2a4:rt:t6:t7 0x2ac:rs:t6:t7 0x2b8:rt:t7:t8 0x2bc:rs:t7:t8 0x2bc:rt:t7:t8 0x2c4:rs:t7:t8 0x2d0:rt:t8:t9 0x2d4:rs:t8:t9 0x2d4:rt:t8:t9 0x2dc:rs:t8:t9 0x2e8:rt:t9:t0 0x2ec:rs:t9:t0 0x2ec:rt:t9:t0 0x2f4:rs:t9:t0 0x300:rt:t0:t1 0x304:rs:t0:t1 0x304:rt:t0:t1 0x30c:rs:t0:t1 0x318:rt:t1:t2 0x31c:rs:t1:t2 0x31c:rt:t1:t2 0x324:rs:t1:t2 0x330:rt:t2:t3 0x334:rs:t2:t3 0x334:rt:t2:t3 0x33c:rs:t2:t3 0x348:rt:t3:t4 0x34c:rs:t3:t4 0x34c:rt:t3:t4 0x354:rs:t3:t4 */
/* bunka_sd_load - BUNKA_SD overlay, Address: 0x80137FEC, Size: 928 bytes (232 insns) */
#include "types.h"

extern void palette_upload_enqueue();
extern void class_clear_n(s32);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void sprite_rec_set(s32, s32, s32);

extern s32 bunka_sd_load_res_3;
extern s32 bunka_sd_load_res_1;
extern s32 bunka_sd_load_res_2;
extern s32 *bunka_sd_load_res;
extern u8 class_w[][68];

void bunka_sd_load(void)
{
    s32 i;
    u8 *sp0;
    s32 t9v;
    s32 k128;
    s32 kn120;
    s32 kn160;
    s32 k1;
    s32 k64;
    s32 k16m;
    s32 k4;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_load_res_3, (u8)(i + 16), 1, 1, i);
    }

    sp0 = class_w[0];
    i = 0;
    k128 = 128;
    kn120 = -120;
    kn160 = -160;
    k1 = 1;
    k64 = 64;
    k16m = 0x01000000;
    k4 = 4;
    do {
        class_clear_n(i + 96);
        sp0[6529] = k4;
        *(s32 *)&sp0[6584] = k16m;
        sp0[6531] = 0;
        sp0[6530] = k64;
        *(s32 *)&sp0[6540] = bunka_sd_load_res_1;
        *(s32 *)&sp0[6544] = bunka_sd_load_res_2;
        t9v = (s32)bunka_sd_load_res;
        i += 1;
        sp0 += 68;
        *(s16 *)&sp0[6484] = 0;
        sp0[6466] = k1;
        sp0[6467] = 0;
        *(s16 *)&sp0[6498] = kn160;
        *(s16 *)&sp0[6502] = kn120;
        sp0[6465] = k128;
        sp0[6527] = 0;
        sp0[6464] = 0;
        *(s32 *)&sp0[6512] = t9v;
    } while (i != 32);

    load_tpage_buf_lock(5, bunka_sd_load_res[0], 1);
    load_tpage_buf_lock(6, bunka_sd_load_res[1], 1);
    load_tpage_buf_lock(7, bunka_sd_load_res[2], 1);
    load_tpage_buf_lock(8, bunka_sd_load_res[3], 1);
    load_tpage_buf_lock(9, bunka_sd_load_res[4], 1);
    load_tpage_buf_lock(10, bunka_sd_load_res[5], 1);
    load_tpage_buf_lock(11, bunka_sd_load_res[6], 1);
    load_tpage_buf_lock(12, bunka_sd_load_res[7], 1);
    load_tpage_buf_lock(13, bunka_sd_load_res[8], 1);
    load_tpage_buf_lock(14, bunka_sd_load_res[9], 1);
    load_tpage_buf_lock(15, bunka_sd_load_res[10], 1);
    load_tpage_buf_lock(37, bunka_sd_load_res[11], 1);
    load_tpage_buf_lock(38, bunka_sd_load_res[12], 1);
    load_tpage_buf_lock(39, bunka_sd_load_res[13], 1);
    load_tpage_buf_lock(40, bunka_sd_load_res[14], 1);
    load_tpage_buf_lock(41, bunka_sd_load_res[15], 1);
    load_tpage_buf_lock(42, bunka_sd_load_res[16], 1);
    load_tpage_buf_lock(43, bunka_sd_load_res[17], 1);
    load_tpage_buf_lock(44, bunka_sd_load_res[18], 1);
    load_tpage_buf_lock(45, bunka_sd_load_res[19], 1);
    load_tpage_buf_lock(46, bunka_sd_load_res[20], 1);
    load_tpage_buf_lock(47, bunka_sd_load_res[21], 1);
    load_tpage_buf_lock(21, bunka_sd_load_res[22], 1);
    load_tpage_buf_lock(22, bunka_sd_load_res[23], 1);
    load_tpage_buf_lock(23, bunka_sd_load_res[24], 1);

    sprite_rec_set(0, 91, 1);
    sprite_rec_set(31, 86, 1);
}
