	.verstamp	3 18
	.option	pic0
	.text	
	.align	2
	.file	2 "/tmp/test_basic.c"
	.globl	SD_CmdPut
	.loc	2 3
 #   1	extern void cmd_que_put(unsigned short);
 #   2	
 #   3	void SD_CmdPut(const int arg0) {
	.ent	SD_CmdPut 2
SD_CmdPut:
	.option	O2
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	move	$5, $4
	.loc	2 3
	.loc	2 4
 #   4	    cmd_que_put(arg0 & 0xFFFF);
	and	$4, $5, 65535
	and	$14, $4, 65535
	move	$4, $14
	.livereg	0x0800000E,0x00000000
	jal	cmd_que_put
	.loc	2 5
 #   5	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	SD_CmdPut
