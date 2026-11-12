/* @SymbolStart: 0x80142A60 */
/* @SymbolSize: 992 */
#include "types.h"

extern u8  screen_bri;
extern s32 movie_main_tbl1;
extern u8  date2_menu_select_tbl0;
extern u8  date2_menu_select_tbl1;
extern u8  etc_init_tbl0;

extern u8 name_ent_movie_exit_str0[];
extern u8 name_ent_movie_exit_str1[];
extern u8 name_ent_movie_exit_str2[];
extern u8 name_ent_movie_exit_str3[];
extern u8 name_ent_movie_exit_str4[];
extern u8 name_ent_movie_exit_str5[];
extern u8 name_ent_movie_exit_str6[];
extern u8 name_ent_movie_exit_str7[];
extern u8 name_ent_movie_exit_str8[];
extern u8 name_ent_movie_exit_str9[];
extern u8 name_ent_movie_exit_str10[];
extern u8 name_ent_movie_exit_str11[];
extern u8 name_ent_movie_exit_str12[];
extern u8 name_ent_movie_exit_str13[];
extern u8 name_ent_movie_exit_str14[];
extern u8 name_ent_movie_exit_str15[];
extern u8 name_ent_movie_exit_str16[];
extern u8 name_ent_movie_exit_str17[];
extern u8 name_ent_movie_exit_str18[];
extern u8 name_ent_movie_exit_str19[];
extern u8 name_ent_movie_exit_str20[];

extern s32  get_pn_flag();
extern void pn_set();
extern void class_clear();
extern void konami_mark_set();
extern void set_flag_b3_high_bit();
extern void back_clear_switch();
extern void text_sys_init();
extern s32  get_k_speed();
extern void k_speed_set(u8);
extern void set_kanji_string();
extern void k_disp_start();
extern void set_movie_offset();
extern void movie_main();
extern void StUnSetRing();
extern void cd_command_issue();
extern void dec_env_set();
extern void ResetGraph();
extern void vram_area_clear();
extern void DrawSync();
extern void goto_step();
extern void cursor_init();

void name_ent_movie_exit(void)
{
    s32 pad;
    u8  local67;
    s32 local60;
    u8  buf52[8];

    screen_bri = 128;
    movie_main_tbl1 = movie_main_tbl1 + 1;
    local67 = get_pn_flag();
    pn_set(0);
    class_clear(0);
    konami_mark_set(1);
    set_flag_b3_high_bit(0);
    back_clear_switch(1);
    text_sys_init();
    local60 = get_k_speed();
    k_speed_set(3);

    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str0, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str1, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str2, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str3, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str4, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str5, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str6, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str7, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str8, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str9, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str10, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str11, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str12, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str13, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str14, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str15, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str16, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str17, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str18, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str19, 0);
    set_kanji_string(-136, 92, 8, name_ent_movie_exit_str20, 0);
    k_disp_start(0);

    if (movie_main_tbl1 & 1) {
        set_movie_offset(24, 16);
    } else {
        set_movie_offset(16, 16);
    }
    movie_main(0, 827, 2080);
    StUnSetRing();

    buf52[0] = 128;
    cd_command_issue(14, 0, buf52);
    cd_command_issue(9, 0, 0);
    cd_command_issue(10, 0, 0);
    dec_env_set(0x80167000, 0x80162000, 0x80162000);
    ResetGraph(1);
    vram_area_clear(0);
    DrawSync(0);
    class_clear(0);
    goto_step(18);
    pn_set(local67);
    k_speed_set(local60);
    konami_mark_set(0);
    cursor_init(0);
    set_flag_b3_high_bit(0);
    date2_menu_select_tbl0 = 0;
    date2_menu_select_tbl1 = 0;
    etc_init_tbl0 = 0;
}
