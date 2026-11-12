/* Function: hud_palette_init */
/* Address: 0x8005A144 */
/* Size: 380 bytes (95 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* - `@CompileEnv: IDO52_LDA_NOINREG_CLASSES=4:1:2`: retail rematerialises */
/* Likely Symbol: boot_data_load (from o.bin) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4:1:2 IDO52_UGEN_NO_ARG_COALESCE=1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void palette_upload_enqueue(void *a0, s32 a1, s32 a2, s32 a3, s32 stack_arg);
extern void hizuke_init(void);
extern void hizuke_disp_switch(s32 a0);
extern void parameter_show_init(void);
extern void parameter_disp_switch(s32 a0);
extern void icon_init(void);
extern void icon_disp_switch(s32 a0);
extern void message_window_init(void);
extern void message_disp_switch(s32 a0);
extern void back_scroll_init(void);
extern void mascot_disp_switch(s32 a0);
extern void mes_window_switch(s32 a0);

extern s32 hud_palette_init_tbl0;
extern s32 hud_palette_init_tbl1;
extern s32 hud_palette_init_tbl2;
extern s32 hud_palette_init_tbl3;
extern s32 hud_palette_init_tbl4;

void hud_palette_init(void) {
    s32 i;
    s32 *p;
    s32 val;

    for (i = 0; i != 16; i++) {
        p = &hud_palette_init_tbl3;
        do {
            val = *p;
            if (val & 0x7FFF0000) {
                val = *p = val | (s32)0x80000000;
            }
            if (val & 0x7FFF) {
                *p = val | 0x8000;
            }
            p++;
        } while ((u32)p < (u32)&hud_palette_init_tbl4);

        p = &hud_palette_init_tbl1;
        do {
            val = *p;
            if (val & 0x7FFF0000) {
                val = *p = val | (s32)0x80000000;
            }
            if (val & 0x7FFF) {
                *p = val | 0x8000;
            }
            p++;
        } while (p != &hud_palette_init_tbl2);

        palette_upload_enqueue(&hud_palette_init_tbl0, i & 0xFF, 1, 1, i & 0xFF);
    }

    hizuke_init();
    hizuke_disp_switch(0);
    parameter_show_init();
    parameter_show_init();
    parameter_disp_switch(0);
    icon_init();
    icon_disp_switch(0);
    message_window_init();
    message_disp_switch(0);
    back_scroll_init();
    mascot_disp_switch(0);
    mes_window_switch(0);
}
