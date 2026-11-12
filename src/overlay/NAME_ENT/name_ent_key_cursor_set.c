/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad0[2];
    u8  f2;
    u8  pad3[21];
    s16 f24;
    u8  pad26[42];
} Rec;

extern u8  name_ent_cell_no;
extern s32 pad_trig;
extern Rec class_w[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void name_ent_key_cursor_set(mode)
s32 mode;
{
    s32 i;
    s32 unk1, unk2, unk3;

    if (mode == 0 && name_ent_cell_no >= 3) {
        name_ent_cell_no = 12;
    }
    if (mode == 1 && name_ent_cell_no >= 6) {
        name_ent_cell_no = 12;
    }
    if (mode == 2 && name_ent_cell_no >= 11) {
        name_ent_cell_no = 12;
    }
    if (pad_trig & 0x20) {
        if (mode == 0) {
            for (i = 0; i < 3; i++) {
                if (cursor_cw_x < i * 16 - 88 && i * 16 - 104 < cursor_cw_x &&
                    cursor_cw_y < -70 && cursor_cw_y >= -85) {
                    name_ent_cell_no = i;
                }
            }
        } else if (mode == 1) {
            for (i = 0; i < 3; i++) {
                if (cursor_cw_x < i * 16 - 32 && i * 16 - 48 < cursor_cw_x &&
                    cursor_cw_y < -70 && cursor_cw_y >= -85) {
                    name_ent_cell_no = i + 3;
                }
            }
        } else if (mode == 2) {
            for (i = 0; i < 5; i++) {
                if (cursor_cw_x < i * 16 + 88 && i * 16 < cursor_cw_x - 72 &&
                    cursor_cw_y < -70 && cursor_cw_y >= -85) {
                    name_ent_cell_no = i + 6;
                }
            }
        }
    }
    for (i = 0; i < 11; i++) {
        if (i == name_ent_cell_no) {
            if (class_w[i + 98].f24 == 13) {
                class_w[i + 98].f24 = 0;
            }
            name_ent_key_set(i + 98);
        } else {
            class_w[i + 98].f2 = 64;
            class_w[i + 98].f24 = 13;
        }
    }
}
