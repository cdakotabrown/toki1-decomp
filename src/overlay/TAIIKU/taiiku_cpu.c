/* Address: 0x80147708 */
/* Size: 40 bytes (10 insns) */
#include "types.h"

extern void taiiku_cpu_steer();
extern void taiiku_pad_steer();

void taiiku_cpu(void)
{
    taiiku_cpu_steer();
    taiiku_pad_steer();
}
