/* @RodataBase: 0x80134204 */
/* @DataBase: 0x80134368 */
/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 808 */
/* @PostProc: patch_insn_reg_field 0x68:rt:v1:v0 0xcc:rt:a1:a0 0xdc:rt:a1:a0 0xe0:rs:a1:a0 0xec:rs:v1:v0 0xf0:rt:v0:v1 0xf4:rt:a2:a1 0xf8:rt:v0:v1 0xf8:rs:v0:v1 0xfc:rt:v0:v1 0x108:rd:v0:v1 0x110:rt:a2:a1 0x110:rs:a2:a1 0x118:rt:a2:a1 0x118:rd:v1:v0 0x120:rt:t6:v0 0x120:rs:v1:t6 0x128:rs:v1:v0 0x130:rt:v0:v1 0x134:rt:v0:v1 0x138:rd:v0:v1 0x140:rs:v1:v0 0x148:rs:v0:v1 0x148:rt:v0:v1 0x14c:rt:v0:v1 0x150:rd:v0:v1 0x158:rt:v0:v1 0x184:rt:v0:v1 0x188:rs:v0:v1 0x188:rt:t8:t6 0x194:rt:t8:t6 0x198:rs:a1:a0 0x1a0:rd:t6:t7 0x1a4:rd:t7:t0 0x1a4:rs:v1:v0 0x1a4:rt:t6:t7 0x1a8:rs:t7:t0 0x1a8:rt:v0:v1 0x1b0:rs:v0:v1 0x1b0:rt:v0:v1 0x1b4:rt:v0:v1 0x1b4:rd:t0:t1 0x1b8:rt:t0:t1 0x1b8:rd:t1:t2 0x1c0:rd:v0:v1 0x1c0:rs:t1:t2 0x1c4:rs:t1:t2 0x1c4:rt:v0:v1 0x1c8:rt:v0:v1 0x1c8:rd:t2:t3 0x1cc:rd:v0:v1 0x1cc:rt:t2:t3 0x1d4:rt:v0:v1 0x1e0:rt:t5:t9 0x1e0:rs:v0:v1 0x1e8:rt:t5:t9 0x20c:rt:t4:t8 0x210:rs:t4:t8 0x210:rt:t4:t8 0x214:rt:a1:a0 0x218:rs:t4:t8 0x224:rs:a1:a0 0x224:rt:a1:a0 0x22c:rs:a1:a0 0x230:rt:t8:v1 0x234:rt:t9:v1 0x240:rt:t9:v1 0x24c:rt:t8:v1 0x26c:rt:t0:v1 0x278:rt:t0:v1 0x27c:rt:t1:v1 0x284:rt:t1:v1 0x298:rt:a1:a0 0x29c:rt:a1:a0 0x29c:rs:a1:a0 0x2a4:rs:a1:a0 0x2cc:rt:a1:a0 0x2d0:rs:a1:a0 0x2d0:rt:a1:a0 0x2d8:rs:a1:a0 */
#include "types.h"

extern void bustup_ani(void);
extern void device_port_init(u32 a0, u32 a1);
extern void zyotai_sprite_set_chk(void);
extern void indexed_byte_toggle(u32 a0);
extern void fade_in(void);
extern void serifu_run(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(u32 a0);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);
extern void fade_down_main(void);
extern void fade_out_step(void);
extern void fade_out(void);

typedef struct {
    u16 hwords[13];
} StructCopy26;


extern u8  now_sub_sub_step;
extern u8  rensyu_succ_cnt;
extern u8  rensyu_girl_no;
extern u8  girl4_basyo;
extern u16 basyo_pack;
extern u8  girl6_basyo;
extern u8  now_year;
extern u8  now_month;
extern volatile s16 now_serifu;

void rensyu_girl_reply_main(void)
{
    u8 skill;
    u8 t2;
    s16 v1;
    StructCopy26 local = { {
        41, 45, 49, 53, 57, 63, 67, 72, 76, 80, 84, 88, 90
    } };
    u32 val;
    u32 t6;
    u8 a0_idx;
    s16 *base;

    bustup_ani();

    skill = now_sub_sub_step;

    switch (skill) {
    case 0:
        device_port_init(1, 0);
        zyotai_sprite_set_chk();
        indexed_byte_toggle(1);
        break;
    case 1:
        fade_in();
        break;
    case 2:
        t2 = rensyu_succ_cnt;
        base = (s16 *)&local;
        if ((u32)t2 >= 3U) {
            a0_idx = rensyu_girl_no;
            v1 = (s16)(base[a0_idx] + 2);
            if (a0_idx == 4) {
                t6 = girl4_basyo;
                val = basyo_pack;
                if ((val >> 12) != (t6 >> 4)) {
                    if ((t6 >> 4) == 7) {
                        v1 = (s16)(v1 + 1);
                    }
                    if ((t6 >> 4) == 6) {
                        v1 = (s16)(v1 + 2);
                    }
                }
            }
            now_serifu = v1;
            if (a0_idx == 6) {
                t6 = girl6_basyo;
                val = basyo_pack;
                now_serifu = v1;
                if ((t6 >> 4) != (val >> 12)) {
                    now_serifu = v1 + 1;
                }
            }
        } else {
            a0_idx = rensyu_girl_no;
            v1 = (s16)(base[a0_idx] + 3);
            if (a0_idx == 4) {
                v1 = (s16)(v1 + 2);
            }
            now_serifu = v1;
            if (a0_idx == 6) {
                now_serifu = v1 + 1;
            }
        }
        sub_sub_step_up();
        break;
    case 3:
    case 6:
        serifu_run();
        break;
    case 4:
        t2 = rensyu_succ_cnt;
        if ((u32)t2 >= 3U) {
            a0_idx = rensyu_girl_no;
            if (a0_idx == 12) {
                now_serifu = 38;
            } else {
                now_serifu = 37;
            }
        } else {
            t6 = now_year;
            if (t6 == 97) {
                t6 = now_month;
                if (t6 == 12) {
                    now_serifu = 40;
                    goto done4;
                }
            }
            now_serifu = 39;
        }
done4:
        sub_sub_step_up();
        break;
    case 5:
        a0_idx = rensyu_girl_no;
        if (a0_idx == 12) {
            fade_down_main();
        } else {
            sub_sub_step_up();
        }
        break;
    case 7:
        a0_idx = rensyu_girl_no;
        if (a0_idx == 12) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 8:
        device_cmd_send(193);
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
