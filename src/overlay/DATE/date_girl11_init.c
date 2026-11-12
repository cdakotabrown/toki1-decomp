/* Function: date_girl11_init */
/* Overlay: DATE */
/* Address: 0x8015A404 */
/* Size: 152 bytes (38 instructions) */
#include "types.h"

extern void mes_window_switch(s32 arg);
extern void strcpy(u8 *a, u8 *b);
extern void event_log_add(s32 a, s32 b);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern u8  g_name[];
extern u8  date_girl11_init_str0[];
extern u8  now_date_spot;
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl11_init(void)
{
    mes_window_switch(1);
    now_girl = 11;
    strcpy(g_name, date_girl11_init_str0);
    event_log_add(11, now_date_spot);
    date_ss_serifuev = date_serifuev_tbl[76];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[76];
    date_sss_soundev = date_soundev_tbl[76];
    now_serifuev = 0;
    now_serifuev_in = 0;
    sub_sub_step_up();
}
