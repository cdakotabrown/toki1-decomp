/* @CompileFlags: -cckr */
/* bunka_sd_rec_init - BUNKA_SD overlay, Address: 0x80136F3C, Size: 640 bytes (160 insns) */
#include "types.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void class_clear_n(s32);
extern void bunka_sd_class_flag_set(s32, s32, s32);

extern s32 bunka_sd_rec_res_3;
extern s32 bunka_sd_rec_res_1;
extern s32 bunka_sd_rec_res_2;
extern s32 *bunka_sd_rec_res;

typedef struct {
    u8 pad0;
    u8 f1;
    u8 f2;
    u8 f3;
    u8 f4;
    u8 f5;
    u8 f6;
    u8 f7;
    u8 pad8[4];
    u32 f12;
    u32 f16;
    u8 pad20[4];
    u16 f24;
    u8 pad26[12];
    s16 f38;
    u8 pad40[2];
    s16 f42;
    u8 pad44[8];
    u32 f52;
    u32 f56;
    u8 pad60[7];
    u8 f67;
} Rec;

extern Rec class_w[];

bunka_sd_rec_init()
{
    int i;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_rec_res_3, (i + 16) & 0xFF, 1, 1, i);
    }
    for (i = 0; i < 64; i++) {
        class_clear_n(i + 96);
        class_w[i + 96].f1 = 4;
        class_w[i + 96].f56 = 0x01000000;
        class_w[i + 96].f3 = 0;
        class_w[i + 96].f2 = 64;
        class_w[i + 96].f12 = bunka_sd_rec_res_1;
        class_w[i + 96].f16 = bunka_sd_rec_res_2;
        class_w[i + 96].f52 = (u32)bunka_sd_rec_res;
        class_w[i + 96].f24 = 0;
        class_w[i + 96].f6 = 1;
        class_w[i + 96].f7 = 0;
        class_w[i + 96].f38 = -160;
        class_w[i + 96].f42 = -120;
        class_w[i + 96].f5 = 128;
        class_w[i + 96].f67 = 0;
        class_w[i + 96].f4 = 0;
    }
    bunka_sd_class_flag_set(0, 64, 1);
    bunka_sd_class_flag_set(31, 68, 1);
    load_tpage_buf_lock(5, bunka_sd_rec_res[0], 1);
    load_tpage_buf_lock(6, bunka_sd_rec_res[1], 1);
    load_tpage_buf_lock(7, bunka_sd_rec_res[2], 1);
    load_tpage_buf_lock(8, bunka_sd_rec_res[3], 1);
    load_tpage_buf_lock(9, bunka_sd_rec_res[4], 1);
    load_tpage_buf_lock(10, bunka_sd_rec_res[5], 1);
    load_tpage_buf_lock(37, bunka_sd_rec_res[6], 1);
    load_tpage_buf_lock(38, bunka_sd_rec_res[7], 1);
    load_tpage_buf_lock(39, bunka_sd_rec_res[8], 1);
    load_tpage_buf_lock(40, bunka_sd_rec_res[9], 1);
    load_tpage_buf_lock(41, bunka_sd_rec_res[10], 1);
    load_tpage_buf_lock(42, bunka_sd_rec_res[11], 1);
    load_tpage_buf_lock(21, bunka_sd_rec_res[12], 1);
}
