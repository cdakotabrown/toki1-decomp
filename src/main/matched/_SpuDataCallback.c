/* Likely Symbol: _SpuDataCallback (from o.bin, offset +28 bytes from 0x80098D64) */
/* Address: 0x80098D80 */
/* Size: 36 bytes */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* Post-processing required: convert_or_to_addu.py (OR -> ADDU move encoding)
 */
extern void DMACallback(int, int);

void _SpuDataCallback(int arg0) {
    DMACallback(4, arg0);
}
