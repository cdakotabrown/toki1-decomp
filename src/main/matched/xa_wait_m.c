/* Address: 0x8005257C */
/* Size: 1260 bytes (315 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=29:t5,32:t8,35:t1,36:t2,33:t9,34:t0,30:t6,31:t7,37:t3 IDO52_FORCE_WEB_COLOR=11:v1,18:a0 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x284 --offset2 0x288 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x294 */
/* @PostProc: patch_insn_reg_field 0x50:rt:a0:a1 0x54:rs:a0:a1 0x54:rt:a0:a1 0x5c:rs:a0:a1 0x60:rs:v0:a0 0x60:rt:a0:v0 0x28c:rd:v0:t4 0x290:rt:v0:t4 0x294:rt:v0:t4 */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
} Pair;

typedef struct {
    s16 x;
    s16 y;
} Pos;

extern s32 get_now_k();
extern s32 k_disp_inc();
extern void k_disp_switch();
extern Pos *get_k_work();
extern u32 get_CD_State();
extern s32 *get_XAsector();
extern void set_XAsector();
extern void device_cmd_send();
extern void cd_command_issue();
extern void mes_arrow_anim();
extern void yazirushi_disp_switch();
extern void k_disp_goto_line_end();

typedef struct {
    u8 prev;
    u8 max;
    u8 top;
    u8 anim;
} FanList;
extern FanList now_girl;
extern u8 voice_level;
extern u8 yazirushi_cw_disp;
extern u8 step_mode;
extern s32 pad_trig;
extern s32 xa_m_mode;
extern Pair *xa_m_tbl;
extern Pair *xa_m_tbl2;
extern s32 xa_m_idx;

void xa_wait_m()
{
    s32 unused0, unused1;
    s32 ok;
    Pos *a;
    Pos *b;
    s32 *cur;
    s32 n;
    s32 x;
    s32 y;

    ok = 0;
    n = get_now_k();
    if (now_girl.anim & 0x80) {
        if (now_girl.top + 2 >= n) {
            ok = k_disp_inc();
            n = get_now_k();
            if (now_girl.top + 3 == n && now_girl.max >= now_girl.top + 3) {
                k_disp_switch(n, 0);
            }
        } else if ((now_girl.anim & 0x7F) == 0) {
            k_disp_switch(now_girl.top, 0);
            a = get_k_work(now_girl.top + 1);
            b = get_k_work(now_girl.top + 2);
            a->y -= 2;
            b->y -= 2;
            now_girl.anim++;
        } else if ((now_girl.anim & 0x7F) > 0) {
            a = get_k_work(now_girl.top + 1);
            b = get_k_work(now_girl.top + 2);
            a->y -= 2;
            b->y -= 2;
            if (a->y <= 50) {
                a->y = 50;
                b->y = 66;
                now_girl.anim--;
                now_girl.top++;
                k_disp_switch(now_girl.top + 2, 1);
            }
        }
    } else {
        if ((voice_level == 1 && xa_m_mode != 0) ||
            (voice_level == 2 && xa_m_mode != 0)) {
            ok = k_disp_inc();
        }
        if (voice_level == 0) {
            ok = k_disp_inc();
        }
        get_now_k();
    }
    if (xa_m_mode == 0 && (get_CD_State() & 4)) {
        xa_m_mode = 1;
    }
    cur = get_XAsector();
    if (xa_m_mode == 1 && (get_CD_State() & 2)) {
        if (xa_m_idx != -1) {
            x = xa_m_tbl[xa_m_idx].a;
            y = xa_m_tbl2[xa_m_idx].a;
            if (x != y && x == cur[2]) {
                set_XAsector(y, xa_m_tbl2[xa_m_idx].b, now_girl.prev);
                device_cmd_send(0x300);
                xa_m_mode = 0;
            } else {
                xa_m_mode = 2;
            }
        } else {
            xa_m_mode = 2;
        }
    }
    if (ok == 1 && xa_m_mode == 2) {
        if (yazirushi_cw_disp & 0x80) {
            mes_arrow_anim();
        } else {
            yazirushi_disp_switch(1);
        }
    }
    if (ok == 1 && voice_level == 0) {
        if (yazirushi_cw_disp & 0x80) {
            mes_arrow_anim();
        } else {
            yazirushi_disp_switch(1);
        }
    }
    if (pad_trig & 0x40) {
        if (ok == 1) {
            if (voice_level == 2) {
                xa_m_mode = 2;
                device_cmd_send(0x74);
                cd_command_issue(9, 0, 0);
            }
            now_girl.anim = 0;
        } else {
            k_disp_goto_line_end();
        }
    }
    if (pad_trig & 0x60) {
        if (ok == 1) {
            if (voice_level == 1) {
                if (xa_m_mode == 2) {
                    yazirushi_disp_switch(0);
                    step_mode++;
                }
                now_girl.anim = 0;
            } else if (voice_level == 2) {
                if (xa_m_mode == 2) {
                    yazirushi_disp_switch(0);
                    step_mode++;
                }
                now_girl.anim = 0;
            } else {
                yazirushi_disp_switch(0);
                step_mode++;
                now_girl.anim = 0;
            }
        } else {
            k_disp_goto_line_end();
        }
    }
}
