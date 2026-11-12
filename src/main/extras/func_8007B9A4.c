/* Address: 0x8007B9A4 */
/* Size: 136 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
#include "common.h"

extern u32 D_8012DDD8;
extern u32 D_8012DDDC;
extern u32 gSDBodyLAdrs;
extern u32 D_8012DDF4;
extern u32 D_8012DDF8;
extern u32 D_8012DDFC;
extern u32 gSDBodySize;
extern u32 D_8012DE04;
extern u32 D_8012DE08;
extern u32 D_8012DE0C;

void func_8007B9A4(void)
{
    u32 size1;
    u32 block1;
    u32 block2;
    u32 size2;
    u32 size3;
    u32 size4;

    D_8012DDD8 = 0x80016000;
    size1 = 4112;
    block2 = 0x28000;
    D_8012DDDC = 0x8001A000;
    block1 = 0x10000;
    size2 = size1 + block1;
    size3 = size2 + block2;
    size4 = size3 + block2;
    D_8012DDFC = size4;
    gSDBodySize = block1;
    D_8012DE04 = block2;
    D_8012DE08 = block2;
    D_8012DE0C = 0x1A768;
    gSDBodyLAdrs = size1;
    D_8012DDF4 = size2;
    D_8012DDF8 = size3;
}
