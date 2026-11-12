/* @PostProc: patch_stack_complete --old 416 --new 400 */
/* @PostProc: remap_stack_slots --from 368 --to 384 --from 404 --to 372 --from 408 --to 368 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1d0 */
/* @PostProc: swap_instructions --offset1 0x4c0 --offset2 0x4cc */
/* @PostProc: swap_instructions --offset1 0x688 --offset2 0x68c */
/* @PostProc: swap_instructions --offset1 0x684 --offset2 0x688 */
/* @PostProc: swap_instructions --offset1 0x680 --offset2 0x684 */
/* @PostProc: swap_instructions --offset1 0x67c --offset2 0x680 */
/* @PostProc: swap_instructions --offset1 0x678 --offset2 0x67c */
/* @PostProc: swap_instructions --offset1 0x674 --offset2 0x678 */
/* @PostProc: swap_instructions --offset1 0x670 --offset2 0x674 */
/* @PostProc: swap_instructions --offset1 0x66c --offset2 0x670 */
/* @PostProc: swap_instructions --offset1 0x668 --offset2 0x66c */
/* @PostProc: swap_instructions --offset1 0x664 --offset2 0x668 */
/* @PostProc: swap_instructions --offset1 0x778 --offset2 0x780 */
/* @PostProc: swap_instructions --offset1 0x77c --offset2 0x784 */
/* @PostProc: swap_instructions --offset1 0x790 --offset2 0x798 */
/* @PostProc: swap_instructions --offset1 0x794 --offset2 0x798 */
/* @PostProc: swap_instructions --offset1 0x7a0 --offset2 0x7ac */
/* @PostProc: swap_instructions --offset1 0x7a4 --offset2 0x7b0 */
/* @PostProc: swap_instructions --offset1 0x7a8 --offset2 0x7b4 */
/* @PostProc: swap_instructions --offset1 0x7ac --offset2 0x7bc */
/* @PostProc: swap_instructions --offset1 0x7b0 --offset2 0x7bc */
/* @PostProc: swap_instructions --offset1 0x7b4 --offset2 0x7bc */
/* @PostProc: swap_instructions --offset1 0x7b8 --offset2 0x7bc */
/* @PostProc: swap_instructions --offset1 0x7c0 --offset2 0x7c8 */
/* @PostProc: swap_instructions --offset1 0x7c4 --offset2 0x7c8 */
/* @PostProc: swap_instructions --offset1 0x84c --offset2 0x850 */
/* @PostProc: swap_instructions --offset1 0xea4 --offset2 0xea8 */
/* @PostProc: swap_instructions --offset1 0xeb8 --offset2 0xebc */
/* @PostProc: swap_instructions --offset1 0xee8 --offset2 0xeec */
/* @PostProc: swap_instructions --offset1 0xef4 --offset2 0xef8 */
/* @PostProc: swap_instructions --offset1 0xf30 --offset2 0xf34 */
/* @PostProc: swap_instructions --offset1 0xf34 --offset2 0xf38 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:v0 0x2c:rs:v1:v0 0x2c:rt:v1:v0 0x34:rs:v1:v0 0x40:rs:v1:v0 0x48:rs:v1:v0 0x58:rs:v1:v0 0x64:rs:v1:v0 0x74:rs:v1:v0 0x7c:rs:v1:v0 0xa4:rs:v1:v0 0xac:rt:v1:v0 0x12c:rt:v1:t8 0x130:rs:v1:t8 0x130:rt:v1:t8 0x138:rs:v1:t8 0x138:rt:t8:t9 0x140:rt:t8:t9 0x164:rt:t9:t2 0x168:rs:t9:t2 0x168:rt:t9:t2 0x16c:rt:t1:t3 0x170:rs:t9:t2 0x17c:rs:t1:t3 0x17c:rt:t1:t3 0x184:rs:t1:t3 0x184:rt:t2:t4 0x188:rs:t2:t4 0x1a8:rt:v1:t5 0x1b4:rt:v1:t5 0x1bc:rt:v1:t6 0x1c4:rt:v1:t6 0x238:rt:v1:t7 0x23c:rs:v1:t7 0x23c:rt:v1:t7 0x244:rs:v1:t7 0x244:rt:t3:t8 0x24c:rt:t3:t8 0x278:rt:t4:t9 0x27c:rs:t4:t9 0x27c:rt:t4:t9 0x280:rt:t5:t2 0x284:rs:t4:t9 0x290:rs:t5:t2 0x290:rt:t5:t2 0x298:rs:t5:t2 0x298:rt:t6:t3 0x29c:rs:t6:t3 0x2cc:rt:v1:t4 0x2d4:rt:v1:t4 0x2e0:rt:t7:t5 0x2e8:rt:t7:t5 0x310:rt:t8:t6 0x324:rt:t9:t7 0x330:rt:t8:t6 0x338:rt:t9:t7 0x344:rt:t1:t8 0x350:rt:t1:t8 0x36c:rt:a3:a2 0x374:rs:a3:a2 0x37c:rt:a3:a2 0x380:rd:t2:t9 0x380:rt:a3:a2 0x388:rt:t2:t9 0x398:rt:a3:a2 0x3d0:rd:t3:t2 0x3d4:rd:t4:t3 0x3d4:rt:t3:t2 0x3d8:rt:t4:t3 0x404:rd:t5:t4 0x408:rd:t6:t5 0x408:rt:t5:t4 0x40c:rt:t6:t5 0x43c:rt:t7:t6 0x440:rs:t7:t6 0x440:rt:t7:t6 0x448:rd:t8:t7 0x448:rt:t7:t6 0x44c:rt:t8:t7 0x484:rt:v1:t8 0x488:rs:v1:t8 0x488:rt:v1:t8 0x490:rs:v1:t8 0x4b8:rt:t1:t0 0x4bc:rs:t1:t0 0x4bc:rt:t1:t0 0x4c4:rt:t1:t0 0x4d8:rt:a3:t0 0x4dc:rs:a3:t0 0x4dc:rt:a3:t0 0x4e4:rd:v0:a3 0x4e4:rt:a3:t0 0x4e8:rs:v0:a3 0x4e8:rt:v0:a3 0x4f0:rt:v0:a3 0x4f4:rd:v0:a3 0x508:rt:v0:a3 0x518:rs:v0:a3 0x520:rt:a3:t0 0x524:rt:a3:t0 0x534:rt:a3:t0 0x5a8:rt:t7:t3 0x5b8:rt:v1:t7 0x5c4:rt:v1:t7 0x5d0:rt:v1:t8 0x5dc:rt:v1:t8 0x5e8:rt:v1:t2 0x5ec:rt:v1:t9 0x5f8:rt:v1:t9 0x600:rt:v1:t2 0x604:rs:t7:t3 0x604:rt:t7:t3 0x60c:rs:t7:t3 0x60c:rt:t8:t4 0x614:rt:t8:t4 0x618:rt:t9:t5 0x61c:rs:t9:t5 0x61c:rt:t9:t5 0x620:rt:t1:t6 0x628:rd:t2:t7 0x628:rs:t9:t5 0x628:rt:t1:t6 0x630:rt:t2:t7 0x63c:rt:v0:t8 0x640:rs:v0:t8 0x640:rt:v0:t8 0x644:rt:a0:v1 0x648:rs:v0:t8 0x648:rt:a0:v1 0x64c:rt:a1:a0 0x650:rs:a1:a0 0x650:rt:a1:a0 0x654:rt:t5:t3 0x658:rs:t5:t3 0x658:rt:t5:t3 0x65c:rt:v0:t6 0x660:rs:t5:t3 0x660:rt:t6:t4 0x664:rt:v0:t3 0x668:rt:a3:t0 0x66c:rd:t3:t9 0x670:rd:v1:v0 0x670:rs:a1:a0 0x670:rt:t3:t9 0x674:rs:v1:v0 0x674:rt:t4:t2 0x67c:rd:t7:t5 0x67c:rs:t4:t2 0x67c:rt:t6:t4 0x680:rs:v1:v0 0x680:rt:t7:t5 0x684:rs:v0:t6 0x684:rt:v0:t6 0x68c:rs:v0:t6 0x68c:rt:a0:v1 0x690:rd:t8:t7 0x694:rd:v1:v0 0x694:rs:a1:a0 0x694:rt:t8:t7 0x698:rs:v1:v0 0x698:rt:t9:t8 0x6a0:rs:t9:t8 0x6a0:rt:t1:t9 0x6a4:rs:v1:v0 0x6a4:rt:t1:t9 0x6a8:rs:v0:t3 0x6a8:rt:v0:t3 0x6b0:rs:v0:t3 0x6b0:rt:a0:v1 0x6b8:rd:v1:v0 0x6b8:rs:a1:a0 0x6bc:rs:v1:v0 0x6bc:rt:t3:t4 0x6c4:rs:t3:t4 0x6cc:rs:v1:v0 0x6d0:rs:a3:t0 0x6d0:rt:a3:t0 0x6d8:rs:a3:t0 0x6e4:rs:a3:t0 0x6f4:rt:t6:t0 0x6f8:rt:t4:t6 0x704:rt:t4:t6 0x708:rs:t6:t0 0x708:rt:t6:t0 0x714:rt:t6:t0 0x720:rt:a3:t0 0x730:rt:t7:t8 0x734:rt:t9:t3 0x738:rs:t7:t8 0x738:rt:t8:t9 0x73c:rs:t9:t3 0x73c:rt:t1:t2 0x740:rd:t2:t4 0x740:rs:t8:t9 0x740:rt:t1:t2 0x744:rt:t3:t5 0x748:rs:t3:t5 0x748:rt:t3:t6 0x74c:rt:t2:t4 0x750:rt:t6:t3 0x754:rd:t5:t7 0x754:rt:t3:t6 0x758:rs:t5:t7 0x758:rt:t4:t8 0x75c:rs:t6:t3 0x75c:rt:t7:t9 0x760:rd:t9:t2 0x760:rs:t4:t8 0x760:rt:t7:t9 0x768:rt:t9:t2 0x770:rt:t1:t7 0x778:rd:t8:t6 0x77c:rs:t1:t7 0x77c:rt:t2:t3 0x784:rt:t6:t4 0x788:rd:t3:t8 0x788:rs:t8:t6 0x788:rt:t2:t3 0x78c:rt:t9:t6 0x790:rs:t9:t6 0x790:rt:t9:t3 0x794:rt:t5:t2 0x798:rs:t6:t4 0x798:rt:t4:t5 0x79c:rd:t8:t4 0x79c:rs:t5:t2 0x79c:rt:t4:t5 0x7a0:rt:t3:t8 0x7a4:rs:t8:t4 0x7a8:rd:t1:t9 0x7a8:rt:t9:t3 0x7ac:rd:t3:t5 0x7ac:rs:t1:t9 0x7b0:rt:t4:t8 0x7b8:rs:t4:t8 0x7b8:rt:t4:t7 0x7bc:rt:t8:t4 0x7c0:rt:t3:t5 0x7c4:rt:t8:t2 0x7c8:rd:t9:t4 0x7c8:rt:t4:t7 0x7cc:rs:t9:t4 0x7cc:rt:t7:t9 0x7d0:rs:t8:t2 0x7d0:rt:t1:t5 0x7d4:rd:t3:t7 0x7d4:rs:t7:t9 0x7d4:rt:t1:t5 0x7d8:rt:t8:t2 0x7dc:rs:t3:t7 0x7dc:rt:t4:t3 0x7e0:rt:t3:t7 0x7e4:rt:t4:t3 0x7e8:rs:t4:t3 0x7e8:rt:t9:t2 0x7ec:rt:t9:t2 0x804:rt:t8:t5 0x808:rs:t8:t5 0x808:rt:t8:t5 0x810:rd:t7:t6 0x810:rt:t8:t5 0x814:rt:t7:t6 0x824:rt:t1:t8 0x828:rs:t1:t8 0x828:rt:t1:t8 0x830:rd:t2:t7 0x830:rt:t1:t8 0x834:rt:t2:t7 0x84c:rt:a3:t0 0x854:rs:a3:t0 0x878:rt:v1:t4 0x87c:rs:v1:t4 0x87c:rt:v1:t4 0x884:rs:v1:t4 0x8c8:rt:a3:a2 0x8d0:rs:a3:a2 0x8d8:rt:a3:a2 0x8dc:rd:t6:t2 0x8dc:rt:a3:a2 0x8e4:rt:t6:t2 0x8f4:rt:a3:a2 0x92c:rd:t4:t9 0x930:rd:t9:t5 0x930:rt:t4:t9 0x934:rt:t9:t5 0x960:rd:t5:t6 0x964:rt:t5:t6 0x9a4:rd:t1:t4 0x9a8:rt:t1:t4 0x9e0:rt:v1:t3 0x9e4:rs:v1:t3 0x9e4:rt:v1:t3 0x9ec:rs:v1:t3 0xa10:rt:t3:t9 0xa14:rs:t3:t9 0xa14:rt:t3:t9 0xa1c:rd:t6:t5 0xa1c:rt:t3:t9 0xa20:rt:t6:t5 0xa30:rt:t4:t6 0xa34:rs:t4:t6 0xa34:rt:t4:t6 0xa3c:rd:t9:t8 0xa3c:rt:t4:t6 0xa40:rt:t9:t8 0xa84:rt:v1:t7 0xa88:rs:v1:t7 0xa88:rt:v1:t7 0xa90:rs:v1:t7 0xa90:rt:t5:t4 0xa98:rt:t5:t4 0xac0:rt:t8:t3 0xac4:rs:t8:t3 0xac4:rt:t8:t3 0xacc:rd:t7:t2 0xacc:rt:t8:t3 0xad0:rt:t7:t2 0xadc:rt:t1:t9 0xae0:rt:v1:t5 0xae4:rs:t1:t9 0xaf4:rt:v1:t5 0xb08:rt:v1:t6 0xb0c:rs:v1:t6 0xb0c:rt:v1:t6 0xb14:rs:v1:t6 0xb14:rt:t2:t8 0xb1c:rt:t2:t8 0xb38:rt:t3:t7 0xb3c:rs:t3:t7 0xb3c:rt:t3:t7 0xb44:rd:t6:t4 0xb44:rt:t3:t7 0xb48:rt:t6:t4 0xb58:rt:t4:t3 0xb5c:rs:t4:t3 0xb5c:rt:t4:t3 0xb64:rd:t9:t2 0xb64:rt:t4:t3 0xb68:rt:t9:t2 0xb74:rt:t5:t9 0xb78:rt:v1:t5 0xb7c:rs:t5:t9 0xb8c:rt:v1:t5 0xba4:rt:v1:t6 0xba8:rs:v1:t6 0xba8:rt:v1:t6 0xbb0:rs:v1:t6 0xc14:rd:t1:t4 0xc18:rt:t1:t4 0xc24:rt:t2:t3 0xc28:rt:v1:t2 0xc2c:rs:t2:t3 0xc3c:rt:v1:t2 0xc50:rt:v1:t9 0xc54:rs:v1:t9 0xc54:rt:v1:t9 0xc5c:rs:v1:t9 0xc5c:rt:t3:t5 0xc64:rt:t3:t5 0xc78:rt:v1:t6 0xc84:rt:v1:t6 0xcb4:rt:v1:t8 0xcb8:rs:v1:t8 0xcb8:rt:v1:t8 0xcc0:rs:v1:t8 0xcc0:rt:t6:t7 0xcc8:rt:t6:t7 0xd5c:rd:t9:t3 0xd60:rd:t5:t2 0xd60:rt:t9:t3 0xd64:rt:t5:t2 0xd90:rd:t8:t9 0xd94:rd:t7:t5 0xd94:rt:t8:t9 0xd98:rt:t7:t5 0xde0:rt:v1:t6 0xde4:rs:v1:t6 0xde4:rt:v1:t6 0xdec:rs:v1:t6 0xdec:rt:t1:t8 0xdf4:rt:t1:t8 0xe08:rt:v1:t7 0xe14:rt:v1:t7 0xe20:rt:t2:t4 0xe24:rs:t2:t4 0xe24:rt:t2:t4 0xe2c:rt:t2:t4 0xe40:rt:v0:t2 0xe44:rs:v0:t2 0xe44:rt:v0:t2 0xe4c:rs:v0:t2 0xe58:rs:v0:t2 0xe5c:rt:t6:t9 0xe60:rs:t6:t9 0xe68:rt:t4:t1 0xe70:rt:t4:t1 0xe74:rt:v0:t5 0xe78:rs:v0:t5 0xe78:rt:v0:t5 0xe7c:rt:a3:t1 0xe80:rs:a3:t1 0xe80:rt:a3:t1 0xe88:rs:v0:t5 0xe8c:rt:a0:a3 0xe90:rs:a0:a3 0xe90:rt:a0:a3 0xe98:rs:a0:a3 0xea0:rt:t0:a1 0xea4:rt:t5:v1 0xeb0:rs:t0:a1 0xeb8:rs:t5:v1 0xeb8:rt:t5:v1 0xec0:rt:a1:a0 0xec4:rt:t5:v1 0xec8:rs:a1:a0 0xec8:rt:a1:a0 0xecc:rs:a1:a0 0xecc:rt:a0:a3 0xee4:rt:a3:t1 0xef8:rt:a1:a0 0xefc:rs:v0:a1 0xefc:rt:t0:v0 0xf00:rs:a1:a0 0xf00:rt:a1:a0 0xf08:rd:t1:t4 0xf08:rs:a1:a0 0xf08:rt:a0:a3 0xf0c:rs:t1:t4 0xf0c:rt:t2:t3 0xf14:rs:t2:t3 0xf18:rd:t3:t2 0xf18:rt:a3:t1 0xf1c:rt:a3:t1 0xf20:rd:t3:t2 0xf20:rt:a3:t1 0xf28:rt:t3:t2 0xf3c:rt:a3:t1 0xf74:rt:v1:t9 0xf78:rs:v1:t9 0xf78:rt:v1:t9 0xf80:rs:v1:t9 0xfb0:rt:t4:t8 0xfb4:rs:t4:t8 0xfb4:rt:t4:t8 0xfbc:rd:t9:t7 0xfbc:rt:t4:t8 0xfc0:rt:t9:t7 0xfdc:rt:v1:t5 0xfe0:rs:v1:t5 0xfe0:rt:v1:t5 0xfe8:rs:v1:t5 0xfe8:rt:t5:t4 0xfec:rt:t5:t4 0x1008:rt:t8:t3 0x100c:rs:t8:t3 0x100c:rt:t8:t3 0x1014:rd:t7:t2 0x1014:rt:t8:t3 0x1018:rt:t7:t2 0x1028:rt:t1:t9 0x102c:rs:t1:t9 0x102c:rt:t1:t9 0x1034:rd:t2:t6 0x1034:rt:t1:t9 0x1038:rt:t2:t6 0x107c:rt:v1:t8 0x1080:rs:v1:t8 0x1080:rt:v1:t8 0x1088:rs:v1:t8 0x1088:rt:t3:t7 0x1090:rt:t3:t7 0x10b8:rt:t6:t5 0x10bc:rs:t6:t5 0x10bc:rt:t6:t5 0x10c4:rt:t6:t5 0x10e4:rt:v1:t3 0x10e8:rs:v1:t3 0x10e8:rt:v1:t3 0x10f0:rs:v1:t3 0x10f0:rt:t9:t2 0x10f8:rt:t9:t2 0x110c:rt:t5:t9 0x1110:rs:t5:t9 0x1110:rt:t5:t9 0x1118:rs:t5:t9 0x111c:rt:v1:t6 0x1128:rt:v1:t6 0x1158:rt:v1:t8 0x115c:rs:v1:t8 0x115c:rt:v1:t8 0x1164:rs:v1:t8 0x1164:rt:t8:t7 0x116c:rt:t8:t7 0x119c:rt:t7:t5 0x11a0:rs:t7:t5 0x11a0:rt:t7:t5 0x11a8:rs:t7:t5 0x11e4:rt:v1:t4 0x11e8:rs:v1:t4 0x11e8:rt:v1:t4 0x11f0:rs:v1:t4 0x11f0:rt:t1:t3 0x11f8:rt:t1:t3 0x120c:rt:v1:t2 0x1218:rt:v1:t2 0x1224:rt:t2:t9 0x1228:rs:t2:t9 0x1228:rt:t2:t9 0x1230:rd:t3:t6 0x1230:rt:t2:t9 0x1234:rt:t3:t6 0x1284:rt:v1:t8 0x1288:rs:v1:t8 0x1288:rt:v1:t8 0x1290:rs:v1:t8 0x1290:rt:t6:t7 0x1298:rt:t6:t7 0x12dc:rt:v1:t5 0x12e0:rs:v1:t5 0x12e0:rt:v1:t5 0x12e8:rs:v1:t5 0x1330:rt:v1:t3 0x1334:rs:v1:t3 0x1334:rt:v1:t3 0x133c:rs:v1:t3 0x133c:rt:t9:t2 0x1344:rt:t9:t2 0x1358:rt:v1:t9 0x1364:rt:v1:t9 0x1370:rt:t5:t6 0x1374:rs:t5:t6 0x1374:rt:t5:t6 0x137c:rt:t5:t6 0x139c:rd:t1:t5 0x13a0:rt:t1:t5 0x13e4:rt:v1:t4 0x13e8:rs:v1:t4 0x13e8:rt:v1:t4 0x13f0:rs:v1:t4 0x13f0:rt:t2:t3 0x13f8:rt:t2:t3 0x1420:rt:t3:t2 0x1424:rs:t3:t2 0x1424:rt:t3:t2 0x142c:rd:t6:t9 0x142c:rt:t3:t2 0x1430:rt:t6:t9 0x144c:rt:v1:t6 0x1450:rs:v1:t6 0x1450:rt:v1:t6 0x1458:rs:v1:t6 0x1458:rt:t4:t8 0x1460:rt:t4:t8 0x147c:rt:t9:t7 0x1480:rs:t9:t7 0x1480:rt:t9:t7 0x1488:rt:t9:t7 0x149c:rt:t8:t4 0x14a0:rs:t8:t4 0x14a0:rt:t8:t4 0x14a8:rd:t7:t3 0x14a8:rt:t8:t4 0x14ac:rt:t7:t3 0x14f0:rt:v1:t2 0x14f4:rs:v1:t2 0x14f4:rt:v1:t2 0x14fc:rs:v1:t2 0x14fc:rt:t1:t9 0x1504:rt:t1:t9 0x1518:rt:t2:t6 0x151c:rs:t2:t6 0x151c:rt:t2:t6 0x1520:rt:a0:t7 0x1524:rd:t3:t8 0x1524:rt:t2:t6 0x1528:rd:a0:t7 0x1528:rs:a0:t7 0x1528:rt:t3:t8 0x152c:rs:a0:t7 0x152c:rt:a0:t7 0x1534:rs:a0:t7 0x1534:rt:t6:t5 0x1538:rt:v1:t4 0x153c:rs:t6:t5 0x1548:rt:v1:t4 0x1560:rt:t4:t3 0x1564:rs:t4:t3 0x1564:rt:t4:t3 0x156c:rd:t9:t2 0x156c:rt:t4:t3 0x1570:rt:t9:t2 0x158c:rt:v1:t9 0x1590:rs:v1:t9 0x1590:rt:v1:t9 0x1598:rs:v1:t9 0x1598:rt:t5:t6 0x15a0:rt:t5:t6 0x15e0:rt:v1:t8 0x15e4:rs:v1:t8 0x15e4:rt:v1:t8 0x15ec:rs:v1:t8 0x15ec:rt:t8:t7 0x15f4:rt:t8:t7 0x1640:rt:v1:t5 0x1644:rs:v1:t5 0x1644:rt:v1:t5 0x164c:rs:v1:t5 0x164c:rt:t7:t4 0x1654:rt:t7:t4 0x1668:rt:v1:t3 0x1674:rt:v1:t3 0x1680:rt:t1:t2 0x1684:rs:t1:t2 0x1684:rt:t1:t2 0x168c:rd:t2:t9 0x168c:rt:t1:t2 0x1690:rt:t2:t9 0x16a0:rt:t3:t6 0x16a4:rs:t3:t6 0x16a4:rt:t3:t6 0x16ac:rd:t6:t8 0x16ac:rt:t3:t6 0x16b0:rt:t6:t8 0x16f4:rt:v1:t7 0x16f8:rs:v1:t7 0x16f8:rt:v1:t7 0x1700:rs:v1:t7 0x1700:rt:t4:t5 0x1708:rt:t4:t5 0x1730:rt:t9:t4 0x1734:rs:t9:t4 0x1734:rt:t9:t4 0x173c:rd:t5:t3 0x173c:rt:t9:t4 0x1740:rt:t5:t3 0x175c:rt:v1:t2 0x1760:rs:v1:t2 0x1760:rt:v1:t2 0x1768:rs:v1:t2 0x1768:rt:t8:t9 0x1770:rt:t8:t9 0x178c:rt:t7:t6 0x1790:rs:t7:t6 0x1790:rt:t7:t6 0x1798:rd:t1:t8 0x1798:rt:t7:t6 0x179c:rt:t1:t8 0x17ac:rt:t2:t7 0x17b0:rs:t2:t7 0x17b0:rt:t2:t7 0x17b8:rd:t3:t5 0x17b8:rt:t2:t7 0x17bc:rt:t3:t5 0x1800:rt:v1:t4 0x1804:rs:v1:t4 0x1804:rt:v1:t4 0x180c:rs:v1:t4 0x180c:rt:t6:t3 0x1814:rt:t6:t3 0x1828:rt:t4:t2 0x182c:rs:t4:t2 0x182c:rt:t4:t2 0x1830:rt:a0:t6 0x1834:rt:t4:t2 0x1838:rd:a0:t6 0x1838:rs:a0:t6 0x183c:rs:a0:t6 0x183c:rt:a0:t6 0x1844:rs:a0:t6 0x1844:rt:t5:t8 0x1848:rt:v1:t7 0x184c:rs:t5:t8 0x1858:rt:v1:t7 0x1870:rt:t8:t5 0x1874:rs:t8:t5 0x1874:rt:t8:t5 0x187c:rd:t7:t4 0x187c:rt:t8:t5 0x1880:rt:t7:t4 0x189c:rt:v1:t3 0x18a0:rs:v1:t3 0x18a0:rt:v1:t3 0x18a8:rs:v1:t3 0x18a8:rt:t1:t2 0x18b0:rt:t1:t2 0x18f0:rt:v1:t9 0x18f4:rs:v1:t9 0x18f4:rt:v1:t9 0x18fc:rs:v1:t9 0x18fc:rt:t2:t6 0x1904:rt:t2:t6 0x1950:rt:v1:t8 0x1954:rs:v1:t8 0x1954:rt:v1:t8 0x195c:rs:v1:t8 0x195c:rt:t3:t7 0x1964:rt:t3:t7 0x1978:rt:v1:t5 0x1984:rt:v1:t5 0x19bc:rt:v1:t4 0x19c8:rt:v1:t4 */
/* @SymbolSize: 6636 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=occ:12,38,42 */
/* @RodataBase: 0x8013F5CC */
/* Address: 0x80136620, Size: 6636 bytes (1659 insns) */
#include "types.h"

typedef union {
    s32 w;
    struct {
        u16 h0;
        u16 h1;
    } h;
    struct {
        u8 b0;
        u8 b1;
        u8 b2;
        u8 b3;
    } b;
} TelPrim;

extern u8 step_mode;
extern volatile u8 now_girl;
extern s8 now_icon;
extern u8 now_month;
extern volatile u8 now_month_v;
extern s32 step_counter;
extern s32 pad_trig;
extern s32 frame_counter;
extern s16 para_zat;
extern s16 para_you;
extern s16 para_kon;
extern s16 para_str;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8 tel_serifu_girl;
extern s32 tel_sasoi_basyo;
extern s32 tel_sasoi_day;
extern s32 tel_sasoi_month;
extern s32 tel_sasoi_year;
extern s32 tel_sasoi_youbi;
extern u8 screen_w[];
extern u8 p_sei[];
extern u8 *date_name_set_tbl0[];
extern u8 *album_info_disp_tbl0[];
extern u8 *machiawase_menu_set_tbl0[];
extern u8 tel_basyo_suki_tbl[];
extern u8 *tel_ok_g1_serifu_tbl[];
extern u8 *tel_ok_g2_serifu_tbl[];
extern u8 *tel_ok_p1_serifu_tbl[];
extern u8 *tel_ok_g3_serifu_tbl[];
extern u8 *tel_ok_p2_serifu_tbl[];
extern u8 *tel_ng1_g1_serifu_tbl[];
extern u8 *tel_ng1_p1_serifu_tbl[];
extern u8 *tel_ng1_g2_serifu_tbl[];
extern u8 *tel_ng1_p2_serifu_tbl[];
extern u8 *tel_ng2_g1_serifu_tbl[];
extern u8 *tel_ng2_p1_serifu_tbl[];
extern u8 *tel_ng2_g2_serifu_tbl[];
extern u8 *tel_ng2_p2_serifu_tbl[];
extern u8 *tel_ng3_g1_serifu_tbl[];
extern u8 *tel_ng3_p1_serifu_tbl[];
extern u8 *tel_ng3_g2_serifu_tbl[];
extern u8 *tel_ng3_p2_serifu_tbl[];
extern u8 tel_basyo_sel_str[];
extern u8 tel_basyo_sel2_str[];
extern u8 tel_sasoi_gatu_str[];
extern u8 tel_sasoi_niti_str[];
extern u8 tel_sasoi_ni_str[];
extern u8 tel_sasoi_ikanai_str[];
extern u8 tel_vect_fmt[];
extern u8 tel_kekka_fmt[];
extern u8 tel_kakunin_str[];
extern u8 tel_kakunin_gatu_str[];
extern u8 tel_kakunin_niti_str[];
extern u8 tel_kakunin_ni_str[];
extern u8 tel_kakunin_machi_str[];
extern u8 tel_ok_gacha_str[];
extern u8 tel_yatta_str[];
extern u8 tel_yatta_gatu_str[];
extern u8 tel_yatta_niti_str[];
extern u8 tel_yatta_tanosimi_str[];
extern u8 tel_ng1_gacha_str[];
extern u8 tel_ng1_uturi_str[];
extern u8 tel_ng1_kuyasi_str[];
extern u8 tel_ng1_mikihara_str[];
extern u8 tel_dialog20_str0[];
extern u8 tel_dialog20_str1[];
extern u8 tel_dialog20_str2[];
extern u8 tel_dialog20_str3[];
extern u8 tel_dialog20_str4[];
extern u8 tel_dialog20_str5[];
extern u8 tel_dialog20_str6[];
extern u8 tel_ok_g1_xa_tbl[];
extern u8 tel_ok_g1_xa_tbl2[];
extern u8 tel_ok_g3_xa_tbl[];
extern u8 tel_ok_g3_xa_tbl2[];
extern u8 tel_ok_g2_xa_tbl[];
extern u8 tel_ok_g2_xa_tbl2[];
extern u8 tel_ng1_g2_xa_tbl[];
extern u8 tel_ng1_g2_xa_tbl2[];
extern u8 tel_ng1_g1_xa_tbl[];
extern u8 tel_ng1_g1_xa_tbl2[];
extern u8 tel_ng2_g1_xa_tbl[];
extern u8 tel_ng2_g1_xa_tbl2[];
extern u8 tel_ng2_g2_xa_tbl[];
extern u8 tel_ng2_g2_xa_tbl2[];
extern u8 tel_ng3_g1_xa_tbl[];
extern u8 tel_ng3_g1_xa_tbl2[];
extern u8 tel_ng3_g2_xa_tbl[];
extern u8 tel_ng3_g2_xa_tbl2[];

extern void cal_base_show();
extern void menu_bar_show();
extern void text_sys_init();
extern void class_clear_f();
extern void tel_spot_menu_set();
extern void k_sub_reset_point_set();
extern void set_kanji_string();
extern s32 check_set_k();
extern void k_disp_start();
extern void menu_check();
extern void menu_cursor_move();
extern void device_cmd_send();
extern void k_sub_reset();
extern void gnsx();
extern void strcpy();
extern void strcat();
extern void sndisp();
extern void k_sub_disp_start();
extern void xa_wait();
extern void xa_wait_m();
extern void xa_set_m();
extern void set_dec_bri();
extern void return_schedule();
extern void sndi();
extern void get_grp();
extern void printf();
extern s32 rand();
extern s32 get_g_zyotai_s();
extern void reserved_date_add();
extern void reserved_date_sort();
extern void parameter_show_init();

void tel_dialog20(void)
{
    s32 v0;
    s32 v1;
    s32 a1;
    s32 a3;
    s32 idx;
    u8 idx6;
    s32 v1b;
    u32 st;
    u32 uidx;
    u32 ub2;
    u8 *p;
    s32 local384;
    u8 buf[256];
    u8 bufA[16];
    u8 bufB[32];
    TelPrim prim;
    u8 pad2C[16];

    cal_base_show();
    menu_bar_show(0);

    st = step_mode;
    switch (st) {
        case 0:
            text_sys_init();
            class_clear_f();
            tel_spot_menu_set(0, tel_sasoi_year, tel_sasoi_month, tel_sasoi_day);
            k_sub_reset_point_set();
            set_kanji_string(-128, 50, 0, tel_basyo_sel_str, 0);
            k_disp_start(check_set_k());
            step_mode = step_mode + 1;
            break;
        case 1:
            menu_check(0, cursor_cw_x, cursor_cw_y);
            menu_cursor_move(0);
            if ((u32)step_counter < 17) break;
            if (!(pad_trig & 0x20)) break;
            v0 = now_icon;
            if (v0 == 10) {
                device_cmd_send(1293);
                step_mode = 2;
                break;
            }
            if (v0 < 0) break;
            step_mode = 4;
            tel_sasoi_basyo = v0;
            break;
        case 2:
            text_sys_init();
            class_clear_f();
            tel_spot_menu_set(1, tel_sasoi_year, tel_sasoi_month, tel_sasoi_day);
            k_sub_reset_point_set();
            set_kanji_string(-128, 50, 0, tel_basyo_sel2_str, 0);
            k_disp_start(check_set_k());
            step_mode = step_mode + 1;
            break;
        case 3:
            menu_check(0, cursor_cw_x, cursor_cw_y);
            menu_cursor_move(0);
            menu_bar_show(0);
            if ((u32)step_counter < 17) break;
            if (!(pad_trig & 0x20)) break;
            v0 = now_icon;
            if (v0 == 10) {
                device_cmd_send(1293);
                step_mode = 0;
                break;
            }
            if (v0 < 0) break;
            step_mode = 4;
            if (v0 < 6) {
                tel_sasoi_basyo = v0 + 10;
            }
            if (v0 == 6 || v0 == 7) {
                v1 = now_month;
                if (v1 == 6U || v1 == 7 || v1 == 8) {
                    tel_sasoi_basyo = v0 + 10;
                } else {
                    tel_sasoi_basyo = v0 + 13;
                }
            }
            if (v0 == 9) {
                tel_sasoi_basyo = 18;
            }
            break;
        case 4:
            k_sub_reset();
            gnsx(p_sei);
            idx = tel_sasoi_month;
            if (idx == 13) idx = 1;
            tel_sasoi_month = idx;
            strcpy(buf, album_info_disp_tbl0[idx]);
            strcat(buf, tel_sasoi_gatu_str);
            if (tel_sasoi_day >= 10) {
                strcat(buf, album_info_disp_tbl0[tel_sasoi_day / 10]);
            }
            strcat(buf, album_info_disp_tbl0[tel_sasoi_day % 10]);
            strcat(buf, tel_sasoi_niti_str);
            strcat(buf, tel_sasoi_ni_str);
            strcat(buf, machiawase_menu_set_tbl0[tel_sasoi_basyo]);
            strcat(buf, tel_sasoi_ikanai_str);
            sndisp(buf, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 5:
            xa_wait();
            break;
        case 6:
            get_grp(bufA, bufB);
            a1 = local384;
            printf(tel_vect_fmt, a1, bufA[now_girl]);
            idx = now_girl;
            ub2 = bufA[idx];
            ub2 = ub2 >> 2;
            v1 = para_zat;
            v1 = v1 / 4;
            if (v1 < (s32)ub2) {
                a1 = ub2;
            } else {
                a1 = v1;
            }
            v0 = tel_basyo_suki_tbl[idx * 21 + tel_sasoi_basyo];
            if (v0 != 0) {
                a1 = v0 + a1;
            } else {
                a1 = 0;
            }
            v0 = para_you;
            if (v0 < 40) {
                a1 = v0 + a1 - 40;
            }
            v0 = rand();
            v1 = v0 & 0xff;
            printf(tel_kekka_fmt, a1, v1);
            if (v1 < a1) {
                step_mode = 32;
            } else {
                v0 = a1 - v1;
                if (v0 < 102) {
                    step_mode = 64;
                } else if (v0 < 179) {
                    step_mode = 96;
                } else {
                    step_mode = 128;
                }
                para_str = para_str + 9;
                v0 = rand();
                para_kon = para_kon + (v0 & 0x7);
                parameter_show_init();
                parameter_show_init();
            }
            uidx = now_girl;
            *(s16 *)&screen_w[uidx * 56 + 450] =
                *(s16 *)&screen_w[uidx * 56 + 450] + (frame_counter & 0x3);
            uidx = now_girl;
            *(s16 *)&screen_w[uidx * 56 + 454] =
                *(s16 *)&screen_w[uidx * 56 + 454] - 12;
            uidx = now_girl;
            if (*(s16 *)&screen_w[uidx * 56 + 454] < 0) {
                *(s16 *)&screen_w[uidx * 56 + 454] = 0;
            }
            idx = now_girl;
            if (idx == 6) {
                if ((u32)(get_g_zyotai_s(idx) & 0x7f) < 2) {
                    tel_serifu_girl = 11;
                } else {
                    tel_serifu_girl = now_girl;
                }
            } else {
                tel_serifu_girl = idx;
            }
            break;
        case 32:
            k_sub_reset();
            prim.b.b0 = (tel_sasoi_basyo & 0x1f) | (prim.b.b0 & 0xffe0);
            prim.h.h0 = ((tel_sasoi_year << 5) & 0xfe0) | (prim.h.h0 & 0xf01f);
            prim.b.b1 = (tel_sasoi_month << 4) | (prim.b.b1 & 0xff0f);
            prim.b.b2 = (tel_sasoi_day & 0x1f) | (prim.b.b2 & 0xffe0);
            prim.b.b2 = (tel_sasoi_youbi << 5) | (prim.b.b2 & 0x1f);
            prim.b.b3 = prim.b.b3 & 0xfffe;
            prim.b.b3 = ((now_girl << 1) & 0x1e) | (prim.b.b3 & 0xe1);
            prim.b.b3 = prim.b.b3 | 0x20;
            prim.b.b3 = prim.b.b3 & 0x3f;
            reserved_date_add(prim);
            reserved_date_sort();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ok_g1_serifu_tbl[tel_serifu_girl], 0, now_girl);
            xa_set_m(tel_ok_g1_xa_tbl, tel_ok_g1_xa_tbl2, tel_serifu_girl, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 33:
            xa_wait_m();
            break;
        case 34:
            k_sub_reset();
            gnsx(p_sei);
            strcpy(buf, tel_kakunin_str);
            idx = tel_sasoi_month;
            if (idx == 13) idx = 1;
            tel_sasoi_month = idx;
            strcat(buf, album_info_disp_tbl0[idx]);
            strcat(buf, tel_kakunin_gatu_str);
            if (tel_sasoi_day >= 10) {
                strcat(buf, album_info_disp_tbl0[tel_sasoi_day / 10]);
            }
            strcat(buf, album_info_disp_tbl0[tel_sasoi_day % 10]);
            strcat(buf, tel_kakunin_niti_str);
            strcat(buf, tel_kakunin_ni_str);
            strcat(buf, machiawase_menu_set_tbl0[tel_sasoi_basyo]);
            strcat(buf, tel_kakunin_machi_str);
            sndisp(buf, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 35:
            xa_wait();
            break;
        case 36:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ok_g2_serifu_tbl[tel_serifu_girl], 0, now_girl);
            xa_set_m(tel_ok_g2_xa_tbl, tel_ok_g2_xa_tbl2, tel_serifu_girl, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 37:
            xa_wait_m();
            break;
        case 38:
            k_sub_reset();
            gnsx(p_sei);
            p = tel_ok_p1_serifu_tbl[tel_serifu_girl];
            if (*p == 32) {
                step_mode = 45;
            } else {
                sndisp(p, 0, 15);
                k_sub_disp_start(0);
                step_mode = step_mode + 1;
            }
            break;
        case 39:
            xa_wait();
            break;
        case 40:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            p = tel_ok_g3_serifu_tbl[tel_serifu_girl];
            if (*p == 32) {
                step_mode = 45;
            } else {
                sndisp(p, 0, now_girl);
                k_sub_disp_start(0);
                step_mode = step_mode + 1;
            }
            xa_set_m(tel_ok_g3_xa_tbl, tel_ok_g3_xa_tbl2, tel_serifu_girl, now_girl);
            break;
        case 41:
            xa_wait_m();
            break;
        case 42:
            k_sub_reset();
            gnsx(p_sei);
            if (*(p = tel_ok_p2_serifu_tbl[tel_serifu_girl]) == 32) {
                step_mode = 45;
            } else {
                sndisp(p, 0, 15);
                k_sub_disp_start(0);
                step_mode = step_mode + 1;
            }
            break;
        case 43:
            xa_wait();
            break;
        case 44:
            step_mode = 51;
            break;
        case 45:
            k_sub_reset();
            device_cmd_send(1291);
            sndi(tel_ok_gacha_str, 0, 47);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 46:
            xa_wait();
            break;
        case 47:
            k_sub_reset();
            device_cmd_send(1292);
            gnsx(p_sei);
            strcpy(buf, tel_yatta_str);
            strcat(buf, album_info_disp_tbl0[tel_sasoi_month]);
            strcat(buf, tel_yatta_gatu_str);
            if (tel_sasoi_day >= 10) {
                strcat(buf, album_info_disp_tbl0[tel_sasoi_day / 10]);
            }
            strcat(buf, album_info_disp_tbl0[tel_sasoi_day % 10]);
            strcat(buf, tel_yatta_niti_str);
            strcat(buf, tel_yatta_tanosimi_str);
            sndisp(buf, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 48:
            xa_wait();
            break;
        case 49:
            step_mode = 255;
            break;
        case 64:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            idx6 = now_girl;
            ub2 = idx6;
            if (ub2 == 6) {
                if ((u32)(get_g_zyotai_s(idx6) & 0x7f) < 2) {
                    tel_serifu_girl = 11;
                }
            }
            v1b = now_girl;
            idx = tel_serifu_girl;
            if (v1b == 4) {
                a3 = tel_sasoi_day;
                v0 = (a3 + 6) / 7;
                if (v0 == 2 && tel_sasoi_month == now_month &&
                    (screen_w + a3)[68] == 1) {
                    idx = 12;
                } else {
                    v1 = now_month_v;
                    if (v0 == 2 && tel_sasoi_month != v1 &&
                        (screen_w + a3)[132] == 1) {
                        idx = 12;
                    }
                }
            }
            tel_serifu_girl = idx;
            sndisp(tel_ng1_g1_serifu_tbl[idx], 0, now_girl);
            k_sub_disp_start(0);
            xa_set_m(tel_ng1_g1_xa_tbl, tel_ng1_g1_xa_tbl2, tel_serifu_girl, now_girl);
            step_mode = step_mode + 1;
            break;
        case 65:
            xa_wait_m();
            break;
        case 66:
            k_sub_reset();
            gnsx(p_sei);
            sndisp(tel_ng1_p1_serifu_tbl[tel_serifu_girl], 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;

        case 67:
            xa_wait();
            break;
        case 68:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ng1_g2_serifu_tbl[tel_serifu_girl], 0, now_girl);
            xa_set_m(tel_ng1_g2_xa_tbl, tel_ng1_g2_xa_tbl2, tel_serifu_girl, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 69:
            xa_wait_m();
            break;
        case 70:
            k_sub_reset();
            gnsx(p_sei);
            sndisp(tel_ng1_p2_serifu_tbl[tel_serifu_girl], 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 71:
            xa_wait();
            break;
        case 72:
            if (now_girl == 9) {
                step_mode = 77;
            } else {
                k_sub_reset();
                device_cmd_send(1291);
                sndi(tel_ng1_gacha_str, 0, 47);
                k_sub_disp_start(0);
                step_mode = step_mode + 1;
            }
            break;
        case 73:
            xa_wait();
            break;
        case 74:
            k_sub_reset();
            gnsx(p_sei);
            device_cmd_send(1292);
            if (now_girl == 5) {
                sndisp(tel_ng1_uturi_str, 0, 31);
            } else {
                sndisp(tel_ng1_kuyasi_str, 0, 31);
            }
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 75:
            xa_wait();
            break;
        case 76:
            step_mode = 255;
            break;
        case 77:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ng1_mikihara_str, 0, now_girl);
            xa_set_m(0x02000163, 0x19000170, -1, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 78:
            xa_wait_m();
            break;
        case 79:
            k_sub_reset();
            gnsx(p_sei);
            sndisp(tel_dialog20_str0, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 80:
            xa_wait();
            break;
        case 81:
            k_sub_reset();
            device_cmd_send(1291);
            sndi(tel_dialog20_str1, 0, 47);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 82:
            xa_wait();
            break;
        case 83:
            step_mode = 74;
            break;
        case 96:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ng2_g1_serifu_tbl[tel_serifu_girl], 0, now_girl);
            xa_set_m(tel_ng2_g1_xa_tbl, tel_ng2_g1_xa_tbl2, tel_serifu_girl, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 97:
            xa_wait_m();
            break;
        case 98:
            k_sub_reset();
            gnsx(p_sei);
            sndisp(tel_ng2_p1_serifu_tbl[tel_serifu_girl], 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 99:
            xa_wait();
            break;
        case 100:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ng2_g2_serifu_tbl[tel_serifu_girl], 0, now_girl);
            xa_set_m(tel_ng2_g2_xa_tbl, tel_ng2_g2_xa_tbl2, tel_serifu_girl, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 101:
            xa_wait_m();
            break;
        case 102:
            p = tel_ng2_p2_serifu_tbl[tel_serifu_girl];
            if (*p == 32) {
                step_mode = 104;
            } else {
                k_sub_reset();
                gnsx(p_sei);
                sndisp(tel_ng2_p2_serifu_tbl[tel_serifu_girl], 0, 15);
                k_sub_disp_start(0);
                step_mode = step_mode + 1;
            }
            break;
        case 103:
            xa_wait();
            break;
        case 104:
            k_sub_reset();
            device_cmd_send(1291);
            sndi(tel_dialog20_str2, 0, 47);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 105:
            xa_wait();
            break;
        case 106:
            k_sub_reset();
            device_cmd_send(1292);
            gnsx(p_sei);
            sndisp(tel_dialog20_str3, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 107:
            xa_wait();
            break;
        case 108:
            step_mode = 255;
            break;
        case 128:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ng3_g1_serifu_tbl[tel_serifu_girl], 0, now_girl);
            xa_set_m(tel_ng3_g1_xa_tbl, tel_ng3_g1_xa_tbl2, tel_serifu_girl, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 129:
            xa_wait_m();
            break;
        case 130:
            k_sub_reset();
            gnsx(p_sei);
            sndisp(tel_ng3_p1_serifu_tbl[tel_serifu_girl], 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 131:
            xa_wait();
            break;
        case 132:
            k_sub_reset();
            gnsx(date_name_set_tbl0[now_girl]);
            sndisp(tel_ng3_g2_serifu_tbl[tel_serifu_girl], 0, now_girl);
            xa_set_m(tel_ng3_g2_xa_tbl, tel_ng3_g2_xa_tbl2, tel_serifu_girl, now_girl);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 133:
            xa_wait_m();
            break;
        case 134:
            p = tel_ng3_p2_serifu_tbl[tel_serifu_girl];
            if (*p == 32) {
                step_mode = 136;
            } else {
                k_sub_reset();
                gnsx(p_sei);
                sndisp(tel_ng3_p2_serifu_tbl[tel_serifu_girl], 0, 15);
                k_sub_disp_start(0);
                step_mode = step_mode + 1;
            }
            break;
        case 135:
            xa_wait();
            break;
        case 136:
            k_sub_reset();
            device_cmd_send(1291);
            sndi(tel_dialog20_str4, 0, 47);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 137:
            xa_wait();
            break;
        case 138:
            k_sub_reset();
            gnsx(p_sei);
            device_cmd_send(1292);
            sndisp(tel_dialog20_str5, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 139:
            xa_wait();
            break;
        case 140:
            step_mode = 255;
            break;
        case 253:
            xa_wait();
            break;
        case 254:
            k_sub_reset();
            device_cmd_send(1291);
            sndi(tel_dialog20_str6, 0, 47);
            step_mode = 253;
            break;
        case 255:
        default:
            text_sys_init();
            class_clear_f();
            set_dec_bri(0);
            return_schedule(0);
            break;
    }
}
