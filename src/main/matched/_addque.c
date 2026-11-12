/* Likely Symbol: _addque (from o.bin, offset +76 bytes from 0x800A1DD0) */
/* Address: 0x800A1E1C */
/* Size: 36 bytes (9 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* Post-processing required:
 *   python3 tools/ido/convert_or_to_addu.py input.o output.o
 */
extern void _addque2(int arg0, int arg1, int arg2, int arg3);

void _addque(int arg0, int arg1, int arg2) {
    _addque2(arg0, arg1, 0, arg2);
}
