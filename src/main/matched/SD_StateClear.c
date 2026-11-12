#include "tokimeki/sd.h"
extern SD_INFO gSDInfo;
/* Address: 0x8007B770 */
/* Size: 84 bytes (21 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Post-processing required: patch_frame_size.py --old 24 --new 40
 * Region: GCC (0x80041000-0x80080FFF)
 *
 * Calls:
 *   serial_cmd(116)
 * Clears globals: D_8012DD88, D_8012DD8C, D_8012DD90, D_8012DD94, D_8012DD98
 *                 D_8012DDCC (byte), D_8012DDCD (byte)
 */
extern void serial_cmd(int);


void SD_StateClear(void) {
    serial_cmd(116);
    (*(int *)&gSDInfo.xa_start) = 0;
    (*(int *)&gSDInfo.xa_end) = 0;
    (*(int *)&gSDInfo.xa_next_start) = 0;
    (*(int *)&gSDInfo.xa_next_end) = 0;
    (*(int *)&gSDInfo.xa_resume) = 0;
    (*(char *)&gSDInfo.xa_chan) = 0;
    (*(char *)&gSDInfo.xa_chan_next) = 0;
}
