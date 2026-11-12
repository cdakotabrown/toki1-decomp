/* Likely Symbol: file_select_goto_game (from o.bin, offset +24 bytes from 0x80070F74) */
/* Address: 0x80070F8C */
/* Size: 72 bytes (18 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
extern void hizuke_advance(void);
extern int get_CD_State(void);
extern void cd_command_issue(int, int, int);
extern void goto_step(int);

void schedule_init4(void) {
    int result;

    hizuke_advance();
    result = get_CD_State();
    if (result & 4) {
        cd_command_issue(9, 0, 0);
    }
    goto_step(49);
}
