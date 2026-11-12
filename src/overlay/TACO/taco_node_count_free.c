/* taco_node_count_free - TACO overlay, Address: 0x8015CC34, 116 bytes. */
#include "types.h"

extern u8 taco_node_own[];

s32 taco_node_count_free(void)
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 64; i++) {
        if (taco_node_own[i] == 0) {
            n++;
        }
    }
    return n;
}
