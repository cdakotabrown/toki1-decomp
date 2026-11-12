/* Address: 0x80042F20 */
/* Size: 864 bytes (216 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1, -cckr, leaf) */
/* @SymbolStart: 0x80042F20 */
/* @SymbolSize: 864 */
/* @CompileEnv: IDO52_COPYPROP_VAR=28@9,-20 IDO52_COPYPROP_SITE=lod:5:deny,lod:6:deny IDO52_FORCE_WEB_COLOR=4:a1,11:a0,58:a2,70:t1,119:t0 IDO52_UGEN_TEMP_REG=36:t5 IDO52_AS1_XBB_VOLATILE_OK=0 IDO52_AS1_TIE_SRC_ORDER=bb:7 IDO52_AS1_XBB_SKIP_CAND=18 IDO52_AS1_TIE_TAKE=142,578 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x348 --offset2 0x35c */
/* @PostProc: swap_branch_operands --offsets 0x230 */
/* @PostProc: patch_insn_reg_field 0x2e0:rd:t0:t5 0x320:rs:t0:t5 0x330:rs:t7:a0 0x330:rt:a0:t7 */
#include "types.h"

typedef struct {
    u32 pad;
    u8 pad4[7];
    u8 speed;
    u8 repeat;
    u8 cnt;
    u8 rkey;
    u8 delay;
    u8 key;
    u8 swap;
    u8 last;
    u8 fresh;
    u8 held;
    u8 pad15;
    s16 x;
    s16 y;
    u8 pad1A[2];
    u32 cur;
    u32 prev;
    u32 trig;
    u32 rel;
} Sys;

extern Sys mouse_pad_st;
extern u8 click_speed;
extern volatile u8 pad1_btn_hi;
extern volatile s8 pad1_ax;
extern volatile s8 pad1_ay;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8 pad2_buf;
extern u8 pad2_id;
extern volatile u8 pad2_btn_lo;
extern volatile u8 pad2_btn_hi;

void mouse_pad_set()
{
    s16 dx;
    s16 dy;
    u8 pressed;
    u32 prev;
    s32 spd;
    u32 np;
    s32 ax;
    s32 ay;
    s32 b2;
    s32 b3;

    spd = mouse_pad_st.speed;
    ax = pad1_ax;
    dx = ax * spd / 3;
    ay = pad1_ay;
    dy = ay * spd / 3;
    mouse_pad_st.x += dx;
    mouse_pad_st.y += dy;
    if (mouse_pad_st.x >= 141) {
        mouse_pad_st.x = 140;
    }
    if (mouse_pad_st.x < -150) {
        mouse_pad_st.x = -150;
    }
    if (mouse_pad_st.y < -100) {
        mouse_pad_st.y = -100;
    }
    if (mouse_pad_st.y >= 101) {
        mouse_pad_st.y = 100;
    }
    cursor_cw_x = mouse_pad_st.x;
    cursor_cw_y = mouse_pad_st.y;
    pressed = ~pad1_btn_hi;
    mouse_pad_st.fresh = pressed & ~mouse_pad_st.held;
    prev = mouse_pad_st.cur;
    if (mouse_pad_st.repeat != 0) {
        if (mouse_pad_st.cnt == 0 || mouse_pad_st.fresh != mouse_pad_st.rkey) {
            if (mouse_pad_st.fresh != 0) {
                if (mouse_pad_st.fresh & 4) {
                    mouse_pad_st.rkey = mouse_pad_st.fresh;
                    mouse_pad_st.cnt = click_speed + 8;
                } else if (mouse_pad_st.fresh & 8) {
                    mouse_pad_st.rkey = mouse_pad_st.fresh;
                    mouse_pad_st.cnt = click_speed + 8;
                }
            }
        }
    }
    mouse_pad_st.cur = 0;
    if (mouse_pad_st.repeat == 0 || (mouse_pad_st.repeat != 0 && mouse_pad_st.cnt != 0 && click_speed + 8 != mouse_pad_st.cnt && mouse_pad_st.fresh == mouse_pad_st.rkey)) {
        mouse_pad_st.rkey = 0;
        mouse_pad_st.cnt = 0;
        if (mouse_pad_st.swap != 0) {
            if (pressed & 4) {
                mouse_pad_st.cur |= 0x20;
            } else if (pressed & 8) {
                mouse_pad_st.cur |= 0x40;
            }
        } else {
            if (pressed & 8) {
                mouse_pad_st.cur |= 0x20;
            } else if (pressed & 4) {
                mouse_pad_st.cur |= 0x40;
            }
        }
    }
    if (mouse_pad_st.cnt != 0) {
        mouse_pad_st.cnt--;
    } else {
        mouse_pad_st.rkey = 0;
    }
    np = ~prev;
    if (pad2_buf == 0 && (pad2_id & 0xF0) == 0x40) {
        b3 = ~pad2_btn_hi;
        b2 = pad2_btn_lo;
        mouse_pad_st.cur |= (u8)~b2 << 24;
        mouse_pad_st.cur |= (u8)b3 << 16;
    }
    mouse_pad_st.trig = np & mouse_pad_st.cur;
    mouse_pad_st.rel = ~mouse_pad_st.cur & prev;
    mouse_pad_st.held = pressed;
    mouse_pad_st.prev = prev;
    mouse_pad_st.last = pressed;
}
