/* @SymbolSize: 804 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_XBB_SKIP_CAND=6,14 */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --map 36:48,24:40 */
/* @PostProc: patch_insn_reg_field 0x27c:rd:v0:v1 0x280:rs:v0:v1 0x2ac:rt:v0:v1 0x2b4:rt:v0:v1 0x2c4:rs:v0:v1 */
/* name_ent_default_set - NAME_ENT overlay, Address: 0x80134BA0, */
/* Size: 804 bytes (201 insns). */
#include "types.h"

extern void strncpy();
extern s32 rand(void);

extern u8 space_str[];
extern u8 name_ent_sei[];
extern u8 name_ent_mei[];
extern u8 name_ent_adana[];
extern u8 name_ent_sei_dflt_tbl[];
extern u8 name_ent_mei_dflt_tbl[];
extern u8 name_ent_adana_dflt_tbl[];
extern u8 name_ent_adana_10[];

void name_ent_default_set(volatile s32 sel)
{
    s32 m;
    s32 i;
    s32 n;
    u8 *p;
    u8 *q;

    m = rand() % 15;
    n = 0;

    if (sel == 0) {
        for (i = 0; i != 6; i++) {
            if (name_ent_sei[i] == space_str[i & 1]) {
                n++;
            }
        }
        if (name_ent_sei[n] == 0) {
            strncpy(name_ent_sei, name_ent_sei_dflt_tbl + m * 8, 8);
        }
        if (n == 6) {
            strncpy(name_ent_sei, name_ent_sei_dflt_tbl + m * 8, 8);
        }
        if (name_ent_sei[0] == 0) {
            strncpy(name_ent_sei, name_ent_sei_dflt_tbl + m * 8, 8);
        }
    }

    if (sel == 1) {
        for (i = 0; i != 6; i++) {
            if (name_ent_mei[i] == space_str[i & 1]) {
                n++;
            }
        }
        if (name_ent_mei[n] == 0) {
            strncpy(name_ent_mei, name_ent_mei_dflt_tbl + m * 8, 8);
        }
        if (n == 6) {
            strncpy(name_ent_mei, name_ent_mei_dflt_tbl + m * 8, 8);
        }
        if (name_ent_mei[0] == 0) {
            strncpy(name_ent_mei, name_ent_mei_dflt_tbl + m * 8, 8);
        }
    }

    if (sel == 2) {
        for (i = 0; i != 10; i++) {
            if (name_ent_adana[i] == space_str[i & 1]) {
                n++;
            }
        }
        p = &name_ent_adana[n];
        if (*p == 0) {
            strncpy(name_ent_adana, name_ent_adana_dflt_tbl + m * 12, 12);
        }
        if (p == name_ent_adana_10) {
            strncpy(name_ent_adana, name_ent_adana_dflt_tbl + m * 12, 12);
        }
        if (name_ent_adana[0] == 0) {
            strncpy(name_ent_adana, name_ent_adana_dflt_tbl + m * 12, 12);
        }
    }
}
