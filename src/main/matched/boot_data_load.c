/* Function: boot_data_load  (0x8005A068, registered name; the real entry point */
/* Address: 0x8005A068 */
/* Size: 220 bytes (55 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8005A060 */
/* @SymbolSize: 228 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:10 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 step_mode;

extern void back_clear_switch(s32 flag);
extern void vram_area_clear(s32 resetId);
extern void cd_read_start2(s32 fileId, s32 destAddr, s32 tag);
extern void cd_read_start(s32 fileId, s32 destAddr, s32 tag);
extern s32 get_CD_State2(void);
extern void hud_palette_init(void);
extern void sub_sub_step_up(void);

s32 boot_data_load(void)
{
    s32 state;

    state = step_mode;
    if (state == 0) {
        back_clear_switch(1);
        vram_area_clear(0xFF);
        cd_read_start2(0x35, 0x80180000, 0x9D85);
        state = (u8)(step_mode + 1);
        step_mode = (u8)state;
        return state;
    }
    if (state == 1) {
        if (get_CD_State2() & 1) {
            hud_palette_init();
            cd_read_start(8, 0x801F0000, 0x9C97);
            state = (u8)(step_mode + 1);
            step_mode = (u8)state;
            return state;
        }
    } else if (state == 2) {
        if (get_CD_State2() & 1) {
            sub_sub_step_up();
        }
    }
}
