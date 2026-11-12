/* Function: bustup_req_clear_step */
/* Likely Symbol: holiday_init_0 (from o.bin, offset +64 bytes from 0x800850B4) */
/* Address: 0x80085074 */
/* Size: 36 bytes */
/* Region: IDO (0x80080000+) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* Post-processing required: patch_frame_size.py --old-size 24 --new-size 40
 */
extern int kuchi_stop_w;
extern void sub_sub_step_up(void);

void bustup_req_clear_step(void)
{
    kuchi_stop_w = 0;
    sub_sub_step_up();
}
