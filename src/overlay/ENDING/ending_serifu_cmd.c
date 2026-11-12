/* @RodataBase: 0x8013BF90 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=-102 */
/* @PostProc: patch_stack_complete --old 592 --new 608 */
/* @PostProc: remap_stack_slots --map '72:80,590:598' */
/* @PostProc: swap_instructions --offset1 0x430 --offset2 0x434 */
/* @PostProc: swap_instructions --offset1 0x58C --offset2 0x590 */
/* @PostProc: patch_insn_reg_field 0x424:rt:v0:v1 0x428:rs:v0:v1 0x428:rt:v0:v1 0x430:rs:v0:v1 0x430:rt:t8:v0 0x434:rs:v0:v1 0x43c:rs:t8:v0 0x464:rt:t0:t9 0x468:rt:t9:t8 0x46c:rd:t1:t0 0x46c:rt:t0:t9 0x474:rs:t9:t8 0x474:rt:t1:t0 0x478:rt:t3:t2 0x47c:rd:t2:t1 0x480:rd:t4:t3 0x480:rs:t3:t2 0x480:rt:t2:t1 0x484:rs:t4:t3 0x484:rt:t5:t4 0x48c:rs:t5:t4 0x48c:rt:t6:t5 0x494:rs:t6:t5 0x4a8:rt:t8:t7 0x4b0:rt:t8:t7 0x4b4:rt:t0:t9 0x4b8:rs:t0:t9 0x4b8:rt:t0:t9 0x4c0:rs:t0:t9 0x4c8:rt:t9:t8 0x4d0:rd:t1:t0 0x4d0:rs:t9:t8 0x4d0:rt:t2:t1 0x4d4:rs:t1:t0 0x4fc:rt:t2:t1 0x500:rs:t2:t1 0x504:rt:t3:t2 0x508:rs:t3:t2 0x508:rt:t3:t2 0x50c:rt:t4:t3 0x510:rs:t3:t2 0x518:rt:t4:t3 0x524:rt:t5:t4 0x52c:rt:t5:t4 0x53c:rt:t6:t5 0x540:rt:t7:t6 0x544:rs:t7:t6 0x544:rt:t7:t6 0x548:rs:t6:t5 0x548:rt:t6:t5 0x554:rt:t6:t5 0x558:rt:t7:t6 0x570:rt:t8:t7 0x57c:rt:t8:t7 0x580:rt:v0:v1 0x584:rs:v0:v1 0x584:rt:v0:v1 0x58c:rs:v0:v1 0x58c:rt:t9:v0 0x590:rs:v0:v1 0x598:rs:t9:v0 0x59c:rt:t0:t9 0x5a4:rt:t0:t9 0x5c0:rt:t2:t0 0x5c4:rt:t1:t8 0x5c8:rd:t3:t1 0x5c8:rt:t2:t0 0x5d0:rs:t1:t8 0x5d0:rt:t3:t1 0x5d4:rt:t5:t3 0x5d8:rd:t4:t2 0x5dc:rd:t6:t4 0x5dc:rs:t5:t3 0x5dc:rt:t4:t2 0x5e0:rs:t6:t4 0x5e0:rt:t7:t5 0x5e8:rs:t7:t5 0x5e8:rt:t8:t6 0x5f0:rs:t8:t6 0x610:rt:t2:t0 0x614:rs:t2:t0 0x614:rt:t2:t0 0x61c:rs:t2:t0 0x624:rt:t1:t8 0x62c:rd:t3:t1 0x62c:rs:t1:t8 0x62c:rt:t4:t2 0x630:rs:t3:t1 0x658:rt:t4:t2 0x65c:rs:t4:t2 0x660:rt:t5:t3 0x664:rs:t5:t3 0x664:rt:t5:t3 0x668:rt:t6:t4 0x66c:rs:t5:t3 0x674:rt:t6:t4 0x680:rt:t7:t5 0x688:rt:t7:t5 0x698:rt:t8:t6 0x69c:rt:t0:t7 0x6a0:rs:t0:t7 0x6a0:rt:t0:t7 0x6a4:rs:t8:t6 0x6a4:rt:t8:t6 0x6b0:rt:t8:t6 0x6b4:rt:t0:t7 */
/* ending_serifu_cmd - ENDING overlay, Address: 0x80132420, Size: 1780 bytes */
#include "types.h"

extern void strcpy(void *dst, void *src);
extern void k_speed_set(s32);
extern void set_kanji_string(s16, s16, s32, void *, s32);
extern void msg_str_disp(s16, s16, s32, void *, s32, void *, void *, s32);
extern void sub_sub_step_up(void);
extern void game_reset(void);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);

extern s32 serifu_state;
extern s32 serifu_voice;
extern s32 serifu_wait;
extern s32 msg_wait;
extern u8 sound_seek_wait;
extern s16 sound_seek_time;
extern s16 last_x_y;
extern u8 now_girl;
extern s32 ending_serifu_frame;
extern u8 ending_wait_cnt;

extern u8 ending_serifu_cmd_str0[];
extern u8 ending_cmd0_suf_str[];
extern u8 ending_cmd0_bul_str[];
extern u8 ending_serifu_cmd_str1[];
extern u8 ending_serifu_cmd_str2[];
extern u8 ending_cmd6_bul_str[];
extern u8 ending_serifu_cmd_str3[];
extern u8 ending_cmd1_suf_str[];
extern u8 ending_cmd1_bul_str[];
extern u8 ending_serifu_cmd_str4[];
extern u8 ending_cmd12_suf_str[];
extern u8 ending_cmd12_bul_str[];
extern u8 ending_cmd13_suf_str[];
extern u8 ending_cmd13_bul_str[];
extern u8 ending_cmd15_suf_str[];
extern u8 ending_cmd15_bul_str[];

typedef struct {
    char text[518];
    s16 y;
} NameBox;

s32 ending_serifu_cmd(str, sel, arg2, tbl, row, col, pal)
char *str;
u8 sel;
s32 arg2;
s32 ***tbl;
u16 row;
u16 col;
volatile u8 pal;
{
    NameBox nb;
    s32 h;
    s32 first;
    u8 cnt;
    s32 *ent;

    if (sel != 255) {
        strcpy(nb.text, str);
    }
    serifu_voice = 0;
    sound_seek_wait = 0;

    switch ((u32)sel) {
    case 0:
        ending_serifu_frame = 0;
        k_speed_set(16);
        set_kanji_string(nb.y = -102, 72, 0, ending_serifu_cmd_str0, 0);
        msg_str_disp((s16)(nb.y + 14), 72, 0, nb.text, 0, ending_cmd0_suf_str, ending_cmd0_bul_str,
                      0);
        serifu_wait = 1;
        break;
    case 6:
        ending_serifu_frame = 0;
        k_speed_set(16);
        set_kanji_string(nb.y = -102, 72, 0, ending_serifu_cmd_str1, 0);
        msg_str_disp((s16)(nb.y + 14), 72, 0, nb.text, 0, ending_serifu_cmd_str2, ending_cmd6_bul_str,
                      0);
        serifu_wait = 1;
        break;
    case 1:
        ending_serifu_frame = 0;
        k_speed_set(8);
        if (tbl[row][col][0] != 0) {
            h = sound_seek_time;
            first = (h == 0);
            sound_seek_time = h + 1;
            if (first && serifu_state == 0) {
                ent = tbl[row][col];
                set_XAsector(ent[0], ent[1], now_girl);
                device_cmd_send(768);
            }
            if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                sound_seek_wait = 1;
                return 0;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        set_kanji_string(-144, 72, pal, ending_serifu_cmd_str3, 0);
        msg_str_disp(-130, 72, pal, nb.text, 0, ending_cmd1_suf_str, ending_cmd1_bul_str, 0);
        serifu_state = 1;
        break;
    case 12:
        ending_serifu_frame = 0;
        k_speed_set(8);
        if (tbl[row][col][0] != 0) {
            h = sound_seek_time;
            first = (h == 0);
            sound_seek_time = h + 1;
            if (first && serifu_state == 0) {
                ent = tbl[row][col];
                set_XAsector(ent[0], ent[1], now_girl);
                device_cmd_send(768);
            }
            if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                sound_seek_wait = 1;
                return 0;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        set_kanji_string(-144, 72, pal, ending_serifu_cmd_str4, 0);
        msg_str_disp(-130, 72, pal, nb.text, 0, ending_cmd12_suf_str, ending_cmd12_bul_str, 0);
        msg_wait = 1;
        break;
    case 13:
        cnt = ending_wait_cnt;
        first = (cnt < 60U);
        ending_serifu_frame = 0;
        cnt = cnt + 1;
        if (first != 0) {
            ending_wait_cnt = cnt;
            return 0;
        }
        ending_wait_cnt = 0;
        k_speed_set(8);
        if (tbl[row][col][0] != 0) {
            h = sound_seek_time;
            first = (h == 0);
            sound_seek_time = h + 1;
            if (first && serifu_state == 0) {
                ent = tbl[row][col];
                set_XAsector(ent[0], ent[1], now_girl);
                device_cmd_send(768);
            }
            if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                sound_seek_wait = 1;
                return 0;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        msg_str_disp(-130, last_x_y, pal, nb.text, 0, ending_cmd13_suf_str, ending_cmd13_bul_str,
                      0);
        msg_wait = 1;
        break;
    case 15:
        cnt = ending_wait_cnt;
        first = (cnt < 60U);
        ending_serifu_frame = 0;
        cnt = cnt + 1;
        if (first != 0) {
            ending_wait_cnt = cnt;
            return 0;
        }
        ending_wait_cnt = 0;
        k_speed_set(8);
        if (tbl[row][col][0] != 0) {
            h = sound_seek_time;
            first = (h == 0);
            sound_seek_time = h + 1;
            if (first && serifu_state == 0) {
                ent = tbl[row][col];
                set_XAsector(ent[0], ent[1], now_girl);
                device_cmd_send(768);
            }
            if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                sound_seek_wait = 1;
                return 0;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        msg_str_disp(-130, last_x_y, pal, nb.text, 0, ending_cmd15_suf_str, ending_cmd15_bul_str,
                      0);
        serifu_state = 1;
        break;
    case 255:
        sub_sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
}
