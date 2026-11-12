/* Debug Symbol: device_init_step (from o.bin, high match) */
/* Address: 0x80084940 */
/* Size: 44 bytes */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF), but uses OR encoding */
extern void device_port_init(int arg0, int arg1);
extern void sub_sub_step_up(void);

void device_init_step(void) {
    device_port_init(0, 512);
    sub_sub_step_up();
}
