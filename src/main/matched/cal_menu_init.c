/* Likely Symbol: func_8005BA28 (from o.bin, offset +112 bytes from 0x8005BA48) */
/* Address: 0x8005BAB8 */
/* Size: 112 bytes (28 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @CompileFlags: -Wc,-pic1 */
extern void text_sys_init(void);
extern void basyo_init(void *);
extern void set_kanji_string(int, int, int, void *, int);
extern void k_disp_start(int);
extern void cal_sprite_init(void);
extern void cal_class_init(void);
extern void icon_disp_switch(int);
extern void sub_sub_step_up(void);

extern int cal_menu_init_str0;
extern int cal_menu_init_str1;

void cal_menu_init(void) {
    text_sys_init();
    basyo_init(&cal_menu_init_str0);
    set_kanji_string(-128, 50, 0, &cal_menu_init_str1, 0);
    k_disp_start(2);
    cal_sprite_init();
    cal_class_init();
    icon_disp_switch(0);
    sub_sub_step_up();
}
