/* Address: 0x8006F5C8 */
/* Size: 1576 bytes (394 instructions) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 b0:1;
    u32 active:1;
    u32 flag2:1;
    u32 b3:15;
    u32 done:1;
    u32 b19:1;
    u32 loc:4;
    u32 rest:8;
    u32 pad4;
    u8 stats[21];
    u8 pad[27];
} Chara;

typedef struct {
    u8 pad[244];
    u16 wlo:12;
    u16 place:4;
    u8 pad1[210];
    Chara chars[11];
} Save;

extern Save screen_w;
extern s32 get_param_top_girl_tbl0[];

s32 get_param_top_girl(key)
u32 key;
{
    u8 stats[20];
    u8 work[32];
    s32 pad1;
    s32 ai;
    s32 a;
    s32 cur;
    s32 bi;
    s32 b;
    Save *sv;
    s32 slot;
    s32 pad2;

    sv = &screen_w;
    a = 0;
    ai = 0;
    cur = 0;
    b = 0;
    bi = 0;
    get_grp(stats, work);
    switch (key % 9) {
    case 0:
        if (sv->chars[1].active) {
            a = stats[1] - get_param_top_girl_tbl0[0];
            ai = 1;
        }
        cur = stats[0] - get_param_top_girl_tbl0[0];
        break;
    case 1:
        if (screen_w.chars[2].active) {
            a = stats[2] - get_param_top_girl_tbl0[4];
            ai = 2;
        }
        cur = stats[0] - get_param_top_girl_tbl0[4];
        break;
    case 2:
        if (screen_w.chars[3].active) {
            a = stats[3] - get_param_top_girl_tbl0[1];
            ai = 3;
        }
        cur = stats[0] - get_param_top_girl_tbl0[1];
        break;
    case 3:
        if (screen_w.chars[5].active) {
            a = stats[5] - get_param_top_girl_tbl0[0];
            ai = 5;
        }
        if (screen_w.chars[6].active) {
            b = stats[6] - get_param_top_girl_tbl0[0];
            bi = 6;
        }
        cur = stats[0] - get_param_top_girl_tbl0[3];
        break;
    case 4:
        slot = screen_w.place;
        if (slot == sv->chars[0].loc) {
            cur = stats[0] - 11;
            if (!sv->chars[0].flag2) {
                cur = 0;
            }
            if (sv->place == 10) {
                if (sv->chars[10].active && sv->chars[10].flag2 && sv->chars[10].loc == 10) {
                    a = stats[10] - 11;
                    ai = 10;
                }
            }
        } else {
            switch (slot) {
            case 0:
                if (sv->chars[1].active && sv->chars[1].flag2 && sv->chars[1].loc == 0) {
                    a = stats[1] - 11;
                    ai = 1;
                }
                break;
            case 1:
                if (sv->chars[1].active && sv->chars[1].flag2 && screen_w.chars[1].loc == 1) {
                    a = stats[1] - 11;
                    ai = 1;
                }
                break;
            case 2:
                if (sv->chars[2].active && sv->chars[2].flag2 && sv->chars[2].loc == 2) {
                    a = stats[2] - 11;
                    ai = 2;
                }
                break;
            case 3:
                if (sv->chars[2].active && sv->chars[2].flag2 && sv->chars[2].loc == 3) {
                    a = stats[2] - 11;
                    ai = 2;
                }
                break;
            case 4:
                if (sv->chars[3].active && sv->chars[3].flag2 && sv->chars[3].loc == 4) {
                    a = stats[3] - 11;
                    ai = 3;
                }
                break;
            case 5:
                if (sv->chars[3].active && sv->chars[3].flag2 && sv->chars[3].loc == 5) {
                    a = stats[3] - 11;
                    ai = 3;
                }
                break;
            case 6:
                if (sv->chars[4].active && sv->chars[4].flag2 && screen_w.chars[4].loc == 6) {
                    a = stats[4] - 11;
                    ai = 4;
                }
                break;
            case 7:
                if (sv->chars[4].active && sv->chars[4].flag2 && sv->chars[4].loc == 7) {
                    a = stats[4] - 11;
                    ai = 4;
                }
                break;
            case 8:
                if (sv->chars[5].active && sv->chars[5].flag2 && sv->chars[5].loc == 8 && screen_w.chars[0].loc != 8) {
                    a = stats[5] - 11;
                    ai = 5;
                }
                break;
            case 9:
                if (sv->chars[6].active && sv->chars[6].flag2 && sv->chars[6].loc == 9 && screen_w.chars[0].loc != 9) {
                    a = stats[6] - 11;
                    ai = 6;
                }
                break;
            case 10:
                if (sv->chars[10].active && sv->chars[10].flag2 && screen_w.chars[10].loc == 10) {
                    a = stats[10] - 11;
                    ai = 10;
                }
                break;
            }
        }
        break;
    case 5:
        if (screen_w.chars[8].active) {
            a = stats[8] - get_param_top_girl_tbl0[2];
            ai = 8;
        }
        cur = stats[0] - get_param_top_girl_tbl0[2];
        break;
    case 6:
        if (screen_w.chars[7].active) {
            a = stats[7] - get_param_top_girl_tbl0[4];
            ai = 7;
        }
        cur = stats[0] - get_param_top_girl_tbl0[4];
        break;
    case 7:
        break;
    }
    if (ai != 0 && bi != 0) {
        if (b < a) {
            if (cur < a && a > 0) {
                return ai;
            }
        } else {
            if (cur < b && b > 0) {
                return bi;
            }
        }
    } else if (ai != 0) {
        if (cur < a && a > 0) {
            return ai;
        }
    } else if (bi != 0) {
        if (cur < b && b > 0) {
            return bi;
        }
    }
    if (cur > 0) {
        return 0;
    }
    return 15;
}
