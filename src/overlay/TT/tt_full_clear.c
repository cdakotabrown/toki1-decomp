/* @SymbolSize: 68 */
/* Address: 0x8013E440, Size: 68 bytes (17 insns) */
#include "types.h"

extern s32 bzero();
extern void tt_buffers_clear(void);

extern s32 tt_game_wk;
extern s32 tt_db_buf;

void tt_full_clear(void)
{
    bzero(tt_game_wk, 168);
    bzero(tt_db_buf, 0x10170);
    tt_buffers_clear();
}
