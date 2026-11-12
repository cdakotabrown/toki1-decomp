/* Function: SD_CmdProc */
/* Address: 0x80079BC4 */
/* Size: 316 bytes (79 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: strip_trailing_instructions --count 1 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 cmd_que_get(void);
extern void SD_CmdDispatch(s32);
extern void seq_cmd(s32);
extern void SD_XACmd(s32);
extern void cmd_voice5(s32);
extern void cmd_voice6(s32);
extern void cmd_voice7(s32);
extern void cmd_voice8(s32);
extern void SD_CDFrame(s32);
extern void SD_FlagsPoll(void);
extern void cmd_que_clear(void);

void SD_CmdProc(void) {
    s32 v0;
    s32 a0;

    v0 = cmd_que_get();
    a0 = v0 & 0xFFFF;
    if (v0 != 0) {
        do {
            switch (v0 & 0xF00) {
            case 0x000:
                SD_CmdDispatch(a0);
                break;
            case 0x200:
                seq_cmd(a0);
                break;
            case 0x300:
                SD_XACmd(a0);
                break;
            case 0x500:
                cmd_voice5(a0);
                break;
            case 0x600:
                cmd_voice6(a0);
                break;
            case 0x700:
                cmd_voice7(a0);
                break;
            case 0x800:
                cmd_voice8(a0);
                break;
            }
            v0 = cmd_que_get();
            a0 = v0 & 0xFFFF;
        } while (v0 != 0);
    }
    SD_CDFrame(a0);
    SD_FlagsPoll();
    cmd_que_clear();
}
