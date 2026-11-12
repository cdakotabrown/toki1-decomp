/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x8014C3A0, Size: 6848 bytes (1712 instructions) */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=9:-1:6:6,9:-1:16:16,12:-1:57:58 IDO52_WEB_BIND_BBS=950:399:402 IDO52_CMOT_KILL_AVAIL=714:402,715:402 IDO52_UGEN_TREG_ORDER_WINDOW=t6+t9:15:15 IDO52_CONST_DEF_BB=962:101:-,962:99:+,962:108:-,962:228:-,962:226:+ */
/* @PostProc: remap_stack_slots --map 0x1e:0x2b,0x1f:0x29 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x340 --offset2 0x344 */
/* @PostProc: swap_instructions --offset1 0x344 --offset2 0x34c */
/* @PostProc: swap_instructions --offset1 0x348 --offset2 0x34c */
/* @PostProc: swap_instructions --offset1 0x554 --offset2 0x558 */
/* @PostProc: swap_instructions --offset1 0x558 --offset2 0x560 */
/* @PostProc: swap_instructions --offset1 0x55c --offset2 0x560 */
/* @PostProc: swap_instructions --offset1 0x770 --offset2 0x774 */
/* @PostProc: swap_instructions --offset1 0x774 --offset2 0x77c */
/* @PostProc: swap_instructions --offset1 0x778 --offset2 0x77c */
/* @PostProc: swap_instructions --offset1 0x988 --offset2 0x98c */
/* @PostProc: swap_instructions --offset1 0x98c --offset2 0x994 */
/* @PostProc: swap_instructions --offset1 0x990 --offset2 0x994 */
/* @PostProc: swap_instructions --offset1 0xbac --offset2 0xbb0 */
/* @PostProc: swap_instructions --offset1 0xbb0 --offset2 0xbb8 */
/* @PostProc: swap_instructions --offset1 0xbb4 --offset2 0xbb8 */
/* @PostProc: swap_instructions --offset1 0xdc0 --offset2 0xdc4 */
/* @PostProc: swap_instructions --offset1 0xdc4 --offset2 0xdcc */
/* @PostProc: swap_instructions --offset1 0xdc8 --offset2 0xdcc */
/* @PostProc: swap_instructions --offset1 0xfc8 --offset2 0xfcc */
/* @PostProc: swap_instructions --offset1 0xfcc --offset2 0xfd4 */
/* @PostProc: swap_instructions --offset1 0xfd0 --offset2 0xfd4 */
/* @PostProc: swap_instructions --offset1 0x1244 --offset2 0x1248 */
/* @PostProc: swap_instructions --offset1 0x1248 --offset2 0x1250 */
/* @PostProc: swap_instructions --offset1 0x124c --offset2 0x1250 */
/* @PostProc: swap_instructions --offset1 0x1254 --offset2 0x1258 */
/* @PostProc: swap_instructions --offset1 0x13f8 --offset2 0x13fc */
/* @PostProc: swap_instructions --offset1 0x13fc --offset2 0x1404 */
/* @PostProc: swap_instructions --offset1 0x1400 --offset2 0x1404 */
/* @PostProc: swap_instructions --offset1 0x158c --offset2 0x1590 */
/* @PostProc: swap_instructions --offset1 0x1590 --offset2 0x1598 */
/* @PostProc: swap_instructions --offset1 0x1594 --offset2 0x1598 */
/* @PostProc: swap_instructions --offset1 0x1798 --offset2 0x179c */
/* @PostProc: swap_instructions --offset1 0x179c --offset2 0x17a4 */
/* @PostProc: swap_instructions --offset1 0x17a0 --offset2 0x17a4 */
/* @PostProc: swap_instructions --offset1 0x1974 --offset2 0x1978 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:t2:a3 0x50:rt:a1:t0 0x58:rt:t1:a2 0x5c:rs:t1:a2 0x5c:rt:t1:a2 0x60:rt:a3:a0 0x64:rs:t1:a2 0x64:rt:a3:a0 0x94:rt:t9:t3 0x98:rs:t9:t3 0x98:rt:t9:t3 0xa0:rs:t9:t3 0xa4:rt:t6:t9 0xac:rt:t6:t9 0xb8:rt:t7:t6 0xc0:rt:t7:t6 0xcc:rt:t0:t7 0xd0:rt:v1:t8 0xd4:rs:t0:t8 0xd4:rt:t0:t8 0xd8:rs:v1:t7 0xd8:rt:v1:t7 0xe0:rs:t1:a2 0xe4:rs:v1:t7 */
/* @PostProc: patch_insn_reg_field 0xec:rs:v1:t7 0xf4:rs:t2:a3 0x148:rt:t9:t2 0x14c:rs:t9:t2 0x14c:rt:t9:t2 0x154:rs:a3:a0 0x154:rt:t9:t2 0x15c:rs:t0:t8 0x160:rt:t6:t9 0x168:rt:t6:t9 0x174:rt:t9:t6 0x17c:rt:t9:t6 0x188:rt:t0:v1 0x18c:rt:a2:t2 0x190:rt:v1:t3 0x194:rs:t0:t3 0x194:rt:t0:t3 0x198:rs:a2:t2 0x198:rt:a2:t2 0x1a0:rs:t1:a2 0x1b4:rs:t2:a3 0x1bc:rt:t4:t1 0x1c4:rt:t6:t7 0x1c8:rs:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x1c8:rt:t6:t7 0x1d0:rs:t6:t7 0x1d4:rt:t4:t1 0x1d8:rs:a2:a0 0x1d8:rt:a3:t2 0x1dc:rt:t4:t1 0x1e0:rs:t0:a0 0x1e0:rt:a3:t3 0x1e4:rt:t7:t8 0x1ec:rt:t7:t8 0x1f8:rt:t8:t9 0x200:rt:t8:t9 0x20c:rt:t4:t1 0x210:rs:t1:a2 0x210:rt:t4:t1 0x218:rs:v1:t1 0x218:rt:t4:v1 0x220:rs:v1:a0 0x220:rt:a3:v1 0x228:rs:v1:t5 0x228:rt:t5:v1 0x230:rs:t2:a3 0x240:rs:a1:t0 0x244:rt:t9:t6 */
/* @PostProc: patch_insn_reg_field 0x248:rs:t9:t6 0x248:rt:t9:t6 0x250:rs:t9:t6 0x254:rt:t6:t7 0x258:rs:t6:t7 0x258:rt:t6:t7 0x260:rs:t6:t7 0x268:rs:a2:a0 0x268:rt:a3:t2 0x270:rs:t0:t3 0x274:rt:t7:t8 0x27c:rt:t7:t8 0x288:rt:t8:t9 0x290:rt:t8:t9 0x29c:rt:a3:a0 0x2a0:rs:v1:a0 0x2a0:rt:a3:v1 0x2a4:rt:t1:a2 0x2a8:rs:t1:a2 0x2a8:rt:t1:a2 0x2ac:rt:t4:t1 0x2b0:rs:t1:a2 0x2b0:rt:t4:t1 0x2d0:rt:t9:t6 */
/* @PostProc: patch_insn_reg_field 0x2d4:rs:t9:t6 0x2d4:rt:t9:t6 0x2dc:rs:t9:t6 0x2e0:rt:t6:t7 0x2e4:rs:t6:t7 0x2e4:rt:t6:t7 0x2ec:rs:t6:t7 0x2f0:rt:t7:t8 0x2f4:rs:t7:t8 0x2f4:rt:t7:t8 0x2fc:rs:t7:t8 0x300:rt:t8:t2 0x304:rs:t8:t2 0x304:rt:t8:t2 0x30c:rs:a3:a0 0x30c:rt:t8:t2 0x310:rt:t9:t3 0x314:rs:t9:t3 0x314:rt:t9:t3 0x31c:rs:t9:t3 0x320:rt:t6:t9 0x328:rt:t6:t9 0x33c:rt:t0:v1 0x340:rt:a2:t2 */
/* @PostProc: patch_insn_reg_field 0x344:rt:v1:t3 0x348:rs:t0:t3 0x348:rt:t0:t3 0x34c:rs:a2:t2 0x34c:rt:a2:t2 0x358:rs:t1:a2 0x36c:rt:t7:t6 0x370:rs:t7:t6 0x370:rt:t7:t6 0x378:rs:t7:t6 0x37c:rs:t2:a3 0x384:rt:t3:t4 0x388:rs:a2:a0 0x388:rt:a3:t2 0x390:rs:t0:t3 0x394:rt:t8:t7 0x39c:rt:t8:t7 0x3a8:rt:t9:t8 0x3b0:rt:t9:t8 0x3bc:rt:t3:t4 0x3c0:rs:t1:a2 0x3c0:rt:t3:t4 0x3d4:rt:v0:a1 0x3d8:rs:v0:a1 */
/* @PostProc: patch_insn_reg_field 0x3d8:rt:v0:a1 0x3e0:rs:v0:a1 0x3e8:rs:a3:a0 0x3e8:rt:v0:a1 0x3ec:rt:t6:t9 0x3f0:rs:t6:t9 0x3f0:rt:t6:t9 0x3f8:rs:t6:t9 0x3f8:rt:t7:t6 0x3fc:rs:t7:t6 0x404:rs:a2:a0 0x404:rt:a3:t2 0x40c:rs:t0:t3 0x410:rt:t8:t7 0x418:rt:t8:t7 0x424:rt:t9:a1 0x42c:rt:t9:a1 0x43c:rs:t1:a2 0x444:rs:v1:t1 0x444:rt:t4:v1 0x44c:rs:v1:a0 0x44c:rt:a3:v1 0x454:rs:v1:t5 0x454:rt:t5:v1 */
/* @PostProc: patch_insn_reg_field 0x458:rt:t6:t8 0x45c:rs:t6:t8 0x45c:rt:t6:t8 0x464:rs:t6:t8 0x468:rt:t7:t9 0x46c:rs:t7:t9 0x46c:rt:t7:t9 0x474:rs:t7:t9 0x47c:rs:a2:a0 0x47c:rt:a3:t2 0x484:rs:t0:t3 0x488:rt:t8:t6 0x490:rt:t8:t6 0x49c:rt:t9:t7 0x4a4:rt:t9:t7 0x4b4:rs:v1:t5 0x4b4:rt:t5:v1 0x4b8:rt:t1:a2 0x4bc:rs:t1:a2 0x4bc:rt:t1:a2 0x4c4:rs:t1:a2 0x4e4:rt:t6:t8 0x4e8:rs:t6:t8 0x4e8:rt:t6:t8 */
/* @PostProc: patch_insn_reg_field 0x4f0:rs:t6:t8 0x4f4:rt:t7:t9 0x4f8:rs:t7:t9 0x4f8:rt:t7:t9 0x500:rs:t7:t9 0x508:rt:t8:t2 0x50c:rs:t8:t2 0x50c:rt:t8:t2 0x514:rs:a3:a0 0x514:rt:t8:t2 0x518:rt:t9:t3 0x51c:rs:t9:t3 0x51c:rt:t9:t3 0x520:rt:at:a1 0x524:rs:t9:a1 0x524:rt:at:t3 0x550:rt:t0:v1 0x554:rt:a2:t2 0x558:rt:v1:t3 0x55c:rs:t0:t3 0x55c:rt:t0:t3 0x560:rs:a2:t2 0x560:rt:a2:t2 0x56c:rs:t1:a2 */
/* @PostProc: patch_insn_reg_field 0x570:rt:a0:a1 0x5a8:rs:a2:a0 0x5a8:rt:a3:t2 0x5b0:rs:t0:a1 0x5b0:rt:a0:t3 0x5dc:rs:t1:a2 0x5dc:rt:a0:a1 0x638:rs:a2:a0 0x638:rt:a3:t2 0x640:rs:t0:t3 0x66c:rs:t1:a2 0x670:rt:t4:t1 0x674:rs:v1:t1 0x674:rt:t4:v1 0x67c:rs:v1:a0 0x67c:rt:a3:v1 0x684:rs:v1:t5 0x684:rt:t5:v1 0x694:rs:t5:t6 0x694:rt:t6:t5 0x69c:rs:a2:a0 0x69c:rt:a3:t2 0x6a4:rs:t0:t3 0x6d0:rt:t3:t1 */
/* @PostProc: patch_insn_reg_field 0x6d4:rs:v1:t1 0x6d4:rt:t3:v1 0x6d8:rt:t1:a2 0x6dc:rs:t1:a2 0x6dc:rt:t1:a2 0x6e0:rt:a1:t0 0x6e4:rs:t1:a2 0x6e4:rt:a1:t0 0x724:rt:t7:t2 0x728:rs:t7:t2 0x728:rt:t7:t2 0x730:rs:a3:a0 0x730:rt:t7:t2 0x734:rt:t8:t3 0x738:rs:t8:t3 0x738:rt:t8:t3 0x73c:rt:at:a1 0x740:rs:t8:a1 0x740:rt:at:t3 0x744:rt:t9:t7 0x74c:rt:t9:t7 0x758:rt:t6:t8 0x760:rt:t6:t8 0x76c:rt:t0:v1 */
/* @PostProc: patch_insn_reg_field 0x770:rt:a2:t2 0x774:rt:v1:t3 0x778:rs:t0:t3 0x778:rt:t0:t3 0x77c:rs:a2:t2 0x77c:rt:a2:t2 0x788:rs:t1:a2 0x79c:rt:t7:t9 0x7a0:rs:t7:t9 0x7a0:rt:t7:t9 0x7a8:rs:t7:t9 0x7b0:rs:a2:a0 0x7b0:rt:a3:t2 0x7b8:rs:t0:t3 0x7bc:rt:t8:t6 0x7c4:rt:t8:t6 0x7d0:rt:t9:t7 0x7d8:rt:t9:t7 0x7e8:rs:t1:a2 0x7fc:rt:t6:t8 0x800:rs:t6:t8 0x800:rt:t6:t8 0x808:rs:t6:t8 0x80c:rt:t7:t9 */
/* @PostProc: patch_insn_reg_field 0x810:rs:t7:t9 0x810:rt:t7:t9 0x818:rs:a3:t9 0x818:rt:t7:a0 0x820:rs:a2:a0 0x820:rt:a3:t2 0x828:rs:t0:ra 0x828:rt:ra:t3 0x82c:rt:t8:t6 0x834:rt:t8:t6 0x840:rt:t9:t7 0x848:rt:t9:t7 0x858:rs:t1:a2 0x85c:rt:t4:t1 0x860:rs:v1:t1 0x860:rt:t4:v1 0x864:rs:t2:a3 0x868:rs:v1:a0 0x868:rt:a3:v1 0x86c:rs:t2:a3 0x870:rs:v1:t5 0x870:rt:t5:v1 0x874:rs:t2:a3 0x884:rt:t6:t8 */
/* @PostProc: patch_insn_reg_field 0x888:rs:t6:t8 0x888:rt:t6:t8 0x890:rs:t6:t8 0x894:rt:t7:t9 0x898:rs:t7:t9 0x898:rt:t7:t9 0x8a0:rs:t7:t9 0x8ac:rs:a2:a0 0x8ac:rt:a3:t2 0x8b4:rt:a1:t3 0x8b8:rt:t8:t6 0x8c0:rt:t8:t6 0x8cc:rt:t9:t7 0x8d4:rt:t9:t7 0x8e8:rt:t1:a2 0x8ec:rs:t1:a2 0x8ec:rt:t1:a2 0x8f4:rs:t1:a2 0x8f4:rt:a3:a0 0x918:rs:t2:a3 0x920:rs:a1:t0 0x92c:rt:t6:t8 0x930:rs:t6:t8 0x930:rt:t6:t8 */
/* @PostProc: patch_insn_reg_field 0x938:rs:t6:t8 0x93c:rt:t7:t2 0x940:rs:t7:t2 0x940:rt:t7:t2 0x948:rs:a3:a0 0x948:rt:t7:t2 0x94c:rt:t8:t3 0x950:rs:t8:t3 0x950:rt:t8:t3 0x958:rs:t8:t3 0x984:rt:t0:v1 0x988:rt:a2:t2 0x98c:rt:v1:t3 0x990:rs:t0:t3 0x990:rt:t0:t3 0x994:rs:a2:t2 0x994:rt:a2:t2 0x9a0:rs:t1:a2 0x9b8:rs:t2:a3 0x9c0:rs:a1:t0 0x9e0:rs:a2:a0 0x9e0:rt:a3:t2 0x9e8:rs:t0:t3 0xa18:rs:t1:a2 */
/* @PostProc: patch_insn_reg_field 0xa64:rs:a2:a0 0xa64:rt:a3:t2 0xa68:rt:t3:t4 0xa6c:rs:t0:t4 0xa98:rs:t1:a2 0xa9c:rt:t4:t1 0xaa0:rs:v1:t1 0xaa0:rt:t4:v1 0xaa8:rs:v1:a0 0xaa8:rt:a3:v1 0xab0:rs:v1:t5 0xab0:rt:t5:v1 0xad8:rs:a2:a0 0xad8:rt:a3:t2 0xae0:rs:t0:t3 0xb0c:rt:a0:a1 0xb10:rs:v1:a1 0xb10:rt:a0:v1 0xb14:rt:t1:a2 0xb18:rs:t1:a2 0xb18:rt:t1:a2 0xb20:rs:t1:a2 0xb20:rt:a0:a1 0xb60:rt:t9:t2 */
/* @PostProc: patch_insn_reg_field 0xb64:rs:t9:t2 0xb64:rt:t9:t2 0xb6c:rs:a3:a0 0xb6c:rt:t9:t2 0xb70:rt:t6:t3 0xb74:rs:t6:t3 0xb74:rt:t6:t3 0xb78:rt:at:a1 0xb7c:rs:t6:a1 0xb7c:rt:at:t3 0xb80:rt:t7:t9 0xb88:rt:t7:t9 0xb94:rt:t8:t6 0xb9c:rt:t8:t6 0xba8:rt:t0:v1 0xbac:rt:a2:t2 0xbb0:rt:v1:t3 0xbb4:rs:t0:t3 0xbb4:rt:t0:t3 0xbb8:rs:a2:t2 0xbb8:rt:a2:t2 0xbc0:rs:t1:a2 0xbc0:rt:t3:t1 0xbc4:rt:a0:a1 */
/* @PostProc: patch_insn_reg_field 0xbd8:rs:t2:a3 0xbe8:rt:t9:t7 0xbec:rs:t9:t7 0xbec:rt:t9:t7 0xbf4:rs:t9:t7 0xbf8:rt:t6:t8 0xbfc:rs:t6:t8 0xbfc:rt:t6:t8 0xc04:rs:t6:t8 0xc10:rs:a2:a0 0xc10:rt:a3:t2 0xc18:rs:t0:a1 0xc18:rt:a0:t3 0xc1c:rt:t7:t9 0xc24:rt:t7:t9 0xc30:rt:t8:t6 0xc38:rt:t8:t6 0xc44:rs:t1:a2 0xc44:rt:a3:a0 0xc58:rt:t9:t7 0xc5c:rs:t9:t7 0xc5c:rt:t9:t7 0xc64:rs:t9:t7 0xc6c:rs:t2:a3 */
/* @PostProc: patch_insn_reg_field 0xc74:rs:a1:t0 0xc84:rs:a2:a0 0xc84:rt:a3:t2 0xc8c:rs:t0:t3 0xc90:rt:t6:t8 0xc98:rt:t6:t8 0xca4:rt:t7:t9 0xcac:rt:t7:t9 0xcbc:rs:t1:a2 0xcc0:rt:t4:t1 0xcc4:rs:v1:t1 0xcc4:rt:t4:v1 0xccc:rs:v1:a0 0xccc:rt:a3:v1 0xcd4:rs:v1:t5 0xcd4:rt:t5:v1 0xcd8:rt:t8:t6 0xcdc:rs:t8:t6 0xcdc:rt:t8:t6 0xce4:rs:t8:t6 0xcec:rs:a2:a0 0xcec:rt:a3:t2 0xcf4:rs:t0:t3 0xcf8:rt:t9:t7 */
/* @PostProc: patch_insn_reg_field 0xd00:rt:t9:t7 0xd0c:rt:t6:t8 0xd14:rt:t6:t8 0xd28:rt:t1:a2 0xd2c:rs:t1:a2 0xd2c:rt:t1:a2 0xd34:rs:t1:a2 0xd54:rt:t7:t9 0xd58:rs:t7:t9 0xd58:rt:t7:t9 0xd60:rs:t7:t9 0xd64:rt:t8:t6 0xd68:rs:t8:t6 0xd68:rt:t8:t6 0xd70:rs:t8:t6 0xd74:rt:t9:t2 0xd78:rs:t9:t2 0xd78:rt:t9:t2 0xd80:rs:a3:a0 0xd80:rt:t9:t2 0xd84:rt:t6:t3 0xd88:rs:t6:t3 0xd88:rt:t6:t3 0xd90:rt:t6:t3 */
/* @PostProc: patch_insn_reg_field 0xdbc:rt:t0:v1 0xdc0:rt:a2:t2 0xdc4:rt:v1:t3 0xdc8:rs:t0:t3 0xdc8:rt:t0:t3 0xdcc:rs:a2:t2 0xdcc:rt:a2:t2 0xdd8:rs:t1:a2 0xdf0:rs:t2:a3 0xdf8:rs:a1:t0 0xe2c:rs:a2:a0 0xe2c:rt:a3:t2 0xe34:rs:t0:t3 0xe60:rs:t1:a2 0xe60:rt:a0:a1 0xe88:rs:a2:a0 0xe88:rt:a3:t2 0xe8c:rt:a0:a1 0xe90:rs:t0:a1 0xe90:rt:a0:t3 0xec0:rs:t1:a2 0xec4:rt:a0:a1 0xec8:rt:t4:t1 0xecc:rs:v1:t1 */
/* @PostProc: patch_insn_reg_field 0xecc:rt:t4:v1 0xed4:rs:v1:a0 0xed4:rt:a3:v1 0xedc:rs:v1:t5 0xedc:rt:t5:v1 0xef4:rs:a2:a0 0xef4:rt:a3:t2 0xefc:rs:t0:a1 0xefc:rt:a0:t3 0xf2c:rs:v1:ra 0xf2c:rt:ra:v1 0xf30:rt:t1:a2 0xf34:rs:t1:a2 0xf34:rt:t1:a2 0xf3c:rs:t1:a2 0xf7c:rt:t9:t2 0xf80:rs:t9:t2 0xf80:rt:t9:t2 0xf88:rs:a3:a0 0xf88:rt:t9:t2 0xf8c:rt:t6:t3 0xf90:rs:t6:t3 0xf90:rt:t6:t3 0xf98:rs:t6:t3 */
/* @PostProc: patch_insn_reg_field 0xf9c:rt:t7:t9 0xfa4:rt:t7:t9 0xfb0:rt:t8:t6 0xfb8:rt:t8:t6 0xfc4:rt:t0:v1 0xfc8:rt:a2:t2 0xfcc:rt:v1:t3 0xfd0:rs:t0:t3 0xfd0:rt:t0:t3 0xfd4:rs:a2:t2 0xfd4:rt:a2:t2 0xfe0:rs:t1:a2 0xff4:rt:t4:t1 0xff8:rs:a2:a0 0xff8:rt:a3:t2 0x1000:rs:t0:t3 0x1004:rt:t9:t7 0x100c:rt:t9:t7 0x1018:rt:t6:t8 0x1020:rt:t6:t8 0x102c:rt:t4:t1 0x1030:rs:t1:a2 0x1030:rt:t4:t1 0x1034:rt:t7:t9 */
/* @PostProc: patch_insn_reg_field 0x1038:rs:t7:t9 0x1038:rt:t7:t9 0x1040:rs:a3:t9 0x1040:rt:t7:a0 0x1054:rt:t8:t6 0x1058:rs:t8:t6 0x1058:rt:t8:t6 0x1060:rs:t8:t6 0x1068:rs:t2:a3 0x1070:rs:a1:t0 0x1080:rs:a2:a0 0x1080:rt:a3:t2 0x1088:rs:t0:t3 0x108c:rt:t9:t7 0x1094:rt:t9:t7 0x10a0:rt:t6:t8 0x10a8:rt:t6:t8 0x10b8:rs:t1:a2 0x10c0:rs:v1:t1 0x10c0:rt:t4:v1 0x10c8:rs:v1:a0 0x10c8:rt:a3:v1 0x10d0:rs:v1:t5 0x10d0:rt:t5:v1 */
/* @PostProc: patch_insn_reg_field 0x10d4:rt:t7:t9 0x10d8:rs:t7:t9 0x10d8:rt:t7:t9 0x10e0:rs:t7:t9 0x10e8:rs:t2:a3 0x10f0:rs:a1:t0 0x1100:rs:a2:a0 0x1100:rt:a3:t2 0x1104:rt:t3:t4 0x1108:rs:t0:t4 0x110c:rt:t8:t6 0x1114:rt:t8:t6 0x1120:rt:t9:t7 0x1128:rt:t9:t7 0x1134:rt:t6:t8 0x1138:rs:t6:t8 0x1138:rt:t6:t8 0x1140:rs:t6:t8 0x1148:rs:t2:a3 0x1150:rs:a1:t0 0x1160:rs:a2:t2 0x1168:rs:t0:t3 0x116c:rt:t7:t9 0x1174:rt:t7:t9 */
/* @PostProc: patch_insn_reg_field 0x1180:rt:t8:t6 0x1188:rt:t8:t6 0x1194:rt:t3:t4 0x1198:rs:v1:t4 0x1198:rt:t3:v1 0x119c:rt:t1:a2 0x11a0:rs:t1:a2 0x11a0:rt:t1:a2 0x11a4:rt:a0:a1 0x11a8:rs:t1:a2 0x11a8:rt:a0:a1 0x11c8:rt:t9:t7 0x11cc:rs:t9:t7 0x11cc:rt:t9:t7 0x11d4:rs:t9:t7 0x11d8:rt:t6:t8 0x11dc:rs:t6:t8 0x11dc:rt:t6:t8 0x11e4:rs:t6:t8 0x11e8:rt:t7:t9 0x11ec:rs:t7:t9 0x11ec:rt:t7:t9 0x11f4:rs:t7:t9 0x11f8:rt:t8:t2 */
/* @PostProc: patch_insn_reg_field 0x11fc:rs:t8:t2 0x11fc:rt:t8:t2 0x1204:rs:a3:a0 0x1204:rt:t8:t2 0x1208:rt:t9:t3 0x120c:rs:t9:t3 0x120c:rt:t9:t3 0x1214:rs:t9:t3 0x1240:rt:t0:v1 0x1244:rt:a2:t2 0x1248:rt:v1:t3 0x124c:rs:t0:t3 0x124c:rt:t0:t3 0x1250:rs:a2:t2 0x1250:rt:a2:t2 0x1258:rt:t4:t1 0x125c:rs:t1:a2 0x125c:rt:t4:t1 0x126c:rs:a3:t8 0x126c:rt:t8:a0 0x1284:rs:t2:a3 0x128c:rs:a1:t0 0x12ac:rs:a2:a0 0x12ac:rt:a3:t2 */
/* @PostProc: patch_insn_reg_field 0x12b4:rs:t0:t3 0x12e4:rs:t1:a2 0x131c:rs:a2:a0 0x131c:rt:a3:t2 0x1324:rs:t0:t3 0x1354:rs:t1:a2 0x135c:rs:v1:t1 0x135c:rt:t4:v1 0x1364:rs:v1:a0 0x1364:rt:a3:v1 0x136c:rs:v1:t5 0x136c:rt:t5:v1 0x13b0:rt:a2:t2 0x13b4:rs:a2:t2 0x13b4:rt:a2:t2 0x13b8:rt:t0:t3 0x13bc:rs:t0:t3 0x13bc:rt:t0:t3 0x13c0:rs:a2:a0 0x13c0:rt:a3:t2 0x13c8:rs:t0:t4 0x13f4:rt:t0:v1 0x13f8:rt:a2:t2 0x13fc:rt:v1:t3 */
/* @PostProc: patch_insn_reg_field 0x1400:rs:t0:t3 0x1400:rt:t0:t3 0x1404:rs:a2:t2 0x1404:rt:a2:t2 0x140c:rs:t1:a2 0x140c:rt:t3:t4 0x1410:rs:v1:t1 0x1410:rt:t4:v1 0x1414:rt:a0:v0 0x1418:rs:a0:v0 0x1428:rt:a0:v0 0x142c:rs:a0:v0 0x142c:rt:a0:v0 0x143c:rs:a0:v0 0x1444:rt:a0:v0 0x1448:rs:t4:t1 0x144c:rt:a0:v0 0x1450:rs:a0:v0 0x1458:rs:a1:t4 0x1458:rt:t3:a1 0x1458:rd:a0:v0 0x145c:rs:a0:v0 0x145c:rt:a0:v0 0x1464:rs:a0:v0 */
/* @PostProc: patch_insn_reg_field 0x1464:rt:t8:t7 0x1468:rs:t8:t7 0x146c:rt:a0:v0 0x1470:rs:t4:t1 0x1474:rt:a0:v0 0x1478:rs:a0:v0 0x1480:rs:a1:ra 0x1480:rt:ra:a1 0x1480:rd:a0:v0 0x1484:rs:a0:v0 0x1484:rt:a0:v0 0x148c:rs:a0:v0 0x148c:rt:t6:t8 0x1490:rs:t6:t8 0x1494:rt:a0:v0 0x1498:rs:t2:a3 0x14a0:rt:t7:t9 0x14a4:rs:t7:t9 0x14a4:rt:t7:t9 0x14ac:rt:t7:t9 0x14ac:rd:t8:t7 0x14b0:rs:t8:t7 0x14b8:rs:a2:a0 0x14b8:rt:a3:t2 */
/* @PostProc: patch_insn_reg_field 0x14c0:rs:t0:t3 0x14c4:rt:t9:t8 0x14cc:rt:t9:t8 0x14d8:rt:t6:t9 0x14e0:rt:t6:t9 0x14ec:rt:a0:a1 0x14f0:rs:v1:a1 0x14f0:rt:a0:v1 0x14f4:rt:t1:a2 0x14f8:rs:t1:a2 0x14f8:rt:t1:a2 0x14fc:rt:t4:t1 0x1500:rs:t1:a2 0x1500:rt:t4:t1 0x1504:rt:t7:t6 0x1508:rs:t7:t6 0x1508:rt:t7:t6 0x1510:rs:t7:t6 0x1530:rt:t8:t7 0x1534:rs:t8:t7 0x1534:rt:t8:t7 0x153c:rs:t8:t7 0x1540:rt:t9:t2 0x1544:rs:t9:t2 */
/* @PostProc: patch_insn_reg_field 0x1544:rt:t9:t2 0x154c:rs:a3:a0 0x154c:rt:t9:t2 0x1550:rt:t6:t3 0x1554:rs:t6:t3 0x1554:rt:t6:t3 0x155c:rs:t6:t3 0x1560:rt:t7:t8 0x1568:rt:t7:t8 0x1574:rt:t8:t9 0x157c:rt:t8:t9 0x1588:rt:t0:v1 0x158c:rt:a2:t2 0x1590:rt:v1:t3 0x1594:rs:t0:t3 0x1594:rt:t0:t3 0x1598:rs:a2:t2 0x1598:rt:a2:t2 0x15a0:rs:t1:a2 0x15a0:rt:a3:a0 0x15b8:rs:t2:a3 0x15c0:rs:a1:t0 0x15cc:rt:t9:t6 0x15d0:rs:t9:t6 */
/* @PostProc: patch_insn_reg_field 0x15d0:rt:t9:t6 0x15d8:rs:t9:t6 0x15e0:rs:a2:t2 0x15e8:rs:t0:t3 0x15ec:rt:t6:t7 0x15f4:rt:t6:t7 0x1600:rt:t7:t8 0x1608:rt:t7:t8 0x1618:rs:t1:a2 0x162c:rt:t8:t9 0x1630:rs:t8:t9 0x1630:rt:t8:t9 0x1638:rs:t8:t9 0x1640:rs:a2:a0 0x1640:rt:a3:t2 0x1648:rs:t0:a1 0x1648:rt:a0:t3 0x164c:rt:t9:t6 0x1654:rt:t9:t6 0x1660:rt:t6:t7 0x1668:rt:t6:t7 0x1678:rs:t1:a2 0x1680:rs:v1:t1 0x1680:rt:t4:v1 */
/* @PostProc: patch_insn_reg_field 0x1688:rs:v1:a0 0x1688:rt:a3:v1 0x1690:rs:v1:t5 0x1690:rt:t5:v1 0x1694:rt:t7:t8 0x1698:rs:t7:t8 0x1698:rt:t7:t8 0x16a0:rs:t7:t8 0x16a8:rs:t2:a3 0x16b0:rs:a1:t0 0x16bc:rt:t8:t9 0x16c0:rs:t8:t9 0x16c0:rt:t8:t9 0x16c8:rs:t8:t9 0x16d4:rs:a2:a0 0x16d4:rt:a3:t2 0x16dc:rs:t0:t4 0x16e0:rt:t9:t6 0x16e8:rt:t9:t6 0x16f4:rt:t6:t7 0x16fc:rt:t6:t7 0x1710:rt:t1:a2 0x1714:rs:t1:a2 0x1714:rt:t1:a2 */
/* @PostProc: patch_insn_reg_field 0x1718:rt:t4:t1 0x171c:rs:t1:a2 0x171c:rt:t4:t1 0x173c:rt:t7:t8 0x1740:rs:t7:t8 0x1740:rt:t7:t8 0x1748:rs:t7:t8 0x174c:rt:t8:t2 0x1750:rs:t8:t2 0x1750:rt:t8:t2 0x1758:rs:a3:a0 0x1758:rt:t8:t2 0x175c:rt:t9:t3 0x1760:rs:t9:t3 0x1760:rt:t9:t3 0x1768:rs:t3:t4 0x1768:rt:t9:t3 0x176c:rt:t6:t9 0x1774:rt:t6:t9 0x1780:rt:t7:t6 0x1788:rt:t7:t6 0x1794:rt:t0:v1 0x1798:rt:a2:t2 0x179c:rt:v1:t3 */
/* @PostProc: patch_insn_reg_field 0x17a0:rs:t0:t3 0x17a0:rt:t0:t3 0x17a4:rs:a2:t2 0x17a4:rt:a2:t2 0x17ac:rs:t1:a2 0x17ac:rt:a3:a0 0x17c4:rs:t2:a3 0x17cc:rs:a1:t0 0x17d8:rt:t8:t7 0x17dc:rs:t8:t7 0x17dc:rt:t8:t7 0x17e4:rs:t8:t7 0x17ec:rs:a2:a0 0x17ec:rt:a3:t2 0x17f4:rs:t0:a0 0x17f4:rt:a3:t3 0x17f8:rt:t9:t8 0x1800:rt:t9:t8 0x180c:rt:t6:t9 0x1814:rt:t6:t9 0x1824:rs:t1:a2 0x1838:rt:t7:t6 0x183c:rs:t7:t6 0x183c:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x1844:rs:t7:t6 0x1848:rt:t8:t7 0x184c:rs:t8:t7 0x184c:rt:t8:t7 0x1854:rs:t8:t7 0x1860:rs:a2:a0 0x1860:rt:a3:t2 0x1868:rs:t0:t4 0x186c:rt:t9:t8 0x1874:rt:t9:t8 0x1880:rt:t6:t9 0x1888:rt:t6:t9 0x1898:rs:t1:a2 0x18a0:rs:v1:t1 0x18a0:rt:t4:v1 0x18a8:rs:v1:a0 0x18a8:rt:a3:v1 0x18b0:rs:v1:t5 0x18b0:rt:t5:v1 0x18b8:rs:t2:a3 0x18c0:rs:a1:t0 0x18cc:rt:t7:t6 0x18d0:rs:t7:t6 0x18d0:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x18d8:rs:t7:t6 0x18e0:rs:a2:a0 0x18e0:rt:a3:t2 0x18e8:rs:t0:t4 0x18ec:rt:t8:t7 0x18f4:rt:t8:t7 0x1900:rt:t9:t8 0x1908:rt:t9:t8 0x1914:rt:t1:a2 0x1918:rs:t1:a2 0x1918:rt:t1:a2 0x191c:rt:a1:t0 0x1920:rs:t1:a2 0x1920:rt:a1:t0 0x1924:rt:t6:t9 0x1928:rs:t6:t9 0x1928:rt:t6:t9 0x1930:rt:t6:t9 0x1934:rt:t7:t2 0x1938:rs:t7:t2 0x1938:rt:t7:t2 0x1940:rs:a3:a0 0x1940:rt:t7:t2 0x1944:rt:t8:t3 */
/* @PostProc: patch_insn_reg_field 0x1948:rs:t8:t3 0x1948:rt:t8:t3 0x1950:rs:a0:a1 0x1950:rt:t8:t3 0x1970:rt:t0:t2 0x1974:rt:a2:t3 0x1978:rs:t0:t3 0x1978:rt:t0:t3 0x197c:rs:a2:t2 0x197c:rt:a2:t2 0x1990:rt:t9:t6 0x1994:rs:t9:t6 0x1994:rt:t9:t6 0x199c:rs:t9:t6 0x19a4:rt:t6:t7 0x19a8:rs:t6:t7 0x19a8:rt:t6:t7 0x19b0:rs:t6:t7 0x19b8:rs:a2:t2 0x19c0:rs:t0:t3 0x19c4:rt:t7:t8 0x19cc:rt:t7:t8 0x19d4:rt:t8:t9 0x19d8:rt:t8:t9 */
/* @PostProc: patch_insn_reg_field 0x19e8:rt:t9:t6 0x19ec:rt:t9:t6 0x19f4:rt:t6:t7 0x19f8:rt:t6:t7 0x1a04:rt:t7:t8 0x1a0c:rt:t7:t8 0x1a1c:rs:t1:a2 0x1a30:rt:t8:t9 0x1a34:rs:t8:t9 0x1a34:rt:t8:t9 0x1a3c:rs:t8:t9 0x1a44:rt:t9:t6 0x1a48:rs:t9:t6 0x1a48:rt:t9:t6 0x1a50:rs:t9:t6 0x1a58:rs:a2:t2 0x1a60:rs:t0:t3 0x1a64:rt:t6:t7 0x1a6c:rt:t6:t7 0x1a74:rt:t7:t8 0x1a78:rt:t7:t8 0x1a88:rt:t8:t9 0x1a8c:rt:t8:t9 0x1a94:rt:t9:t6 */
/* @PostProc: patch_insn_reg_field 0x1a98:rt:t9:t6 0x1aa4:rt:t6:t7 0x1aac:rt:t6:t7 */
#include "types.h"

extern s32 func_8002328C(s32 girl);
extern s32 func_8002336C(s32 girl);
extern void func_80011E28(s32 event);
extern s32 func_80011DFC(void);

extern volatile u8 D_800B1746;
extern volatile u8 D_801243B0;
extern volatile u8 D_800B0895;
extern volatile u8 D_800B0896;
extern volatile u8 D_800B0897;
extern volatile u8 D_800B1AF5;
extern volatile u8 D_800B1AF6;
extern u8 D_800B0A34[];
extern u8 D_800B0A36[];
extern u8 D_800B0A5E;
extern s16 D_800B095E;
extern s32 D_800B0944;
extern s32 D_800B1AE4;

extern u8 D_800B0E2A, D_800B0E2B, D_800B0E2C, D_800B0E2D;
extern u8 D_800B0E31, D_800B0E32, D_800B0E33, D_800B0E34;
extern u8 D_800B0E37, D_800B0E38, D_800B0E39, D_800B0E3A;
extern u8 D_800B0E3D, D_800B0E3E, D_800B0E3F, D_800B0E40;
extern u8 D_800B0E43, D_800B0E44, D_800B0E45, D_800B0E46;
extern u8 D_800B0E4A, D_800B0E4B, D_800B0E4C, D_800B0E4D;
extern u8 D_800B0E50, D_800B0E51, D_800B0E52, D_800B0E53;
extern u8 D_800B0E56, D_800B0E57, D_800B0E58, D_800B0E59;
extern u8 D_800B0E5A, D_800B0E5D, D_800B0E5E, D_800B0E5F;
extern u8 D_800B0E60, D_800B0E61, D_800B0E64, D_800B0E65;
extern u8 D_800B0E66, D_800B0E67, D_800B0E6B, D_800B0E6C;
extern u8 D_800B0E6D, D_800B0E6E, D_800B0E70, D_800B0E72, D_800B0E74;
extern u8 D_800B0F28, D_800B0F29, D_800B0F2A;

extern u8 D_801252F4, D_801252F8;
extern u8 D_80125300, D_80125304, D_80125310, D_80125314;
extern u8 D_80125318, D_8012531C, D_80125320, D_80125324;
extern u8 D_80125328, D_8012532C;

extern s32 D_800EECB0, D_800EECB4, D_800EECBC;

s32 func_8014C3A0(void)
{
    u8 girl;
    u32 g;
    s32 r1;
    u32 r2;
    u8 mhi, mlo;
    u32 st;
    u32 ph2;
    u32 f5, f6;
    u32 day;
    u32 v;

    girl = D_800B1746;
    r1 = func_8002328C(girl);
    girl = D_800B1746;
    mhi = r1 & 0x80;
    mlo = r1 & 0x7F;
    r2 = func_8002336C(girl);
    g = D_800B1746;

    if (g == 0) {
        st = D_801243B0;
        if (st == 1) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E2A == 0 && D_800B1AF6 == 13) {
                D_800EECB0 = 4;
                func_80011E28(96);
                D_800B0E2A = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        ph2 = D_800B0896;
        if (st == 16 && ph2 >= 6U && ph2 < 9U
            && mlo < 3U && r2 < 2 && D_800B0E2B == 0
            && D_800B0A34[D_800B1746 * 52] != 0
            && D_800B1AF5 == 1U && f6 == 10) {
            D_800EECB0 = 6;
            func_80011E28(96);
            D_800B0E2B = 1;
            return 0;
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 0 && g >= 9U && g < 12U
            && mlo < 2U && r2 == 0 && D_800B0E2C == 0
            && f5 == 1 && f6 == 1) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E2C = 1;
            return 0;
        }
        if (st == 12 && (g == 12 || g == 1 || g == 2)
            && mlo == 0 && r2 == 0 && mhi != 0 && D_800B0E2D == 0
            && D_800EECBC == 2 && f5 == 1 && f6 == 35) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E2D = 1;
            return 0;
        }
    } else if (g == 1) {
        st = D_801243B0;
        if (st == 12) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E31 == 0
                && D_800EECBC == 0 && D_80125304 != 0
                && D_800B1AF5 == 1U && D_800B1AF6 == 35) {
                D_800EECB0 = 2;
                func_80011E28(96);
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 17 && g >= 6U && g < 9U
            && D_800B0E32 == 0 && mlo < 2U
            && f5 == 1 && f6 == 10) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E32 = 1;
            return 0;
        }
        if (st == 8 && g >= 9U && g < 12U
            && (D_800B0E33 == 0
                || (D_800B0E33 == 1U && !(D_800B1AE4 & 1)))
            && f5 == 1 && f6 == 10) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E33 = 1;
            return 0;
        }
        if (st == 14 && (g == 12 || g == 1 || g == 2)
            && D_800B0E34 == 0 && D_80125314 == 6
            && f5 == 1 && f6 == 17) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E34 = 1;
            return 0;
        }
    } else if (g == 2) {
        st = D_801243B0;
        if (st == 4) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E37 == 0
                && D_800B0895 >= 0x60U
                && D_800B1AF5 == 1U && D_800B1AF6 == 9) {
                D_800EECB0 = 2;
                func_80011E28(96);
                D_800B0E37 = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 10 && g >= 6U && g < 9U
            && D_800B0E38 == 0 && D_800B0895 == 0x61
            && f5 == 1 && f6 == 9) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E38 = 1;
            return 0;
        }
        if (st == 9 && g >= 9U && g < 12U
            && D_800B0E39 == 0 && D_800B0895 >= 0x60U
            && D_80125300 != 0 && D_800EECBC == 1
            && f5 == 1 && f6 == 13) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E39 = 1;
            return 0;
        }
        if (st == 2 && (g == 12 || g == 1 || g == 2)
            && D_80125324 == 2U
            && f5 == 1 && f6 == 14) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E3A = 1;
            return 0;
        }
    } else if (g == 3) {
        st = D_801243B0;
        if (st == 11) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E3D == 0
                && D_800B0895 == 0x61
                && D_800B1AF5 == 1U && D_800B1AF6 == 9) {
                D_800EECB0 = 2;
                func_80011E28(96);
                D_800B0E3D = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 17 && g >= 6U && g < 9U
            && D_800B0E3E == 0
            && f5 == 1 && f6 == 10) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E3E = 1;
            return 0;
        }
        if (st == 7 && g >= 9U && g < 12U
            && D_800B0E3F == 0 && D_801252F4 == 1U
            && f5 == 1 && f6 == 7) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E3F = 1;
            return 0;
        }
        if (st == 6 && (g == 12 || g == 1 || g == 2)
            && mlo < 2U && r2 == 0 && D_800B0E40 == 0
            && D_800B0895 >= 0x61U
            && f5 == 1 && f6 == 11) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E40 = 1;
            return 0;
        }
    } else if (g == 4) {
        st = D_801243B0;
        if (st == 1) {
            g = D_800B0896;
            if (g >= 3U && g < 6U
                && mlo == 0 && mhi != 0 && r2 == 0 && D_800B0E43 == 0
                && D_800B1AF5 == 1U && D_800B1AF6 == 13) {
                D_800EECB0 = 2;
                func_80011E28(96);
                D_800B0E43 = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 16 && g >= 6U && g < 9U
            && mlo == 0 && mhi != 0 && r2 == 0 && D_800B0E44 == 0
            && f5 == 1 && f6 == 10) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E44 = 1;
            return 0;
        }
        if (st == 13 && D_8012532C == 0 && g >= 9U && g < 12U
            && D_800B0E45 == 0 && D_8012531C < 2U
            && f5 == 1 && f6 == 8) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E45 = 1;
            return 0;
        }
        if (st == 2 && (g == 12 || g == 1 || g == 2)
            && D_800B0E46 == 0 && D_800EECBC == 1
            && f5 == 1 && f6 == 14) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E46 = 1;
            return 0;
        }
    } else if (g == 5) {
        st = D_801243B0;
        if (st == 5) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E4A == 0
                && D_800B0895 == 0x61
                && D_800B1AF5 == 1U && D_800B1AF6 == 9) {
                D_800EECB0 = 3;
                func_80011E28(96);
                D_800B0E4A = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 3 && g >= 6U && g < 9U
            && mlo < 3U && r2 < 2 && D_800B0E4B == 0
            && D_800B0895 >= 0x61U
            && f5 == 1 && f6 == 9) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E4B = 1;
            return 0;
        }
        if (st == 1 && g >= 9U && g < 12U
            && D_800B0E4C == 0 && mlo == 0 && mhi != 0 && r2 == 0
            && f5 == 1 && f6 == 13) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E4C = 1;
            return 0;
        }
        if (st == 20 && (g == 12 || g == 1 || g == 2)
            && D_800B0E4D == 0
            && f5 == 1 && f6 == 0) {
            D_800EECB0 = 6;
            func_80011E28(96);
            D_800B0E4D = 1;
            return 0;
        }
    } else if (g == 6) {
        st = D_801243B0;
        if (st == 7) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E50 == 0
                && D_801252F8 != 0
                && D_800B1AF5 == 1U && D_800B1AF6 == 7U) {
                D_800EECB0 = 2;
                func_80011E28(96);
                D_800B0E50 = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 17 && g >= 6U && g < 9U
            && mlo == 0 && mhi != 0 && r2 == 0 && D_800B0E51 == 0
            && D_800B095E >= 100
            && f5 == 1 && f6 == 10) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E51 = 1;
            return 0;
        }
        if (st == 5 && g >= 9U && g < 12U
            && D_800B0E52 == 0
            && f5 == 1 && f6 == 9) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E52 = 1;
            return 0;
        }
        if (st == 10 && (g == 12 || g == 1 || g == 2)
            && D_800B0E53 == 0
            && f5 == 1 && f6 == 9) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E53 = 1;
            return 0;
        }
    } else if (g == 7) {
        st = D_801243B0;
        if (st == 2 && D_800EECBC == 0) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E56 == 0
                && D_800B1AF5 == 1U && D_800B1AF6 == 15) {
                D_800EECB0 = 2;
                func_80011E28(96);
                D_800B0E56 = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 17 && g >= 6U && g < 9U
            && f5 == 1 && f6 == 10) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E57 = 1;
            return 0;
        }
        if (st == 12 && D_80125320 == 1U && g >= 9U && g < 12U
            && D_800B0E58 == 0 && mlo == 0 && mhi != 0 && r2 == 0
            && f5 == 1 && f6 == 56) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E58 = 1;
            return 0;
        }
        if (st == 20 && (g == 12 || g == 1 || g == 2)
            && D_800B0E59 == 0 && mlo == 0 && mhi != 0 && r2 == 0
            && f5 == 1 && f6 == 8) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E59 = 1;
            return 0;
        }
        if (D_800B0E5A == 0 && mlo == 0 && mhi != 0 && r2 == 0
            && f5 == 0 && f6 == 41) {
            D_800EECB0 = 6;
            func_80011E28(96);
            D_800B0E5A = 1;
            return 0;
        }
    } else if (g == 8) {
        st = D_801243B0;
        if (st == 9) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E5D == 0
                && (D_80125300 == 0 || D_800EECBC == 0)
                && D_800B1AF5 == 1U && D_800B1AF6 == 13) {
                D_800EECB0 = 2;
                func_80011E28(96);
                D_800B0E5D = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 12 && D_80125310 == 1U && g >= 6U && g < 9U
            && mlo == 0 && mhi != 0 && r2 == 0 && D_800B0E5E == 0
            && f5 == 1 && f6 == 60) {
            D_800EECB0 = 3;
            func_80011E28(96);
            D_800B0E5E = 1;
            return 0;
        }
        if (st == 15 && D_80125318 == 10 && g >= 9U && g < 12U
            && D_800B0E5F == 0
            && f5 == 1 && f6 == 17) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E5F = 1;
            return 0;
        }
        if (st == 19 && (g == 12 || g == 1 || g == 2)
            && D_800B0E60 == 0
            && D_800B0A36[D_800B1746 * 52] != 0) {
            f5 = D_800B1AF5;
            f6 = D_800B1AF6;
            if (f5 == 1U && f6 == 8U) {
                D_800EECB0 = 5;
                func_80011E28(96);
                D_800B0E60 = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 8) {
            v = (g == 12);
            if (v != 0) {
                day = D_800B0897;
                v = (day == 10);
            }
            day = D_800B0897;
            if ((D_800B0895 = v) != 0
                || (D_800B0895 = (v = ((12 ^ g) == 0 && day == 8))) != 0
                || (D_800B0895 = (v = ((12 ^ g) == 0 && day == 7))) != 0) {
                if (mlo < 3U && (D_800B0944 << 30) < 0
                    && f5 == 1 && f6 == 0) {
                    D_800EECB0 = 6;
                    func_80011E28(96);
                    D_800B0E61 = 1;
                    return 0;
                }
            }
        }
    } else if (g == 9) {
        st = D_801243B0;
        if (st == 12 && D_80125328 != 0) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E64 == 0
                && D_800B1AF5 == 1U && D_800B1AF6 == 27) {
                D_800EECB0 = 3;
                func_80011E28(96);
                D_800B0E64 = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 1 && g >= 6U && g < 9U
            && mlo == 0 && mhi != 0 && r2 == 0 && D_800B0E65 == 0
            && f5 == 0 && f6 == 41) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E65 = 1;
            return 0;
        }
        if (st == 4 && g >= 9U && g < 12U
            && D_800B0E66 == 0
            && f5 == 1 && f6 == 9) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E66 = 1;
            return 0;
        }
        if (st == 19 && (g == 12 || g == 1 || g == 2)
            && D_800B0E67 == 0 && mlo == 0 && mhi != 0 && r2 == 0
            && D_800B095E >= 80
            && f5 == 1 && f6 == 8) {
            D_800EECB0 = 6;
            func_80011E28(96);
            D_800B0E67 = 1;
            return 0;
        }
    } else if (g == 10) {
        st = D_801243B0;
        if (st == 12) {
            g = D_800B0896;
            if (g >= 3U && g < 6U && D_800B0E6B == 0
                && D_800B1AF5 == 1U && D_800B1AF6 == 8U) {
                D_800EECB0 = 3;
                func_80011E28(96);
                D_800B0E6B = 1;
                return 0;
            }
        }
        f6 = D_800B1AF6;
        f5 = D_800B1AF5;
        g = D_800B0896;
        if (st == 1 && g >= 6U && g < 9U
            && mlo == 0 && mhi != 0 && r2 == 0 && D_800B0E6C == 0
            && f5 == 1 && f6 == 1) {
            D_800EECB0 = 4;
            func_80011E28(96);
            D_800B0E6C = 1;
            return 0;
        }
        if (st == 13 && g >= 9U && g < 12U
            && D_800B0E6D == 0 && D_8012531C >= 2U
            && f5 == 1 && f6 == 8) {
            D_800EECB0 = 5;
            func_80011E28(96);
            D_800B0E6D = 1;
            return 0;
        }
        if (st == 20 && (g == 12 || g == 1 || g == 2)
            && mlo == 0 && mhi != 0 && r2 == 0 && D_800B0E6E == 0
            && f5 == 1 && f6 == 8) {
            D_800EECB0 = 6;
            func_80011E28(96);
            D_800B0E6E = 1;
            return 0;
        }
    } else {
        st = D_801243B0;
        if (st == 11 && D_800B0A5E == 2U
            && D_800B1AF5 == 1U && D_800B1AF6 == 9U) {
            func_80011E28(209);
            return 0;
        }
    }

    g = D_800B0896;
    f6 = D_800B1AF6;
    f5 = D_800B1AF5;
    if (g >= 3U && g < 6U
        && D_800B0E70 >= 2U && D_800B0E72 == 0
        && f5 == 0 && f6 == 41) {
        D_800EECB0 = 3;
        D_800B0F28 = 0x50;
        D_800B0F29 = 0;
        D_800B0F2A = 12;
        D_800EECB4 = 11;
        func_80011E28(96);
        D_800B0E72 = 1;
        return 0;
    }
    if (st == 17 && g >= 3U && g < 6U
        && D_800B0E70 >= 2U && D_800B0E74 == 0
        && f5 == 0 && f6 == 41) {
        D_800EECB0 = 5;
        D_800B0F28 = 0x50;
        D_800B0F29 = 0;
        D_800B0F2A = 12;
        D_800EECB4 = 11;
        func_80011E28(96);
        D_800B0E74 = 1;
        return 0;
    }
    return func_80011DFC();
}
