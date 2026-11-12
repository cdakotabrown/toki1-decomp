/* @SymbolStart: 0x8013828C */
/* @SymbolSize: 148 */
/* kangei_birth_msg_set - KANGEI overlay, Address: 0x8013828C, Size: 148 bytes (37 insns) */
#include "types.h"

extern s32 get_g_zyotai_h(u8 a0);
extern void bustup_face_change(s32 a0);
extern void kangei_girl_msg_set(void);
extern void sub_sub_step_up(void);

extern volatile u8 day_flag;
extern volatile u8 now_girl;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;
extern s32 kangei_bank1_tbl[][13];

void kangei_birth_msg_set(void)
{
    u8 cur_char;
    s32 result;

    day_flag = day_flag | 4;
    cur_char = now_girl;
    result = get_g_zyotai_h(cur_char);
    bustup_face_change(result & 0xFF);
    kangei_ss_s = kangei_bank1_tbl[0][1];
    kangei_ss_s_kind = kangei_bank1_tbl[1][1];
    kangei_sss_so = kangei_bank1_tbl[2][1];
    kangei_now_s = 1;
    kangei_now_s_in = 0;
    kangei_girl_msg_set();
    sub_sub_step_up();
}
