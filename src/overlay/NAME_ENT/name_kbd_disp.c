/* @SymbolStart: 0x8013A0C0 */
/* @SymbolSize: 2712 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=117:a3,153:t0 */
/* @PostProc: swap_instructions --offset1 0x2c8 --offset2 0x2cc */
/* @PostProc: swap_instructions --offset1 0x2d8 --offset2 0x2dc */
#include "types.h"

typedef struct {
    u8 pad0[20];
    u8 flags[13];
} FixedRec;

typedef struct {
    u8 pad0[1672];
    u8 flags[13];
    u8 pad1[3840 - 1685];
} SaveRec;

typedef struct {
    u8 f0;
    u8 mode;
    u8 f2;
    u8 attr;
    u8 pad[64];
} Sprite;

extern s32 name_num_disp(s32 count, s32 xOffset, s32 yOffset);
extern void name_key_init(s32 index, s32 row, s32 col);

extern u8 card_sys_present;
extern u8 album_slot;
extern FixedRec album_work;
extern SaveRec album_chr[];
extern Sprite class_w[];

void name_kbd_disp(void)
{
    s32 i;
    s32 c;
    s32 j;
    s32 k;

    if (card_sys_present != 0) {
        c = album_work.flags[0];
        if (c) {
            name_num_disp(c, -4, 30);
        }
        c = album_work.flags[1];
        if (c) {
            name_num_disp(c, -4, 46);
        }
        c = album_work.flags[2];
        if (c) {
            name_num_disp(c, -4, 62);
        }
        c = album_work.flags[3];
        if (c) {
            name_num_disp(c, -4, 78);
        }
        c = album_work.flags[4];
        if (c) {
            name_num_disp(c, 60, 30);
        }
        c = album_work.flags[5];
        if (c) {
            name_num_disp(c, 60, 46);
        }
        c = album_work.flags[6];
        if (c) {
            name_num_disp(c, 60, 62);
        }
        c = album_work.flags[7];
        if (c) {
            name_num_disp(c, 60, 78);
        }
        if (album_work.flags[12]) {
            name_num_disp(album_work.flags[12], 136, 14);
        }
        if (album_work.flags[8]) {
            name_num_disp(album_work.flags[8], 136, 30);
        }
        if (album_work.flags[9]) {
            name_num_disp(album_work.flags[9], 136, 46);
        }
        if (album_work.flags[10]) {
            name_num_disp(album_work.flags[10], 136, 62);
        }
        if (album_work.flags[11]) {
            name_num_disp(album_work.flags[11], 136, 78);
        }
        for (i = 0; i < 4; i++) {
            if (album_work.flags[i]) {
                name_key_init(i, -35, (i % 4) * 16 + 29);
            }
        }
        for (i = 4; i < 8; i++) {
            if (album_work.flags[i]) {
                name_key_init(i, 30, (i % 4) * 16 + 29);
            }
        }
        for (i = 8; i < 12; i++) {
            if (album_work.flags[i]) {
                name_key_init(i, 105, (i % 4) * 16 + 29);
            }
        }
        if (album_work.flags[12]) {
            name_key_init(12, 105, 13);
        }
        for (i = 0, j = 0; i < 8; i++, j++) {
            if (album_work.flags[i] == 0) {
                class_w[j + 128].attr |= 0x80;
                class_w[j + 128].mode = 4;
            } else {
                class_w[j + 128].attr &= 0x7F;
            }
        }
        for (i = 8; i < 12; i++) {
            if (album_work.flags[i] == 0) {
                class_w[i + 129].attr |= 0x80;
                class_w[i + 129].mode = 4;
            } else {
                class_w[i + 129].attr &= 0x7F;
            }
        }
        if (album_work.flags[12] == 0) {
            class_w[136].attr |= 0x80;
            class_w[136].mode = 4;
        } else {
            class_w[136].attr &= 0x7F;
        }
    } else {
        c = album_chr[album_slot].flags[0];
        if (c) {
            name_num_disp(c, -4, 30);
        }
        c = album_chr[album_slot].flags[1];
        if (c) {
            name_num_disp(c, -4, 46);
        }
        c = album_chr[album_slot].flags[2];
        if (c) {
            name_num_disp(c, -4, 62);
        }
        c = album_chr[album_slot].flags[3];
        if (c) {
            name_num_disp(c, -4, 78);
        }
        c = album_chr[album_slot].flags[4];
        if (c) {
            name_num_disp(c, 60, 30);
        }
        c = album_chr[album_slot].flags[5];
        if (c) {
            name_num_disp(c, 60, 46);
        }
        c = album_chr[album_slot].flags[6];
        if (c) {
            name_num_disp(c, 60, 62);
        }
        c = album_chr[album_slot].flags[7];
        if (c) {
            name_num_disp(c, 60, 78);
        }
        c = album_chr[album_slot].flags[12];
        if (c) {
            name_num_disp(c, 136, 14);
        }
        c = album_chr[album_slot].flags[8];
        if (c) {
            name_num_disp(c, 136, 30);
        }
        c = album_chr[album_slot].flags[9];
        if (c) {
            name_num_disp(c, 136, 46);
        }
        c = album_chr[album_slot].flags[10];
        if (c) {
            name_num_disp(c, 136, 62);
        }
        c = album_chr[album_slot].flags[11];
        if (c) {
            name_num_disp(c, 136, 78);
        }
        for (i = 0; i < 4; i++) {
            if (album_chr[album_slot].flags[i]) {
                name_key_init(i, -35, (i % 4) * 16 + 29);
            }
        }
        for (i = 4; i < 8; i++) {
            if (album_chr[album_slot].flags[i]) {
                name_key_init(i, 30, (i % 4) * 16 + 29);
            }
        }
        for (i = 8; i < 12; i++) {
            if (album_chr[album_slot].flags[i]) {
                name_key_init(i, 105, (i % 4) * 16 + 29);
            }
        }
        if (album_chr[album_slot].flags[12]) {
            name_key_init(12, 105, 13);
        }
        for (i = 0, j = 0; j < 8; i++, j++) {
            if (album_chr[album_slot].flags[i] == 0) {
                class_w[j + 128].attr |= 0x80;
                class_w[j + 128].mode = 4;
            } else {
                class_w[j + 128].attr &= 0x7F;
            }
        }
        for (j = 8; j < 12; j++) {
            if (album_chr[album_slot].flags[j] == 0) {
                class_w[j + 129].attr |= 0x80;
                class_w[j + 129].mode = 4;
            } else {
                class_w[j + 129].attr &= 0x7F;
            }
        }
        if (album_chr[album_slot].flags[12] == 0) {
            class_w[136].attr |= 0x80;
            class_w[136].mode = 4;
        } else {
            class_w[136].attr &= 0x7F;
        }
    }
}
