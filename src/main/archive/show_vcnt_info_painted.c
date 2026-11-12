/* Function: show_vcnt_info */
/* Address: 0x80050038 */
/* Size: 1804 bytes (451 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 48 --new 80 */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: custom_script custom_func_80050038.py */
#include "types.h"

extern u8 screen_w[];
extern u8 menu_mode;
extern s32 pad_data;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

extern void step_directional_scroll_once(void);
extern s32 nearest_menu_marker_find(s32, s32, s32);
extern s32 menu_sel_find(s32, s32);

void show_vcnt_info(s32 a0, s32 a1)
{
    s32 ch;
    u8 *base;
    u8 *base2;
    u8 *chp;
    u8 *ep;
    s32 dir;
    s32 limit;
    s32 one;
    s32 negone;
    s32 fwd;
    s32 v1;
    s32 v0;
    s32 buttons;
    s32 choff;
    s32 idx;

    base = screen_w;
    base2 = base;
    ch = a0;
    dir = 0;
    limit = 16;

    if (a1 & 0x100) {
        menu_mode++;
        negone = -1;
        *(s8 *)(base + ch + 4243) = negone;
        return;
    }

    v0 = menu_mode;
    one = 1;
    v0 &= 3;

    if (v0 == 0) {
        goto case0;
    }
    if (v0 == one) {
        goto case1;
    }
    if (v0 == 2) {
        goto case2;
    }
    if (v0 == 3) {
        buttons = a1 & 0xF000;
        goto case3;
    }
    return;

case0:
    step_directional_scroll_once();
    base = screen_w;
    chp = base + ch;
    if (chp[4240] == 0) {
        return;
    }
    v1 = 0;
    base = screen_w;
    choff = ch << 3;
    ep = base + choff;
    chp = base2 + ch;
    if (ep[3990] != 0) {
        *(s8 *)(chp + 4243) = v1;
        return;
    }
    do {
        v1++;
        ep += 16;
    } while ((u32)v1 < (u32)chp[4240] && ep[3990] == 0);
    return;

case1:
    v0 = pad_data;
    buttons = a1 & 0xF000;
    if ((v0 & 0xF) != 0) {
        chp = base + ch;
        return;
    }
    chp = base + ch;
    if (buttons == 0x1000) {
        v1 = *(s8 *)(chp + 4243);
        dir = -1;
        *(s8 *)(chp + 4243) = v1 - 1;
    } else if (buttons == 0x2000 || buttons == 0x4000) {
        v1 = *(s8 *)(chp + 4243);
        dir = 1;
        *(s8 *)(chp + 4243) = v1 + 1;
    } else if (buttons == (s32)0x8000) {
        v1 = *(s8 *)(chp + 4243);
        dir = -1;
        *(s8 *)(chp + 4243) = v1 - 1;
    }

    v1 = *(s8 *)(chp + 4243);
    choff = ch << 3;
    negone = -1;
    if (v1 >= (s32)(u32)chp[4240]) {
        *(s8 *)(chp + 4243) = 0;
    }
    v1 = *(s8 *)(chp + 4243);
    fwd = 1;
    if (v1 < 0) {
        *(s8 *)(chp + 4243) = chp[4240] - 1;
    }
    v1 = *(s8 *)(chp + 4243);

    ep = base + (v1 << 4) + choff;
    while (one == ep[3991]) {
        if (limit == 0) {
            goto upd1;
        }
        limit--;
        if (dir == fwd) {
            *(s8 *)(chp + 4243) = v1 + 1;
            v1 = *(s8 *)(chp + 4243);
        }
        if (dir == negone) {
            *(s8 *)(chp + 4243) = v1 - 1;
            v1 = *(s8 *)(chp + 4243);
        }
        if (v1 >= (s32)(u32)chp[4240]) {
            *(s8 *)(chp + 4243) = 0;
        }
        v1 = *(s8 *)(chp + 4243);
        if (v1 < 0) {
            *(s8 *)(chp + 4243) = chp[4240] - 1;
        }
        v1 = *(s8 *)(chp + 4243);
        chp = base2 + ch;
        idx = *(s8 *)(chp + 4243);
        ep = base2 + (idx << 4) + choff;
        if (one != ep[3991]) {
            break;
        }
    }
    ep = base + (v1 << 4) + choff;
upd1:
    cursor_cw_x = (s32)ep[3988] + *(s16 *)(ep + 3984) - 4;
    v1 = *(s8 *)(chp + 4243);
    ep = base + (v1 << 4) + choff;
    cursor_cw_y = *(s16 *)(ep + 3986) + 8;
    return;

case2:
    v0 = pad_data;
    if ((v0 & 4) == 0) {
        goto case2_reset;
    }
    if (a1 & 4) {
        chp = base + ch;
        choff = ch << 3;
        *(s8 *)(chp + 4243) = *(s8 *)(chp + 4245);
        v1 = *(s8 *)(chp + 4243);
        ep = base + (v1 << 4) + choff;
        goto upd2;
    }
    chp = base + ch;
    buttons = a1 & 0xF000;
    if (buttons == 0x1000) {
        v1 = *(s8 *)(chp + 4243);
        dir = 1;
        *(s8 *)(chp + 4243) = v1 - 1;
    } else if (buttons == 0x2000 || buttons == 0x4000) {
        v1 = *(s8 *)(chp + 4243);
        dir = -1;
        *(s8 *)(chp + 4243) = v1 + 1;
    } else if (buttons == (s32)0x8000) {
        v1 = *(s8 *)(chp + 4243);
        dir = 1;
        *(s8 *)(chp + 4243) = v1 - 1;
    }

    v1 = *(s8 *)(chp + 4243);
    choff = ch << 3;
    negone = -1;
    if (v1 >= (s32)(u32)chp[4240]) {
        *(s8 *)(chp + 4243) = 0;
    }
    v1 = *(s8 *)(chp + 4243);
    fwd = 1;
    if (v1 < 0) {
        *(s8 *)(chp + 4243) = chp[4240] - 1;
    }
    v1 = *(s8 *)(chp + 4243);

    ep = base + (v1 << 4) + choff;
    while (one == ep[3991]) {
        if (limit == 0) {
            goto upd2;
        }
        limit--;
        if (dir == fwd) {
            *(s8 *)(chp + 4243) = v1 + 1;
            v1 = *(s8 *)(chp + 4243);
        }
        if (dir == negone) {
            *(s8 *)(chp + 4243) = v1 - 1;
            v1 = *(s8 *)(chp + 4243);
        }
        if (v1 >= (s32)(u32)chp[4240]) {
            *(s8 *)(chp + 4243) = 0;
        }
        v1 = *(s8 *)(chp + 4243);
        if (v1 < 0) {
            *(s8 *)(chp + 4243) = chp[4240] - 1;
        }
        v1 = *(s8 *)(chp + 4243);
        chp = base2 + ch;
        idx = *(s8 *)(chp + 4243);
        ep = base2 + (idx << 4) + choff;
        if (one != ep[3991]) {
            break;
        }
    }
    ep = base + (v1 << 4) + choff;
upd2:
    cursor_cw_x = (s32)ep[3988] + *(s16 *)(ep + 3984) - 4;
    v1 = *(s8 *)(chp + 4243);
    ep = base + (v1 << 4) + choff;
    cursor_cw_y = *(s16 *)(ep + 3986) + 8;
    return;

case2_reset:
    step_directional_scroll_once();
    base = screen_w;
    chp = base + ch;
    if (chp[4240] == 0) {
        return;
    }
    v1 = 0;
    base = screen_w;
    choff = ch << 3;
    ep = base + choff;
    chp = base2 + ch;
    if (ep[3990] != 0) {
        *(s8 *)(chp + 4243) = v1;
        do {
            v1++;
            if ((u32)v1 >= (u32)chp[4240]) {
                return;
            }
            ep += 16;
        } while (ep[3990] != 0);
    }
    return;

case3:
    v1 = -1;
    if (buttons == 0x1000) {
        v1 = nearest_menu_marker_find(1, -1, ch);
        base = screen_w;
    } else if (buttons == 0x2000) {
        v1 = nearest_menu_marker_find(0, 1, ch);
        base = screen_w;
    } else if (buttons == 0x4000) {
        v1 = nearest_menu_marker_find(1, 1, ch);
        base = screen_w;
    } else if (buttons == (s32)0x8000) {
        v1 = nearest_menu_marker_find(0, -1, ch);
        base = screen_w;
    }

    negone = -1;
    chp = base + ch;
    if (v1 != negone) {
        *(s8 *)(base + ch + 4243) = v1;
    }
    v1 = *(s8 *)(chp + 4243);
    if (v1 >= (s32)(u32)chp[4240]) {
        v0 = menu_sel_find(ch, 1);
        *(s8 *)(chp + 4243) = v0;
        v1 = *(s8 *)(chp + 4243);
        base = screen_w;
    }
    if (v1 < 0) {
        v0 = menu_sel_find(ch, 0);
        *(s8 *)(chp + 4243) = v0;
        v1 = *(s8 *)(chp + 4243);
        base = screen_w;
    }
    choff = ch << 3;
    ep = base + (v1 << 4) + choff;
    cursor_cw_x = (s32)ep[3988] + *(s16 *)(ep + 3984) - 4;
    v1 = *(s8 *)(chp + 4243);
    ep = base + (v1 << 4) + choff;
    cursor_cw_y = *(s16 *)(ep + 3986) + 8;

    if (cursor_cw_x >= 141) {
        cursor_cw_x = 140;
    }
    if (cursor_cw_x < -150) {
        cursor_cw_x = -150;
    }
    if (cursor_cw_y < -100) {
        cursor_cw_y = -100;
    }
    if (cursor_cw_y >= 101) {
        cursor_cw_y = 100;
    }
}
