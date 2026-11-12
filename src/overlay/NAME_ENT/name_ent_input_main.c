/* @SymbolStart: 0x8013209C */
/* @SymbolSize: 152 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1 */
/* Address: 0x8013209C (head) / 0x801320A4 */
/* Size: 152 bytes (38 instructions) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern void name_ent_input_main0(void);
extern void name_ent_entry_step(void);
extern void name_ent_fuji_main(void);
extern void dec_bg_show(s32);
extern void debug_hex_show(s32, s32, s32);

extern u8 now_sub_step;
extern s32 Bg;

void name_ent_input_main(void)
{
    switch (now_sub_step) {
    case 0:
        name_ent_input_main0();
        break;
    case 1:
        name_ent_entry_step();
        break;
    case 2:
        name_ent_fuji_main();
        break;
    }

    dec_bg_show(2);
    debug_hex_show(Bg, 0, 80);
    debug_hex_show((*(s32 *)&((LIBGS_BG *)&Bg)[1].attribute), 0, 90);
}
