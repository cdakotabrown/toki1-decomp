/* @SymbolStart: 0x80136E24 */
/* @SymbolSize: 544 */
/* @CompileFlags: -Wo,-notail */
/* @CompileEnv: IDO52_COPYPROP_VAR=-24 IDO52_UGEN_TEMP_REG=19:t5,20:t6,21:t7,22:t8,23:t9,24:t0 IDO52_FORCE_WEB_COLOR=8:v1,74:v1,66:a1 IDO52_COPYPROP_SITE=lod:7:deny,lod:8:deny */
/* @PostProc: patch_insn_reg_field 0x174:rt:v0:v1 0x178:rs:v0:v1 */
/* Address: 0x80136E2C, Size: 536 bytes (134 instructions) */
#include "types.h"

extern void goto_sub_sub_step();
extern void cd_read_start2();
extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void master_serifu_set3(void);
extern void device_cmd_send(s32);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);

extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern volatile s16 now_serifu;
extern volatile s16 now_serifu_in;

extern u32 bukatu_pt6;
typedef struct { u8 pad0[2]; s16 f2; u8 pad4[50]; s16 f54; } Rec56;
extern Rec56 screen_w[];

extern s32 master_serifu_ptr[];
extern u8 master_cw_rank;
extern u8 master_talk_girl_no;
extern u8 now_sub_sub_step;

void master_girl_result1a(void) {
    s32 hidden_v1;
    u32 fval;
    u32 rot;
    s32 i;
    u32 cidx;
    Rec56 *p;

    hidden_v1 = now_sub_sub_step;

    switch ((u32)hidden_v1) {
    case 0:
        fval = bukatu_pt6;
        rot = (fval << 5) >> 30;
        if (rot < 3) {
            goto_sub_sub_step(2);
        } else {
            cd_read_start2(11, (void *)0x801E8000, 0xA9C5);
            sub_sub_step_up();
        }
        break;
    case 1:
        if (get_CD_State2() & 1) {
            master_serifu_set3();
            ss_serifu = master_serifu_ptr[0];
            ss_serifu_kind = master_serifu_ptr[1];
            sss_sound = master_serifu_ptr[2];
            now_serifu = 0;
            now_serifu_in = 0;
            sub_sub_step_up();
        }
        break;
    case 2:
        if ((u32)master_cw_rank >= 3) {
            for (i = 0; i < 13; i++) {
                screen_w[i + 7].f54 += 5;
            }
            p = screen_w;
            p[29].pad4[43] |= 0x40;
        }
        p = screen_w;
        cidx = master_talk_girl_no;
        if (cidx == 255) {
            sub_step_up();
            sub_step_up();
            sub_step_up();
        } else {
            p[cidx + 8].f2 += 2;
            device_cmd_send(512);
            if ((u32)master_cw_rank >= 3) {
                sub_step_up();
            } else {
                sub_step_up();
                sub_step_up();
            }
        }
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
