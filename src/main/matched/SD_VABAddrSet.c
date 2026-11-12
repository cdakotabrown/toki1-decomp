/* Likely Symbol: SD_VABAddrSet (from o.bin, offset +136 bytes from 0x8007B9A4) */
/* Address: 0x8007BA2C */
/* Size: 184 bytes (46 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) but OR encoding */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required:
 *   python3 tools/ido/patch_frame_size.py input.o output.o --old 24 --new 40
 *   Then patch sw a0 and lhu offsets from 24 to 40
 *
 * This function selects VRAM bank pointers based on arg0:
 *   17: gSDBodyAdrs = base, D_8012DDE4 = base + 0x10000
 *   18: D_8012DDE8 = base, D_8012DDEC = base + 0x28000
 *   19: gSDBodyAdrs = base
 *   20: D_8012DDE4 = base
 *   21: D_8012DDE8 = base
 *   22: D_8012DDEC = base
 *
 * Jump table at D_800B87A0 (0x800C87A0 - calculated from lui 0x800C + offset -30816)
 * @CompileFlags: -Wc,-pic1
 */
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

extern u32 gSDBodyAdrs[];

extern u32 now_trans_pointer(void);

void SD_VABAddrSet(u16 arg0)
{
    u32 base;

    base = now_trans_pointer();

    switch (arg0 - 17) {
    case 0:
        gSDBodyAdrs[0] = base;
        gSDBodyAdrs[1] = base + 0x10000;
        break;
    case 1:
        gSDBodyAdrs[2] = base;
        gSDBodyAdrs[3] = base + 0x28000;
        break;
    case 2:
        gSDBodyAdrs[0] = base;
        break;
    case 3:
        gSDBodyAdrs[1] = base;
        break;
    case 4:
        gSDBodyAdrs[2] = base;
        break;
    case 5:
        gSDBodyAdrs[3] = base;
        break;
    }
}
