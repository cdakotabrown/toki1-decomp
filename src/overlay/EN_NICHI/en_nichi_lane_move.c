/* @CompileFlags: -Wc,-pic0 -cckr */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:3 */
/* en_nichi_lane_move - EN_NICHI overlay, Address: 0x8013903C, Size: 1028 bytes */
/* @SymbolSize: 1028 */
#include "types.h"

extern s32 rand();

extern u8  class_w[];
extern u8  screen_w[];
extern s32 girl11_flag;

typedef struct {
    s16 v[13];
} PitchTable;

typedef struct {
    s32 w[32];
} NoteTable;

extern PitchTable en_nichi_lane_move_tbl1;
extern NoteTable en_nichi_lane_move_tbl0;
extern u8 mascot_flag[];

en_nichi_lane_move()
{
    s16 *p;
    u8 *rec;
    s32 i;
    s32 j;
    NoteTable notes;
    PitchTable pitch;
    u8 n;
    u8 list[15];
    s32 v;
    s16 w;
    s32 k;

    notes = en_nichi_lane_move_tbl0;
    pitch = en_nichi_lane_move_tbl1;

    n = 0;
    rec = screen_w;
    for (i = 0; i < 11; i++) {
        if (((u32) (*(s32 *) (rec + 0x1C8) << 30) >> 31) == 1) {
            list[n] = (u8) i;
            n = (u8) (n + 1);
        }
        rec += 56;
    }

    if (mascot_flag[11] != 0 || ((u32) (girl11_flag << 20) >> 29) != 0) {
        list[n] = 11;
        n = (u8) (n + 1);
    }
    if (mascot_flag[12] != 0) {
        list[n] = 12;
        n = (u8) (n + 1);
    }

    p = &((s16 *) &notes)[32];
    while (p < &((s16 *) &notes)[48]) {
        *p = pitch.v[list[(u32) rand() % n]];
        p++;
    }

    for (i = 0; i < 4; i++) {
        *(s16 *) &class_w[i * 68 + 0x1C0A] += 1;
        if (*(s16 *) &class_w[i * 68 + 0x1C0A] >= 193) {
            v = rand() & 0x3F;
            j = (i + 1) % 4;
            *(s16 *) &class_w[i * 68 + 0x1C0A] = *(s16 *) &class_w[j * 68 + 0x1C0A] - 0x60;
            *(s16 *) &class_w[i * 68 + 0x1BFC] = ((s16 *) &notes)[v];
            if ((v & 1) != 0 && v < 32) {
                *(s16 *) &class_w[i * 68 + 0x1C0E] = -0x38;
            } else {
                *(s16 *) &class_w[i * 68 + 0x1C0E] = -0x48;
            }
            *(s32 *) &class_w[i * 68 + 0x1C1C] = 0x01000000;
        }
    }

    for (i = 0; i < 4; i++) {
        *(s32 *) &class_w[i * 68 + 0x1D18] += (s32) 0xFFFF2000;
        if (*(s16 *) &class_w[i * 68 + 0x1D1A] < -0xC0) {
            v = rand() & 0x3F;
            j = (i + 3) % 4;
            *(s16 *) &class_w[i * 68 + 0x1D1A] = *(s16 *) &class_w[j * 68 + 0x1D1A] + 0x60;
            *(s16 *) &class_w[i * 68 + 0x1D0C] = ((s16 *) &notes)[v];
            if ((v & 1) != 0 && v < 32) {
                *(s16 *) &class_w[i * 68 + 0x1D1E] = -0x10;
            } else {
                *(s16 *) &class_w[i * 68 + 0x1D1E] = -0x20;
            }
            *(s32 *) &class_w[i * 68 + 0x1D2C] = 0x01000000;
        }
    }

    for (i = 0; i < 4; i++) {
        *(s32 *) &class_w[i * 68 + 0x1E28] += 0xC000;
        if (*(s16 *) &class_w[i * 68 + 0x1E2A] >= 193) {
            v = rand() & 0x3F;
            j = (i + 1) % 4;
            *(s16 *) &class_w[i * 68 + 0x1E2A] = *(s16 *) &class_w[j * 68 + 0x1E2A] - 0x60;
            *(s16 *) &class_w[i * 68 + 0x1E1C] = ((s16 *) &notes)[v];
            if ((v & 1) != 0 && v < 32) {
                *(s16 *) &class_w[i * 68 + 0x1E2E] = 0x18;
            } else {
                *(s16 *) &class_w[i * 68 + 0x1E2E] = 8;
            }
            *(s32 *) &class_w[i * 68 + 0x1E3C] = 0x01000000;
        }
    }
}
