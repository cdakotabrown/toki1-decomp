/* Likely Symbol: _SpuDataCallback (from o.bin, offset +64 bytes from 0x80098D64) */
/* Address: 0x80098DA4 */
/* Size: 36 bytes */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* Post-processing required: convert_or_to_addu.py
 * Region: IDO (0x80081000-0x800BFFFF)
 */
typedef unsigned int u32;
typedef signed int s32;

extern void InterruptCallback(s32 cmd, u32 arg);

void _SpuCallback(u32 arg) {
    InterruptCallback(9, arg);
}
