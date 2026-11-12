/* Function: file_select_main4e */
/* Address: 0x8006F5C8 */
/* Size: 1576 bytes (394 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_8006F5C8.py */
#include "common.h"

extern u8 screen_w[];

extern s32 get_param_top_girl_tbl0;
extern s32 D_800BCC4C;
extern s32 D_800BCC50;
extern s32 D_800BCC54;
extern s32 D_800BCC58;

extern s32 D_800BCC48b;
extern s32 D_800BCC4Cb;
extern s32 D_800BCC50b;
extern s32 D_800BCC58b;

extern u16 basyo_pack;

extern void get_grp(u8 *, s16 *);

s32 file_select_main4e(s32 arg0) {
    u8 sp64[16];
    s16 sp44[16];
    s32 a2val, a3val, t0val, t1val, t2val;
    u8 * const v1 = screen_w;
    u32 flags;
    u32 v0val;

    a2val = 0;
    a3val = 0;
    t0val = 0;
    t1val = 0;
    t2val = 0;

    get_grp(sp64, sp44);

    switch ((u32)arg0 % 9U) {
    case 0:
        flags = *(u32 *)(v1 + 512);
        if ((s32)(flags << 30) < 0) {
            a3val = 1;
            a2val = sp64[1] - get_param_top_girl_tbl0;
            t0val = sp64[0] - D_800BCC48b;
        }
        break;

    case 1:
        flags = *(u32 *)(screen_w + 568);
        if ((s32)(flags << 30) < 0) {
            a3val = 2;
            a2val = sp64[2] - D_800BCC58;
            t0val = sp64[0] - D_800BCC58;
        }
        break;

    case 2:
        flags = *(u32 *)(screen_w + 624);
        if ((s32)(flags << 30) < 0) {
            a3val = 3;
            a2val = sp64[3] - D_800BCC4C;
            t0val = sp64[0] - D_800BCC4Cb;
        }
        break;

    case 3:
        flags = *(u32 *)(screen_w + 736);
        if ((s32)(flags << 30) < 0) {
            a3val = 5;
            a2val = sp64[5] - get_param_top_girl_tbl0;
        }
        flags = *(u32 *)(screen_w + 792);
        if ((s32)(flags << 30) < 0) {
            t2val = 6;
            t1val = sp64[6] - get_param_top_girl_tbl0;
            t0val = sp64[0] - D_800BCC54;
        }
        break;

    case 4:
        v0val = *(u16 *)&basyo_pack;
        v0val >>= 12;
        if (v0val == (u32)(v1[458] >> 4)) {
            flags = *(u32 *)(v1 + 456);
            t0val = sp64[0];
            if ((s32)(flags << 29) < 0) {
                t0val -= 11;
            } else {
                t0val = 0;
            }
            if (10 != v0val) {
                break;
            }
            flags = *(u32 *)(v1 + 1016);
            if ((s32)(flags << 30) >= 0) {
                break;
            }
            if ((s32)(flags << 29) < 0) {
                if ((u32)(v1[1018] >> 4) == 10) {
                    a2val = sp64[10] - 11;
                    a3val = 10;
                }
            }
            break;
        }
        if (v0val >= 11U) {
            break;
        }
        switch (v0val) {
        case 0:
            flags = *(u32 *)(v1 + 512);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[514] >> 4) == 0) {
                    a2val = sp64[1] - 11;
                    a3val = 1;
                }
            }
            break;

        case 1:
            flags = *(u32 *)(v1 + 512);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[514] >> 4) == 1) {
                    a2val = sp64[1] - 11;
                    a3val = 1;
                }
            }
            break;

        case 2:
            flags = *(u32 *)(v1 + 568);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[570] >> 4) == 2) {
                    a2val = sp64[2] - 11;
                    a3val = 2;
                }
            }
            break;

        case 3:
            flags = *(u32 *)(v1 + 568);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[570] >> 4) == 3) {
                    a2val = sp64[2] - 11;
                    a3val = 2;
                }
            }
            break;

        case 4:
            flags = *(u32 *)(v1 + 624);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[626] >> 4) == 4) {
                    a2val = sp64[3] - 11;
                    a3val = 3;
                }
            }
            break;

        case 5:
            flags = *(u32 *)(v1 + 624);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[626] >> 4) == 5) {
                    a2val = sp64[3] - 11;
                    a3val = 2;
                }
            }
            break;

        case 6:
            flags = *(u32 *)(v1 + 680);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[682] >> 4) == 6) {
                    a2val = sp64[4] - 11;
                    a3val = 4;
                }
            }
            break;

        case 7:
            flags = *(u32 *)(v1 + 680);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[682] >> 4) == 7) {
                    a2val = sp64[4] - 11;
                    a3val = 4;
                }
            }
            break;

        case 8:
            flags = *(u32 *)(v1 + 736);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                v0val = 8;
                if ((u32)(v1[738] >> 4) == v0val) {
                    if ((u32)(v1[458] >> 4) != v0val) {
                        a2val = sp64[5] - 11;
                        a3val = 5;
                    }
                }
            }
            break;

        case 9:
            flags = *(u32 *)(v1 + 792);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                v0val = 9;
                if ((u32)(v1[794] >> 4) == v0val) {
                    if ((u32)(v1[458] >> 4) != v0val) {
                        a2val = sp64[6] - 11;
                        a3val = 6;
                    }
                }
            }
            break;

        case 10:
            flags = *(u32 *)(v1 + 1016);
            if ((s32)(flags << 30) < 0 && (s32)(flags << 29) < 0) {
                if ((u32)(v1[1018] >> 4) == 10) {
                    a2val = sp64[10] - 11;
                    a3val = 10;
                }
            }
            break;
        }
        break;

    case 5:
        flags = *(u32 *)(screen_w + 904);
        if ((s32)(flags << 30) < 0) {
            a3val = 8;
            a2val = sp64[8] - D_800BCC50;
            t0val = sp64[0] - D_800BCC50b;
        }
        break;

    case 6:
        flags = *(u32 *)(screen_w + 848);
        if ((s32)(flags << 30) < 0) {
            a3val = 7;
            a2val = sp64[7] - D_800BCC58;
            t0val = sp64[0] - D_800BCC58b;
        }
        break;

    case 7:
    default:
        break;
    }

    if (a3val != 0 && t2val != 0) {
        if (t1val < a2val) {
            if (t0val < a2val && a2val > 0) {
                return a3val;
            }
        } else {
            if (t0val < t1val && t1val > 0) {
                return t2val;
            }
        }
    } else if (a3val != 0) {
        if (t0val < a2val && a2val > 0) {
            return a3val;
        }
    } else if (t2val != 0) {
        if (t0val < t1val && t1val > 0) {
            return t2val;
        }
    }

    if (t0val > 0) {
        return 0;
    }
    return 15;
}
