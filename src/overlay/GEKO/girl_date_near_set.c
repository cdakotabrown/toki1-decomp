/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:5:deny,lod:7:deny IDO52_COPYPROP_VAR=-72 IDO52_FORCE_WEB_COLOR=43:a0,49:a1,55:a2,64:a2,69:a1,109:a3,115:t0,59:v1 */
/* @SymbolStart: 0x80132B24 */
/* @SymbolSize: 480 */
/* @PostProc: patch_insn_reg_field 0x188:rt:t4:t6 0x198:rs:t4:t6 0x1a0:rd:t2:t4 0x1a8:rd:t1:t7 0x1a8:rt:t2:t4 0x1bc:rd:t7:t9 0x1c0:rd:t6:t3 0x1c0:rt:t7:t9 0x1c4:rd:t9:t2 0x1c4:rt:t6:t3 0x1c8:rs:t9:t2 0x1c8:rt:t3:t1 0x1d0:rs:t3:t7 */
/* girl_date_near_set - GEKO overlay, Address: 0x80132B30 (true start 0x80132B24) */
#include "types.h"

typedef struct {
    s16 h[11];
} SrcA;
typedef struct {
    s16 h[12];
} SrcB;

extern SrcA girl_date_near_set_tbl0;
extern SrcB girl_date_near_set_tbl1;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_girl;
extern s32 frame_counter;
extern u8 geko_serifu_state;
extern u8 screen_w[];

s32 girl_date_near_set(void)
{
    u32 base;
    SrcA dst;
    SrcB tbl;
    u8 *rec;
    s32 w460;
    u32 slot;
    u32 sel;
    u32 d;

    dst = girl_date_near_set_tbl0;
    tbl = girl_date_near_set_tbl1;
    if (now_year == 96) {
        tbl.h[1] = 29;
    }
    rec = screen_w + now_girl * 56;
    if (((u32)(*(s32 *)(rec + 456) << 25) >> 31) == 0) {
        return 0;
    }
    if ((u32)(frame_counter & 7) >= (u32)dst.h[now_girl]) {
        if (geko_serifu_state != 0) {
            return;
        }
    }
    w460 = *(s32 *)(rec + 460);
    sel = now_month;
    slot = w460 & 0xF;
    if (sel == slot) {
        base = now_day;
        d = ((u32)(w460 << 23) >> 27) - base;
        if (d >= 21) {
            return;
        }
        if (d == 0) {
            return;
        }
        geko_serifu_state = 10;
        return;
    }
    if ((slot - sel + 12) % 12 != 1) {
        return;
    }
    base = now_day;
    if (((u32)(w460 << 23) >> 27) + tbl.h[(sel + 10) % 12] - base < 20) {
        geko_serifu_state = 10;
    }
}
