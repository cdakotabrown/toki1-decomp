/* Function: SD_ChCmd */
/* Address: 0x8007A3E4 (true start; the old symbol 0x8007A3EC was the */
/* Size: 232 bytes (58 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x8007A3E4 */
/* @SymbolSize: 232 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: rename_register_chain --chain t6:t8:t0:t2:t4:t6 --chain t7:t9:t1:t3:t5:t7 */
#include "types.h"
#include "tokimeki/sd.h"

extern u16 sd_vab_ch2;
extern u16 sd_vab_ch3;
extern u16 sd_vab_ch0;
extern u16 sd_vab_ch1;
extern SD_INFO gSDInfo;
extern void SD_SEQTableSet(u32 a0, s32 a1);

void SD_ChCmd(s32 arg0) {
    switch ((u8)arg0) {
        case '!':
            sd_vab_ch2 = 0;
            sd_vab_ch3 = 3;
            break;
        case '"':
            sd_vab_ch0 = 1;
            sd_vab_ch1 = 2;
            break;
        case '#':
            sd_vab_ch0 = 2;
            sd_vab_ch1 = 1;
            break;
        case '$':
            sd_vab_ch2 = 3;
            sd_vab_ch3 = 0;
            break;
        case '/':
            gSDInfo.seq_bank = 0;
            SD_SEQTableSet(0x8001C000, 0);
            break;
        case '.':
            gSDInfo.seq_bank = 1;
            SD_SEQTableSet(0x8002E800, 1);
            break;
    }
}
