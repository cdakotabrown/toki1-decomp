/* Address: 0x8007A118 (true start 0x8007A110: `andi a1,a0,0xffff` and the */
/* Size: 468 bytes (117 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x8007A110 */
/* @SymbolSize: 468 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void SD_VabLoadCmd(u16);
extern void func_8007A3E4(u16);
extern void seq_ctrl(s32);
extern void SD_ReverbReset(void);
extern void serial_cmd(s32);
extern void SD_SerialClear(void);
extern void seq_vol_fade_set(u16);
extern void serial_fade_set(u16);
extern void SD_ModeCmd(u16);

void SD_CmdDispatch(flags)
    u16 flags;
{
    switch (flags & 0xF0) {
    case 16:
        SD_VabLoadCmd(flags);
        return;
    case 32:
        func_8007A3E4(flags);
        return;
    case 112:
        if (flags & 1) {
            seq_ctrl(113);
        }
        if (flags & 2) {
            SD_ReverbReset();
        }
        if (flags & 4) {
            serial_cmd(116);
            SD_SerialClear();
        }
        return;
    case 224:
        if (flags & 1) {
            seq_ctrl(225);
        }
        if (flags & 4) {
            serial_cmd(228);
            SD_SerialClear();
        }
        return;
    case 240:
        if (flags & 1) {
            seq_ctrl(241);
        }
        if (flags & 4) {
            serial_cmd(244);
            SD_SerialClear();
        }
        return;
    case 176:
    case 192:
    case 208:
        if (flags & 1) {
            seq_vol_fade_set(flags & 0xFFF1);
        }
        if (flags & 4) {
            serial_fade_set(flags & 0xFFF4);
        }
        return;
    default:
        SD_ModeCmd(flags & 0xFF);
    }
}
