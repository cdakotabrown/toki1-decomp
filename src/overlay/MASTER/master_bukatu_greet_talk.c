/* @CompileFlags: -Wc,-pic0 */
/* @RodataBase: 0x8013C154 */
/* master_bukatu_greet_talk - MASTER overlay, Address: 0x80139910, Size: 952 bytes (238 insns) */
/* @SymbolSize: 952 */
/* @PostProc: patch_insn_reg_field 0x32c:rs:t7:t9 0xf0:rt:t8:v1 0xf4:rs:t8:v1 0xf4:rt:t8:v1 0xfc:rs:t8:v1 0x10c:rt:t6:t8 0x110:rs:t6:t8 0x110:rt:t6:t8 0x118:rd:t7:t6 0x118:rt:t6:t8 0x11c:rs:t7:t6 0x13c:rt:t0:t7 0x140:rs:t0:t7 0x144:rt:t1:t0 0x148:rs:t1:t0 0x148:rt:t1:t0 0x150:rd:t2:t1 0x150:rt:t1:t0 0x154:rs:t2:t1 0x158:rt:t3:t2 0x15c:rs:t3:t2 0x15c:rt:t3:t2 0x164:rd:t4:t3 0x164:rt:t3:t2 0x168:rs:t4:t3 0x184:rt:t5:t4 0x18c:rt:t5:t4 0x1a8:rt:t9:t5 0x1ac:rs:t9:t5 0x1ac:rt:t9:t5 0x1b4:rd:t8:t9 0x1b4:rt:t9:t5 0x1b8:rs:t8:t9 0x1d8:rt:t6:v1 0x1dc:rs:t6:v1 0x1dc:rt:t6:v1 0x1e4:rs:t6:v1 0x1e8:rt:t7:t8 0x1ec:rs:t7:t8 0x1ec:rt:t7:t8 0x1f4:rs:t7:t8 0x21c:rt:t0:t6 0x220:rs:t0:t6 0x220:rt:t0:t6 0x228:rs:t0:t6 0x260:rt:v0:v1 0x264:rs:v0:v1 0x264:rt:v0:v1 0x26c:rs:v0:v1 0x270:rt:t1:t7 0x274:rs:t1:t7 0x274:rt:t1:t7 0x278:rt:t5:t3 0x27c:rd:t2:t0 0x27c:rt:t1:t7 0x280:rd:t3:t1 0x280:rt:t2:t0 0x284:rs:t3:t1 0x28c:rt:t4:t2 0x298:rt:t4:t2 0x29c:rt:t5:t3 0x2b4:rs:v0:v1 0x2b8:rt:t8:t5 0x2bc:rt:t9:t4 0x2c8:rt:t9:t4 0x2dc:rt:t8:t5 0x2e8:rt:t6:v1 0x2ec:rs:t6:v1 0x2ec:rt:t6:v1 0x2f4:rs:t6:v1 0x328:rd:t7:t9 0x338:rs:t7:t9 0x344:rs:t7:t9 0x350:rs:t7:t9 */
#include "types.h"

typedef struct {
    s16 v[13];
} Table26;

extern Table26 master_talk2_tbl;
extern u8 now_sub_sub_step;
extern u8 master_bukatu_girl_no;
extern u8 now_girl;
extern u16 basyo_pack;
extern s32 girl11_flag;
extern u8 master_talk2_str0;
extern s16 mes_base_y;
extern s16 now_serifu;
extern s16 now_serifu_in;

extern void bustup_ani(void);
extern s32 get_CD_State2(void);
extern void date_name_set(s32);
extern void bustup_ptr_init(void);
extern void zyotai_sprite_set_chk(void);
extern void bustup_face_change(s32);
extern void value_cache_find_or_load(s32, s32);
extern void k_reset(s32);
extern void basyo_init(void *);
extern void dec_bg_init(s32);
extern void sub_sub_step_up(void);
extern void sprite_fade_main(void);
extern void device_cmd_send(s32);
extern void don_init(void);
extern void serifu_run(void);
extern void device_port_init(s32, s32);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);

void master_bukatu_greet_talk(void) {
    u8 state;
    u8 sb;
    s16 unused_hi;
    Table26 buf;
    u8 unused_lo[12];

    buf = master_talk2_tbl;
    bustup_ani();
    state = now_sub_sub_step;

    switch (state) {
    case 0:
        if (get_CD_State2() & 1) {
            date_name_set((s32)master_bukatu_girl_no);
            sb = master_bukatu_girl_no;
            now_serifu = buf.v[sb];
            now_serifu_in = 0;
            bustup_ptr_init();
            now_girl = master_bukatu_girl_no;
            zyotai_sprite_set_chk();
            if (master_bukatu_girl_no == 12) {
                bustup_face_change(0);
            }
            if (((u32)basyo_pack >> 12) != 7) {
                value_cache_find_or_load(16835, 1);
            }
        }
        break;
    case 1:
        if ((get_CD_State2() & 1) || ((u32)basyo_pack >> 12) == 7) {
            if (((u32)basyo_pack >> 12) != 7) {
                k_reset(0);
                basyo_init(&master_talk2_str0);
                mes_base_y = 192;
            }
            dec_bg_init(1);
            sub_sub_step_up();
        }
        break;
    case 2:
        if (((u32)basyo_pack >> 12) == 7) {
            sub_sub_step_up();
        } else {
            sprite_fade_main();
        }
        if (master_bukatu_girl_no == 11 && now_sub_sub_step == 3) {
            device_cmd_send(1280);
            don_init();
        }
        break;
    case 3:
        serifu_run();
        if (now_sub_sub_step == 4) {
            device_port_init(0, 0);
        }
        break;
    case 5:
    case 7:
        serifu_run();
        break;
    case 4:
        fade_up_main();
        break;
    case 6:
        if (master_bukatu_girl_no == 11) {
            if (((u32)(girl11_flag << 20) >> 29) == 0) {
                now_serifu = 30;
            } else {
                now_serifu = 31;
            }
            fade_down_main();
        } else if (master_bukatu_girl_no == 12) {
            now_serifu = 32;
            sub_sub_step_up();
        } else {
            now_serifu = 29;
            sub_sub_step_up();
        }
        break;
    case 8:
        if (master_bukatu_girl_no == 11) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 9:
        switch ((u32)basyo_pack >> 12) {
        case 7:
            device_port_init(1, 7);
            break;
        case 8:
            device_port_init(1, 9);
            break;
        case 9:
            device_port_init(1, 8);
            break;
        case 10:
            device_port_init(1, 1);
            break;
        default:
            break;
        }
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }

    common_disp();
}
