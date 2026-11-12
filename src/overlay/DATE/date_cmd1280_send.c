/* Function: date_cmd1280_send */
/* Overlay: DATE */
/* Address: 0x80151140 */
/* Size: 32 bytes (8 instructions) */
#include "types.h"

extern s32 device_cmd_send();

void date_cmd1280_send(void) {
    device_cmd_send(1280);
}
