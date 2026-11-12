/* Address: 0x8006D1D4 */
/* Size: 880 bytes (220 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1, -cckr, leaf) */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny IDO52_FORCE_WEB_COLOR=0:v1 IDO52_UGEN_TEMP_REG=3:t9,5:t1,4:t0,6:t2,9:t5,8:t4,10:t7,7:t0,12:t9,15:t2,14:t1,16:t4,17:t5,19:t7,20:t8,22:t1,25:t4,26:t6,27:t7,28:t8,30:t0,29:t9,31:t1,34:t4,33:t3 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_branch_operands --offsets 0x48 0x70 0x90 0xac 0xc8 0xe8 0x104 0x120 0x15c 0x178 0x194 0x1b0 0x1cc 0x234 0x23c 0x25c 0x280 0x2a0 0x2c0 0x2dc 0x2fc 0x31c 0x32c 0x34c */
#include "types.h"

typedef struct {
    u32 b0 : 1; u32 b1 : 1; u32 b2 : 1; u32 b3 : 1;
    u32 b4 : 1; u32 b5 : 1; u32 b6 : 1; u32 b7 : 1;
    u32 b8 : 1; u32 b9 : 1; u32 b10 : 1; u32 b11 : 1;
    u32 b12 : 1; u32 b13 : 1; u32 b14 : 1; u32 b15 : 1;
    u32 rest : 16;
} SpotFlags;

typedef struct {
    u8 pad0[62];
    u8 year;
    u8 month;
    u8 pad64[224];
    u8 spot[14];
    u8 movie;
    u8 pad303[3637];
    SpotFlags flags;
} Save;

extern Save screen_w;
extern volatile u8 now_year;
extern volatile u8 now_month;
extern u8 basyo_flag1, basyo_flag3, basyo_flag5, basyo_flag6, basyo_flag7;
extern u8 basyo_flaga, basyo_flagb, basyo_flagd, basyo_flage;

void basyo_flag_set()
{
    Save *sv;
    u32 w;
    int ch;
    int mode;

    sv = &screen_w;
    w = *(u32 *)&sv->flags;

    if (screen_w.flags.b0) { }
    switch (now_month) {
    case 6:
    case 7:
        ch = now_year;
        if (ch == 95) {
            if ((s32)(w << 30) < 0) {
                basyo_flag1 = 1;
            }
            basyo_flage = 1;
        }
        if (ch == 96) {
            if (sv->flags.b5) {
                basyo_flag5 = 1;
            }
        }
        if (ch == 97) {
        }
        break;
    case 9:
    case 10:
        ch = now_year;
        if (ch == 95) {
            if ((s32)(w << 29) < 0) {
                basyo_flag7 = 1;
            }
        }
        if (ch == 96) {
            if (sv->flags.b6) {
                basyo_flaga = 1;
            }
        }
        if (ch == 97) {
        }
        break;
    case 1:
        ch = now_year;
        if (ch == 96) {
            if ((s32)(w << 28) < 0) {
                basyo_flag3 = 1;
            }
        }
        if (ch == 97) {
            if (sv->flags.b7) {
                basyo_flag6 = 1;
            }
        }
        if (ch == 98) {
        }
        break;
    case 12:
        ch = now_year;
        if (ch == 95) {
            if ((s32)(w << 28) < 0) {
                basyo_flag3 = 1;
            }
        }
        if (ch == 96) {
            if (sv->flags.b7) {
                basyo_flag6 = 1;
            }
        }
        if (ch == 97) {
        }
        break;
    case 3:
    case 4:
        ch = now_year;
        if (ch == 96) {
            if ((s32)(w << 27) < 0) {
                basyo_flagd = 1;
            }
        }
        if (ch == 97) {
            if (sv->flags.b8) {
                basyo_flagb = 1;
            }
        }
        break;
    }
    mode = now_month;
    ch = now_year;
    switch (mode) {
    case 5:
        if (ch == 95) {
            if (ch == 96) {
                if (!sv->flags.b4) {
                    basyo_flagd = 128;
                }
            }
        }
        if (ch == 97) {
            if (!sv->flags.b8) {
                basyo_flagb = 128;
            }
        }
        break;
    case 8:
        if (ch == 95) {
            if (!sv->flags.b1) {
                basyo_flag1 = 128;
            }
        }
        if (ch == 96) {
            if (!sv->flags.b5) {
                basyo_flag5 = 128;
            }
        }
        if (ch == 97) {
        }
        break;
    case 11:
        sv->flags.b15 = 0;
        if (ch == 95) {
            if (!sv->flags.b2) {
                basyo_flag7 = 128;
            }
        }
        if (ch == 96) {
            if (!sv->flags.b6) {
                basyo_flaga = 128;
            }
        }
        if (ch == 97) {
        }
        break;
    case 2:
        if (ch == 96) {
            if (!sv->flags.b3) {
                basyo_flag3 = 128;
            }
        }
        if (ch == 97) {
            if (!sv->flags.b7) {
                basyo_flag6 = 128;
            }
        }
        break;
    }
}
