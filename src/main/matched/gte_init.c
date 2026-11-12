/* Likely Symbol: gte_init (from o.bin, offset +20 bytes from 0x8009D27C) */
/* Address: 0x8009D290 */
/* Size: 72 bytes (18 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80086000-0x800BFFFF) */
/* Post-processing required:
 *   1. convert_or_to_addu.py
 *   2. patch_ra_offset.py --old 20 --new 16
 *
 * Note: 8 bytes of trailing nop padding (harmless, target also has nop padding).
 */
extern void InitGeom(void);
extern void SetFarColor(int, int, int);
extern void SetGeomOffset(int, int);

extern short GsORGOFSX;
extern short GsORGOFSY;

void gte_init(void) {
    InitGeom();
    SetFarColor(0, 0, 0);
    SetGeomOffset(0, 0);
    GsORGOFSY = 0;
    GsORGOFSX = 0;
}
