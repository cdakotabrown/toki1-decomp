/* @SymbolStart: 0x8014A44C */
/* @SymbolSize: 820 */
/* @RodataBase: 0x8015B394 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Function: date_open_load */
/* Overlay: DATE */
#include "types.h"

extern s16 date_open_load_no;
extern s32 date_move_src_tbl;
extern s32 date_move_src_tbl_1;
extern s32 date_move_src_tbl_2;
s32 date_move_tbl[3] = {0};
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;

extern void date_angle_ptr_init(void);
extern void date_model_link(void);
extern void date_light_init(void);
extern void date_step_next(void);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void image_upload_queue_add_flags(s32, s32, s32, s32, s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start(s32, s32, s32);
extern void tpage_buf_clear(void);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void class_switch(s32);
extern void initView(void);
extern void initCoordinate(void);
extern void initModelingData_init(s32);
extern void hizuke_disp_switch(s32);

void date_open_load(void)
{
    s32 p;
    s32 i;
    s32 scene0;
    s32 scene1;
    s32 scene2;

    switch (*(u16 *)&date_open_load_no) {
    case 0:
        date_angle_ptr_init();
        scene0 = date_move_src_tbl;
        scene1 = date_move_src_tbl_1;
        scene2 = date_move_src_tbl_2;
        date_move_tbl[0] = scene0;
        date_move_tbl[1] = scene1;
        date_move_tbl[2] = scene2;
        cd_read_start(1, 0x80195000, 0xBDAC);
        date_open_load_no = date_open_load_no + 1;
        break;

    case 1:
        if ((get_CD_State2() & 1) != 0) {
            date_open_load_no = date_open_load_no + 1;
        }
        break;

    case 2:
        device_cmd_send(207);
        device_cmd_send(514);
        tpage_buf_clear();
        back_clear_switch(1);
        class_switch(1);
        back_clear_g = 0;
        back_clear_r = 0;
        back_clear_b = 0;
        hizuke_disp_switch(0);
        cd_read_start(2, 0x801F8000, 0xB471);
        date_open_load_no = date_open_load_no + 1;
        break;

    case 3:
        if ((get_CD_State2() & 1) != 0) {
            cd_read_start(1, 0x801F9000, 0xB470);
            date_open_load_no = date_open_load_no + 1;
        }
        break;

    case 4:
        if ((get_CD_State2() & 1) != 0) {
            draw2d3d(1, 1);
            initView();
            date_light_init();
            initCoordinate();
            initModelingData_init(0x801F8000);
            date_model_link();
            cd_read_start(33, 0x80180000, 0xB57F);
            date_open_load_no = date_open_load_no + 1;
        }
        break;

    case 5:
        if ((get_CD_State2() & 1) != 0) {
            cd_read_start(1, 0x80190800, 0xB456);
            date_open_load_no = date_open_load_no + 1;
        }
        break;

    case 6:
        if ((get_CD_State2() & 1) != 0) {
            palette_upload_enqueue(0x80190800, 16, 1, 2, 0);
            i = 0;
            p = 0x80180000;
            for (; i < 2; i++) {
                load_tpage_buf_lock(i + 5, p, 1);
                p += 16384;
            }
            p = 0x80188000;
            i = 0;
            do {
                load_tpage_buf_lock(i + 37, p, 1);
                i++;
                p += 16384;
            } while (i != 2);
            cd_read_start(9, 0x80180000, 0xB467);
            date_open_load_no = date_open_load_no + 1;
        }
        break;

    case 7:
        if ((get_CD_State2() & 1) != 0) {
            palette_upload_enqueue(0x80180000, 17, 1, 2, 0);
            image_upload_queue_add_flags(0x80180200, 7, 1, 0, 0, 128, 128);
            date_step_next();
        }
        break;
    }
}
