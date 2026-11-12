/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x158 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x160 */
/* @PostProc: patch_insn_reg_field 0x14c:rt:t1:t0 0x150:rt:t0:t1 0x154:rs:t0:t1 0x158:rs:t1:t0 0x158:rt:t1:t0 0x15c:rt:t1:t0 0x190:rs:s4:s1 0x190:rt:s1:s4 */
/* @SymbolStart: 0x801328C4 */
/* @SymbolSize: 460 */
/* Address: 0x8013297C, Instructions: 69 (BYTE-PERFECT 69/69, len t=c=276) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 GetClut();

typedef struct {
    u8  unk0;
    u8  unk1;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} OptRec;

extern u8  bg_map_cellh;
extern s16 bg_map_ncellw;
extern s16 bg_map_ncellh;
extern s32 bg_map_base;
extern s32 bg_map_index;
extern OptRec Cell[];
extern s16 C_index[];
extern u8  bg_pri;
extern u8  Map;
extern u8  screen_bri;
extern s32 Bg;

void option_bg_init(void)
{
    OptRec *rec;
    s16 *hw;
    s32 off;
    s32 hoff;
    s32 j;

    Bg = 0x01000000;
    ((LIBGS_BG *)&Bg)[0].x = -160;
    ((LIBGS_BG *)&Bg)[0].y = -120;
    ((LIBGS_BG *)&Bg)[0].w = 512;
    ((LIBGS_BG *)&Bg)[0].h = 320;
    ((LIBGS_BG *)&Bg)[0].scrollx = 0;
    ((LIBGS_BG *)&Bg)[0].scrolly = 0;
    ((LIBGS_BG *)&Bg)[0].r = screen_bri;
    ((LIBGS_BG *)&Bg)[0].g = screen_bri;
    ((LIBGS_BG *)&Bg)[0].b = screen_bri;
    (*(s32 *)&((LIBGS_BG *)&Bg)[0].map) = (s32)&Map;
    ((LIBGS_BG *)&Bg)[0].mx = 0;
    ((LIBGS_BG *)&Bg)[0].my = 0;
    ((LIBGS_BG *)&Bg)[0].scalex = 4096;
    ((LIBGS_BG *)&Bg)[0].scaley = 4096;
    ((LIBGS_BG *)&Bg)[0].rotate = 0;
    Map = 128;

    bg_map_cellh = 64;
    bg_map_ncellw = 4;
    bg_map_ncellh = 5;
    bg_map_base = (s32)Cell;
    bg_map_index = (s32)C_index;

    for (off = 0, hoff = 0; off != 128; off += 32, hoff += 8) {
        rec = (OptRec *)((u8 *)Cell + off);
        hw = (s16 *)((u8 *)C_index + hoff);
        for (j = 0; &C_index[j] != &C_index[5]; j++) {
            hw[j] = 0;
            rec[j].unk2 = (s16)GetClut(0, 506);
            rec[j].unk0 = 0;
            rec[j].unk1 = 0;
            rec[j].unk6 = 5;
            rec[j].unk4 = 0;
        }
    }

    bg_pri = 13;
}
