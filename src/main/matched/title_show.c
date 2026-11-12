/* Likely Symbol: title_show (from o.bin, offset +148 bytes from 0x800626A4) */
/* Address: 0x80062738 */
/* Size: 156 bytes */
/* Compiler: IDO 5.2 (compiled) for GCC region (0x80040000-0x80080000) */
typedef int s32;
typedef unsigned int u32;

extern u32 pad_trig;
extern u32 step_counter;

extern void cursor_select(void);
extern void set_flag_b3_high_bit(s32 a0);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern void class_clear_all(s32 a0);
extern void goto_step(s32 a0);

void title_show(void) {
    cursor_select();

    if ((pad_trig & 0x820) != 0) {
        set_flag_b3_high_bit(0);
        sub_sub_step_up();
        device_cmd_send(0x501);
    }

    if (step_counter < 601) {
        if ((pad_trig & 0x100) == 0) {
            return;
        }
    }

    class_clear_all(0);
    device_cmd_send(0x7F);
    set_flag_b3_high_bit(0);
    goto_step(0x11);
}
