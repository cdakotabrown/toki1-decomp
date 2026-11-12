/* Address: 0x80136FCC */
/* Size: 504 bytes (126 insns) */
#include "types.h"

extern u8 taco_panel_disp_str0[];
extern u8 taco_panel_disp_str1[];
extern u8 taco_panel_disp_str2[];
extern u8 taco_panel_disp_str3[];
extern u8 taco_rank_name[];
extern u8 taco_rank_name_1_0[];
extern u8 taco_rank_name_2_0[];
extern u8 taco_panel_disp_str4[];
extern u8 taco_panel_disp_str5[];
extern u8 taco_panel_disp_str6[];
extern void *taco_rank_score;
extern void *taco_rank_score_1;
extern void *taco_rank_score_2;

extern void taco_str_disp(void *p, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void taco_num_set(void *p, void *q);

void taco_panel_disp(s32 arg0, s32 arg1)
{
    taco_str_disp(taco_panel_disp_str0, arg0, 0, arg1, 2, 1);
    taco_str_disp(taco_panel_disp_str1, arg0, 1, arg1, 1, 4);
    taco_str_disp(taco_panel_disp_str2, arg0, 4, arg1, 1, 5);
    taco_str_disp(taco_panel_disp_str3, arg0, 3, arg1, 1, 6);
    taco_str_disp(taco_rank_name, arg0, 1, arg1, 15, 4);
    taco_str_disp(taco_rank_name_1_0, arg0, 4, arg1, 15, 5);
    taco_str_disp(taco_rank_name_2_0, arg0, 3, arg1, 15, 6);
    taco_num_set(taco_panel_disp_str4, taco_rank_score);
    taco_num_set(taco_panel_disp_str5, taco_rank_score_1);
    taco_num_set(taco_panel_disp_str6, taco_rank_score_2);
    taco_str_disp(taco_panel_disp_str4, arg0, 2, arg1, 6, 4);
    taco_str_disp(taco_panel_disp_str5, arg0, 5, arg1, 6, 5);
    taco_str_disp(taco_panel_disp_str6, arg0, 6, arg1, 6, 6);
}
