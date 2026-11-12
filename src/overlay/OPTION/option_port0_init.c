/* Address: 0x8013A030, Size: 40 bytes (10 instructions) */
#include "types.h"
extern u8 option_seq_no;
extern void device_port_init();

void option_port0_init(void) {
    device_port_init(0, option_seq_no);
}
