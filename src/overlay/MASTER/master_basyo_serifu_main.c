/* @SymbolStart: 0x80132444 */
/* @SymbolSize: 624 */
/* @RodataBase: 0x8013BA54 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x200:rt:t5:t1 0x204:rs:t5:t1 0x204:rt:t5:t1 0x20c:rt:t6:t2 0x210:rt:t5:t1 0x214:rs:t6:t2 0x214:rt:t6:t2 0x21c:rs:t6:t2 0x21c:rt:t7:t3 0x220:rt:t8:v1 0x224:rt:t7:t3 0x228:rs:t8:v1 0x238:rt:t9:t0 0x23c:rt:t9:t0 */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void master_basyo0_serifu_ptr_set(void);
extern void master_basyo1_serifu_ptr_set(void);
extern void master_basyo2_serifu_ptr_set(void);
extern void master_basyo3_serifu_ptr_set(void);
extern void master_basyo4_serifu_ptr_set(void);
extern void master_basyo5_serifu_ptr_set(void);
extern void game_reset(void);
extern void sub_sub_step_up(void);
extern s32  get_CD_State2(void);
extern void serifu_run(void);
extern void fade_out_step(void);
extern void device_cmd_send(s32);
extern void goto_step(s32);
extern void sub_step_up(void);
extern void common_disp(void);

extern u16 basyo_pack;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 master_basyo_serifu_ptr[];
extern s16 now_serifu;
extern s16 now_serifu_in;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  taco_rec;
extern u8 now_sub_sub_step;

void master_basyo_serifu_main(void)
{
    s32 sel;

    sel = now_sub_sub_step;
    switch (sel) {
    case 0:
        switch ((u32)basyo_pack >> 12) {
        case 0:
            cd_read_start2(3, 0x801E8000, 0xA9DE);
            master_basyo0_serifu_ptr_set();
            break;
        case 1:
            cd_read_start2(3, 0x801E8000, 0xA9B4);
            master_basyo1_serifu_ptr_set();
            break;
        case 2:
            cd_read_start2(3, 0x801E8000, 0xA9BF);
            master_basyo2_serifu_ptr_set();
            break;
        case 3:
            cd_read_start2(3, 0x801E8000, 0xA9B7);
            master_basyo3_serifu_ptr_set();
            break;
        case 4:
            cd_read_start2(3, 0x801E8000, 0xA9BC);
            master_basyo4_serifu_ptr_set();
            break;
        case 5:
            cd_read_start2(3, 0x801E8000, 0xA9C2);
            master_basyo5_serifu_ptr_set();
            break;
        default:
            game_reset();
            break;
        }
        ss_serifu = master_basyo_serifu_ptr[0];
        ss_serifu_kind = master_basyo_serifu_ptr[1];
        sss_sound = master_basyo_serifu_ptr[2];
        sub_sub_step_up();
        break;
    case 1:
        if (get_CD_State2() & 1) {
            now_serifu = 0;
            now_serifu_in = 0;
            sub_sub_step_up();
        }
        break;
    case 2:
        serifu_run();
        break;
    case 3:
        fade_out_step();
        break;
    case 4:
        if ((u32)basyo_pack >> 12 == 3) {
            device_cmd_send(191);
            ret_step = now_step;
            ret_sub_step = now_sub_step + 1;
            ret_sub_sub_step = 0;
            taco_rec = taco_rec | 3;
            goto_step(144);
        } else {
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
