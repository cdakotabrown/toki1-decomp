/* @RodataBase: 0x8015C3B0 */
/* @SymbolSize: 2228 */
/* Address: 0x80142F70 */
/* Size: 2228 bytes (557 insns) */
#include "types.h"

extern s32 palette_upload_enqueue(s32, s32, s32, s32, s32);
extern s32 sub_step_up(void);

extern s32 rpg_bat_res_pal[];
extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_bukatu_no;
extern s32 rpg_bat_bukatu_flag;

void rpg_bat_palette_load(void)
{
    palette_upload_enqueue(rpg_bat_res_pal[0], 16, 1, 4, 0);
    palette_upload_enqueue(rpg_bat_res_pal[4], 30, 1, 4, 0);
    palette_upload_enqueue(rpg_bat_res_pal[4], 31, 1, 4, 15);

    switch (rpg_bat_mode) {
    case 0x00010000:
        palette_upload_enqueue(rpg_bat_res_pal[0], 17, 1, 1, 1);
        palette_upload_enqueue(rpg_bat_res_pal[0], 23, 1, 3, 1);
        palette_upload_enqueue(rpg_bat_res_pal[2], 24, 1, 4, 0);
        palette_upload_enqueue(rpg_bat_res_pal[1], 19, 1, 3, 0);
        palette_upload_enqueue(rpg_bat_res_pal[1], 18, 1, 4, 0);
        break;
    case 0x00020000:
        palette_upload_enqueue(rpg_bat_res_pal[0], 18, 1, 1, 2);
        palette_upload_enqueue(rpg_bat_res_pal[0], 23, 1, 3, 2);
        palette_upload_enqueue(rpg_bat_res_pal[2], 24, 1, 4, 1);
        palette_upload_enqueue(rpg_bat_res_pal[1], 19, 1, 4, 1);
        break;
    case 0x00040000:
        palette_upload_enqueue(rpg_bat_res_pal[0], 19, 1, 1, 3);
        palette_upload_enqueue(rpg_bat_res_pal[0], 23, 1, 3, 3);
        palette_upload_enqueue(rpg_bat_res_pal[2], 24, 1, 4, 2);
        palette_upload_enqueue(rpg_bat_res_pal[1], 18, 1, 4, 2);
        break;
    case 0x80000000:
        palette_upload_enqueue(rpg_bat_res_pal[0], 22, 1, 1, 6);
        palette_upload_enqueue(rpg_bat_res_pal[0], 23, 1, 3, 6);
        palette_upload_enqueue(rpg_bat_res_pal[1], 18, 1, 4, 5);
        break;
    case 0x08000000:
        palette_upload_enqueue(rpg_bat_res_pal[0], 17, 1, 1, 1);
        palette_upload_enqueue(rpg_bat_res_pal[0], 23, 1, 3, 1);
        palette_upload_enqueue(rpg_bat_res_pal[1], 18, 1, 4, 4);
        break;
    case 0x00100000:
        palette_upload_enqueue(rpg_bat_res_pal[0], 21, 1, 1, 5);
        palette_upload_enqueue(rpg_bat_res_pal[0], 20, 1, 1, 4);
        palette_upload_enqueue(rpg_bat_res_pal[0], 23, 1, 3, 4);
        palette_upload_enqueue(rpg_bat_res_pal[1], 18, 1, 4, 3);
        palette_upload_enqueue(rpg_bat_res_pal[6], 19, 1, 1, 0);
        break;
    }

    if (rpg_bat_bukatu_flag == 1) {
        switch ((u32)rpg_bat_bukatu_no) {
        case 0:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 6);
            break;
        case 1:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 7);
            break;
        case 2:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 9);
            break;
        case 3:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 8);
            break;
        case 4:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 10);
            break;
        case 5:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 11);
            break;
        case 6:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 1);
            break;
        case 7:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 2);
            break;
        case 8:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 4);
            break;
        case 9:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 5);
            break;
        case 10:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 3);
            break;
        case 11:
            palette_upload_enqueue(rpg_bat_res_pal[4], 29, 1, 4, 12);
            break;
        }
    }

    switch ((u32)rpg_bat_girl) {
    case 0:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 0);
        break;
    case 1:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 1);
        break;
    case 2:
        if (rpg_bat_mode != (s32)0x80000000) {
            palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 2);
            break;
        }
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 3);
        break;
    case 3:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 3);
        break;
    case 4:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 4);
        break;
    case 5:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 5);
        break;
    case 6:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 6);
        break;
    case 7:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 7);
        break;
    case 8:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 8);
        break;
    case 9:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 9);
        break;
    case 10:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 10);
        break;
    case 13:
        palette_upload_enqueue(rpg_bat_res_pal[3], 28, 1, 4, 13);
        break;
    case 12:
    case 14:
        break;
    }

    sub_step_up();
}
