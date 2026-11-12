/* @SymbolStart: 0x80133C08 */
/* @RodataBase: 0x8015D578 */
/* Address: 0x80133C10 */
/* Size: 648 bytes (162 insns) */
/* @SymbolSize: 656 */
#include "types.h"

extern void cd_read_start(s32, s32, s32);
extern s32  get_CD_State2(void);
extern void tile_pages_load(void);
extern void taco_tile_load(s32, s32);
extern void load_image_rect(s32, s32, s32, s32, s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void sub_sub_step_up(void);

extern u8 step_mode;

void taco_tile_page_load(void) {
    u8 idx = step_mode;

    s32 i;

    switch (idx) {
    case 0:
        cd_read_start(5, 0x801B8000, 0xB473);
        cd_read_start(3, 0x801C0000, 0xB4BC);
        cd_read_start(33, 0x801C8000, 0xB57F);
        step_mode = step_mode + 1;
        break;
    case 1:
    case 2:
    case 3:
        if (get_CD_State2() & 1) {
            step_mode = step_mode + 1;
        }
        break;
    case 4:
        tile_pages_load();
        step_mode = step_mode + 1;
        break;
    case 5:
        cd_read_start(32, 0x80180000, 0xB489);
        cd_read_start(32, 0x80190000, 0xB4D3);
        cd_read_start(8, 0x80170000, 0xB47E);
        step_mode = step_mode + 1;
        break;
    case 6:
    case 7:
    case 8:
        if (get_CD_State2() & 1) {
            step_mode = step_mode + 1;
        }
        break;
    case 9:
        taco_tile_load(0x80180000, 24);
        taco_tile_load(0x80190000, 26);
        taco_tile_load(0x80170000, 10);
        step_mode = step_mode + 1;
        break;
    case 10:
        cd_read_start(20, 0x80180000, 0xB4BF);
        step_mode = step_mode + 1;
        break;
    case 11:
        if (get_CD_State2() & 1) {
            step_mode = step_mode + 1;
        }
        break;
    case 12:
        load_image_rect(896, 256, 32, 128, 0x80180000);
        load_image_rect(928, 256, 32, 128, 0x80182000);
        load_image_rect(896, 384, 32, 128, 0x80184000);
        load_image_rect(928, 384, 32, 128, 0x80186000);
        for (i = 0; i != 15; i++) {
            palette_upload_enqueue(0x80188000, i & 0xFF, 1, 3, i & 0xFF);
        }
        sub_sub_step_up();
        break;
    }
}
