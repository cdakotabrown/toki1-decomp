/* Address: 0x8004D3E8 */
/* Size: 664 bytes (166 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* The +16 is the ugen PIC linkage area: `@CompileFlags: -Wc,-pic1` */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_NO_BOPT=0 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x150 */
/* @PostProc: remap_stack_slots --map '152:148,136:132,132:128' */
/* `@PostProc: custom_script custom_func_8004D3E8.py`, now retired and deleted. */
#include "types.h"

extern u8 srn_w[];
extern char srn_vram_set_str0[];
extern u8 srn_vram_set_tbl0;

extern void DrawSync(s32);
extern void printf(char *);
extern void mod_trans_vram(s32, s32, s32, s32, s32);
extern void move_255_line(s32, s32, s32);

s32 srn_vram_set(s32 arg0, s32 arg1, s32 maxX, s32 maxY) {
    u8 *base;
    s32 w, h, tw, th, tx, ty;
    s32 srcW, srcH;
    u32 *tilemap;
    s32 pixW, pixH;
    s32 col, row, tileCount;

    DrawSync(0);

    base = srn_w + (arg0 * 5) * 8;

    w = base[14];
    h = base[15];
    tx = base[18];
    tw = base[16];
    th = base[17];
    ty = base[19];
    srcW = base[20];
    srcH = base[21];
    tilemap = *(u32 **)(base + 8);

    if (h == 0 || w == 0) {
        printf(srn_vram_set_str0);
        return -1;
    }

    pixW = (srcW * tw) / w;
    tileCount = *(s32 *)(base + 28);
    row = 0;
    pixH = (srcH * th) / h;

    if (pixH > 0) {
        do {
            if (pixW > 0) {
                col = 0;

                do {
                    if ((col - 1) * tw + tx * w < maxX) {
                        if ((row - 1) * th + ty * h < maxY) {
                            if ((*tilemap & 0xFFFF) != 0) {
                                mod_trans_vram(0, *tilemap & 0xFFFF, arg1, tw * col, th * row);
                            }
                        }
                    }
                    tileCount--;
                    tilemap++;
                    if (tileCount == 0) {
                        row = pixH;
                        col = pixW;
                    }
                    col++;
                } while (col < pixW);
            }
            row++;
        } while (row < pixH);
    }

    if (srn_vram_set_tbl0 != 0) {
        move_255_line(arg1, arg1 + 11, 15);
    }
}
