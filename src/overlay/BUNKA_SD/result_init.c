/* Address: 0x80133984 */
/* Size: 832 bytes (208 insns) */
/* @SymbolSize: 832 */
#include "types.h"
extern u8 class_w[];

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void sprite_flag_set(s32, s32, s32);

extern s32 bunka_sd_result_res_3;
extern s32 bunka_sd_result_res_1;
extern s32 bunka_sd_result_res_2;
extern s32 *bunka_sd_result_res;

void result_init(void) {
    s32 i;
    u8 *p;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_result_res_3, (u8)(i + 16), 1, 1, i);
    }

    p = (void *)class_w;
    for (i = 0; i != 32; i++) {
        class_clear_n(i + 96);
        p[6529] = 4;
        *(s32 *)(p + 6584) = 0x01000000;
        p[6531] = 0;
        p[6530] = 64;
        *(s32 *)(p + 6540) = bunka_sd_result_res_1;
        *(s32 *)(p + 6544) = bunka_sd_result_res_2;
        *(s32 *)(p + 6580) = (s32)bunka_sd_result_res;
        p += 68;
        *(s16 *)(p + 6484) = 0;
        p[6466] = 1;
        p[6467] = 0;
        *(s16 *)(p + 6498) = -160;
        *(s16 *)(p + 6502) = -120;
        p[6465] = 128;
        p[6527] = 0;
        p[6464] = 0;
    }

    sprite_flag_set(31, 191, 1);
    sprite_flag_set(1, 0, 1);

    load_tpage_buf_lock(5, bunka_sd_result_res[0], 1);
    load_tpage_buf_lock(6, bunka_sd_result_res[1], 1);
    load_tpage_buf_lock(7, bunka_sd_result_res[2], 1);
    load_tpage_buf_lock(8, bunka_sd_result_res[3], 1);
    load_tpage_buf_lock(9, bunka_sd_result_res[4], 1);
    load_tpage_buf_lock(10, bunka_sd_result_res[5], 1);
    load_tpage_buf_lock(11, bunka_sd_result_res[6], 1);
    load_tpage_buf_lock(12, bunka_sd_result_res[7], 1);
    load_tpage_buf_lock(13, bunka_sd_result_res[8], 1);
    load_tpage_buf_lock(14, bunka_sd_result_res[9], 1);
    load_tpage_buf_lock(15, bunka_sd_result_res[10], 1);
    load_tpage_buf_lock(37, bunka_sd_result_res[11], 1);
    load_tpage_buf_lock(38, bunka_sd_result_res[12], 1);
    load_tpage_buf_lock(39, bunka_sd_result_res[13], 1);
    load_tpage_buf_lock(40, bunka_sd_result_res[14], 1);
    load_tpage_buf_lock(41, bunka_sd_result_res[15], 1);
    load_tpage_buf_lock(42, bunka_sd_result_res[16], 1);
    load_tpage_buf_lock(43, bunka_sd_result_res[17], 1);
    load_tpage_buf_lock(44, bunka_sd_result_res[18], 1);
    load_tpage_buf_lock(45, bunka_sd_result_res[19], 1);
    load_tpage_buf_lock(46, bunka_sd_result_res[20], 1);
}
