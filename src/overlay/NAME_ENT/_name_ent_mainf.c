/* @SymbolStart: 0x80149D70 */
/* @SymbolSize: 1944 */
/* @RodataBase: 0x8014CAD8 */
/* @PostProc: remap_stack_slots --from 64 --to 56 --from 192 --to 184 --keep-frame */
/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=8,9 IDO52_FORCE_WEB_COLOR=2:a0 */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x3ec --offset2 0x3fc */
/* @PostProc: swap_instructions --offset1 0x3f0 --offset2 0x3fc */
/* @PostProc: swap_instructions --offset1 0x3f4 --offset2 0x3fc */
/* @PostProc: patch_insn_reg_field 0x44:rt:a1:v1 0x48:rs:a1:v1 0x48:rt:a1:v1 0x4c:rt:t8:t9 0x50:rs:a1:v1 0x58:rs:a1:v1 0x58:rt:t7:t8 0x60:rt:t7:t8 0x68:rt:t7:t8 0x74:rt:t8:t9 0x78:rt:t9:a0 0x7c:rs:t9:a0 0x7c:rt:t9:a0 0x84:rs:t9:a0 0x90:rt:a1:v1 0x94:rs:a1:v1 0x94:rt:a1:v1 0x9c:rs:a1:v1 0xa4:rs:a1:v1 0xa4:rt:t1:t2 0xac:rt:t1:t2 0xb4:rt:t1:t2 0xd0:rt:t2:t3 0xdc:rt:t2:t3 0xe0:rt:t3:a0 0xe4:rs:t3:a0 0xe4:rt:t3:a0 0xec:rs:t3:a0 0x110:rt:t5:a0 0x114:rs:t5:a0 0x114:rt:t5:a0 0x11c:rs:t5:a0 0x11c:rt:t6:t5 0x124:rt:t6:t5 0x128:rt:t7:t6 0x12c:rs:t7:t6 0x12c:rt:t7:t6 0x130:rt:t9:t8 0x134:rs:t7:t6 0x134:rt:t8:t7 0x138:rs:t8:t7 0x144:rt:t9:t8 0x148:rt:t0:t9 0x154:rt:t0:t9 0x158:rt:t1:a0 0x15c:rs:t1:a0 0x15c:rt:t1:a0 0x164:rs:t1:a0 0x164:rt:t2:t0 0x16c:rt:t2:t0 0x188:rt:t3:t1 0x194:rt:t3:t1 0x198:rt:t4:a0 0x19c:rs:t4:a0 0x19c:rt:t4:a0 0x1a4:rs:t4:a0 0x1a4:rt:t5:t2 0x1ac:rt:t5:t2 0x1c8:rt:t6:t3 0x1d4:rt:t6:t3 0x1d8:rt:t7:a0 0x1dc:rs:t7:a0 0x1dc:rt:t7:a0 0x1e4:rs:t7:a0 0x1e4:rt:t8:t4 0x1ec:rt:t8:t4 0x204:rt:t0:t6 0x20c:rt:t0:t6 0x23c:rt:v0:v1 0x240:rt:v1:v0 0x244:rs:v0:v1 0x244:rt:v0:v1 0x248:rs:v0:v1 0x24c:rs:v0:v1 0x250:rs:v0:v1 0x254:rs:v0:v1 0x254:rt:v1:v0 0x258:rs:v0:v1 0x25c:rt:t1:a3 0x260:rs:t1:a3 0x260:rt:t1:a3 0x268:rs:t1:a3 0x268:rt:t2:t7 0x26c:rd:t3:t8 0x26c:rt:t2:t7 0x270:rt:t3:t8 0x294:rt:v0:a3 0x298:rs:v0:a3 0x298:rt:v0:a3 0x2a0:rd:t4:v1 0x2a0:rt:v0:a3 0x2a4:rd:t5:t9 0x2a4:rt:t4:v1 0x2a8:rs:t5:t9 0x2b0:rs:t5:t9 0x2b8:rs:t5:t9 0x2c8:rd:t6:t0 0x2cc:rd:t7:t1 0x2cc:rt:t6:t0 0x2d0:rt:t7:t1 0x2e0:rt:v1:a3 0x2e4:rs:v1:a3 0x2e4:rt:v1:a3 0x2ec:rd:t8:v1 0x2ec:rt:v1:a3 0x2f0:rd:t9:t2 0x2f0:rt:t8:v1 0x2f4:rs:t9:t2 0x30c:rd:t0:t3 0x310:rd:t1:t4 0x310:rt:t0:t3 0x314:rt:t1:t4 0x348:rt:t2:t5 0x350:rt:t2:t5 0x35c:rt:t3:a0 0x360:rs:t3:a0 0x360:rt:t3:a0 0x368:rs:t3:a0 0x368:rt:t4:t6 0x370:rt:t4:t6 0x38c:rt:t5:t7 0x398:rt:t5:t7 0x39c:rt:t6:a0 0x3a0:rs:t6:a0 0x3a0:rt:t6:a0 0x3a8:rs:t6:a0 0x3a8:rt:t7:t8 0x3b0:rt:t7:t8 0x3e0:rt:v0:v1 0x3e4:rt:v1:v0 0x3e8:rs:v0:v1 0x3e8:rt:v0:v1 0x3ec:rs:v0:v1 0x3f0:rs:v0:v1 0x3f4:rs:v0:v1 0x3f8:rs:v0:v1 0x3f8:rt:v1:v0 0x3fc:rs:v0:v1 0x400:rt:t8:a3 0x404:rs:t8:a3 0x404:rt:t8:a3 0x40c:rt:t8:a3 0x460:rt:t3:a0 0x464:rs:t3:a0 0x464:rt:t3:a0 0x46c:rs:t3:a0 0x46c:rt:t4:t3 0x474:rt:t4:t3 0x490:rt:t5:t4 0x49c:rt:t5:t4 0x4a0:rt:t6:a0 0x4a4:rs:t6:a0 0x4a4:rt:t6:a0 0x4ac:rs:t6:a0 0x4ac:rt:t7:t5 0x4b4:rt:t7:t5 0x4d0:rt:t8:t6 0x4dc:rt:t8:t6 0x4e0:rt:t9:a0 0x4e4:rs:t9:a0 0x4e4:rt:t9:a0 0x4ec:rs:t9:a0 0x4ec:rt:t0:t7 0x4f4:rt:t0:t7 0x510:rt:t1:t8 0x51c:rt:t1:t8 0x520:rt:t2:a0 0x524:rs:t2:a0 0x524:rt:t2:a0 0x52c:rs:t2:a0 0x52c:rt:t3:t9 0x534:rt:t3:t9 0x550:rt:t4:t0 0x55c:rt:t4:t0 0x560:rt:t5:a0 0x564:rs:t5:a0 0x564:rt:t5:a0 0x56c:rs:t5:a0 0x56c:rt:t6:t1 0x574:rt:t6:t1 0x588:rt:a1:v1 0x58c:rs:a1:v1 0x58c:rt:a1:v1 0x590:rt:t8:t5 0x594:rs:a1:v1 0x59c:rs:a1:v1 0x59c:rt:t7:t4 0x5a4:rt:t7:t4 0x5ac:rt:t7:t4 0x5b4:rt:t7:t4 0x5bc:rt:t7:t4 0x5c4:rt:t7:t4 0x5d0:rt:t7:t4 0x5dc:rt:t8:t5 0x5e0:rt:t9:t6 0x5ec:rt:t9:t6 0x5f0:rt:t0:a0 0x5f4:rs:t0:a0 0x5f4:rt:t0:a0 0x5fc:rs:t0:a0 0x5fc:rt:t1:t7 0x604:rt:t1:t7 0x618:rt:t2:t8 0x624:rt:t2:t8 0x628:rt:t3:a0 0x62c:rs:t3:a0 0x62c:rt:t3:a0 0x634:rs:t3:a0 0x634:rt:t4:t9 0x63c:rt:t4:t9 0x65c:rt:t5:a0 0x660:rs:t5:a0 0x660:rt:t5:a0 0x668:rs:t5:a0 0x668:rt:t6:t0 0x670:rt:t6:t0 0x67c:rt:t7:t1 0x680:rs:t7:t1 0x698:rt:t8:a0 0x69c:rs:t8:a0 0x69c:rt:t8:a0 0x6a4:rs:t8:a0 0x6a4:rt:t9:t2 0x6ac:rt:t9:t2 0x6b8:rt:t0:t3 0x6bc:rs:t0:t3 0x6d4:rt:t1:a0 0x6d8:rs:t1:a0 0x6d8:rt:t1:a0 0x6e0:rs:t1:a0 0x6e0:rt:t2:t4 0x6e8:rt:t2:t4 0x6f4:rt:t3:t5 0x6f8:rs:t3:t5 0x714:rt:t4:t6 0x718:rs:t4:t6 0x718:rt:t4:t6 0x720:rs:t4:t6 */
/* _name_ent_mainf - NAME_ENT overlay, Address: 0x80149D78, */
/* Size: 1936 bytes (484 insns). */
#include "types.h"

extern s16 cal_cw_x;
extern s16 cw97_x;
extern s16 cw98_x;
extern s16 cw99_x;
extern s16 cw100_x;
extern s16 cw101_x;
extern s16 cw100_y;
extern u8  cw99_anim_flag;
extern u8  name_ent_serifu_no;
extern u8  step_mode;
extern u8  now_girl;
extern u32 girl0_birth;
extern s32 dec_bg_handle;
extern u8 *album_info_disp_tbl0[];
extern u8 *name_ent_blood_str_tbl[];
extern u8  _name_ent_mainf_str0[];
extern u8  _name_ent_mainf_str1[];
extern u8  _name_ent_mainf_str2[];
extern u8  p_sei[];

extern void name_ent_serifu_set(void);
extern void name_ent_text_scroll(void);
extern void name_ent_kuchi_anif(void);
extern void name_ent_cw99_setf(void);
extern void name_ent_obj2_init(void);
extern void name_ent_flag_set(void);
extern void k_sub_reset(void);
extern void gnsx(u8 *p);
extern void strcpy(u8 *dst, u8 *src);
extern void strncat(u8 *dst, u8 *src, s32 n);
extern void sndisp(u8 *p, s32 a, s32 b);
extern void k_sub_disp_start(s32 a);
extern void value_cache_find_or_load(s32 a, s32 b);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32 a, s32 b, s32 c);
extern s32  dec_bg_init(s32 a);
extern void indexed_byte_lsb_set(s32 a, s32 b);
extern void dec_bg_reset(void);
extern void class_clear_f(void);
extern void name_ent_bg_set(s32 a, s32 b);
extern void memcpy(s32 a, s32 b, s32 c);
extern void name_ent_cal_cw_set(void);
extern void name_ent_cw99_set0(void);
extern void sub_step_up(void);
extern void dec_bg_show(s32 a);

void _name_ent_mainf(void)
{
    u8 buf[128];
    s32 i;
    s32 v;
    u32 n;

    switch (step_mode) {
    case 0:
        v = cal_cw_x;
        if (v < -160) {
            cw97_x = v + 1;
            cal_cw_x = v + 1;
        }
        name_ent_serifu_no = 20;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 1:
        v = cal_cw_x;
        if (v < -160) {
            cw97_x = v + 1;
            cal_cw_x = v + 1;
        }
        name_ent_text_scroll();
        name_ent_kuchi_anif();
        break;
    case 2:
        name_ent_serifu_no = 21;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 3:
        name_ent_text_scroll();
        break;
    case 4:
        name_ent_cw99_setf();
        step_mode = step_mode + 1;
        break;
    case 5:
        if ((cw99_anim_flag & 1) == 0) {
            step_mode = step_mode + 1;
        }
        break;
    case 6:
        name_ent_serifu_no = 22;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 7:
        name_ent_text_scroll();
        name_ent_kuchi_anif();
        break;
    case 8:
        name_ent_serifu_no = 23;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 9:
        name_ent_text_scroll();
        break;
    case 10:
        name_ent_obj2_init();
        name_ent_serifu_no = 24;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 11:
        v = cw100_y;
        if (v >= -119) {
            cw100_y = v - 1;
        }
        name_ent_text_scroll();
        name_ent_kuchi_anif();
        break;
    case 12:
        k_sub_reset();
        gnsx(p_sei);
        for (i = 0; i < 128; i++) {
            buf[i] = 0;
        }
        strcpy(buf, album_info_disp_tbl0[girl0_birth & 0xF]);
        strncat(buf, _name_ent_mainf_str0, 2);
        n = ((u32)girl0_birth << 23) >> 27;
        if (n >= 10) {
            strncat(buf, album_info_disp_tbl0[n / 10], 2);
            n = ((u32)girl0_birth << 23) >> 27;
        }
        strncat(buf, album_info_disp_tbl0[n % 10], 2);
        strncat(buf, _name_ent_mainf_str1, 4);
        sndisp(buf, 0, 15);
        now_girl = 15;
        k_sub_disp_start(1);
        step_mode = step_mode + 1;
        break;
    case 13:
        name_ent_text_scroll();
        name_ent_flag_set();
        break;
    case 14:
        name_ent_serifu_no = 25;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 15:
        name_ent_text_scroll();
        name_ent_kuchi_anif();
        break;
    case 16:
        k_sub_reset();
        gnsx(p_sei);
        for (i = 0; i < 128; i++) {
            buf[i] = 0;
        }
        strcpy(buf, name_ent_blood_str_tbl[((u32)girl0_birth << 21) >> 30]);
        strncat(buf, _name_ent_mainf_str2, 4);
        sndisp(buf, 0, 15);
        now_girl = 15;
        k_sub_disp_start(1);
        step_mode = step_mode + 1;
        break;
    case 17:
        name_ent_text_scroll();
        name_ent_flag_set();
        break;
    case 18:
        name_ent_serifu_no = 26;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 19:
        name_ent_text_scroll();
        name_ent_kuchi_anif();
        break;
    case 20:
        name_ent_serifu_no = 27;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 21:
        name_ent_text_scroll();
        name_ent_flag_set();
        break;
    case 22:
        name_ent_serifu_no = 28;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 23:
        name_ent_text_scroll();
        name_ent_kuchi_anif();
        break;
    case 24:
        name_ent_serifu_no = 29;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 25:
        name_ent_text_scroll();
        break;
    case 26:
        v = cal_cw_x;
        if (v >= -399) {
            cw101_x = v - 2;
            cw100_x = v - 2;
            cw99_x = v - 2;
            cw98_x = v - 2;
            cw97_x = v - 2;
            cal_cw_x = v - 2;
        } else {
            step_mode = step_mode + 1;
        }
        break;
    case 27:
        name_ent_serifu_no = 30;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 28:
        name_ent_text_scroll();
        break;
    case 29:
        name_ent_serifu_no = 31;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 30:
        name_ent_text_scroll();
        break;
    case 31:
        value_cache_find_or_load(0xACD4, 1);
        step_mode = step_mode + 1;
        break;
    case 32:
        if (get_CD_State2() & 1) {
            cd_read_start2(49, 0x801A4000, 0xAA54);
            step_mode = step_mode + 1;
        }
        break;
    case 33:
        if (get_CD_State2() & 1) {
            cd_read_start2(25, 0x801E8000, 0xABCA);
            step_mode = step_mode + 1;
        }
        break;
    case 34:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                cd_read_start2(49, 0x801A4000, 0xAA54);
            } else {
                class_clear_f();
                name_ent_bg_set(0x801A4000, 15);
                memcpy(0x80180000, 0x801E8000, 0xC800);
                name_ent_cal_cw_set();
                name_ent_cw99_set0();
                sub_step_up();
            }
        }
        break;
    default:
        break;
    }
    dec_bg_show(1);
}
