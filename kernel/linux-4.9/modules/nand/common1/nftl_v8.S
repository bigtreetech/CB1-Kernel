/*SPDX-Licensen-Identifier: GPL-2.0*/

	.arch armv8-a
	.file	"nftl.c"
	.global	micron_read_retry_mode
	.bss
	.type	micron_read_retry_mode, %object
	.size	micron_read_retry_mode, 1
micron_read_retry_mode:
	.zero	1
	.global	micron_read_retry_cycle
	.type	micron_read_retry_cycle, %object
	.size	micron_read_retry_cycle, 1
micron_read_retry_cycle:
	.zero	1
	.global	micron_p1_1
	.data
	.align	3
	.type	micron_p1_1, %object
	.size	micron_p1_1, 10
micron_p1_1:
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.global	micron_p1_2
	.align	3
	.type	micron_p1_2, %object
	.size	micron_p1_2, 12
micron_p1_2:
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	12
	.global	micron_p1_3
	.align	3
	.type	micron_p1_3, %object
	.size	micron_p1_3, 7
micron_p1_3:
	.byte	1
	.byte	2
	.byte	3
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.global	micron_p1_4
	.align	3
	.type	micron_p1_4, %object
	.size	micron_p1_4, 18
micron_p1_4:
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.local	micron_p1
	.comm	micron_p1,8,8
	.section	.rodata
	.align	3
.LC3:
	.string	"set feature(addr %d) p0: %d readretry Configuration failed!\n"
	.text
	.align	2
	.global	micron_intel_set_feature
	.type	micron_intel_set_feature, %function
micron_intel_set_feature:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strb	w1, [x29, 23]
	strb	w2, [x29, 22]
	ldrb	w0, [x29, 23]
	strb	w0, [x29, 47]
	ldrb	w0, [x29, 22]
	strb	w0, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 42]
	strb	wzr, [x29, 43]
	add	x1, x29, 40
	add	x0, x29, 23
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_set_feature
	add	x1, x29, 32
	add	x0, x29, 23
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_get_feature
	ldrb	w1, [x29, 32]
	ldrb	w0, [x29, 40]
	cmp	w1, w0
	beq	.L2
	ldrb	w1, [x29, 47]
	ldrb	w2, [x29, 22]
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	nand_print
	mov	w0, -72
	b	.L4
.L2:
	mov	w0, 0
.L4:
	ldp	x29, x30, [sp], 48
	ret
	.size	micron_intel_set_feature, .-micron_intel_set_feature
	.align	2
	.global	micron_set_readretry
	.type	micron_set_readretry, %function
micron_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 64
	beq	.L6
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 65
	beq	.L6
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 80
	beq	.L6
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 66
	bne	.L13
.L6:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	strb	w0, [x29, 43]
	ldrb	w0, [x29, 43]
	cmp	w0, 255
	bne	.L8
	strb	wzr, [x29, 43]
.L8:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 80
	bne	.L9
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	ldr	x1, [x0]
	ldrb	w0, [x29, 43]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L10
	mov	w2, 1
	mov	w1, -109
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L14
.L10:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 255
	bne	.L9
	mov	w2, 0
	mov	w1, -119
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	str	w0, [x29, 44]
	mov	w2, 0
	mov	w1, -109
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	mov	w1, w0
	ldr	w0, [x29, 44]
	orr	w0, w0, w1
	str	w0, [x29, 44]
	b	.L7
.L9:
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	ldr	x1, [x0]
	ldrb	w0, [x29, 43]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w2, w0
	mov	w1, -119
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	str	w0, [x29, 44]
	b	.L7
.L13:
	nop
	b	.L7
.L14:
	nop
.L7:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 255
	bne	.L11
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
.L11:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	micron_set_readretry, .-micron_set_readretry
	.section	.rodata
	.align	3
.LC4:
	.string	"micron retry!\n"
	.align	3
.LC5:
	.string	"micron ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	micron_read_page_end
	.type	micron_read_page_end, %function
micron_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L16
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L17
.L22:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	bl	micron_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L24
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L20
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L19
.L20:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	nand_print_dbg
	b	.L21
.L24:
	nop
.L19:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L17:
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L22
.L21:
	ldr	x0, [x29, 32]
	mov	w1, 255
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	bl	micron_set_readretry
.L16:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	micron_read_page_end, .-micron_read_page_end
	.section	.rodata
	.align	3
.LC6:
	.string	"micron read retry cannot support!\n"
	.align	3
.LC7:
	.string	"micron read retry conut: %d !\n"
	.text
	.align	2
	.global	micron_readretry_init
	.type	micron_readretry_init, %function
micron_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	strb	wzr, [x0]
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_2
	add	x1, x1, :lo12:micron_p1_2
	str	x1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	strb	w1, [x0]
	mov	w2, 0
	mov	w1, -119
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 65
	bne	.L26
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 12
	strb	w1, [x0]
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_2
	add	x1, x1, :lo12:micron_p1_2
	str	x1, [x0]
	b	.L27
.L26:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 64
	bne	.L28
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_1
	add	x1, x1, :lo12:micron_p1_1
	str	x1, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 10
	strb	w1, [x0]
	b	.L27
.L28:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 80
	bne	.L29
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_3
	add	x1, x1, :lo12:micron_p1_3
	str	x1, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 7
	strb	w1, [x0]
	mov	w2, 0
	mov	w1, -109
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	b	.L27
.L29:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 66
	bne	.L30
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_4
	add	x1, x1, :lo12:micron_p1_4
	str	x1, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 18
	strb	w1, [x0]
	b	.L27
.L30:
	adrp	x0, .LC6
	add	x0, x0, :lo12:.LC6
	bl	nand_print
	mov	w0, -71
	b	.L31
.L27:
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC7
	add	x0, x0, :lo12:.LC7
	bl	nand_print_dbg
	mov	w0, 0
.L31:
	ldp	x29, x30, [sp], 32
	ret
	.size	micron_readretry_init, .-micron_readretry_init
	.align	2
	.global	micron_readretry_exit
	.type	micron_readretry_exit, %function
micron_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	strb	wzr, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	strb	wzr, [x0]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	micron_readretry_exit, .-micron_readretry_exit
	.section	.rodata
	.align	3
.LC8:
	.string	" micron_special_init micron_read_retry_mode:%d micron_read_retry_cycle:%d \n"
	.align	3
.LC9:
	.string	" micron_special_init error micron_read_retry_mode:%d micron_read_retry_cycle:%d \n"
	.text
	.align	2
	.global	micron_special_init
	.type	micron_special_init, %function
micron_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L35
.L36:
	ldr	x0, [x29, 16]
	bl	micron_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L35:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L36
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L37
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, micron_read_page_end
	add	x1, x1, :lo12:micron_read_page_end
	str	x1, [x0]
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC8
	add	x0, x0, :lo12:.LC8
	bl	nand_print_dbg
	b	.L38
.L37:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC9
	add	x0, x0, :lo12:.LC9
	bl	nand_print
.L38:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	micron_special_init, .-micron_special_init
	.section	.rodata
	.align	3
.LC10:
	.string	" micron_special_exit \n"
	.text
	.align	2
	.global	micron_special_exit
	.type	micron_special_exit, %function
micron_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L41
.L42:
	ldr	x0, [x29, 24]
	bl	micron_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L41:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L42
	adrp	x0, .LC10
	add	x0, x0, :lo12:.LC10
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	micron_special_exit, .-micron_special_exit
	.global	samsung_read_retry_mode
	.bss
	.type	samsung_read_retry_mode, %object
	.size	samsung_read_retry_mode, 1
samsung_read_retry_mode:
	.zero	1
	.global	samsung_read_retry_cycle
	.type	samsung_read_retry_cycle, %object
	.size	samsung_read_retry_cycle, 1
samsung_read_retry_cycle:
	.zero	1
	.global	samsung_read_retry_addr
	.data
	.align	3
	.type	samsung_read_retry_addr, %object
	.size	samsung_read_retry_addr, 4
samsung_read_retry_addr:
	.byte	-89
	.byte	-92
	.byte	-91
	.byte	-90
	.global	samsung_read_retry_cmd
	.align	3
	.type	samsung_read_retry_cmd, %object
	.size	samsung_read_retry_cmd, 4
samsung_read_retry_cmd:
	.byte	-95
	.byte	-95
	.byte	-95
	.byte	-95
	.global	samsung_p1
	.align	3
	.type	samsung_p1, %object
	.size	samsung_p1, 60
samsung_p1:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	5
	.byte	10
	.byte	0
	.byte	0
	.byte	40
	.byte	0
	.byte	-20
	.byte	-40
	.byte	-19
	.byte	-11
	.byte	-19
	.byte	-26
	.byte	10
	.byte	15
	.byte	5
	.byte	0
	.byte	15
	.byte	10
	.byte	-5
	.byte	-20
	.byte	-24
	.byte	-17
	.byte	-24
	.byte	-36
	.byte	-15
	.byte	-5
	.byte	-2
	.byte	-16
	.byte	10
	.byte	0
	.byte	-5
	.byte	-20
	.byte	-48
	.byte	-30
	.byte	-48
	.byte	-62
	.byte	20
	.byte	15
	.byte	-5
	.byte	-20
	.byte	-24
	.byte	-5
	.byte	-24
	.byte	-36
	.byte	30
	.byte	20
	.byte	-5
	.byte	-20
	.byte	-5
	.byte	-1
	.byte	-5
	.byte	-8
	.byte	7
	.byte	12
	.byte	2
	.byte	0
	.section	.rodata
	.align	3
.LC11:
	.string	"rr value %x %x %x %x!\n"
	.text
	.align	2
	.global	samsung_vender_set_param
	.type	samsung_vender_set_param, %function
samsung_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	adrp	x0, samsung_read_retry_cmd
	add	x0, x0, :lo12:samsung_read_retry_cmd
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC11
	add	x0, x0, :lo12:.LC11
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	samsung_vender_set_param, .-samsung_vender_set_param
	.section	.rodata
	.align	3
.LC12:
	.string	"m5 set readretry error ! %x \n"
	.align	3
.LC13:
	.string	"samsung set readretry error ! %x \n"
	.text
	.align	2
	.global	samsung_set_readretry
	.type	samsung_set_readretry, %function
samsung_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 16]
	cmp	w0, 236
	bne	.L47
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 17]
	cmp	w0, 222
	bne	.L47
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 18]
	cmp	w0, 148
	bne	.L47
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 19]
	cmp	w0, 195
	bne	.L47
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 20]
	cmp	w0, 164
	bne	.L47
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 21]
	cmp	w0, 202
	bne	.L47
	ldr	x0, [x29, 24]
	bl	samsung_set_readretry_K9GCGD8U0F
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L49
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 184]
	adrp	x0, .LC12
	add	x0, x0, :lo12:.LC12
	bl	nand_print
	b	.L49
.L47:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, samsung_p1
	add	x0, x0, :lo12:samsung_p1
	add	x1, x1, x0
	adrp	x0, samsung_read_retry_addr
	add	x0, x0, :lo12:samsung_read_retry_addr
	mov	w3, 4
	mov	x2, x0
	ldr	x0, [x29, 24]
	bl	samsung_vender_set_param
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L49
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 184]
	adrp	x0, .LC13
	add	x0, x0, :lo12:.LC13
	bl	nand_print
.L49:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	samsung_set_readretry, .-samsung_set_readretry
	.section	.rodata
	.align	3
.LC14:
	.string	"samsung retry!\n"
	.align	3
.LC15:
	.string	"samsung ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	samsung_read_page_end
	.type	samsung_read_page_end, %function
samsung_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L52
	adrp	x0, .LC14
	add	x0, x0, :lo12:.LC14
	bl	nand_print_dbg
	mov	w0, 1
	str	w0, [x29, 44]
	b	.L53
.L60:
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 16]
	cmp	w0, 236
	bne	.L54
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 17]
	cmp	w0, 222
	bne	.L54
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 18]
	cmp	w0, 148
	bne	.L54
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 19]
	cmp	w0, 195
	bne	.L54
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 20]
	cmp	w0, 164
	bne	.L54
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 21]
	cmp	w0, 202
	bne	.L54
	mov	w1, 256
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	b	.L55
.L54:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
.L55:
	ldr	x0, [x29, 32]
	bl	samsung_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L62
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L58
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L57
.L58:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC15
	add	x0, x0, :lo12:.LC15
	bl	nand_print_dbg
	b	.L59
.L62:
	nop
.L57:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L53:
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L60
.L59:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	samsung_set_readretry
.L52:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	samsung_read_page_end, .-samsung_read_page_end
	.section	.rodata
	.align	3
.LC16:
	.string	"samsung read retry conut: %d !\n"
	.text
	.align	2
	.global	samsung_readretry_init
	.type	samsung_readretry_init, %function
samsung_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 16]
	cmp	w0, 236
	bne	.L64
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 17]
	cmp	w0, 222
	bne	.L64
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 18]
	cmp	w0, 148
	bne	.L64
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 19]
	cmp	w0, 195
	bne	.L64
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 20]
	cmp	w0, 164
	bne	.L64
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 21]
	cmp	w0, 202
	bne	.L64
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	mov	w1, -1
	strb	w1, [x0]
	b	.L65
.L64:
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	mov	w1, 15
	strb	w1, [x0]
.L65:
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC16
	add	x0, x0, :lo12:.LC16
	bl	nand_print_dbg
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 32
	beq	.L66
	mov	w0, -100
	b	.L67
.L66:
	mov	w0, 0
.L67:
	ldp	x29, x30, [sp], 32
	ret
	.size	samsung_readretry_init, .-samsung_readretry_init
	.align	2
	.global	samsung_readretry_exit
	.type	samsung_readretry_exit, %function
samsung_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	samsung_readretry_exit, .-samsung_readretry_exit
	.section	.rodata
	.align	3
.LC17:
	.string	"%s feature set fail\n"
	.text
	.align	2
	.global	samsung_set_readretry_K9GCGD8U0F
	.type	samsung_set_readretry_K9GCGD8U0F, %function
samsung_set_readretry_K9GCGD8U0F:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 60]
	strb	wzr, [x29, 57]
	str	wzr, [x29, 48]
	str	wzr, [x29, 40]
	strb	wzr, [x29, 59]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	strb	w0, [x29, 58]
	strb	wzr, [x29, 59]
	b	.L71
.L72:
	ldrb	w0, [x29, 59]
	sxtw	x0, w0
	sub	x1, x29, #4032
	add	x0, x1, x0
	mov	x1, x0
	ldrb	w0, [x29, 58]
	strb	w0, [x1, 4080]
	ldrb	w0, [x29, 59]
	sxtw	x0, w0
	sub	x1, x29, #4032
	add	x0, x1, x0
	mov	x1, x0
	mov	w0, -1
	strb	w0, [x1, 4072]
	ldrb	w0, [x29, 59]
	add	w0, w0, 1
	strb	w0, [x29, 59]
.L71:
	ldrb	w0, [x29, 59]
	cmp	w0, 3
	bls	.L72
	add	x1, x29, 48
	add	x0, x29, 57
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_set_read_retry_K9GCGD8U0F
	str	w0, [x29, 60]
	add	x1, x29, 40
	add	x0, x29, 57
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_get_read_retry_K9GCGD8U0F
	str	w0, [x29, 60]
	ldrb	w1, [x29, 40]
	ldrb	w0, [x29, 48]
	cmp	w1, w0
	bne	.L73
	ldrb	w1, [x29, 41]
	ldrb	w0, [x29, 49]
	cmp	w1, w0
	bne	.L73
	ldrb	w1, [x29, 42]
	ldrb	w0, [x29, 50]
	cmp	w1, w0
	bne	.L73
	ldrb	w1, [x29, 43]
	ldrb	w0, [x29, 51]
	cmp	w1, w0
	beq	.L74
.L73:
	adrp	x0, __func__.4479
	add	x1, x0, :lo12:__func__.4479
	adrp	x0, .LC17
	add	x0, x0, :lo12:.LC17
	bl	nand_print
	mov	w0, -1
	str	w0, [x29, 60]
.L74:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 255
	bne	.L75
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
.L75:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	samsung_set_readretry_K9GCGD8U0F, .-samsung_set_readretry_K9GCGD8U0F
	.section	.rodata
	.align	3
.LC18:
	.string	" samsung_special_init samsung_read_retry_mode:%d samsung_read_retry_cycle :%d \n"
	.align	3
.LC19:
	.string	" samsung_special_init error samsung_read_retry_mode:%d samsung_read_retry_cycle :%d \n"
	.text
	.align	2
	.global	samsung_special_init
	.type	samsung_special_init, %function
samsung_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L78
.L79:
	ldr	x0, [x29, 16]
	bl	samsung_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L78:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L79
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L80
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, samsung_read_page_end
	add	x1, x1, :lo12:samsung_read_page_end
	str	x1, [x0]
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC18
	add	x0, x0, :lo12:.LC18
	bl	nand_print_dbg
	b	.L81
.L80:
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC19
	add	x0, x0, :lo12:.LC19
	bl	nand_print
.L81:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	samsung_special_init, .-samsung_special_init
	.section	.rodata
	.align	3
.LC20:
	.string	" samsung_special_exit \n"
	.text
	.align	2
	.global	samsung_special_exit
	.type	samsung_special_exit, %function
samsung_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L84
.L85:
	ldr	x0, [x29, 24]
	bl	samsung_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L84:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L85
	adrp	x0, .LC20
	add	x0, x0, :lo12:.LC20
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	samsung_special_exit, .-samsung_special_exit
	.global	com
	.section	.rodata
	.align	3
	.type	com, %object
	.size	com, 11
com:
	.byte	99
	.byte	111
	.byte	109
	.byte	112
	.byte	97
	.byte	116
	.byte	105
	.byte	98
	.byte	108
	.byte	101
	.byte	0
	.global	mod
	.align	3
	.type	mod, %object
	.size	mod, 6
mod:
	.byte	109
	.byte	111
	.byte	100
	.byte	101
	.byte	108
	.byte	0
	.global	sandisk_read_retry_mode
	.bss
	.type	sandisk_read_retry_mode, %object
	.size	sandisk_read_retry_mode, 1
sandisk_read_retry_mode:
	.zero	1
	.global	sandisk_read_retry_cycle
	.type	sandisk_read_retry_cycle, %object
	.size	sandisk_read_retry_cycle, 1
sandisk_read_retry_cycle:
	.zero	1
	.global	sandisk_read_retry_reg_cnt
	.type	sandisk_read_retry_reg_cnt, %object
	.size	sandisk_read_retry_reg_cnt, 1
sandisk_read_retry_reg_cnt:
	.zero	1
	.global	sandisk_acti_start_cmd
	.data
	.align	3
	.type	sandisk_acti_start_cmd, %object
	.size	sandisk_acti_start_cmd, 2
sandisk_acti_start_cmd:
	.byte	59
	.byte	-71
	.global	sandisk_acti_start_addr
	.align	3
	.type	sandisk_acti_start_addr, %object
	.size	sandisk_acti_start_addr, 9
sandisk_acti_start_addr:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.global	sandisk_read_retry_enable_cmd_19
	.align	3
	.type	sandisk_read_retry_enable_cmd_19, %object
	.size	sandisk_read_retry_enable_cmd_19, 1
sandisk_read_retry_enable_cmd_19:
	.byte	-74
	.global	sandisk_read_retry_disable_cmd_19
	.align	3
	.type	sandisk_read_retry_disable_cmd_19, %object
	.size	sandisk_read_retry_disable_cmd_19, 1
sandisk_read_retry_disable_cmd_19:
	.byte	-42
	.global	sandisk_read_retry_cmd_19
	.align	3
	.type	sandisk_read_retry_cmd_19, %object
	.size	sandisk_read_retry_cmd_19, 3
sandisk_read_retry_cmd_19:
	.byte	83
	.byte	83
	.byte	83
	.global	sandisk_read_retry_reg_adr_19
	.align	3
	.type	sandisk_read_retry_reg_adr_19, %object
	.size	sandisk_read_retry_reg_adr_19, 9
sandisk_read_retry_reg_adr_19:
	.byte	4
	.byte	5
	.byte	7
	.zero	6
	.global	sandisk_19_low
	.align	3
	.type	sandisk_19_low, %object
	.size	sandisk_19_low, 48
sandisk_19_low:
	.byte	-16
	.byte	0
	.byte	-16
	.byte	-32
	.byte	0
	.byte	-32
	.byte	-48
	.byte	0
	.byte	-48
	.byte	16
	.byte	0
	.byte	16
	.byte	32
	.byte	0
	.byte	32
	.byte	48
	.byte	0
	.byte	48
	.byte	-64
	.byte	0
	.byte	-48
	.byte	0
	.byte	0
	.byte	16
	.byte	0
	.byte	0
	.byte	32
	.byte	16
	.byte	0
	.byte	32
	.byte	-80
	.byte	0
	.byte	-48
	.byte	-96
	.byte	0
	.byte	-48
	.byte	-112
	.byte	0
	.byte	-48
	.byte	-80
	.byte	0
	.byte	-64
	.byte	-96
	.byte	0
	.byte	-64
	.byte	-112
	.byte	0
	.byte	-64
	.global	sandisk_19_high
	.align	3
	.type	sandisk_19_high, %object
	.size	sandisk_19_high, 60
sandisk_19_high:
	.byte	0
	.byte	-16
	.byte	0
	.byte	15
	.byte	-32
	.byte	0
	.byte	15
	.byte	-48
	.byte	0
	.byte	14
	.byte	-32
	.byte	0
	.byte	14
	.byte	-48
	.byte	0
	.byte	13
	.byte	-16
	.byte	0
	.byte	13
	.byte	-32
	.byte	0
	.byte	13
	.byte	-48
	.byte	0
	.byte	1
	.byte	16
	.byte	0
	.byte	2
	.byte	32
	.byte	0
	.byte	2
	.byte	16
	.byte	0
	.byte	3
	.byte	32
	.byte	0
	.byte	15
	.byte	0
	.byte	0
	.byte	14
	.byte	-16
	.byte	0
	.byte	13
	.byte	-64
	.byte	0
	.byte	15
	.byte	-16
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	13
	.byte	-80
	.byte	0
	.byte	12
	.byte	-96
	.byte	0
	.global	sandisk_read_retry_cmd_24
	.align	3
	.type	sandisk_read_retry_cmd_24, %object
	.size	sandisk_read_retry_cmd_24, 3
sandisk_read_retry_cmd_24:
	.byte	83
	.byte	83
	.byte	83
	.global	sandisk_read_retry_reg_adr_24
	.align	3
	.type	sandisk_read_retry_reg_adr_24, %object
	.size	sandisk_read_retry_reg_adr_24, 9
sandisk_read_retry_reg_adr_24:
	.byte	4
	.byte	5
	.byte	7
	.zero	6
	.global	sandisk_24
	.align	3
	.type	sandisk_24, %object
	.size	sandisk_24, 27
sandisk_24:
	.byte	0
	.byte	-16
	.byte	0
	.byte	0
	.byte	-32
	.byte	0
	.byte	-1
	.byte	-16
	.byte	-16
	.byte	-18
	.byte	-32
	.byte	-32
	.byte	-34
	.byte	-48
	.byte	-48
	.byte	-51
	.byte	-64
	.byte	-64
	.byte	1
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	3
	.byte	0
	.byte	0
	.global	sandisk_read_retry_enable_cmd_1y
	.align	3
	.type	sandisk_read_retry_enable_cmd_1y, %object
	.size	sandisk_read_retry_enable_cmd_1y, 1
sandisk_read_retry_enable_cmd_1y:
	.byte	93
	.global	sandisk_read_retry_exit_cmd_1y
	.align	3
	.type	sandisk_read_retry_exit_cmd_1y, %object
	.size	sandisk_read_retry_exit_cmd_1y, 1
sandisk_read_retry_exit_cmd_1y:
	.byte	-1
	.global	sandisk_read_retry_cmd_1y
	.align	3
	.type	sandisk_read_retry_cmd_1y, %object
	.size	sandisk_read_retry_cmd_1y, 1
sandisk_read_retry_cmd_1y:
	.byte	-17
	.global	sandisk_read_retry_reg_adr_1y
	.align	3
	.type	sandisk_read_retry_reg_adr_1y, %object
	.size	sandisk_read_retry_reg_adr_1y, 1
sandisk_read_retry_reg_adr_1y:
	.byte	17
	.global	sandisk_1y
	.align	3
	.type	sandisk_1y, %object
	.size	sandisk_1y, 76
sandisk_1y:
	.byte	4
	.byte	0
	.byte	124
	.byte	124
	.byte	8
	.byte	0
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	0
	.byte	124
	.byte	124
	.byte	124
	.byte	0
	.byte	0
	.byte	12
	.byte	0
	.byte	120
	.byte	120
	.byte	16
	.byte	0
	.byte	8
	.byte	8
	.byte	124
	.byte	124
	.byte	120
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	120
	.byte	116
	.byte	20
	.byte	4
	.byte	4
	.byte	0
	.byte	120
	.byte	120
	.byte	0
	.byte	124
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	116
	.byte	120
	.byte	120
	.byte	4
	.byte	0
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	116
	.byte	112
	.byte	120
	.byte	116
	.byte	120
	.byte	120
	.byte	120
	.byte	112
	.byte	120
	.byte	116
	.byte	120
	.byte	112
	.byte	112
	.byte	112
	.local	sandisk_sclk0_bak
	.comm	sandisk_sclk0_bak,4,4
	.local	sandisk_sclk1_bak
	.comm	sandisk_sclk1_bak,4,4
	.local	sandisk_toggle_mode_flag
	.comm	sandisk_toggle_mode_flag,4,4
	.text
	.align	2
	.global	sandisk_read_retry_clock_save
	.type	sandisk_read_retry_clock_save, %function
sandisk_read_retry_clock_save:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_sclk1_bak
	add	x3, x0, :lo12:sandisk_sclk1_bak
	adrp	x0, sandisk_sclk0_bak
	add	x2, x0, :lo12:sandisk_sclk0_bak
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_get_clk
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	mov	w3, 20
	mov	w2, 10
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_read_retry_clock_save, .-sandisk_read_retry_clock_save
	.align	2
	.global	sandisk_vender_set_param
	.type	sandisk_vender_set_param, %function
sandisk_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 60]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L90
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L91
.L90:
	adrp	x0, sandisk_acti_start_cmd
	add	x0, x0, :lo12:sandisk_acti_start_cmd
	mov	w6, 2
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	adrp	x0, sandisk_read_retry_cmd_19
	add	x0, x0, :lo12:sandisk_read_retry_cmd_19
	mov	w6, 3
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
.L91:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	bne	.L92
	adrp	x0, sandisk_read_retry_cmd_1y
	add	x0, x0, :lo12:sandisk_read_retry_cmd_1y
	mov	w6, 1
	mov	w5, 4
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
.L92:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_vender_set_param, .-sandisk_vender_set_param
	.align	2
	.global	sandisk_read_retry_clock_recover
	.type	sandisk_read_retry_clock_recover, %function
sandisk_read_retry_clock_recover:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_sclk0_bak
	add	x0, x0, :lo12:sandisk_sclk0_bak
	ldr	w2, [x0]
	adrp	x0, sandisk_sclk1_bak
	add	x0, x0, :lo12:sandisk_sclk1_bak
	ldr	w3, [x0]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_read_retry_clock_recover, .-sandisk_read_retry_clock_recover
	.align	2
	.global	sandisk_end_one_read_retry
	.type	sandisk_end_one_read_retry, %function
sandisk_end_one_read_retry:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L97
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	beq	.L97
	mov	w0, 0
	b	.L98
.L97:
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L99
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L99:
	adrp	x0, sandisk_read_retry_disable_cmd_19
	add	x0, x0, :lo12:sandisk_read_retry_disable_cmd_19
	mov	w6, 1
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L100
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L100:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
.L98:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_end_one_read_retry, .-sandisk_end_one_read_retry
	.align	2
	.global	sandisk_set_readretry
	.type	sandisk_set_readretry, %function
sandisk_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 24]
	bl	sandisk_read_retry_clock_save
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L102
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L102:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	bne	.L103
	ldrh	w0, [x29, 22]
	cmp	w0, 255
	beq	.L104
	ldrh	w0, [x29, 22]
	cmp	w0, 0
	beq	.L105
	ldrh	w0, [x29, 22]
	and	w0, w0, 1
	uxth	w0, w0
	cmp	w0, 0
	beq	.L104
.L105:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	adrp	x1, sandisk_19_low
	add	x1, x1, :lo12:sandisk_19_low
	add	x0, x0, x1
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_19
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_19
	str	x0, [x29, 40]
	mov	w0, 3
	str	w0, [x29, 36]
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	mov	w1, 16
	strb	w1, [x0]
	b	.L106
.L104:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	adrp	x1, sandisk_19_high
	add	x1, x1, :lo12:sandisk_19_high
	add	x0, x0, x1
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_19
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_19
	str	x0, [x29, 40]
	mov	w0, 3
	str	w0, [x29, 36]
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	mov	w1, 20
	strb	w1, [x0]
.L106:
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	mov	w2, 0
	mov	w1, -74
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	b	.L107
.L103:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L108
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	adrp	x1, sandisk_24
	add	x1, x1, :lo12:sandisk_24
	add	x0, x0, x1
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_24
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_24
	str	x0, [x29, 40]
	mov	w0, 3
	str	w0, [x29, 36]
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	mov	w2, 0
	mov	w1, -74
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	b	.L107
.L108:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	bne	.L107
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_1y
	add	x0, x0, :lo12:sandisk_1y
	add	x0, x1, x0
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_1y
	str	x0, [x29, 40]
	mov	w0, 1
	str	w0, [x29, 36]
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	mov	w2, 0
	mov	w1, 93
	ldr	x0, [x29, 24]
	bl	set_one_cmd
.L107:
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L109
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L109:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	x0, [x29, 24]
	bl	sandisk_read_retry_clock_recover
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_set_readretry, .-sandisk_set_readretry
	.section	.rodata
	.align	3
.LC21:
	.string	"sandisk retry!\n"
	.align	3
.LC22:
	.string	"retry spare all 0xff! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.align	3
.LC23:
	.string	"sandisk ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	sandisk_read_page_end
	.type	sandisk_read_page_end, %function
sandisk_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L112
	adrp	x0, .LC21
	add	x0, x0, :lo12:.LC21
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L113
.L119:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L121
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	bl	sandisk_end_one_read_retry
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L116
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC22
	add	x0, x0, :lo12:.LC22
	bl	nand_print_dbg
	b	.L115
.L116:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L117
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L115
.L117:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC23
	add	x0, x0, :lo12:.LC23
	bl	nand_print_dbg
	b	.L118
.L121:
	nop
.L115:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L113:
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L119
.L118:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	sandisk_exit_readretry
.L112:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_read_page_end, .-sandisk_read_page_end
	.section	.rodata
	.align	3
.LC24:
	.string	"sandisk activation seq fail 0x%x\n"
	.text
	.align	2
	.global	sandisk_activation_seq
	.type	sandisk_activation_seq, %function
sandisk_activation_seq:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC0
	add	x1, x0, :lo12:.LC0
	add	x0, x29, 48
	ldr	x2, [x1]
	str	x2, [x0]
	ldrb	w1, [x1, 8]
	strb	w1, [x0, 8]
	strb	wzr, [x29, 32]
	strb	wzr, [x29, 33]
	strb	wzr, [x29, 34]
	strb	wzr, [x29, 35]
	strb	wzr, [x29, 36]
	strb	wzr, [x29, 37]
	strb	wzr, [x29, 38]
	strb	wzr, [x29, 39]
	strb	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	adrp	x0, sandisk_acti_start_cmd
	add	x0, x0, :lo12:sandisk_acti_start_cmd
	mov	w6, 2
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	add	x2, x29, 32
	adrp	x0, sandisk_acti_start_addr
	add	x1, x0, :lo12:sandisk_acti_start_addr
	add	x0, x29, 48
	mov	w6, 9
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L123
	adrp	x0, .LC24
	add	x0, x0, :lo12:.LC24
	ldr	w1, [x29, 60]
	bl	nand_print
.L123:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_activation_seq, .-sandisk_activation_seq
	.section	.rodata
	.align	3
.LC0:
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.text
	.section	.rodata
	.align	3
.LC25:
	.string	"sandisk read retry conut: %d !\n"
	.text
	.align	2
	.global	sandisk_readretry_init
	.type	sandisk_readretry_init, %function
sandisk_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, sandisk_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_read_retry_reg_cnt
	strb	w1, [x0]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L126
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L127
.L126:
	ldr	x0, [x29, 24]
	bl	sandisk_activation_seq
.L127:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L128
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	beq	.L128
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	beq	.L128
	mov	w0, -99
	b	.L129
.L128:
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC25
	add	x0, x0, :lo12:.LC25
	bl	nand_print_dbg
	mov	w0, 0
.L129:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_readretry_init, .-sandisk_readretry_init
	.align	2
	.global	sandisk_readretry_exit
	.type	sandisk_readretry_exit, %function
sandisk_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	sandisk_readretry_exit, .-sandisk_readretry_exit
	.align	2
	.global	sandisk_exit_readretry
	.type	sandisk_exit_readretry, %function
sandisk_exit_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 60]
	strb	wzr, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 42]
	strb	wzr, [x29, 43]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L133
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L133:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	bne	.L134
	adrp	x0, sandisk_read_retry_reg_adr_19
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_19
	str	x0, [x29, 48]
	mov	w0, 3
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	b	.L135
.L134:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L136
	adrp	x0, sandisk_read_retry_reg_adr_24
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_24
	str	x0, [x29, 48]
	mov	w0, 3
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	b	.L135
.L136:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	bne	.L135
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	adrp	x0, sandisk_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_1y
	str	x0, [x29, 48]
	mov	w0, 1
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
.L135:
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L137
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L137:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_exit_readretry, .-sandisk_exit_readretry
	.section	.rodata
	.align	3
.LC26:
	.string	" sandisk_special_init sandisk_read_retry_mode:%d sandisk_read_retry_cycle :%d sandisk_read_retry_reg_cnt %d \n"
	.align	3
.LC27:
	.string	" sandisk_special_init error sandisk_read_retry_mode:%d sandisk_read_retry_cycle :%d sandisk_read_retry_reg_cnt %d \n"
	.text
	.align	2
	.global	sandisk_special_init
	.type	sandisk_special_init, %function
sandisk_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L140
.L141:
	ldr	x0, [x29, 16]
	bl	sandisk_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L140:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L141
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L142
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, sandisk_read_page_end
	add	x1, x1, :lo12:sandisk_read_page_end
	str	x1, [x0]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC26
	add	x0, x0, :lo12:.LC26
	bl	nand_print_dbg
	b	.L143
.L142:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC27
	add	x0, x0, :lo12:.LC27
	bl	nand_print
.L143:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_special_init, .-sandisk_special_init
	.section	.rodata
	.align	3
.LC28:
	.string	" sandisk_special_exit \n"
	.text
	.align	2
	.global	sandisk_special_exit
	.type	sandisk_special_exit, %function
sandisk_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L146
.L147:
	ldr	x0, [x29, 24]
	bl	sandisk_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L146:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L147
	adrp	x0, .LC28
	add	x0, x0, :lo12:.LC28
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_special_exit, .-sandisk_special_exit
	.section	.rodata
	.align	3
.LC29:
	.string	" generic special init \n"
	.text
	.align	2
	.global	generic_special_init
	.type	generic_special_init, %function
generic_special_init:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, generic_read_page_end_not_retry
	add	x1, x1, :lo12:generic_read_page_end_not_retry
	str	x1, [x0]
	adrp	x0, .LC29
	add	x0, x0, :lo12:.LC29
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	generic_special_init, .-generic_special_init
	.section	.rodata
	.align	3
.LC30:
	.string	" generic special exit \n"
	.text
	.align	2
	.global	generic_special_exit
	.type	generic_special_exit, %function
generic_special_exit:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, .LC30
	add	x0, x0, :lo12:.LC30
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	generic_special_exit, .-generic_special_exit
	.global	hynix26nm_read_retry_mode
	.bss
	.type	hynix26nm_read_retry_mode, %object
	.size	hynix26nm_read_retry_mode, 1
hynix26nm_read_retry_mode:
	.zero	1
	.global	hynix26nm_read_retry_cycle
	.type	hynix26nm_read_retry_cycle, %object
	.size	hynix26nm_read_retry_cycle, 1
hynix26nm_read_retry_cycle:
	.zero	1
	.global	hynix26nm_read_retry_reg_num
	.type	hynix26nm_read_retry_reg_num, %object
	.size	hynix26nm_read_retry_reg_num, 1
hynix26nm_read_retry_reg_num:
	.zero	1
	.global	hynix26nm_read_retry_reg_adr
	.align	3
	.type	hynix26nm_read_retry_reg_adr, %object
	.size	hynix26nm_read_retry_reg_adr, 4
hynix26nm_read_retry_reg_adr:
	.zero	4
	.global	hynix26nm_read_retry_val
	.align	3
	.type	hynix26nm_read_retry_val, %object
	.size	hynix26nm_read_retry_val, 56
hynix26nm_read_retry_val:
	.zero	56
	.global	hynix26nm_lsb_mode_reg_adr
	.align	3
	.type	hynix26nm_lsb_mode_reg_adr, %object
	.size	hynix26nm_lsb_mode_reg_adr, 5
hynix26nm_lsb_mode_reg_adr:
	.zero	5
	.global	hynix26nm_lsb_mode_default_val
	.align	3
	.type	hynix26nm_lsb_mode_default_val, %object
	.size	hynix26nm_lsb_mode_default_val, 5
hynix26nm_lsb_mode_default_val:
	.zero	5
	.global	hynix26nm_lsb_mode_val
	.align	3
	.type	hynix26nm_lsb_mode_val, %object
	.size	hynix26nm_lsb_mode_val, 5
hynix26nm_lsb_mode_val:
	.zero	5
	.global	hynix26nm_lsb_mode_reg_num
	.type	hynix26nm_lsb_mode_reg_num, %object
	.size	hynix26nm_lsb_mode_reg_num, 1
hynix26nm_lsb_mode_reg_num:
	.zero	1
	.global	para0
	.section	.rodata
	.align	3
	.type	para0, %object
	.size	para0, 56
para0:
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	6
	.hword	10
	.hword	6
	.hword	0
	.hword	-3
	.hword	-7
	.hword	-8
	.hword	0
	.hword	-6
	.hword	-13
	.hword	-15
	.hword	0
	.hword	-11
	.hword	-20
	.hword	-23
	.hword	0
	.hword	0
	.hword	-26
	.hword	-30
	.hword	0
	.hword	0
	.hword	-32
	.hword	-37
	.global	para1
	.align	3
	.type	para1, %object
	.size	para1, 56
para1:
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	6
	.hword	10
	.hword	6
	.hword	0
	.hword	-3
	.hword	-7
	.hword	-8
	.hword	0
	.hword	-6
	.hword	-13
	.hword	-15
	.hword	0
	.hword	-9
	.hword	-20
	.hword	-23
	.hword	0
	.hword	0
	.hword	-26
	.hword	-30
	.hword	0
	.hword	0
	.hword	-32
	.hword	-37
	.text
	.align	2
	.global	hynix26nm_vender_set_param
	.type	hynix26nm_vender_set_param, %function
hynix26nm_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 54
	strb	w0, [x29, 54]
	mov	w0, 22
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L154
.L155:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 54
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x2
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	ldrb	w1, [x29, 55]
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L154:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L155
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC11
	add	x0, x0, :lo12:.LC11
	bl	nand_print_dbg
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix26nm_vender_set_param, .-hynix26nm_vender_set_param
	.section	.rodata
	.align	3
.LC31:
	.string	"hynix26nm retry param:"
	.align	3
.LC32:
	.string	" %x "
	.align	3
.LC33:
	.string	"\n"
	.text
	.align	2
	.global	hynix26nm_set_readretry
	.type	hynix26nm_set_readretry, %function
hynix26nm_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 52]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	adrp	x1, hynix26nm_read_retry_cycle
	add	x1, x1, :lo12:hynix26nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L158
	mov	w0, -53
	b	.L170
.L158:
	str	wzr, [x29, 60]
	b	.L160
.L164:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x1, x0
	ldrb	w0, [x0, 56]
	strh	w0, [x29, 50]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 184]
	adrp	x0, hynix26nm_read_retry_val
	add	x0, x0, :lo12:hynix26nm_read_retry_val
	ldr	w1, [x29, 60]
	uxtw	x2, w2
	lsl	x2, x2, 2
	add	x1, x2, x1
	ldrsh	w0, [x0, x1, lsl 1]
	uxth	w1, w0
	ldrh	w0, [x29, 50]
	add	w0, w1, w0
	uxth	w0, w0
	strh	w0, [x29, 58]
	ldrsh	w0, [x29, 58]
	cmp	w0, 255
	ble	.L161
	mov	w0, 255
	strh	w0, [x29, 58]
	b	.L162
.L161:
	ldrsh	w0, [x29, 58]
	cmp	w0, 0
	bge	.L163
	strh	wzr, [x29, 58]
	b	.L162
.L163:
	ldrh	w0, [x29, 58]
	and	w0, w0, 255
	strh	w0, [x29, 58]
.L162:
	ldrh	w0, [x29, 58]
	uxtb	w2, w0
	ldr	w0, [x29, 60]
	sub	x1, x29, #4032
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4072]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L160:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bhi	.L164
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 1
	bls	.L165
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 6
	bhi	.L165
	strb	wzr, [x29, 40]
.L165:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 5
	beq	.L166
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 6
	bne	.L167
.L166:
	strb	wzr, [x29, 41]
.L167:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix26nm_read_retry_reg_adr
	add	x0, x29, 40
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	str	w0, [x29, 52]
	adrp	x0, .LC31
	add	x0, x0, :lo12:.LC31
	bl	nand_print_dbg
	str	wzr, [x29, 60]
	b	.L168
.L169:
	ldr	w0, [x29, 60]
	sub	x1, x29, #4032
	add	x0, x1, x0
	ldrb	w0, [x0, 4072]
	mov	w1, w0
	adrp	x0, .LC32
	add	x0, x0, :lo12:.LC32
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L168:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bhi	.L169
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
	ldr	w0, [x29, 52]
.L170:
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix26nm_set_readretry, .-hynix26nm_set_readretry
	.section	.rodata
	.align	3
.LC34:
	.string	"m0 retry!\n"
	.align	3
.LC35:
	.string	"hynix26nm ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	hynix26nm_read_page_end
	.type	hynix26nm_read_page_end, %function
hynix26nm_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L172
	adrp	x0, .LC34
	add	x0, x0, :lo12:.LC34
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L173
.L179:
	str	wzr, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	adrp	x1, hynix26nm_read_retry_cycle
	add	x1, x1, :lo12:hynix26nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L174
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
.L174:
	ldr	x0, [x29, 32]
	bl	hynix26nm_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L175
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix26nm_set_readretry
	b	.L176
.L175:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L177
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L178
.L177:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC35
	add	x0, x0, :lo12:.LC35
	bl	nand_print_dbg
	b	.L176
.L178:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L173:
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L179
.L176:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix26nm_set_readretry
.L172:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_read_page_end, .-hynix26nm_read_page_end
	.align	2
	.global	hynix26nm_vender_get_param
	.type	hynix26nm_vender_get_param, %function
hynix26nm_vender_get_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 55
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L182
.L183:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 55
	mov	w4, 1
	mov	x3, x1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	get_data_with_nand_bus_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L182:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L183
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix26nm_vender_get_param, .-hynix26nm_vender_get_param
	.align	2
	.global	hynix26nm_lsb_init
	.type	hynix26nm_lsb_init, %function
hynix26nm_lsb_init:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	mov	w1, 5
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -92
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -91
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -80
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -79
	strb	w1, [x0, 3]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -55
	strb	w1, [x0, 4]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0, 3]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 1
	strb	w1, [x0, 4]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	hynix26nm_lsb_init, .-hynix26nm_lsb_init
	.align	2
	.global	hynix26nm_lsb_enable
	.type	hynix26nm_lsb_enable, %function
hynix26nm_lsb_enable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	adrp	x0, hynix26nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_get_param
	str	wzr, [x29, 44]
	b	.L188
.L189:
	adrp	x0, hynix26nm_lsb_mode_default_val
	add	x1, x0, :lo12:hynix26nm_lsb_mode_default_val
	ldr	w0, [x29, 44]
	ldrb	w2, [x1, x0]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w1, [x0, 4080]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x2, x0, :lo12:hynix26nm_lsb_mode_val
	ldr	w0, [x29, 44]
	ldrb	w0, [x2, x0]
	add	w0, w1, w0
	uxtb	w2, w0
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L188:
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L189
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_lsb_enable, .-hynix26nm_lsb_enable
	.align	2
	.global	hynix26nm_lsb_disable
	.type	hynix26nm_lsb_disable, %function
hynix26nm_lsb_disable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	adrp	x0, hynix26nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_lsb_disable, .-hynix26nm_lsb_disable
	.section	.rodata
	.align	3
.LC36:
	.string	"hynix26nm_lsb_exit\n"
	.text
	.align	2
	.global	hynix26nm_lsb_exit
	.type	hynix26nm_lsb_exit, %function
hynix26nm_lsb_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 1048575
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w2, 1
	mov	w1, 48
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	nop
.L194:
	ldr	w0, [x29, 44]
	sub	w1, w0, #1
	str	w1, [x29, 44]
	cmp	w0, 0
	bgt	.L194
	adrp	x0, .LC36
	add	x0, x0, :lo12:.LC36
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_lsb_exit, .-hynix26nm_lsb_exit
	.align	2
	.global	hynix26nm_getdefaultparam
	.type	hynix26nm_getdefaultparam, %function
hynix26nm_getdefaultparam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	add	x1, x0, 56
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w3, w2
	mov	x2, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_get_param
	str	w0, [x29, 40]
	str	wzr, [x29, 44]
	b	.L197
.L198:
	ldrsw	x0, [x29, 44]
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldr	x2, [x29, 24]
	ldrsw	x1, [x29, 44]
	add	x1, x2, x1
	ldrb	w1, [x1, 56]
	strb	w1, [x0]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L197:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L198
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_getdefaultparam, .-hynix26nm_getdefaultparam
	.section	.rodata
	.align	3
.LC37:
	.string	"set retry default value: "
	.align	3
.LC38:
	.string	" %x"
	.text
	.align	2
	.global	hynix26nm_setdefaultparam
	.type	hynix26nm_setdefaultparam, %function
hynix26nm_setdefaultparam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L201
.L202:
	ldr	x1, [x29, 24]
	ldrsw	x0, [x29, 44]
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldrsw	x0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L201:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L202
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix26nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC37
	add	x0, x0, :lo12:.LC37
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L203
.L204:
	ldrsw	x0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC38
	add	x0, x0, :lo12:.LC38
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L203:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L204
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_setdefaultparam, .-hynix26nm_setdefaultparam
	.section	.rodata
	.align	3
.LC39:
	.string	"hynix26nm_get_default_param: ch: %d, chip: %d, value: 0x%x 0x%x 0x%x 0x%x \n"
	.text
	.align	2
	.global	hynix26nm_get_default_param
	.type	hynix26nm_get_default_param, %function
hynix26nm_get_default_param:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_getdefaultparam
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 28]
	ldrb	w0, [x29, 40]
	mov	w3, w0
	ldrb	w0, [x29, 41]
	mov	w4, w0
	ldrb	w0, [x29, 42]
	mov	w5, w0
	ldrb	w0, [x29, 43]
	mov	w6, w0
	adrp	x0, .LC39
	add	x0, x0, :lo12:.LC39
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix26nm_setdefaultparam
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_get_default_param, .-hynix26nm_get_default_param
	.section	.rodata
	.align	3
.LC40:
	.string	"NFC_ReadRetryInit, unknown read retry mode 0x%x\n"
	.text
	.align	2
	.global	hynix26nm_readretry_init
	.type	hynix26nm_readretry_init, %function
hynix26nm_readretry_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L209
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -84
	strb	w1, [x0]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -83
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -82
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -81
	strb	w1, [x0, 3]
	str	wzr, [x29, 44]
	b	.L210
.L213:
	str	wzr, [x29, 40]
	b	.L211
.L212:
	adrp	x0, para0
	add	x0, x0, :lo12:para0
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	ldrsh	w3, [x0, x1, lsl 1]
	adrp	x0, hynix26nm_read_retry_val
	add	x0, x0, :lo12:hynix26nm_read_retry_val
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	mov	w2, w3
	strh	w2, [x0, x1, lsl 1]
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
.L211:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bhi	.L212
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L210:
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	add	w0, w0, 1
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L213
	b	.L214
.L209:
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L215
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -89
	strb	w1, [x0]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -83
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -82
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -81
	strb	w1, [x0, 3]
	str	wzr, [x29, 44]
	b	.L216
.L219:
	str	wzr, [x29, 40]
	b	.L217
.L218:
	adrp	x0, para1
	add	x0, x0, :lo12:para1
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	ldrsh	w3, [x0, x1, lsl 1]
	adrp	x0, hynix26nm_read_retry_val
	add	x0, x0, :lo12:hynix26nm_read_retry_val
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	mov	w2, w3
	strh	w2, [x0, x1, lsl 1]
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
.L217:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bhi	.L218
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L216:
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	add	w0, w0, 1
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L219
	b	.L214
.L215:
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC40
	add	x0, x0, :lo12:.LC40
	bl	nand_print
	mov	w0, -54
	b	.L220
.L214:
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	str	wzr, [x29, 44]
	b	.L221
.L222:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	strb	wzr, [x0, 56]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L221:
	ldr	w0, [x29, 44]
	cmp	w0, 127
	bls	.L222
	ldr	x0, [x29, 24]
	bl	hynix26nm_get_default_param
	mov	w0, 0
.L220:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_readretry_init, .-hynix26nm_readretry_init
	.section	.rodata
	.align	3
.LC41:
	.string	"hynix26nm_readretry_exit. \n"
	.text
	.align	2
	.global	hynix26nm_readretry_exit
	.type	hynix26nm_readretry_exit, %function
hynix26nm_readretry_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC41
	add	x0, x0, :lo12:.LC41
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix26nm_setdefaultparam
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_readretry_exit, .-hynix26nm_readretry_exit
	.section	.rodata
	.align	3
.LC42:
	.string	" hynix26nm_special_init hynix26nm_read_retry_mode: %d hynix26nm_read_retry_cycle:%d hynix26nm_read_retry_reg_num:%d\n"
	.align	3
.LC43:
	.string	" hynix26nm_special_init error hynix26nm_read_retry_mode: %d hynix26nm_read_retry_cycle:%d hynix26nm_read_retry_reg_num:%d\n"
	.text
	.align	2
	.global	hynix26nm_special_init
	.type	hynix26nm_special_init, %function
hynix26nm_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L226
.L227:
	ldr	x0, [x29, 16]
	bl	hynix26nm_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L226:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L227
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L228
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, hynix26nm_read_page_end
	add	x1, x1, :lo12:hynix26nm_read_page_end
	str	x1, [x0]
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC42
	add	x0, x0, :lo12:.LC42
	bl	nand_print_dbg
	b	.L229
.L228:
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC43
	add	x0, x0, :lo12:.LC43
	bl	nand_print
.L229:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_special_init, .-hynix26nm_special_init
	.section	.rodata
	.align	3
.LC44:
	.string	" hynix26nm_special_exit \n"
	.text
	.align	2
	.global	hynix26nm_special_exit
	.type	hynix26nm_special_exit, %function
hynix26nm_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L232
.L233:
	ldr	x0, [x29, 24]
	bl	hynix26nm_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L232:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L233
	adrp	x0, .LC44
	add	x0, x0, :lo12:.LC44
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_special_exit, .-hynix26nm_special_exit
	.global	hynix20nm_read_retry_mode
	.bss
	.type	hynix20nm_read_retry_mode, %object
	.size	hynix20nm_read_retry_mode, 1
hynix20nm_read_retry_mode:
	.zero	1
	.global	hynix20nm_read_retry_cycle
	.type	hynix20nm_read_retry_cycle, %object
	.size	hynix20nm_read_retry_cycle, 1
hynix20nm_read_retry_cycle:
	.zero	1
	.global	hynix20nm_read_retry_reg_num
	.type	hynix20nm_read_retry_reg_num, %object
	.size	hynix20nm_read_retry_reg_num, 1
hynix20nm_read_retry_reg_num:
	.zero	1
	.global	hynix20nm_read_retry_reg_adr
	.align	3
	.type	hynix20nm_read_retry_reg_adr, %object
	.size	hynix20nm_read_retry_reg_adr, 8
hynix20nm_read_retry_reg_adr:
	.zero	8
	.global	hynix20nm_read_retry_reg_adr_1
	.data
	.align	3
	.type	hynix20nm_read_retry_reg_adr_1, %object
	.size	hynix20nm_read_retry_reg_adr_1, 8
hynix20nm_read_retry_reg_adr_1:
	.byte	-52
	.byte	-65
	.byte	-86
	.byte	-85
	.byte	-51
	.byte	-83
	.byte	-82
	.byte	-81
	.global	hynix20nm_read_retry_reg_adr_2
	.align	3
	.type	hynix20nm_read_retry_reg_adr_2, %object
	.size	hynix20nm_read_retry_reg_adr_2, 8
hynix20nm_read_retry_reg_adr_2:
	.byte	-80
	.byte	-79
	.byte	-78
	.byte	-77
	.byte	-76
	.byte	-75
	.byte	-74
	.byte	-73
	.global	hynix20nm_lsb_mode_reg_adr
	.bss
	.align	3
	.type	hynix20nm_lsb_mode_reg_adr, %object
	.size	hynix20nm_lsb_mode_reg_adr, 4
hynix20nm_lsb_mode_reg_adr:
	.zero	4
	.global	hynix20nm_lsb_mode_default_val
	.align	3
	.type	hynix20nm_lsb_mode_default_val, %object
	.size	hynix20nm_lsb_mode_default_val, 4
hynix20nm_lsb_mode_default_val:
	.zero	4
	.global	hynix20nm_lsb_mode_val
	.align	3
	.type	hynix20nm_lsb_mode_val, %object
	.size	hynix20nm_lsb_mode_val, 4
hynix20nm_lsb_mode_val:
	.zero	4
	.global	hynix20nm_lsb_mode_reg_num
	.type	hynix20nm_lsb_mode_reg_num, %object
	.size	hynix20nm_lsb_mode_reg_num, 1
hynix20nm_lsb_mode_reg_num:
	.zero	1
	.text
	.align	2
	.global	hynix20nm_vender_set_param
	.type	hynix20nm_vender_set_param, %function
hynix20nm_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 54
	strb	w0, [x29, 54]
	mov	w0, 22
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L236
.L237:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 54
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x2
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	ldrb	w1, [x29, 55]
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L236:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L237
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix20nm_vender_set_param, .-hynix20nm_vender_set_param
	.section	.rodata
	.align	3
.LC45:
	.string	"retry para:"
	.text
	.align	2
	.global	hynix20nm_set_readretry
	.type	hynix20nm_set_readretry, %function
hynix20nm_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	adrp	x1, hynix20nm_read_retry_cycle
	add	x1, x1, :lo12:hynix20nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L240
	mov	w0, -92
	b	.L246
.L240:
	str	wzr, [x29, 44]
	b	.L242
.L243:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	lsl	w1, w0, 3
	ldr	w0, [x29, 44]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	uxtw	x0, w0
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L242:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L243
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix20nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L244
.L245:
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC32
	add	x0, x0, :lo12:.LC32
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L244:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L245
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
.L246:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_set_readretry, .-hynix20nm_set_readretry
	.section	.rodata
	.align	3
.LC46:
	.string	"hynix20nm retry!\n"
	.align	3
.LC47:
	.string	"sdata:0x%x %x %x %x "
	.align	3
.LC48:
	.string	"hynix20nm ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	hynix20nm_read_page_end
	.type	hynix20nm_read_page_end, %function
hynix20nm_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L248
	adrp	x0, .LC46
	add	x0, x0, :lo12:.LC46
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L249
.L256:
	str	wzr, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	adrp	x1, hynix20nm_read_retry_cycle
	add	x1, x1, :lo12:hynix20nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L250
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
.L250:
	ldr	x0, [x29, 32]
	bl	hynix20nm_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L251
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix20nm_set_readretry
	b	.L252
.L251:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L253
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC22
	add	x0, x0, :lo12:.LC22
	bl	nand_print_dbg
	b	.L254
.L253:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L255
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L254
.L255:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC47
	add	x0, x0, :lo12:.LC47
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC48
	add	x0, x0, :lo12:.LC48
	bl	nand_print_dbg
	b	.L252
.L254:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L249:
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L256
.L252:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix20nm_set_readretry
.L248:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_read_page_end, .-hynix20nm_read_page_end
	.align	2
	.global	hynix20nm_vender_get_param
	.type	hynix20nm_vender_get_param, %function
hynix20nm_vender_get_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w0, 55
	strb	w0, [x29, 55]
	str	wzr, [x29, 60]
	b	.L259
.L260:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 55
	mov	w4, 1
	mov	x3, x1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	get_data_with_nand_bus_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L259:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L260
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix20nm_vender_get_param, .-hynix20nm_vender_get_param
	.align	2
	.global	hynix20nm_lsb_init
	.type	hynix20nm_lsb_init, %function
hynix20nm_lsb_init:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L263
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	mov	w1, 4
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -80
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -79
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -96
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -95
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 3]
	b	.L264
.L263:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 3
	bne	.L264
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	mov	w1, 4
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -96
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -95
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -89
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -88
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 3]
.L264:
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	hynix20nm_lsb_init, .-hynix20nm_lsb_init
	.align	2
	.global	hynix20nm_lsb_enable
	.type	hynix20nm_lsb_enable, %function
hynix20nm_lsb_enable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	adrp	x0, hynix20nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_get_param
	str	wzr, [x29, 44]
	b	.L267
.L268:
	adrp	x0, hynix20nm_lsb_mode_default_val
	add	x1, x0, :lo12:hynix20nm_lsb_mode_default_val
	ldr	w0, [x29, 44]
	ldrb	w2, [x1, x0]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4088]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w1, [x0, 4088]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x2, x0, :lo12:hynix20nm_lsb_mode_val
	ldr	w0, [x29, 44]
	ldrb	w0, [x2, x0]
	add	w0, w1, w0
	uxtb	w2, w0
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4088]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L267:
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L268
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	add	x0, x29, 40
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_lsb_enable, .-hynix20nm_lsb_enable
	.align	2
	.global	hynix20nm_lsb_disable
	.type	hynix20nm_lsb_disable, %function
hynix20nm_lsb_disable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	adrp	x0, hynix20nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_lsb_disable, .-hynix20nm_lsb_disable
	.section	.rodata
	.align	3
.LC49:
	.string	"hynix20nm_lsb_exit\n"
	.text
	.align	2
	.global	hynix20nm_lsb_exit
	.type	hynix20nm_lsb_exit, %function
hynix20nm_lsb_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 1048575
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w2, 1
	mov	w1, 48
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	nop
.L273:
	ldr	w0, [x29, 44]
	sub	w1, w0, #1
	str	w1, [x29, 44]
	cmp	w0, 0
	bgt	.L273
	adrp	x0, .LC49
	add	x0, x0, :lo12:.LC49
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_lsb_exit, .-hynix20nm_lsb_exit
	.section	.rodata
	.align	3
.LC50:
	.string	"hynix OTP RegCount value error: 0x%x, 0x%x \n"
	.align	3
.LC51:
	.string	"otp copy %d is ok!\n"
	.align	3
.LC52:
	.string	"otp copy failed!\n"
	.text
	.align	2
	.global	hynix20nm_vender_get_param_otp_hynix
	.type	hynix20nm_vender_get_param_otp_hynix, %function
hynix20nm_vender_get_param_otp_hynix:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 128]
	strb	wzr, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 32]
	strb	wzr, [x29, 33]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L276
	mov	w0, -1
	strb	w0, [x29, 40]
	mov	w0, -52
	strb	w0, [x29, 41]
	mov	w0, 64
	strb	w0, [x29, 32]
	mov	w0, 77
	strb	w0, [x29, 33]
	b	.L277
.L276:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 3
	bne	.L278
	mov	w0, -82
	strb	w0, [x29, 40]
	mov	w0, -80
	strb	w0, [x29, 41]
	strb	wzr, [x29, 32]
	mov	w0, 77
	strb	w0, [x29, 33]
	b	.L277
.L278:
	mov	w0, -97
	b	.L291
.L277:
	mov	w0, 1024
	bl	nand_get_temp_buf
	str	x0, [x29, 120]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w0, 54
	strb	w0, [x29, 112]
	add	x2, x29, 32
	add	x1, x29, 40
	add	x0, x29, 112
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	add	x0, x29, 32
	add	x1, x0, 1
	add	x0, x29, 40
	add	x0, x0, 1
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x0
	mov	w2, 0
	mov	x1, 0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	mov	w1, 22
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 23
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 4
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 25
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 2
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w0, 48
	strb	w0, [x29, 112]
	add	x0, x29, 112
	mov	w4, 2
	ldr	x3, [x29, 120]
	mov	x2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_data_with_nand_bus_one_cmd
	ldr	x0, [x29, 120]
	ldrb	w0, [x0]
	cmp	w0, 8
	bne	.L280
	ldr	x0, [x29, 120]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 8
	beq	.L281
.L280:
	ldr	x0, [x29, 120]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 120]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC50
	add	x0, x0, :lo12:.LC50
	bl	nand_print
	mov	w0, -96
	str	w0, [x29, 128]
.L281:
	mov	w4, 1024
	ldr	x3, [x29, 120]
	mov	x2, 0
	mov	x1, 0
	ldr	x0, [x29, 24]
	bl	get_data_with_nand_bus_one_cmd
	str	wzr, [x29, 136]
	b	.L282
.L289:
	str	wzr, [x29, 132]
	str	wzr, [x29, 140]
	b	.L283
.L286:
	ldr	w0, [x29, 136]
	lsl	w0, w0, 7
	uxtw	x1, w0
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	ldr	x1, [x29, 120]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 56]
	ldr	w0, [x29, 136]
	lsl	w0, w0, 7
	uxtw	x1, w0
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	add	x0, x0, 64
	ldr	x1, [x29, 120]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 140]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4000]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	ldrb	w0, [x0, 56]
	mov	w2, w0
	ldr	w0, [x29, 140]
	sub	x1, x29, #3952
	add	x0, x1, x0
	ldrb	w0, [x0, 4000]
	add	w0, w2, w0
	cmp	w0, 255
	beq	.L284
	mov	w0, 1
	str	w0, [x29, 132]
	b	.L285
.L284:
	ldr	w0, [x29, 140]
	add	w0, w0, 1
	str	w0, [x29, 140]
.L283:
	ldr	w0, [x29, 140]
	cmp	w0, 63
	bls	.L286
.L285:
	ldr	w0, [x29, 132]
	cmp	w0, 0
	bne	.L287
	adrp	x0, .LC51
	add	x0, x0, :lo12:.LC51
	ldr	w1, [x29, 136]
	bl	nand_print_dbg
	b	.L288
.L287:
	ldr	w0, [x29, 136]
	add	w0, w0, 1
	str	w0, [x29, 136]
.L282:
	ldr	w0, [x29, 136]
	cmp	w0, 7
	bls	.L289
.L288:
	ldr	w0, [x29, 132]
	cmp	w0, 0
	beq	.L290
	adrp	x0, .LC52
	add	x0, x0, :lo12:.LC52
	bl	nand_print_dbg
	mov	w0, -95
	str	w0, [x29, 128]
.L290:
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 1
	mov	w1, 56
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	x0, [x29, 120]
	bl	nand_free_temp_buf
	ldr	w0, [x29, 128]
.L291:
	ldp	x29, x30, [sp], 144
	ret
	.size	hynix20nm_vender_get_param_otp_hynix, .-hynix20nm_vender_get_param_otp_hynix
	.align	2
	.global	hynix20nm_saveotpvalue
	.type	hynix20nm_saveotpvalue, %function
hynix20nm_saveotpvalue:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	str	wzr, [sp, 28]
	b	.L293
.L294:
	ldr	w0, [sp, 28]
	ldr	x1, [sp]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	x1, [sp, 8]
	ldr	w0, [sp, 28]
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 56]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L293:
	ldr	w0, [sp, 28]
	cmp	w0, 63
	bls	.L294
	nop
	add	sp, sp, 32
	ret
	.size	hynix20nm_saveotpvalue, .-hynix20nm_saveotpvalue
	.section	.rodata
	.align	3
.LC53:
	.string	"_vender_get_param_otp_hynix time %d!\n"
	.align	3
.LC54:
	.string	"_vender_get_param_otp_hynix error!\n"
	.text
	.align	2
	.global	hynix20nm_getotpparam
	.type	hynix20nm_getotpparam, %function
hynix20nm_getotpparam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 32]
	b	.L296
.L299:
	adrp	x0, .LC53
	add	x0, x0, :lo12:.LC53
	ldr	w1, [x29, 32]
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_get_param_otp_hynix
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L306
	ldr	w0, [x29, 32]
	add	w0, w0, 1
	str	w0, [x29, 32]
.L296:
	ldr	w0, [x29, 32]
	cmp	w0, 4
	bls	.L299
	b	.L298
.L306:
	nop
.L298:
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L300
	adrp	x0, .LC54
	add	x0, x0, :lo12:.LC54
	bl	nand_print
	ldr	w0, [x29, 44]
	b	.L301
.L300:
	str	wzr, [x29, 40]
	b	.L302
.L305:
	str	wzr, [x29, 36]
	b	.L303
.L304:
	ldr	w0, [x29, 40]
	lsl	w1, w0, 3
	ldr	w0, [x29, 36]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldr	w1, [x29, 40]
	lsl	w2, w1, 3
	ldr	w1, [x29, 36]
	add	w1, w2, w1
	ldr	x2, [x29, 24]
	uxtw	x1, w1
	add	x1, x2, x1
	ldrb	w1, [x1, 56]
	strb	w1, [x0]
	ldr	w0, [x29, 36]
	add	w0, w0, 1
	str	w0, [x29, 36]
.L303:
	ldr	w0, [x29, 36]
	cmp	w0, 7
	bls	.L304
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
.L302:
	ldr	w0, [x29, 40]
	cmp	w0, 7
	bls	.L305
	mov	w0, 0
.L301:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_getotpparam, .-hynix20nm_getotpparam
	.align	2
	.global	hynix20nm_set_default_param
	.type	hynix20nm_set_default_param, %function
hynix20nm_set_default_param:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L308
.L309:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L308:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L309
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix20nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC37
	add	x0, x0, :lo12:.LC37
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L310
.L311:
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC38
	add	x0, x0, :lo12:.LC38
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L310:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L311
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_set_default_param, .-hynix20nm_set_default_param
	.section	.rodata
	.align	3
.LC55:
	.string	"hynix20nm get default param error !\n"
	.align	3
.LC56:
	.string	"[PHY_GetDefaultParam]:readbuf malloc fail\n"
	.align	3
.LC57:
	.string	"ch %d, chip %d Read Retry value Table from uboot:\n"
	.align	3
.LC58:
	.string	"0x%x "
	.align	3
.LC59:
	.string	"[PHY_DBG] ch %d, can't get right otp value from nand otp blocks, then use otp command\n"
	.align	3
.LC60:
	.string	"[PHY_DBG] repair ch %d otp value end\n"
	.text
	.align	2
	.global	hynix20nm_init_readretry_param
	.type	hynix20nm_init_readretry_param, %function
hynix20nm_init_readretry_param:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 124]
	str	xzr, [x29, 112]
	str	xzr, [x29, 104]
	str	wzr, [x29, 120]
	str	wzr, [x29, 100]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 20]
	lsl	w0, w0, 9
	bl	nand_get_temp_buf
	str	x0, [x29, 112]
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L314
	adrp	x0, .LC55
	add	x0, x0, :lo12:.LC55
	bl	nand_print_dbg
	mov	w0, -94
	b	.L323
.L314:
	mov	w0, 32768
	bl	nand_get_temp_buf
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L316
	ldr	x0, [x29, 112]
	bl	nand_free_temp_buf
	adrp	x0, .LC56
	add	x0, x0, :lo12:.LC56
	bl	nand_print
	mov	w0, -76
	b	.L323
.L316:
	str	wzr, [x29, 100]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 24]
	mov	w3, w0
	mov	w2, 64
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 104]
	bl	hynix16nm_get_special_info
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	bne	.L317
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 24]
	adrp	x0, .LC57
	add	x0, x0, :lo12:.LC57
	bl	nand_print_dbg
	str	wzr, [x29, 124]
	b	.L318
.L320:
	ldr	w0, [x29, 124]
	ldr	x1, [x29, 112]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC58
	add	x0, x0, :lo12:.LC58
	bl	nand_print_dbg
	ldr	w0, [x29, 124]
	and	w0, w0, 7
	cmp	w0, 7
	bne	.L319
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
.L319:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L318:
	ldr	w0, [x29, 124]
	cmp	w0, 63
	bls	.L320
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	hynix20nm_saveotpvalue
	ldr	x0, [x29, 24]
	bl	hynix20nm_set_default_param
	str	w0, [x29, 120]
	b	.L321
.L317:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, .LC59
	add	x0, x0, :lo12:.LC59
	bl	nand_print_dbg
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_getotpparam
	str	w0, [x29, 120]
	ldr	w0, [x29, 120]
	cmp	w0, 0
	bne	.L324
	ldr	x0, [x29, 24]
	bl	hynix20nm_set_default_param
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, .LC60
	add	x0, x0, :lo12:.LC60
	bl	nand_print_dbg
	b	.L321
.L324:
	nop
.L321:
	ldr	x0, [x29, 112]
	bl	nand_free_temp_buf
	ldr	w0, [x29, 120]
.L323:
	ldp	x29, x30, [sp], 128
	ret
	.size	hynix20nm_init_readretry_param, .-hynix20nm_init_readretry_param
	.align	2
	.global	hynix20nm_readretry_init
	.type	hynix20nm_readretry_init, %function
hynix20nm_readretry_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L326
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -52
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -65
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -86
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -85
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -51
	strb	w1, [x0, 4]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -83
	strb	w1, [x0, 5]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -82
	strb	w1, [x0, 6]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -81
	strb	w1, [x0, 7]
	b	.L327
.L326:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 3
	bne	.L328
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -80
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -79
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -78
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -77
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -76
	strb	w1, [x0, 4]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -75
	strb	w1, [x0, 5]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -74
	strb	w1, [x0, 6]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -73
	strb	w1, [x0, 7]
	b	.L327
.L328:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC40
	add	x0, x0, :lo12:.LC40
	bl	nand_print
	mov	w0, -93
	b	.L329
.L327:
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	str	wzr, [x29, 44]
	b	.L330
.L331:
	ldr	x1, [x29, 24]
	ldrsw	x0, [x29, 44]
	add	x0, x1, x0
	strb	wzr, [x0, 56]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L330:
	ldr	w0, [x29, 44]
	cmp	w0, 127
	ble	.L331
	ldr	x0, [x29, 24]
	bl	hynix20nm_init_readretry_param
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
.L329:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_readretry_init, .-hynix20nm_readretry_init
	.section	.rodata
	.align	3
.LC61:
	.string	"hynix20nm_readretry_exit. \n"
	.text
	.align	2
	.global	hynix20nm_readretry_exit
	.type	hynix20nm_readretry_exit, %function
hynix20nm_readretry_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC61
	add	x0, x0, :lo12:.LC61
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix20nm_set_default_param
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_readretry_exit, .-hynix20nm_readretry_exit
	.section	.rodata
	.align	3
.LC62:
	.string	" hynix20nm_special_init hynix20nm_read_retry_mode: %d hynix20nm_read_retry_cycle:%d hynix20nm_read_retry_reg_num:%d\n"
	.align	3
.LC63:
	.string	" hynix20nm_special_init error hynix20nm_read_retry_mode: %d hynix20nm_read_retry_cycle:%d hynix20nm_read_retry_reg_num:%d\n"
	.text
	.align	2
	.global	hynix20nm_special_init
	.type	hynix20nm_special_init, %function
hynix20nm_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L335
.L336:
	ldr	x0, [x29, 16]
	bl	hynix20nm_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L335:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L336
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L337
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, hynix20nm_read_page_end
	add	x1, x1, :lo12:hynix20nm_read_page_end
	str	x1, [x0]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC62
	add	x0, x0, :lo12:.LC62
	bl	nand_print_dbg
	b	.L338
.L337:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC63
	add	x0, x0, :lo12:.LC63
	bl	nand_print
.L338:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_special_init, .-hynix20nm_special_init
	.section	.rodata
	.align	3
.LC64:
	.string	" hynix20nm_special_exit \n"
	.text
	.align	2
	.global	hynix20nm_special_exit
	.type	hynix20nm_special_exit, %function
hynix20nm_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L341
.L342:
	ldr	x0, [x29, 24]
	bl	hynix20nm_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L341:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L342
	adrp	x0, .LC64
	add	x0, x0, :lo12:.LC64
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_special_exit, .-hynix20nm_special_exit
	.global	hynix16nm_read_retry_mode
	.bss
	.type	hynix16nm_read_retry_mode, %object
	.size	hynix16nm_read_retry_mode, 1
hynix16nm_read_retry_mode:
	.zero	1
	.global	hynix16nm_read_retry_cycle
	.type	hynix16nm_read_retry_cycle, %object
	.size	hynix16nm_read_retry_cycle, 1
hynix16nm_read_retry_cycle:
	.zero	1
	.global	hynix16nm_read_retry_reg_num
	.type	hynix16nm_read_retry_reg_num, %object
	.size	hynix16nm_read_retry_reg_num, 1
hynix16nm_read_retry_reg_num:
	.zero	1
	.global	hynix16nm_read_retry_reg_adr
	.align	3
	.type	hynix16nm_read_retry_reg_adr, %object
	.size	hynix16nm_read_retry_reg_adr, 4
hynix16nm_read_retry_reg_adr:
	.zero	4
	.text
	.align	2
	.global	hynix16nm_vender_set_param
	.type	hynix16nm_vender_set_param, %function
hynix16nm_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 54
	strb	w0, [x29, 54]
	mov	w0, 22
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L345
.L346:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 54
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x2
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	ldrb	w1, [x29, 55]
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L345:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L346
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC11
	add	x0, x0, :lo12:.LC11
	bl	nand_print_dbg
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_vender_set_param, .-hynix16nm_vender_set_param
	.align	2
	.global	hynix16nm_set_readretry
	.type	hynix16nm_set_readretry, %function
hynix16nm_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	adrp	x1, hynix16nm_read_retry_cycle
	add	x1, x1, :lo12:hynix16nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L349
	mov	w0, -74
	b	.L353
.L349:
	str	wzr, [x29, 44]
	b	.L351
.L352:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	lsl	w1, w0, 2
	ldr	w0, [x29, 44]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	uxtw	x0, w0
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L351:
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L352
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix16nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix16nm_vender_set_param
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
.L353:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_set_readretry, .-hynix16nm_set_readretry
	.section	.rodata
	.align	3
.LC65:
	.string	"get_hynix_special_info start!!\n"
	.align	3
.LC66:
	.string	"hynix_special_info:can't find uboot head\n"
	.align	3
.LC67:
	.string	"physic info start_block %d\n"
	.text
	.align	2
	.global	hynix16nm_get_special_info
	.type	hynix16nm_get_special_info, %function
hynix16nm_get_special_info:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	adrp	x0, .LC65
	add	x0, x0, :lo12:.LC65
	bl	nand_print_dbg
	add	x0, x29, 60
	bl	physic_info_get_offset
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L355
	adrp	x0, .LC66
	add	x0, x0, :lo12:.LC66
	bl	nand_print
	mov	w0, 1
	b	.L365
.L355:
	mov	w0, -1
	str	w0, [x29, 88]
	mov	w0, 4
	str	w0, [x29, 84]
	b	.L357
.L364:
	adrp	x0, .LC67
	add	x0, x0, :lo12:.LC67
	ldr	w1, [x29, 84]
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	ldrb	w0, [x0, 1536]
	cmp	w0, 165
	beq	.L358
	ldr	w0, [x29, 60]
	add	x1, x29, 56
	ldr	x3, [x29, 40]
	mov	x2, x1
	mov	w1, w0
	ldr	w0, [x29, 84]
	bl	physic_info_get_one_copy
.L358:
	ldr	x0, [x29, 40]
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	ldrb	w0, [x0, 1536]
	cmp	w0, 165
	bne	.L359
	strb	wzr, [x29, 83]
	mov	w0, 2
	str	w0, [x29, 92]
	b	.L360
.L361:
	ldr	x1, [x29, 64]
	ldr	w0, [x29, 92]
	add	x0, x1, x0
	add	x0, x0, 8192
	ldrb	w1, [x0, 1536]
	ldrb	w0, [x29, 83]
	add	w0, w1, w0
	strb	w0, [x29, 83]
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L360:
	ldr	w0, [x29, 92]
	cmp	w0, 1023
	bls	.L361
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	ldrb	w0, [x0, 1537]
	ldrb	w1, [x29, 83]
	cmp	w1, w0
	bne	.L362
	str	wzr, [x29, 88]
	ldr	w1, [x29, 24]
	ldr	w0, [x29, 28]
	mul	w0, w1, w0
	add	w0, w0, 2
	uxtw	x0, w0
	add	x0, x0, 8192
	add	x0, x0, 1536
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 28]
	mov	x1, x0
	ldr	x0, [x29, 32]
	bl	nand_memcpy
	b	.L363
.L362:
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	strb	wzr, [x0, 1536]
.L359:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L357:
	ldr	w0, [x29, 84]
	cmp	w0, 50
	bls	.L364
.L363:
	ldr	w0, [x29, 88]
.L365:
	ldp	x29, x30, [sp], 96
	ret
	.size	hynix16nm_get_special_info, .-hynix16nm_get_special_info
	.section	.rodata
	.align	3
.LC68:
	.string	"m1 retry!\n"
	.align	3
.LC69:
	.string	"m1 ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	hynix16nm_read_page_end
	.type	hynix16nm_read_page_end, %function
hynix16nm_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L367
	adrp	x0, .LC68
	add	x0, x0, :lo12:.LC68
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L368
.L374:
	str	wzr, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	adrp	x1, hynix16nm_read_retry_cycle
	add	x1, x1, :lo12:hynix16nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L369
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
.L369:
	ldr	x0, [x29, 32]
	bl	hynix16nm_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L370
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix16nm_set_readretry
	b	.L371
.L370:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L372
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L373
.L372:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC69
	add	x0, x0, :lo12:.LC69
	bl	nand_print_dbg
	b	.L371
.L373:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L368:
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L374
.L371:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix16nm_set_readretry
.L367:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_read_page_end, .-hynix16nm_read_page_end
	.section	.rodata
	.align	3
.LC70:
	.string	"fatal err -0, wrong input parameter, ch: %d  chip: %d/%d  block: %d/%d \n"
	.align	3
.LC71:
	.string	"m1 write page FF wrong\n"
	.text
	.align	2
	.global	hynix16nm_write_page_FF
	.type	hynix16nm_write_page_FF, %function
hynix16nm_write_page_FF:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 76]
	str	wzr, [x29, 72]
	str	wzr, [x29, 68]
	str	wzr, [x29, 56]
	str	wzr, [x29, 64]
	str	wzr, [x29, 60]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	str	x0, [x29, 40]
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	add	x0, x0, 40
	str	x0, [x29, 32]
	ldr	x0, [x29, 48]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 16]
	cmp	w1, w0
	bcs	.L377
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 48]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bcc	.L378
.L377:
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 48]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	w3, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 4]
	ldr	x0, [x29, 48]
	ldr	w5, [x0, 32]
	adrp	x0, .LC70
	add	x0, x0, :lo12:.LC70
	bl	nand_print
	mov	w0, -108
	b	.L379
.L378:
	ldr	x0, [x29, 48]
	bl	nand_read_chip_status_ready
	ldr	x0, [x29, 48]
	bl	nand_enable_chip
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_ecc
	ldr	x0, [x29, 32]
	bl	ndfc_clean_cmd_seq
	ldr	w0, [x29, 20]
	cmp	w0, 8
	bne	.L380
	mov	w0, 9
	str	w0, [x29, 72]
	mov	w0, 832
	str	w0, [x29, 68]
	b	.L381
.L380:
	ldr	w0, [x29, 20]
	cmp	w0, 16
	bne	.L381
	mov	w0, 18
	str	w0, [x29, 72]
	mov	w0, 640
	str	w0, [x29, 68]
.L381:
	str	wzr, [x29, 76]
	b	.L382
.L386:
	ldr	w0, [x29, 76]
	cmp	w0, 0
	bne	.L383
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 128
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 56]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 48]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 52]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 32]
	str	x1, [x0, 72]
	ldr	x0, [x29, 32]
	mov	w1, 1024
	str	w1, [x0, 60]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 44]
	ldr	x0, [x29, 48]
	ldr	w3, [x0, 44]
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	mov	w2, w0
	mov	w0, w3
	bl	get_row_addr
	str	w0, [x29, 64]
	ldr	x0, [x29, 32]
	mov	w1, 5
	str	w1, [x0, 40]
	ldr	x0, [x29, 32]
	add	x0, x0, 24
	mov	x4, x0
	mov	w3, 3
	ldr	w2, [x29, 64]
	mov	w1, 2
	ldr	w0, [x29, 60]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
	b	.L384
.L383:
	ldr	w0, [x29, 72]
	sub	w1, w0, #1
	ldr	w0, [x29, 76]
	cmp	w1, w0
	bne	.L385
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 133
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 56]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 48]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 52]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 16]
	ldr	w0, [x29, 76]
	lsl	w0, w0, 10
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 72]
	ldr	x0, [x29, 32]
	ldr	w1, [x29, 68]
	str	w1, [x0, 60]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 44]
	ldr	x0, [x29, 32]
	mov	w1, 2
	str	w1, [x0, 40]
	ldr	x0, [x29, 32]
	add	x0, x0, 24
	mov	x4, x0
	mov	w3, 0
	ldr	w2, [x29, 64]
	mov	w1, 2
	ldr	w0, [x29, 60]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
	b	.L384
.L385:
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 133
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 56]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 48]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 52]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 16]
	ldr	w0, [x29, 76]
	lsl	w0, w0, 10
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 72]
	ldr	x0, [x29, 32]
	mov	w1, 1024
	str	w1, [x0, 60]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 44]
	ldr	x0, [x29, 32]
	mov	w1, 2
	str	w1, [x0, 40]
	ldr	x0, [x29, 32]
	add	x0, x0, 24
	mov	x4, x0
	mov	w3, 0
	ldr	w2, [x29, 64]
	mov	w1, 2
	ldr	w0, [x29, 60]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
.L384:
	ldr	w0, [x29, 60]
	add	w0, w0, 1024
	str	w0, [x29, 60]
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
.L382:
	ldr	w1, [x29, 76]
	ldr	w0, [x29, 72]
	cmp	w1, w0
	bcc	.L386
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 16
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 92]
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
	ldr	x0, [x29, 48]
	bl	nand_read_chip_status_ready
	str	w0, [x29, 56]
	ldr	w0, [x29, 56]
	cmp	w0, 0
	beq	.L387
	adrp	x0, .LC71
	add	x0, x0, :lo12:.LC71
	bl	nand_print
.L387:
	ldr	x0, [x29, 48]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
.L379:
	ldp	x29, x30, [sp], 80
	ret
	.size	hynix16nm_write_page_FF, .-hynix16nm_write_page_FF
	.align	2
	.global	hynix16nm_vender_get_param
	.type	hynix16nm_vender_get_param, %function
hynix16nm_vender_get_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 55
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L389
.L390:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 55
	mov	w4, 1
	mov	x3, x1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	get_data_with_nand_bus_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L389:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L390
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_vender_get_param, .-hynix16nm_vender_get_param
	.section	.rodata
	.align	3
.LC72:
	.string	"_major_check_byte, wrong input para, level %d, len %d\n"
	.align	3
.LC73:
	.string	"%d:  '1'-'0' : %d - %d\n"
	.align	3
.LC74:
	.string	"get bit %d failed!\n"
	.text
	.align	2
	.global	hynix16nm_major_check_byte
	.type	hynix16nm_major_check_byte, %function
hynix16nm_major_check_byte:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 56]
	str	w1, [x29, 52]
	str	w2, [x29, 48]
	str	x3, [x29, 40]
	str	x4, [x29, 32]
	str	w5, [x29, 28]
	strb	wzr, [x29, 71]
	ldr	w0, [x29, 28]
	lsr	w1, w0, 1
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L393
	adrp	x0, .LC72
	add	x0, x0, :lo12:.LC72
	ldr	w2, [x29, 28]
	ldr	w1, [x29, 48]
	bl	nand_print
	ldr	x0, [x29, 56]
	mov	w1, -1
	strb	w1, [x0]
	mov	w0, -107
	b	.L394
.L393:
	str	wzr, [x29, 72]
	str	wzr, [x29, 92]
	b	.L395
.L410:
	str	wzr, [x29, 84]
	str	wzr, [x29, 80]
	str	wzr, [x29, 76]
	str	wzr, [x29, 88]
	b	.L396
.L399:
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 92]
	lsr	w0, w1, w0
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L397
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
	b	.L398
.L397:
	ldr	w0, [x29, 80]
	add	w0, w0, 1
	str	w0, [x29, 80]
.L398:
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L396:
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L399
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L400
	ldr	w0, [x29, 92]
	mov	w1, 1
	lsl	w0, w1, w0
	uxtb	w1, w0
	ldrb	w0, [x29, 71]
	orr	w0, w1, w0
	strb	w0, [x29, 71]
	mov	w0, 1
	str	w0, [x29, 76]
.L400:
	ldr	w1, [x29, 80]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L401
	mov	w0, 1
	str	w0, [x29, 76]
.L401:
	ldr	w0, [x29, 76]
	cmp	w0, 0
	bne	.L402
	ldr	w0, [x29, 52]
	cmp	w0, 1
	bne	.L402
	str	wzr, [x29, 84]
	str	wzr, [x29, 80]
	str	wzr, [x29, 76]
	str	wzr, [x29, 88]
	b	.L403
.L406:
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 92]
	lsr	w0, w1, w0
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L404
	ldr	w0, [x29, 80]
	add	w0, w0, 1
	str	w0, [x29, 80]
	b	.L405
.L404:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L405:
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L403:
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L406
	ldr	w1, [x29, 80]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L407
	mov	w0, 1
	str	w0, [x29, 76]
.L407:
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L402
	ldr	w0, [x29, 92]
	mov	w1, 1
	lsl	w0, w1, w0
	uxtb	w1, w0
	ldrb	w0, [x29, 71]
	orr	w0, w1, w0
	strb	w0, [x29, 71]
	mov	w0, 1
	str	w0, [x29, 76]
.L402:
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L408
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
	b	.L409
.L408:
	adrp	x0, .LC73
	add	x0, x0, :lo12:.LC73
	ldr	w3, [x29, 80]
	ldr	w2, [x29, 84]
	ldr	w1, [x29, 92]
	bl	nand_print
	adrp	x0, .LC74
	add	x0, x0, :lo12:.LC74
	ldr	w1, [x29, 92]
	bl	nand_print
.L409:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L395:
	ldr	w0, [x29, 92]
	cmp	w0, 7
	bls	.L410
	ldr	w0, [x29, 72]
	cmp	w0, 8
	bne	.L411
	ldr	x0, [x29, 56]
	ldrb	w1, [x29, 71]
	strb	w1, [x0]
	mov	w0, 0
	b	.L394
.L411:
	ldr	x0, [x29, 56]
	mov	w1, -1
	strb	w1, [x0]
	mov	w0, -80
.L394:
	ldp	x29, x30, [sp], 96
	ret
	.size	hynix16nm_major_check_byte, .-hynix16nm_major_check_byte
	.section	.rodata
	.align	3
.LC75:
	.string	"_get_read_retry_parameters, get rr count failed!\n"
	.align	3
.LC76:
	.string	"rr cnt: %d\n"
	.align	3
.LC77:
	.string	"_get_read_retry_parameters, get rr reg count failed!\n"
	.align	3
.LC78:
	.string	"rr reg cnt: %d\n"
	.align	3
.LC79:
	.string	"read retry value from otp error: rr_cnt %d rr_reg_cnt %d!\n"
	.align	3
.LC80:
	.string	"_get_read_retry_parameters, get the %d-th byte of rr table failed!\n"
	.align	3
.LC81:
	.string	"%02x "
	.text
	.align	2
	.global	hynix16nm_get_read_retry_cfg
	.type	hynix16nm_get_read_retry_cfg, %function
hynix16nm_get_read_retry_cfg:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	x3, [x29, 16]
	str	wzr, [x29, 140]
	str	wzr, [x29, 136]
	add	x0, x29, 88
	stp	xzr, xzr, [x0]
	add	x0, x29, 104
	stp	xzr, xzr, [x0]
	add	x0, x29, 56
	stp	xzr, xzr, [x0]
	add	x0, x29, 72
	stp	xzr, xzr, [x0]
	mov	w0, 32
	str	w0, [x29, 120]
	str	wzr, [x29, 132]
	b	.L413
.L414:
	ldr	w0, [x29, 132]
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 132]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4040]
	ldr	w0, [x29, 132]
	add	w0, w0, 1
	str	w0, [x29, 132]
.L413:
	ldr	w0, [x29, 132]
	cmp	w0, 7
	bls	.L414
	add	x1, x29, 56
	add	x0, x29, 88
	mov	w5, 8
	mov	x4, x1
	mov	x3, x0
	mov	w2, 4
	mov	w1, 0
	ldr	x0, [x29, 40]
	bl	hynix16nm_major_check_byte
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	bge	.L415
	adrp	x0, .LC75
	add	x0, x0, :lo12:.LC75
	bl	nand_print
	ldr	w0, [x29, 136]
	b	.L433
.L415:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC76
	add	x0, x0, :lo12:.LC76
	bl	nand_print_dbg
	str	wzr, [x29, 132]
	b	.L417
.L418:
	ldr	w0, [x29, 132]
	add	w0, w0, 8
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 132]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4040]
	ldr	w0, [x29, 132]
	add	w0, w0, 1
	str	w0, [x29, 132]
.L417:
	ldr	w0, [x29, 132]
	cmp	w0, 7
	bls	.L418
	add	x1, x29, 56
	add	x0, x29, 88
	mov	w5, 8
	mov	x4, x1
	mov	x3, x0
	mov	w2, 4
	mov	w1, 0
	ldr	x0, [x29, 32]
	bl	hynix16nm_major_check_byte
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	bge	.L419
	adrp	x0, .LC77
	add	x0, x0, :lo12:.LC77
	bl	nand_print
	ldr	w0, [x29, 136]
	b	.L433
.L419:
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC78
	add	x0, x0, :lo12:.LC78
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrb	w0, [x0]
	cmp	w0, 8
	bne	.L420
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 4
	beq	.L421
.L420:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC79
	add	x0, x0, :lo12:.LC79
	bl	nand_print
	mov	w0, -81
	b	.L433
.L421:
	str	wzr, [x29, 128]
	b	.L422
.L427:
	str	wzr, [x29, 124]
	b	.L423
.L424:
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	strb	wzr, [x0, 4040]
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	strb	wzr, [x0, 4008]
	ldr	w1, [x29, 124]
	ldr	w0, [x29, 120]
	mul	w0, w1, w0
	add	w0, w0, 8
	lsl	w1, w0, 1
	ldr	w0, [x29, 128]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4040]
	ldr	w1, [x29, 124]
	ldr	w0, [x29, 120]
	mul	w0, w1, w0
	add	w0, w0, 8
	lsl	w1, w0, 1
	ldr	w0, [x29, 120]
	add	w1, w1, w0
	ldr	w0, [x29, 128]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4008]
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L423:
	ldr	w0, [x29, 124]
	cmp	w0, 7
	bls	.L424
	ldr	w0, [x29, 128]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	add	x2, x29, 56
	add	x1, x29, 88
	mov	w5, 8
	mov	x4, x2
	mov	x3, x1
	mov	w2, 4
	mov	w1, 1
	bl	hynix16nm_major_check_byte
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	bge	.L425
	adrp	x0, .LC80
	add	x0, x0, :lo12:.LC80
	ldr	w1, [x29, 128]
	bl	nand_print
	ldr	w0, [x29, 136]
	str	w0, [x29, 140]
	b	.L426
.L425:
	ldr	w0, [x29, 128]
	add	w0, w0, 1
	str	w0, [x29, 128]
.L422:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 120]
	cmp	w1, w0
	bcc	.L427
.L426:
	str	wzr, [x29, 128]
	b	.L428
.L430:
	ldr	w0, [x29, 128]
	and	w0, w0, 7
	cmp	w0, 0
	bne	.L429
	ldr	w0, [x29, 128]
	cmp	w0, 0
	beq	.L429
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
.L429:
	ldr	w0, [x29, 128]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC81
	add	x0, x0, :lo12:.LC81
	bl	nand_print_dbg
	ldr	w0, [x29, 128]
	add	w0, w0, 1
	str	w0, [x29, 128]
.L428:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 120]
	cmp	w1, w0
	bcc	.L430
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
	ldr	w0, [x29, 140]
	cmp	w0, 0
	beq	.L431
	ldr	w0, [x29, 140]
	str	w0, [x29, 136]
	b	.L432
.L431:
	str	wzr, [x29, 136]
.L432:
	ldr	w0, [x29, 136]
.L433:
	ldp	x29, x30, [sp], 144
	ret
	.size	hynix16nm_get_read_retry_cfg, .-hynix16nm_get_read_retry_cfg
	.section	.rodata
	.align	3
.LC82:
	.string	"start get read retry param from: ce %d, rb %d...\n"
	.align	3
.LC83:
	.string	"invalid buffer for otp info!\n"
	.text
	.align	2
	.global	hynix16nm_read_otp_info_hynix
	.type	hynix16nm_read_otp_info_hynix, %function
hynix16nm_read_otp_info_hynix:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 24]
	ldr	w1, [x1, 28]
	uxtw	x1, w1
	add	x1, x1, 316
	ldr	w3, [x0, x1, lsl 2]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 24]
	ldr	w1, [x1, 28]
	uxtw	x1, w1
	add	x1, x1, 324
	ldr	w1, [x0, x1, lsl 2]
	adrp	x0, .LC82
	add	x0, x0, :lo12:.LC82
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x0, [x29, 16]
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	bne	.L435
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	adrp	x0, .LC83
	add	x0, x0, :lo12:.LC83
	bl	nand_print
	mov	w0, -79
	b	.L439
.L435:
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w0, 54
	strb	w0, [x29, 48]
	mov	w0, 56
	strb	w0, [x29, 56]
	mov	w0, 82
	strb	w0, [x29, 40]
	add	x2, x29, 40
	add	x1, x29, 56
	add	x0, x29, 48
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	mov	w1, 22
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 23
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 4
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 25
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 2
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w0, 48
	strb	w0, [x29, 48]
	add	x0, x29, 48
	mov	w4, 528
	ldr	x3, [x29, 72]
	mov	x2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_data_with_nand_bus_one_cmd
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w0, 54
	strb	w0, [x29, 48]
	mov	w0, 56
	strb	w0, [x29, 56]
	strb	wzr, [x29, 40]
	add	x2, x29, 40
	add	x1, x29, 56
	add	x0, x29, 48
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	mov	w1, 22
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w2, 1
	mov	w1, 48
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	ldr	x0, [x29, 72]
	ldrb	w0, [x0]
	cmp	w0, 8
	bne	.L437
	ldr	x0, [x29, 72]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 8
	beq	.L438
.L437:
	ldr	x0, [x29, 72]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 72]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC50
	add	x0, x0, :lo12:.LC50
	bl	nand_print
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, -96
	str	w0, [x29, 68]
	ldr	w0, [x29, 68]
	b	.L439
.L438:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
.L439:
	ldp	x29, x30, [sp], 80
	ret
	.size	hynix16nm_read_otp_info_hynix, .-hynix16nm_read_otp_info_hynix
	.section	.rodata
	.align	3
.LC84:
	.string	"otp_info_hynix_16nm : allocate memory fail\n"
	.align	3
.LC85:
	.string	"m1 CH %d chip %d get otp info failed!\n"
	.align	3
.LC86:
	.string	"m1 CH %d chip %d get read retry cfg from otp info failed!\n"
	.text
	.align	2
	.global	hynix16nm_get_rr_value_otp_hynix
	.type	hynix16nm_get_rr_value_otp_hynix, %function
hynix16nm_get_rr_value_otp_hynix:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 60]
	mov	w0, 528
	bl	nand_get_temp_buf
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	bne	.L441
	adrp	x0, .LC84
	add	x0, x0, :lo12:.LC84
	bl	nand_print
	mov	w0, -78
	b	.L445
.L441:
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	hynix16nm_read_otp_info_hynix
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	bge	.L443
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 28]
	adrp	x0, .LC85
	add	x0, x0, :lo12:.LC85
	bl	nand_print
.L443:
	ldr	x0, [x29, 24]
	add	x2, x0, 56
	add	x1, x29, 46
	add	x0, x29, 47
	ldr	x3, [x29, 48]
	bl	hynix16nm_get_read_retry_cfg
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	bge	.L444
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 28]
	adrp	x0, .LC86
	add	x0, x0, :lo12:.LC86
	bl	nand_print
.L444:
	ldr	x0, [x29, 48]
	bl	nand_free_temp_buf
	ldr	w0, [x29, 60]
.L445:
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_get_rr_value_otp_hynix, .-hynix16nm_get_rr_value_otp_hynix
	.align	2
	.global	hynix16nm_save_otp_value
	.type	hynix16nm_save_otp_value, %function
hynix16nm_save_otp_value:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	str	wzr, [sp, 28]
	b	.L447
.L448:
	ldr	w0, [sp, 28]
	ldr	x1, [sp]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	x1, [sp, 8]
	ldr	w0, [sp, 28]
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 56]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L447:
	ldr	w0, [sp, 28]
	cmp	w0, 31
	bls	.L448
	nop
	add	sp, sp, 32
	ret
	.size	hynix16nm_save_otp_value, .-hynix16nm_save_otp_value
	.align	2
	.global	hynix16nm_get_otp_param
	.type	hynix16nm_get_otp_param, %function
hynix16nm_get_otp_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 52]
	str	wzr, [x29, 48]
	b	.L450
.L452:
	adrp	x0, .LC53
	add	x0, x0, :lo12:.LC53
	ldr	w1, [x29, 52]
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix16nm_get_rr_value_otp_hynix
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L451
	mov	w0, 1
	str	w0, [x29, 48]
.L451:
	ldr	w0, [x29, 52]
	add	w0, w0, 1
	str	w0, [x29, 52]
.L450:
	ldr	w0, [x29, 48]
	cmp	w0, 0
	beq	.L452
	str	wzr, [x29, 60]
	b	.L453
.L456:
	str	wzr, [x29, 56]
	b	.L454
.L455:
	ldr	w0, [x29, 60]
	lsl	w1, w0, 2
	ldr	w0, [x29, 56]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldr	w1, [x29, 60]
	lsl	w2, w1, 2
	ldr	w1, [x29, 56]
	add	w1, w2, w1
	ldr	x2, [x29, 24]
	uxtw	x1, w1
	add	x1, x2, x1
	ldrb	w1, [x1, 56]
	strb	w1, [x0]
	ldr	w0, [x29, 56]
	add	w0, w0, 1
	str	w0, [x29, 56]
.L454:
	ldr	w0, [x29, 56]
	cmp	w0, 3
	bls	.L455
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L453:
	ldr	w0, [x29, 60]
	cmp	w0, 7
	bls	.L456
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_get_otp_param, .-hynix16nm_get_otp_param
	.align	2
	.global	hynix16nm_set_default_param
	.type	hynix16nm_set_default_param, %function
hynix16nm_set_default_param:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L459
.L460:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L459:
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L460
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix16nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix16nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC37
	add	x0, x0, :lo12:.LC37
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L461
.L462:
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC38
	add	x0, x0, :lo12:.LC38
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L461:
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L462
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_set_default_param, .-hynix16nm_set_default_param
	.section	.rodata
	.align	3
.LC87:
	.string	"ch %d, chip %d Read Retry value Table from uboot\n"
	.align	3
.LC88:
	.string	"[PHY_DBG] repair ch %d chip %d otp value end\n"
	.text
	.align	2
	.global	hynix16nm_init_readretry_param
	.type	hynix16nm_init_readretry_param, %function
hynix16nm_init_readretry_param:
	stp	x29, x30, [sp, -176]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 172]
	str	xzr, [x29, 160]
	str	wzr, [x29, 156]
	str	xzr, [x29, 144]
	add	x0, x29, 40
	stp	xzr, xzr, [x0]
	add	x0, x29, 56
	stp	xzr, xzr, [x0]
	str	xzr, [x29, 72]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 20]
	lsl	w0, w0, 9
	bl	nand_get_temp_buf
	str	x0, [x29, 160]
	ldr	x0, [x29, 160]
	cmp	x0, 0
	bne	.L465
	mov	w0, -77
	b	.L473
.L465:
	mov	w0, 32768
	bl	nand_get_temp_buf
	str	x0, [x29, 144]
	ldr	x0, [x29, 144]
	cmp	x0, 0
	bne	.L467
	ldr	x0, [x29, 160]
	bl	nand_free_temp_buf
	adrp	x0, .LC56
	add	x0, x0, :lo12:.LC56
	bl	nand_print
	mov	w0, -76
	b	.L473
.L467:
	str	wzr, [x29, 156]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 24]
	mov	w3, w0
	mov	w2, 32
	ldr	x1, [x29, 160]
	ldr	x0, [x29, 144]
	bl	hynix16nm_get_special_info
	str	w0, [x29, 156]
	ldr	w0, [x29, 156]
	cmp	w0, 0
	bne	.L468
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	w2, [x29, 40]
	adrp	x0, .LC87
	add	x0, x0, :lo12:.LC87
	bl	nand_print_dbg
	str	wzr, [x29, 172]
	b	.L469
.L471:
	ldr	w0, [x29, 172]
	ldr	x1, [x29, 160]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC58
	add	x0, x0, :lo12:.LC58
	bl	nand_print_dbg
	ldr	w0, [x29, 172]
	and	w0, w0, 7
	cmp	w0, 7
	bne	.L470
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print_dbg
.L470:
	ldr	w0, [x29, 172]
	add	w0, w0, 1
	str	w0, [x29, 172]
.L469:
	ldr	w0, [x29, 172]
	cmp	w0, 31
	bls	.L471
	ldr	x1, [x29, 160]
	ldr	x0, [x29, 24]
	bl	hynix16nm_save_otp_value
	ldr	x0, [x29, 24]
	bl	hynix16nm_set_default_param
	b	.L472
.L468:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, .LC59
	add	x0, x0, :lo12:.LC59
	bl	nand_print_dbg
	add	x0, x29, 80
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix16nm_get_otp_param
	ldr	x0, [x29, 24]
	bl	hynix16nm_set_default_param
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	w2, [x29, 40]
	adrp	x0, .LC88
	add	x0, x0, :lo12:.LC88
	bl	nand_print_dbg
	nop
.L472:
	ldr	x0, [x29, 144]
	bl	nand_free_temp_buf
	ldr	x0, [x29, 160]
	bl	nand_free_temp_buf
	mov	w0, 0
.L473:
	ldp	x29, x30, [sp], 176
	ret
	.size	hynix16nm_init_readretry_param, .-hynix16nm_init_readretry_param
	.align	2
	.global	hynix16nm_readretry_init
	.type	hynix16nm_readretry_init, %function
hynix16nm_readretry_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 4
	bne	.L475
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 56
	strb	w1, [x0]
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 57
	strb	w1, [x0, 1]
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 58
	strb	w1, [x0, 2]
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 59
	strb	w1, [x0, 3]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	str	wzr, [x29, 44]
	b	.L478
.L475:
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC40
	add	x0, x0, :lo12:.LC40
	bl	nand_print
	mov	w0, -75
	b	.L477
.L479:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	strb	wzr, [x0, 56]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L478:
	ldr	w0, [x29, 44]
	cmp	w0, 127
	bls	.L479
	ldr	x0, [x29, 24]
	bl	hynix16nm_init_readretry_param
	mov	w0, 0
.L477:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_readretry_init, .-hynix16nm_readretry_init
	.section	.rodata
	.align	3
.LC89:
	.string	"hynix16nm_readretry_exit. \n"
	.text
	.align	2
	.global	hynix16nm_readretry_exit
	.type	hynix16nm_readretry_exit, %function
hynix16nm_readretry_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC89
	add	x0, x0, :lo12:.LC89
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix16nm_set_default_param
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix16nm_readretry_exit, .-hynix16nm_readretry_exit
	.section	.rodata
	.align	3
.LC90:
	.string	" hynix16nm_special_init hynix16nm_read_retry_mode: %d hynix16nm_read_retry_cycle:%d hynix16nm_read_retry_reg_num:%d\n"
	.align	3
.LC91:
	.string	" hynix16nm_special_init error hynix16nm_read_retry_mode: %d hynix16nm_read_retry_cycle:%d hynix16nm_read_retry_reg_num:%d\n"
	.text
	.align	2
	.global	hynix16nm_special_init
	.type	hynix16nm_special_init, %function
hynix16nm_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L483
.L484:
	ldr	x0, [x29, 16]
	bl	hynix16nm_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L483:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L484
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L485
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, hynix16nm_read_page_end
	add	x1, x1, :lo12:hynix16nm_read_page_end
	str	x1, [x0]
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC90
	add	x0, x0, :lo12:.LC90
	bl	nand_print_dbg
	b	.L486
.L485:
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC91
	add	x0, x0, :lo12:.LC91
	bl	nand_print
.L486:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix16nm_special_init, .-hynix16nm_special_init
	.section	.rodata
	.align	3
.LC92:
	.string	" hynix16nm_special_exit \n"
	.text
	.align	2
	.global	hynix16nm_special_exit
	.type	hynix16nm_special_exit, %function
hynix16nm_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L489
.L490:
	ldr	x0, [x29, 24]
	bl	hynix16nm_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L489:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L490
	adrp	x0, .LC92
	add	x0, x0, :lo12:.LC92
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix16nm_special_exit, .-hynix16nm_special_exit
	.global	toshiba_read_retry_mode
	.bss
	.type	toshiba_read_retry_mode, %object
	.size	toshiba_read_retry_mode, 1
toshiba_read_retry_mode:
	.zero	1
	.global	toshiba_read_retry_cycle
	.type	toshiba_read_retry_cycle, %object
	.size	toshiba_read_retry_cycle, 1
toshiba_read_retry_cycle:
	.zero	1
	.global	toshiba_read_retry_reg_cnt
	.type	toshiba_read_retry_reg_cnt, %object
	.size	toshiba_read_retry_reg_cnt, 1
toshiba_read_retry_reg_cnt:
	.zero	1
	.global	toshiba_read_retry_start_cmd
	.data
	.align	3
	.type	toshiba_read_retry_start_cmd, %object
	.size	toshiba_read_retry_start_cmd, 2
toshiba_read_retry_start_cmd:
	.byte	92
	.byte	-59
	.global	toshiba_read_retry_end_cmd
	.align	3
	.type	toshiba_read_retry_end_cmd, %object
	.size	toshiba_read_retry_end_cmd, 2
toshiba_read_retry_end_cmd:
	.byte	38
	.byte	93
	.global	toshiba_read_retry_cmd_1
	.align	3
	.type	toshiba_read_retry_cmd_1, %object
	.size	toshiba_read_retry_cmd_1, 4
toshiba_read_retry_cmd_1:
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.global	toshiba_read_retry_addr_1
	.align	3
	.type	toshiba_read_retry_addr_1, %object
	.size	toshiba_read_retry_addr_1, 4
toshiba_read_retry_addr_1:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.global	toshiba_p1_1
	.align	3
	.type	toshiba_p1_1, %object
	.size	toshiba_p1_1, 28
toshiba_p1_1:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	116
	.byte	116
	.byte	116
	.byte	116
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.global	toshiba_read_retry_cmd_2
	.align	3
	.type	toshiba_read_retry_cmd_2, %object
	.size	toshiba_read_retry_cmd_2, 5
toshiba_read_retry_cmd_2:
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.global	toshiba_read_retry_addr_2
	.align	3
	.type	toshiba_read_retry_addr_2, %object
	.size	toshiba_read_retry_addr_2, 5
toshiba_read_retry_addr_2:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	13
	.global	toshiba_p1_2
	.align	3
	.type	toshiba_p1_2, %object
	.size	toshiba_p1_2, 40
toshiba_p1_2:
	.byte	4
	.byte	4
	.byte	124
	.byte	126
	.byte	0
	.byte	0
	.byte	124
	.byte	120
	.byte	120
	.byte	0
	.byte	124
	.byte	118
	.byte	116
	.byte	114
	.byte	0
	.byte	8
	.byte	8
	.byte	0
	.byte	0
	.byte	0
	.byte	11
	.byte	126
	.byte	118
	.byte	116
	.byte	0
	.byte	16
	.byte	118
	.byte	114
	.byte	112
	.byte	0
	.byte	2
	.byte	0
	.byte	126
	.byte	124
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.global	toshiba_p1_3
	.align	3
	.type	toshiba_p1_3, %object
	.size	toshiba_p1_3, 55
toshiba_p1_3:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	4
	.byte	2
	.byte	0
	.byte	0
	.byte	124
	.byte	0
	.byte	124
	.byte	124
	.byte	0
	.byte	122
	.byte	0
	.byte	122
	.byte	122
	.byte	0
	.byte	120
	.byte	2
	.byte	120
	.byte	122
	.byte	0
	.byte	126
	.byte	4
	.byte	126
	.byte	122
	.byte	0
	.byte	118
	.byte	4
	.byte	118
	.byte	120
	.byte	0
	.byte	4
	.byte	4
	.byte	4
	.byte	118
	.byte	0
	.byte	6
	.byte	10
	.byte	6
	.byte	2
	.byte	0
	.byte	116
	.byte	124
	.byte	116
	.byte	118
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.global	toshiba_read_retry_last_cmd
	.align	3
	.type	toshiba_read_retry_last_cmd, %object
	.size	toshiba_read_retry_last_cmd, 1
toshiba_read_retry_last_cmd:
	.byte	-77
	.global	toshiba_read_retry_exit_cmd
	.align	3
	.type	toshiba_read_retry_exit_cmd, %object
	.size	toshiba_read_retry_exit_cmd, 1
toshiba_read_retry_exit_cmd:
	.byte	-1
	.local	toshiba_toggle_mode_flag
	.comm	toshiba_toggle_mode_flag,4,4
	.local	toshiba_retry_flag
	.comm	toshiba_retry_flag,4,4
	.local	toshiba_sclk0_bak
	.comm	toshiba_sclk0_bak,4,4
	.local	toshiba_sclk1_bak
	.comm	toshiba_sclk1_bak,4,4
	.text
	.align	2
	.global	toshiba_read_retry_clock_save
	.type	toshiba_read_retry_clock_save, %function
toshiba_read_retry_clock_save:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, toshiba_sclk1_bak
	add	x3, x0, :lo12:toshiba_sclk1_bak
	adrp	x0, toshiba_sclk0_bak
	add	x2, x0, :lo12:toshiba_sclk0_bak
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_get_clk
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	mov	w3, 20
	mov	w2, 10
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_read_retry_clock_save, .-toshiba_read_retry_clock_save
	.align	2
	.global	toshiba_read_retry_clock_recover
	.type	toshiba_read_retry_clock_recover, %function
toshiba_read_retry_clock_recover:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, toshiba_sclk0_bak
	add	x0, x0, :lo12:toshiba_sclk0_bak
	ldr	w2, [x0]
	adrp	x0, toshiba_sclk1_bak
	add	x0, x0, :lo12:toshiba_sclk1_bak
	ldr	w3, [x0]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_read_retry_clock_recover, .-toshiba_read_retry_clock_recover
	.section	.rodata
	.align	3
.LC93:
	.string	"toshiba rr value 3 %x %x %x %x!\n"
	.text
	.align	2
	.global	toshiba_vender_set_param
	.type	toshiba_vender_set_param, %function
toshiba_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 60]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	bne	.L497
	adrp	x0, toshiba_read_retry_cmd_2
	add	x0, x0, :lo12:toshiba_read_retry_cmd_2
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L497
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L497
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L497
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L497
	adrp	x0, toshiba_retry_flag
	add	x0, x0, :lo12:toshiba_retry_flag
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L497
	adrp	x0, toshiba_read_retry_exit_cmd
	add	x0, x0, :lo12:toshiba_read_retry_exit_cmd
	ldrb	w0, [x0]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	b	.L498
.L497:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	bne	.L499
	adrp	x0, toshiba_read_retry_cmd_2
	add	x0, x0, :lo12:toshiba_read_retry_cmd_2
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L500
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L500
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L500
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L500
	adrp	x0, toshiba_read_retry_exit_cmd
	add	x0, x0, :lo12:toshiba_read_retry_exit_cmd
	ldrb	w0, [x0]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	b	.L498
.L500:
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L499
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L499
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 126
	bne	.L499
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 124
	bne	.L499
	adrp	x0, toshiba_read_retry_last_cmd
	add	x0, x0, :lo12:toshiba_read_retry_last_cmd
	ldrb	w0, [x0]
	mov	w2, 0
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
.L499:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 16
	bne	.L501
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L502
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L502
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L502
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L502
	adrp	x0, toshiba_read_retry_exit_cmd
	add	x0, x0, :lo12:toshiba_read_retry_exit_cmd
	ldrb	w0, [x0]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	b	.L498
.L502:
	adrp	x0, toshiba_read_retry_cmd_1
	add	x0, x0, :lo12:toshiba_read_retry_cmd_1
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
.L501:
	adrp	x0, toshiba_read_retry_end_cmd
	add	x0, x0, :lo12:toshiba_read_retry_end_cmd
	ldrb	w0, [x0]
	mov	w2, 0
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	adrp	x0, toshiba_read_retry_end_cmd
	add	x0, x0, :lo12:toshiba_read_retry_end_cmd
	ldrb	w0, [x0, 1]
	mov	w2, 0
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC93
	add	x0, x0, :lo12:.LC93
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
.L498:
	ldp	x29, x30, [sp], 64
	ret
	.size	toshiba_vender_set_param, .-toshiba_vender_set_param
	.section	.rodata
	.align	3
.LC94:
	.string	"toshiba vender_pre_condition error!\n"
	.text
	.align	2
	.global	toshiba_vender_pre_condition
	.type	toshiba_vender_pre_condition, %function
toshiba_vender_pre_condition:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, toshiba_read_retry_start_cmd
	add	x0, x0, :lo12:toshiba_read_retry_start_cmd
	mov	w6, 2
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L504
	adrp	x0, .LC94
	add	x0, x0, :lo12:.LC94
	bl	nand_print
	ldr	w0, [x29, 44]
	b	.L505
.L504:
	mov	w0, 0
.L505:
	ldp	x29, x30, [sp], 48
	ret
	.size	toshiba_vender_pre_condition, .-toshiba_vender_pre_condition
	.align	2
	.global	toshiba_set_readretry
	.type	toshiba_set_readretry, %function
toshiba_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 16
	beq	.L507
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	beq	.L507
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	beq	.L507
	mov	w0, -89
	b	.L508
.L507:
	ldr	x0, [x29, 24]
	bl	toshiba_read_retry_clock_save
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 0
	bne	.L509
	ldr	x0, [x29, 24]
	bl	toshiba_vender_pre_condition
.L509:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L510
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L510:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	bne	.L511
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	adrp	x1, toshiba_p1_2
	add	x1, x1, :lo12:toshiba_p1_2
	add	x0, x0, x1
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L512
.L511:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	bne	.L513
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	adrp	x1, toshiba_p1_3
	add	x1, x1, :lo12:toshiba_p1_3
	add	x0, x0, x1
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L512
.L513:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, toshiba_p1_1
	add	x0, x0, :lo12:toshiba_p1_1
	add	x0, x1, x0
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_1
	add	x0, x0, :lo12:toshiba_read_retry_addr_1
	str	x0, [x29, 48]
	mov	w0, 4
	str	w0, [x29, 44]
.L512:
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	toshiba_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L514
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L514:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	x0, [x29, 24]
	bl	toshiba_read_retry_clock_recover
	ldr	w0, [x29, 40]
.L508:
	ldp	x29, x30, [sp], 64
	ret
	.size	toshiba_set_readretry, .-toshiba_set_readretry
	.section	.rodata
	.align	3
.LC95:
	.string	"toshiba retry!\n"
	.align	3
.LC96:
	.string	"toshiba ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	toshiba_read_page_end
	.type	toshiba_read_page_end, %function
toshiba_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L516
	adrp	x0, .LC95
	add	x0, x0, :lo12:.LC95
	bl	nand_print_dbg
	adrp	x0, toshiba_retry_flag
	add	x0, x0, :lo12:toshiba_retry_flag
	mov	w1, 1
	str	w1, [x0]
	str	wzr, [x29, 44]
	b	.L517
.L522:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	bl	toshiba_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L524
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L520
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L519
.L520:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC96
	add	x0, x0, :lo12:.LC96
	bl	nand_print_dbg
	b	.L521
.L524:
	nop
.L519:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L517:
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L522
.L521:
	adrp	x0, toshiba_retry_flag
	add	x0, x0, :lo12:toshiba_retry_flag
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	toshiba_exit_readretry
.L516:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	toshiba_read_page_end, .-toshiba_read_page_end
	.section	.rodata
	.align	3
.LC97:
	.string	"toshiba read retry conut: %d !\n"
	.text
	.align	2
	.global	toshiba_readretry_init
	.type	toshiba_readretry_init, %function
toshiba_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, toshiba_read_retry_reg_cnt
	add	x0, x0, :lo12:toshiba_read_retry_reg_cnt
	strb	w1, [x0]
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC97
	add	x0, x0, :lo12:.LC97
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_readretry_init, .-toshiba_readretry_init
	.align	2
	.global	toshiba_readretry_exit
	.type	toshiba_readretry_exit, %function
toshiba_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	toshiba_readretry_exit, .-toshiba_readretry_exit
	.align	2
	.global	toshiba_exit_readretry
	.type	toshiba_exit_readretry, %function
toshiba_exit_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 16
	beq	.L530
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	beq	.L530
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	beq	.L530
	mov	w0, -88
	b	.L531
.L530:
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L532
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L532:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	bne	.L533
	adrp	x0, toshiba_p1_2+35
	add	x0, x0, :lo12:toshiba_p1_2+35
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L534
.L533:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	bne	.L535
	adrp	x0, toshiba_p1_3+50
	add	x0, x0, :lo12:toshiba_p1_3+50
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L534
.L535:
	adrp	x0, toshiba_p1_1+24
	add	x0, x0, :lo12:toshiba_p1_1+24
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_1
	add	x0, x0, :lo12:toshiba_read_retry_addr_1
	str	x0, [x29, 48]
	str	wzr, [x29, 44]
.L534:
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	toshiba_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L536
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L536:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 40]
.L531:
	ldp	x29, x30, [sp], 64
	ret
	.size	toshiba_exit_readretry, .-toshiba_exit_readretry
	.section	.rodata
	.align	3
.LC98:
	.string	" toshiba_special_init toshiba_read_retry_mode :%d toshiba_read_retry_cycle :%d toshiba_read_retry_reg_cnt :%d \n"
	.align	3
.LC99:
	.string	" toshiba_special_init error toshiba_read_retry_mode :%d toshiba_read_retry_cycle :%d toshiba_read_retry_reg_cnt :%d \n"
	.text
	.align	2
	.global	toshiba_special_init
	.type	toshiba_special_init, %function
toshiba_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L538
.L539:
	ldr	x0, [x29, 16]
	bl	toshiba_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L538:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L539
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L540
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, toshiba_read_page_end
	add	x1, x1, :lo12:toshiba_read_page_end
	str	x1, [x0]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, toshiba_read_retry_reg_cnt
	add	x0, x0, :lo12:toshiba_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC98
	add	x0, x0, :lo12:.LC98
	bl	nand_print_dbg
	b	.L541
.L540:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, toshiba_read_retry_reg_cnt
	add	x0, x0, :lo12:toshiba_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC99
	add	x0, x0, :lo12:.LC99
	bl	nand_print
.L541:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_special_init, .-toshiba_special_init
	.section	.rodata
	.align	3
.LC100:
	.string	" toshiba_special_exit \n"
	.text
	.align	2
	.global	toshiba_special_exit
	.type	toshiba_special_exit, %function
toshiba_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L544
.L545:
	ldr	x0, [x29, 24]
	bl	toshiba_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L544:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L545
	adrp	x0, .LC100
	add	x0, x0, :lo12:.LC100
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_special_exit, .-toshiba_special_exit
	.global	sandisk_A19_read_retry_mode
	.bss
	.type	sandisk_A19_read_retry_mode, %object
	.size	sandisk_A19_read_retry_mode, 1
sandisk_A19_read_retry_mode:
	.zero	1
	.global	sandisk_A19_read_retry_cycle
	.type	sandisk_A19_read_retry_cycle, %object
	.size	sandisk_A19_read_retry_cycle, 1
sandisk_A19_read_retry_cycle:
	.zero	1
	.global	sandisk_A19_read_retry_reg_cnt
	.type	sandisk_A19_read_retry_reg_cnt, %object
	.size	sandisk_A19_read_retry_reg_cnt, 1
sandisk_A19_read_retry_reg_cnt:
	.zero	1
	.global	sandisk_A19_read_retry_enable_cmd_1y
	.data
	.align	3
	.type	sandisk_A19_read_retry_enable_cmd_1y, %object
	.size	sandisk_A19_read_retry_enable_cmd_1y, 1
sandisk_A19_read_retry_enable_cmd_1y:
	.byte	93
	.global	sandisk_A19_read_retry_exit_cmd_1y
	.align	3
	.type	sandisk_A19_read_retry_exit_cmd_1y, %object
	.size	sandisk_A19_read_retry_exit_cmd_1y, 1
sandisk_A19_read_retry_exit_cmd_1y:
	.byte	-1
	.global	sandisk_A19_read_retry_cmd_1y
	.align	3
	.type	sandisk_A19_read_retry_cmd_1y, %object
	.size	sandisk_A19_read_retry_cmd_1y, 1
sandisk_A19_read_retry_cmd_1y:
	.byte	-17
	.global	sandisk_A19_read_retry_reg_adr_1y
	.align	3
	.type	sandisk_A19_read_retry_reg_adr_1y, %object
	.size	sandisk_A19_read_retry_reg_adr_1y, 1
sandisk_A19_read_retry_reg_adr_1y:
	.byte	17
	.global	sandisk_A19_1y
	.align	3
	.type	sandisk_A19_1y, %object
	.size	sandisk_A19_1y, 124
sandisk_A19_1y:
	.byte	124
	.byte	0
	.byte	0
	.byte	124
	.byte	4
	.byte	0
	.byte	124
	.byte	120
	.byte	120
	.byte	0
	.byte	120
	.byte	116
	.byte	8
	.byte	124
	.byte	0
	.byte	124
	.byte	0
	.byte	124
	.byte	124
	.byte	120
	.byte	124
	.byte	124
	.byte	120
	.byte	116
	.byte	0
	.byte	124
	.byte	116
	.byte	112
	.byte	0
	.byte	120
	.byte	0
	.byte	124
	.byte	0
	.byte	120
	.byte	124
	.byte	120
	.byte	0
	.byte	120
	.byte	120
	.byte	116
	.byte	0
	.byte	120
	.byte	116
	.byte	112
	.byte	0
	.byte	120
	.byte	112
	.byte	108
	.byte	0
	.byte	4
	.byte	4
	.byte	0
	.byte	0
	.byte	4
	.byte	0
	.byte	124
	.byte	12
	.byte	4
	.byte	124
	.byte	120
	.byte	12
	.byte	4
	.byte	120
	.byte	116
	.byte	16
	.byte	8
	.byte	0
	.byte	124
	.byte	16
	.byte	8
	.byte	4
	.byte	0
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	116
	.byte	112
	.byte	120
	.byte	116
	.byte	112
	.byte	108
	.byte	120
	.byte	116
	.byte	108
	.byte	104
	.byte	120
	.byte	112
	.byte	120
	.byte	116
	.byte	120
	.byte	112
	.byte	116
	.byte	112
	.byte	120
	.byte	112
	.byte	108
	.byte	104
	.byte	120
	.byte	112
	.byte	112
	.byte	108
	.byte	120
	.byte	108
	.byte	112
	.byte	108
	.byte	120
	.byte	108
	.byte	108
	.byte	104
	.byte	120
	.byte	108
	.byte	104
	.byte	100
	.byte	116
	.byte	104
	.byte	108
	.byte	104
	.byte	116
	.byte	104
	.byte	104
	.byte	100
	.global	sandisk_A19_1z_16g
	.align	3
	.type	sandisk_A19_1z_16g, %object
	.size	sandisk_A19_1z_16g, 128
sandisk_A19_1z_16g:
	.byte	124
	.byte	0
	.byte	4
	.byte	0
	.byte	4
	.byte	0
	.byte	0
	.byte	124
	.byte	120
	.byte	0
	.byte	8
	.byte	4
	.byte	8
	.byte	0
	.byte	124
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	0
	.byte	0
	.byte	124
	.byte	0
	.byte	124
	.byte	120
	.byte	124
	.byte	8
	.byte	4
	.byte	4
	.byte	124
	.byte	116
	.byte	120
	.byte	4
	.byte	4
	.byte	4
	.byte	0
	.byte	8
	.byte	4
	.byte	0
	.byte	124
	.byte	0
	.byte	4
	.byte	8
	.byte	4
	.byte	12
	.byte	4
	.byte	124
	.byte	120
	.byte	124
	.byte	4
	.byte	12
	.byte	8
	.byte	120
	.byte	120
	.byte	4
	.byte	0
	.byte	124
	.byte	120
	.byte	0
	.byte	124
	.byte	116
	.byte	120
	.byte	112
	.byte	120
	.byte	8
	.byte	8
	.byte	4
	.byte	0
	.byte	4
	.byte	8
	.byte	8
	.byte	4
	.byte	12
	.byte	8
	.byte	12
	.byte	8
	.byte	0
	.byte	0
	.byte	12
	.byte	8
	.byte	4
	.byte	0
	.byte	120
	.byte	116
	.byte	124
	.byte	124
	.byte	120
	.byte	116
	.byte	120
	.byte	120
	.byte	120
	.byte	116
	.byte	8
	.byte	8
	.byte	0
	.byte	124
	.byte	116
	.byte	116
	.byte	0
	.byte	124
	.byte	120
	.byte	116
	.byte	124
	.byte	120
	.byte	112
	.byte	116
	.byte	116
	.byte	116
	.byte	124
	.byte	116
	.byte	112
	.byte	112
	.byte	12
	.byte	12
	.byte	8
	.byte	4
	.byte	16
	.byte	12
	.byte	12
	.byte	8
	.byte	112
	.byte	112
	.byte	112
	.byte	116
	.byte	116
	.byte	112
	.byte	112
	.byte	112
	.global	sandisk_A19_1z_8g
	.align	3
	.type	sandisk_A19_1z_8g, %object
	.size	sandisk_A19_1z_8g, 124
sandisk_A19_1z_8g:
	.byte	4
	.byte	4
	.byte	120
	.byte	120
	.byte	4
	.byte	4
	.byte	124
	.byte	116
	.byte	0
	.byte	0
	.byte	4
	.byte	120
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	0
	.byte	0
	.byte	0
	.byte	124
	.byte	0
	.byte	0
	.byte	0
	.byte	116
	.byte	8
	.byte	8
	.byte	4
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	120
	.byte	124
	.byte	124
	.byte	0
	.byte	124
	.byte	4
	.byte	4
	.byte	124
	.byte	112
	.byte	124
	.byte	124
	.byte	116
	.byte	116
	.byte	0
	.byte	0
	.byte	120
	.byte	112
	.byte	12
	.byte	12
	.byte	8
	.byte	120
	.byte	120
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	4
	.byte	8
	.byte	4
	.byte	120
	.byte	120
	.byte	8
	.byte	120
	.byte	124
	.byte	124
	.byte	120
	.byte	112
	.byte	120
	.byte	120
	.byte	112
	.byte	108
	.byte	0
	.byte	0
	.byte	116
	.byte	108
	.byte	8
	.byte	8
	.byte	0
	.byte	116
	.byte	124
	.byte	124
	.byte	120
	.byte	108
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	116
	.byte	116
	.byte	116
	.byte	108
	.byte	120
	.byte	120
	.byte	124
	.byte	112
	.byte	12
	.byte	12
	.byte	0
	.byte	116
	.byte	4
	.byte	4
	.byte	12
	.byte	8
	.byte	120
	.byte	120
	.byte	124
	.byte	116
	.byte	120
	.byte	120
	.byte	112
	.byte	104
	.byte	8
	.byte	8
	.byte	0
	.byte	112
	.byte	16
	.byte	16
	.byte	12
	.byte	120
	.byte	0
	.byte	0
	.byte	12
	.byte	8
	.local	sandisk_A19_sclk0_bak
	.comm	sandisk_A19_sclk0_bak,4,4
	.local	sandisk_A19_sclk1_bak
	.comm	sandisk_A19_sclk1_bak,4,4
	.text
	.align	2
	.global	sandisk_A19_dsp_on
	.type	sandisk_A19_dsp_on, %function
sandisk_A19_dsp_on:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 38
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_dsp_on, .-sandisk_A19_dsp_on
	.align	2
	.global	sandisk_A19_cmd25
	.type	sandisk_A19_cmd25, %function
sandisk_A19_cmd25:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 37
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_cmd25, .-sandisk_A19_cmd25
	.align	2
	.global	sandisk_A19_set_lmflgfix_next
	.type	sandisk_A19_set_lmflgfix_next, %function
sandisk_A19_set_lmflgfix_next:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strb	w1, [x29, 23]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 92
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, -59
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	strb	wzr, [x29, 39]
	mov	w0, 1
	strb	w0, [x29, 38]
	mov	w0, 85
	strb	w0, [x29, 37]
	add	x2, x29, 38
	add	x1, x29, 39
	add	x0, x29, 37
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L552
	mov	w0, 34
	strb	w0, [x29, 39]
	b	.L553
.L552:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L554
	mov	w0, 37
	strb	w0, [x29, 39]
	b	.L553
.L554:
	mov	w0, 35
	strb	w0, [x29, 39]
.L553:
	ldrb	w0, [x29, 23]
	strb	w0, [x29, 38]
	mov	w0, 85
	strb	w0, [x29, 37]
	add	x2, x29, 38
	add	x1, x29, 39
	add	x0, x29, 37
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	strb	wzr, [x29, 39]
	strb	wzr, [x29, 38]
	mov	w0, 85
	strb	w0, [x29, 37]
	add	x2, x29, 38
	add	x1, x29, 39
	add	x0, x29, 37
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_set_lmflgfix_next, .-sandisk_A19_set_lmflgfix_next
	.section	.rodata
	.align	3
.LC101:
	.string	"sandisk_A19 retry!\n"
	.align	3
.LC102:
	.string	"sandisk_A19 set readretry error\n"
	.align	3
.LC103:
	.string	"sandisk_A19 ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.align	3
.LC104:
	.string	"sandisk_A19 ReadRetry ok with ecc limit! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	sandisk_A19_read_page_end
	.type	sandisk_A19_read_page_end, %function
sandisk_A19_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L557
	adrp	x0, .LC101
	add	x0, x0, :lo12:.LC101
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L558
.L566:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L559
	adrp	x0, .LC102
	add	x0, x0, :lo12:.LC102
	bl	nand_print_dbg
	b	.L560
.L559:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L561
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC22
	add	x0, x0, :lo12:.LC22
	bl	nand_print_dbg
	b	.L560
.L561:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L562
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L560
.L562:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L563
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC103
	add	x0, x0, :lo12:.LC103
	bl	nand_print_dbg
	b	.L564
.L563:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC104
	add	x0, x0, :lo12:.LC104
	bl	nand_print_dbg
.L564:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L565
.L560:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L558:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L566
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 51
	beq	.L602
	str	wzr, [x29, 44]
	b	.L569
.L576:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L570
	adrp	x0, .LC102
	add	x0, x0, :lo12:.LC102
	bl	nand_print_dbg
	b	.L571
.L570:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_dsp_on
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L572
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC22
	add	x0, x0, :lo12:.LC22
	bl	nand_print_dbg
	b	.L571
.L572:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L573
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L571
.L573:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L574
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC103
	add	x0, x0, :lo12:.LC103
	bl	nand_print_dbg
	b	.L575
.L574:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC104
	add	x0, x0, :lo12:.LC104
	bl	nand_print_dbg
.L575:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L565
.L571:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L569:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L576
	str	wzr, [x29, 44]
	b	.L577
.L584:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L578
	adrp	x0, .LC102
	add	x0, x0, :lo12:.LC102
	bl	nand_print_dbg
	b	.L579
.L578:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_cmd25
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L580
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC22
	add	x0, x0, :lo12:.LC22
	bl	nand_print_dbg
	b	.L579
.L580:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L581
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L579
.L581:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L582
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC103
	add	x0, x0, :lo12:.LC103
	bl	nand_print_dbg
	b	.L583
.L582:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC104
	add	x0, x0, :lo12:.LC104
	bl	nand_print_dbg
.L583:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L565
.L579:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L577:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L584
	b	.L568
.L602:
	nop
.L568:
	str	wzr, [x29, 44]
	b	.L585
.L600:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L586
	adrp	x0, .LC102
	add	x0, x0, :lo12:.LC102
	bl	nand_print_dbg
	b	.L587
.L586:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L588
	mov	w1, -58
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L589
.L588:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L590
	mov	w1, 26
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L589
.L590:
	mov	w1, -64
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
.L589:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_cmd25
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	beq	.L591
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	beq	.L591
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L592
.L591:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_dsp_on
.L592:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L593
	mov	w1, -62
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L594
.L593:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L595
	mov	w1, 10
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L594
.L595:
	mov	w1, 64
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
.L594:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L596
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC22
	add	x0, x0, :lo12:.LC22
	bl	nand_print_dbg
	b	.L587
.L596:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L597
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L587
.L597:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L598
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC103
	add	x0, x0, :lo12:.LC103
	bl	nand_print_dbg
	b	.L599
.L598:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC104
	add	x0, x0, :lo12:.LC104
	bl	nand_print_dbg
.L599:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L565
.L587:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L585:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L600
.L565:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	sandisk_A19_exit_readretry
.L557:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_read_page_end, .-sandisk_A19_read_page_end
	.section	.rodata
	.align	3
.LC105:
	.string	"sandisk_A19 retry mode err : %d !\n"
	.align	3
.LC106:
	.string	"sandisk A19 read retry conut: %d !\n"
	.text
	.align	2
	.global	sandisk_A19_readretry_init
	.type	sandisk_A19_readretry_init, %function
sandisk_A19_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, sandisk_A19_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_cnt
	strb	w1, [x0]
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	beq	.L604
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 51
	beq	.L604
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	beq	.L604
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	beq	.L604
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC105
	add	x0, x0, :lo12:.LC105
	bl	nand_print
	mov	w0, -99
	b	.L605
.L604:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC106
	add	x0, x0, :lo12:.LC106
	bl	nand_print_dbg
	mov	w0, 0
.L605:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_readretry_init, .-sandisk_A19_readretry_init
	.align	2
	.global	sandisk_A19_readretry_exit
	.type	sandisk_A19_readretry_exit, %function
sandisk_A19_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	sandisk_A19_readretry_exit, .-sandisk_A19_readretry_exit
	.align	2
	.global	sandisk_A19_read_retry_clock_save
	.type	sandisk_A19_read_retry_clock_save, %function
sandisk_A19_read_retry_clock_save:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_A19_sclk1_bak
	add	x3, x0, :lo12:sandisk_A19_sclk1_bak
	adrp	x0, sandisk_A19_sclk0_bak
	add	x2, x0, :lo12:sandisk_A19_sclk0_bak
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_get_clk
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	mov	w3, 20
	mov	w2, 10
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_read_retry_clock_save, .-sandisk_A19_read_retry_clock_save
	.align	2
	.global	sandisk_A19_read_retry_clock_recover
	.type	sandisk_A19_read_retry_clock_recover, %function
sandisk_A19_read_retry_clock_recover:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_A19_sclk0_bak
	add	x0, x0, :lo12:sandisk_A19_sclk0_bak
	ldr	w2, [x0]
	adrp	x0, sandisk_A19_sclk1_bak
	add	x0, x0, :lo12:sandisk_A19_sclk1_bak
	ldr	w3, [x0]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_read_retry_clock_recover, .-sandisk_A19_read_retry_clock_recover
	.align	2
	.global	sandisk_A19_vender_set_param
	.type	sandisk_A19_vender_set_param, %function
sandisk_A19_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 60]
	adrp	x0, sandisk_A19_read_retry_cmd_1y
	add	x0, x0, :lo12:sandisk_A19_read_retry_cmd_1y
	mov	w6, 1
	mov	w5, 4
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_A19_vender_set_param, .-sandisk_A19_vender_set_param
	.align	2
	.global	sandisk_A19_set_readretry
	.type	sandisk_A19_set_readretry, %function
sandisk_A19_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	wzr, [x29, 52]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L615
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_A19_1z_16g
	add	x0, x0, :lo12:sandisk_A19_1z_16g
	add	x0, x1, x0
	str	x0, [x29, 56]
	b	.L616
.L615:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L617
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_A19_1z_8g
	add	x0, x0, :lo12:sandisk_A19_1z_8g
	add	x0, x1, x0
	str	x0, [x29, 56]
	b	.L616
.L617:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_A19_1y
	add	x0, x0, :lo12:sandisk_A19_1y
	add	x0, x1, x0
	str	x0, [x29, 56]
.L616:
	adrp	x0, sandisk_A19_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_adr_1y
	str	x0, [x29, 40]
	mov	w0, 1
	str	w0, [x29, 36]
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	sandisk_A19_vender_set_param
	str	w0, [x29, 52]
	mov	w2, 0
	mov	w1, 93
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 52]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_A19_set_readretry, .-sandisk_A19_set_readretry
	.align	2
	.global	sandisk_A19_exit_readretry
	.type	sandisk_A19_exit_readretry, %function
sandisk_A19_exit_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 60]
	strb	wzr, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 42]
	strb	wzr, [x29, 43]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	adrp	x0, sandisk_A19_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_adr_1y
	str	x0, [x29, 48]
	mov	w0, 1
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_A19_vender_set_param
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_A19_exit_readretry, .-sandisk_A19_exit_readretry
	.section	.rodata
	.align	3
.LC107:
	.string	" sandisk_A19_special_init sandisk_A19_read_retry_mode:%d sandisk_A19_read_retry_cycle :%d sandisk_A19_read_retry_reg_cnt %d \n"
	.align	3
.LC108:
	.string	" sandisk_A19_special_init error sandisk_A19_read_retry_mode:%d sandisk_A19_read_retry_cycle :%d sandisk_A19_read_retry_reg_cnt %d \n"
	.text
	.align	2
	.global	sandisk_A19_special_init
	.type	sandisk_A19_special_init, %function
sandisk_A19_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L622
.L623:
	ldr	x0, [x29, 16]
	bl	sandisk_A19_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L622:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L623
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L624
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, sandisk_A19_read_page_end
	add	x1, x1, :lo12:sandisk_A19_read_page_end
	str	x1, [x0]
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_A19_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC107
	add	x0, x0, :lo12:.LC107
	bl	nand_print_dbg
	b	.L625
.L624:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_A19_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC108
	add	x0, x0, :lo12:.LC108
	bl	nand_print
.L625:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_special_init, .-sandisk_A19_special_init
	.section	.rodata
	.align	3
.LC109:
	.string	"%s sandisk toggle vendor specific setting failed!\n"
	.align	3
.LC110:
	.string	" sandisk_A19_special_exit \n"
	.text
	.align	2
	.global	sandisk_A19_special_exit
	.type	sandisk_A19_special_exit, %function
sandisk_A19_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L628
.L631:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_readretry_exit
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 262]
	cmp	w0, 0
	beq	.L629
	ldr	x0, [x29, 24]
	str	wzr, [x0, 204]
	ldr	x0, [x29, 24]
	bl	sandisk_toggle_vendor_specific_setting
	str	w0, [x29, 20]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L629
	adrp	x0, __func__.6341
	add	x1, x0, :lo12:__func__.6341
	adrp	x0, .LC109
	add	x0, x0, :lo12:.LC109
	bl	nand_print
	ldr	w0, [x29, 20]
	b	.L630
.L629:
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L628:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L631
	adrp	x0, .LC110
	add	x0, x0, :lo12:.LC110
	bl	nand_print_dbg
	mov	w0, 0
.L630:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_special_exit, .-sandisk_A19_special_exit
	.align	2
	.global	sandisk_A19_check_bad_block_first_burn
	.type	sandisk_A19_check_bad_block_first_burn, %function
sandisk_A19_check_bad_block_first_burn:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 76]
	str	wzr, [x29, 72]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	add	x0, x0, 40
	str	x0, [x29, 48]
	ldr	x0, [x29, 64]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 16]
	cmp	w1, w0
	bcs	.L633
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 64]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bcc	.L634
.L633:
	ldr	x0, [x29, 56]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 64]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	w3, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 4]
	ldr	x0, [x29, 64]
	ldr	w5, [x0, 32]
	adrp	x0, .LC70
	add	x0, x0, :lo12:.LC70
	bl	nand_print
	mov	w0, -10
	b	.L635
.L634:
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 64]
	bl	nand_read_chip_status_ready
	ldr	x0, [x29, 64]
	bl	nand_enable_chip
	ldr	x0, [x29, 48]
	bl	ndfc_clean_cmd_seq
	ldr	x0, [x29, 48]
	str	wzr, [x0]
	ldr	x0, [x29, 48]
	mov	w1, 162
	str	w1, [x0, 12]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 48]
	str	wzr, [x0, 20]
	ldr	x0, [x29, 48]
	mov	w1, 128
	str	w1, [x0, 84]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 80]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 88]
	ldr	x0, [x29, 48]
	str	wzr, [x0, 92]
	ldr	x0, [x29, 64]
	ldr	w3, [x0, 44]
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	mov	w2, w0
	mov	w0, w3
	bl	get_row_addr
	str	w0, [x29, 76]
	ldr	x0, [x29, 48]
	mov	w1, 5
	str	w1, [x0, 112]
	ldr	x0, [x29, 48]
	add	x0, x0, 96
	mov	x4, x0
	mov	w3, 3
	ldr	w2, [x29, 76]
	mov	w1, 2
	ldr	w0, [x29, 72]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	mov	w1, 16
	str	w1, [x0, 156]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 152]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 160]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 164]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 48]
	bl	ndfc_execute_cmd
	str	w0, [x29, 44]
	ldr	x0, [x29, 64]
	bl	nand_read_chip_status_ready
	str	w0, [x29, 44]
	ldr	x0, [x29, 64]
	bl	nand_disable_chip
	ldr	w0, [x29, 44]
.L635:
	ldp	x29, x30, [sp], 80
	ret
	.size	sandisk_A19_check_bad_block_first_burn, .-sandisk_A19_check_bad_block_first_burn
	.global	phyinfo
	.bss
	.align	3
	.type	phyinfo, %object
	.size	phyinfo, 20
phyinfo:
	.zero	20
	.section	.rodata
	.align	3
.LC111:
	.string	"[NE]malloc size too large %d!\n"
	.text
	.align	2
	.global	nftl_malloc
	.type	nftl_malloc, %function
nftl_malloc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	ldr	w0, [x29, 28]
	cmp	w0, 1572864
	bls	.L637
	adrp	x0, .LC111
	add	x0, x0, :lo12:.LC111
	ldr	w1, [x29, 28]
	bl	nand_print
.L637:
	ldr	w0, [x29, 28]
	bl	nand_malloc
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_malloc, .-nftl_malloc
	.align	2
	.global	nftl_free
	.type	nftl_free, %function
nftl_free:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	nand_free
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_free, .-nftl_free
	.align	2
	.global	add_nftl_blk_list
	.type	add_nftl_blk_list, %function
add_nftl_blk_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	b	.L641
.L642:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L641:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L642
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	nop
	add	sp, sp, 32
	ret
	.size	add_nftl_blk_list, .-add_nftl_blk_list
	.align	2
	.global	del_last_nftl_blk
	.type	del_last_nftl_blk, %function
del_last_nftl_blk:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	xzr, [sp, 16]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	b	.L644
.L647:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 16]
	ldr	x0, [sp, 16]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L645
	ldr	x0, [sp, 24]
	str	xzr, [x0, 16]
	ldr	x0, [sp, 16]
	b	.L646
.L645:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L644:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L647
	mov	x0, 0
.L646:
	add	sp, sp, 32
	ret
	.size	del_last_nftl_blk, .-del_last_nftl_blk
	.align	2
	.global	nftl_support_boot_check_crc_from_dts
	.type	nftl_support_boot_check_crc_from_dts, %function
nftl_support_boot_check_crc_from_dts:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	bl	nand_get_support_boot_check_crc
	cmp	w0, 0
	cset	w0, ne
	uxtb	w0, w0
	ldp	x29, x30, [sp], 16
	ret
	.size	nftl_support_boot_check_crc_from_dts, .-nftl_support_boot_check_crc_from_dts
	.section	.rodata
	.align	3
.LC112:
	.string	"16:33"
	.align	3
.LC113:
	.string	"2021.01.21"
	.align	3
.LC114:
	.string	"001"
	.align	3
.LC115:
	.string	"0"
	.align	3
.LC116:
	.string	"5"
	.align	3
.LC117:
	.string	"sunxi nftl version %s.%s.%s  %s-%s\n"
	.text
	.align	2
	.type	nftl_version, %function
nftl_version:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, .LC112
	add	x5, x0, :lo12:.LC112
	adrp	x0, .LC113
	add	x4, x0, :lo12:.LC113
	adrp	x0, .LC114
	add	x3, x0, :lo12:.LC114
	adrp	x0, .LC115
	add	x2, x0, :lo12:.LC115
	adrp	x0, .LC116
	add	x1, x0, :lo12:.LC116
	adrp	x0, .LC117
	add	x0, x0, :lo12:.LC117
	bl	nand_print
	nop
	ldp	x29, x30, [sp], 16
	ret
	.size	nftl_version, .-nftl_version
	.align	2
	.global	get_nftl_version
	.type	get_nftl_version, %function
get_nftl_version:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	x3, [x29, 32]
	str	x4, [x29, 24]
	adrp	x0, .LC116
	add	x0, x0, :lo12:.LC116
	mov	w2, 2
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	nand_memcpy
	adrp	x0, .LC115
	add	x0, x0, :lo12:.LC115
	mov	w2, 2
	mov	x1, x0
	ldr	x0, [x29, 48]
	bl	nand_memcpy
	adrp	x0, .LC114
	add	x0, x0, :lo12:.LC114
	mov	w2, 4
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	nand_memcpy
	adrp	x0, .LC113
	add	x0, x0, :lo12:.LC113
	mov	w2, 11
	mov	x1, x0
	ldr	x0, [x29, 32]
	bl	nand_memcpy
	adrp	x0, .LC112
	add	x0, x0, :lo12:.LC112
	mov	w2, 6
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	nop
	ldp	x29, x30, [sp], 64
	ret
	.size	get_nftl_version, .-get_nftl_version
	.section	.rodata
	.align	3
.LC118:
	.string	"[NE]%s:malloc fail for nftl_cfg\n"
	.align	3
.LC119:
	.string	"[ND]max_erase_times = %i\n"
	.text
	.align	2
	.global	nftl_initialize
	.type	nftl_initialize, %function
nftl_initialize:
	sub	sp, sp, #112
	stp	x29, x30, [sp, 32]
	add	x29, sp, 32
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 72]
	str	xzr, [x29, 64]
	adrp	x0, .LC1
	add	x1, x0, :lo12:.LC1
	add	x0, x29, 48
	ldr	x2, [x1]
	str	x2, [x0]
	ldrb	w1, [x1, 8]
	strb	w1, [x0, 8]
	mov	w0, 47
	strb	w0, [x29, 40]
	strb	wzr, [x29, 41]
	adrp	x0, com
	add	x1, x0, :lo12:com
	add	x0, x29, 40
	mov	x2, x1
	mov	x1, x0
	mov	x0, 0
	bl	nand_fdt_get_prop_string_by_path
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L653
	mov	w0, -1
	b	.L658
.L653:
	add	x0, x29, 48
	mov	x2, 9
	mov	x1, x0
	ldr	x0, [x29, 64]
	bl	nand_memcmp
	cmp	w0, 0
	beq	.L655
	mov	w0, -1
	b	.L658
.L655:
	bl	nftl_version
	adrp	x0, phyinfo
	add	x0, x0, :lo12:phyinfo
	bl	nftl_get_nand_phyinfo
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 34]
	cmp	w0, 15
	bhi	.L670
	mov	w0, -1
	b	.L658
.L670:
	mov	w0, 36
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 24]
	str	x1, [x0, 32]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	bne	.L659
	adrp	x0, __func__.6431
	add	x1, x0, :lo12:__func__.6431
	adrp	x0, .LC118
	add	x0, x0, :lo12:.LC118
	bl	nand_print
	mov	w0, 1
	b	.L658
.L659:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 6]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0, 8]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0, 10]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldrh	w1, [x1, 2498]
	strh	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldrh	w1, [x1, 2500]
	strh	w1, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 8
	strh	w1, [x0, 14]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 2
	strh	w1, [x0, 18]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 50816
	movk	w1, 0x13, lsl 16
	str	w1, [x0, 32]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	cmp	w0, 0
	bne	.L660
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 30
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 24]
	b	.L661
.L660:
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	cmp	w0, 255
	bne	.L662
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 50
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 40
	strh	w1, [x0, 24]
	b	.L661
.L662:
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	cmp	w0, 10
	bls	.L663
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	mov	w1, 10
	strh	w1, [x0, 2494]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 30
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 24]
	b	.L661
.L663:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldrh	w2, [x1, 2494]
	mov	w1, 50
	mul	w1, w2, w1
	uxth	w1, w1
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	mov	w2, w0
	mov	w0, w2
	ubfiz	w0, w0, 2, 14
	add	w0, w0, w2
	ubfiz	w0, w0, 3, 13
	uxth	w0, w0
	strh	w0, [x1, 24]
.L661:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldrh	w1, [x1, 2]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldrh	w0, [x0, 26]
	cmp	w0, 0
	beq	.L664
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 24]
	b	.L665
.L664:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 24]
.L665:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldrh	w0, [x0, 22]
	cmp	w0, 30
	bls	.L666
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 30
	strh	w1, [x0, 22]
.L666:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 24]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldr	w1, [x0, 2512]
	mov	w0, 45752
	movk	w0, 0x76, lsl 16
	cmp	w1, w0
	bhi	.L667
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldr	w1, [x0, 2512]
	mov	w0, 20863
	movk	w0, 0x1, lsl 16
	cmp	w1, w0
	bls	.L667
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldr	w1, [x1, 2512]
	str	w1, [x0, 32]
.L667:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 28]
	bl	nftl_support_boot_check_crc_from_dts
	cmp	w0, 0
	bne	.L668
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0, 28]
.L668:
	ldr	x0, [x29, 24]
	ldr	x8, [x0, 32]
	ldr	x0, [x29, 24]
	add	x2, x0, 24
	ldr	x0, [x29, 72]
	ldr	x3, [x0, 64]
	ldr	x0, [x29, 72]
	ldr	x4, [x0, 72]
	ldr	x0, [x29, 72]
	ldr	w5, [x0, 44]
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 40]
	mov	w6, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 34]
	mov	w7, w0
	ldr	x0, [x29, 72]
	strb	wzr, [sp, 16]
	ldr	w1, [x29, 20]
	str	w1, [sp, 8]
	str	x0, [sp]
	mov	x1, x8
	ldr	x0, [x29, 24]
	bl	nftl_start
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L669
	ldr	w0, [x29, 60]
	b	.L658
.L669:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldr	w1, [x1, 2504]
	uxth	w1, w1
	strh	w1, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldr	w1, [x1, 2508]
	uxth	w1, w1
	strh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	adrp	x0, .LC119
	add	x0, x0, :lo12:.LC119
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	nftl_get_zone_cap
	mov	w1, w0
	ldr	x0, [x29, 24]
	str	w1, [x0]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_read
	add	x1, x1, :lo12:_nand_read
	str	x1, [x0, 72]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_write
	add	x1, x1, :lo12:_nand_write
	str	x1, [x0, 80]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_discard
	add	x1, x1, :lo12:_nand_discard
	str	x1, [x0, 96]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_flush_write_cache
	add	x1, x1, :lo12:_nand_flush_write_cache
	str	x1, [x0, 88]
	ldr	x0, [x29, 24]
	adrp	x1, _shutdown_op
	add	x1, x1, :lo12:_shutdown_op
	str	x1, [x0, 104]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_sector_read
	add	x1, x1, :lo12:_nand_sector_read
	str	x1, [x0, 112]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_sector_write
	add	x1, x1, :lo12:_nand_sector_write
	str	x1, [x0, 120]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_flush_sector_write_cache
	add	x1, x1, :lo12:_nand_flush_sector_write_cache
	str	x1, [x0, 128]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 16]
	mov	w0, 0
.L658:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 80
	ret
	.size	nftl_initialize, .-nftl_initialize
	.section	.rodata
	.align	3
.LC1:
	.byte	97
	.byte	108
	.byte	108
	.byte	119
	.byte	105
	.byte	110
	.byte	110
	.byte	101
	.byte	114
	.text
	.align	2
	.global	nftl_exit
	.type	nftl_exit, %function
nftl_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	bl	free_nand_partition
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	nftl_zone_release
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_exit, .-nftl_exit
	.align	2
	.global	_nand_read
	.type	_nand_read, %function
_nand_read:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	__nand_read
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	_nand_read, .-_nand_read
	.align	2
	.global	_nand_write
	.type	_nand_write, %function
_nand_write:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	__nand_write
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	_nand_write, .-_nand_write
	.align	2
	.global	_nand_discard
	.type	_nand_discard, %function
_nand_discard:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	w2, [x29, 16]
	ldr	w1, [x29, 20]
	bl	__nand_discard
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	_nand_discard, .-_nand_discard
	.align	2
	.global	_nand_flush_write_cache
	.type	_nand_flush_write_cache, %function
_nand_flush_write_cache:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	w1, [x29, 20]
	bl	__nand_flush_write_cache
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	_nand_flush_write_cache, .-_nand_flush_write_cache
	.align	2
	.global	_nand_sector_read
	.type	_nand_sector_read, %function
_nand_sector_read:
	sub	sp, sp, #48
	str	x0, [sp, 24]
	str	w1, [sp, 20]
	str	w2, [sp, 16]
	str	x3, [sp, 8]
	str	wzr, [sp, 44]
	ldr	w0, [sp, 44]
	add	sp, sp, 48
	ret
	.size	_nand_sector_read, .-_nand_sector_read
	.align	2
	.global	_nand_sector_write
	.type	_nand_sector_write, %function
_nand_sector_write:
	sub	sp, sp, #48
	str	x0, [sp, 24]
	str	w1, [sp, 20]
	str	w2, [sp, 16]
	str	x3, [sp, 8]
	str	wzr, [sp, 44]
	ldr	w0, [sp, 44]
	add	sp, sp, 48
	ret
	.size	_nand_sector_write, .-_nand_sector_write
	.align	2
	.global	_nand_flush_sector_write_cache
	.type	_nand_flush_sector_write_cache, %function
_nand_flush_sector_write_cache:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	wzr, [sp, 28]
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	_nand_flush_sector_write_cache, .-_nand_flush_sector_write_cache
	.align	2
	.global	_shutdown_op
	.type	_shutdown_op, %function
_shutdown_op:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	__shutdown_op
	ldp	x29, x30, [sp], 32
	ret
	.size	_shutdown_op, .-_shutdown_op
	.global	nftl_blk_head
	.bss
	.align	3
	.type	nftl_blk_head, %object
	.size	nftl_blk_head, 136
nftl_blk_head:
	.zero	136
	.section	.rodata
	.align	3
.LC120:
	.string	"[NE]nftl_build_all error!\n"
	.text
	.align	2
	.global	nftl_build_all
	.type	nftl_build_all, %function
nftl_build_all:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 72]
	str	x0, [x29, 40]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	xzr, [x0, 16]
	b	.L690
.L693:
	ldr	x0, [x29, 40]
	bl	nftl_add
	cmp	x0, 0
	bne	.L691
	adrp	x0, .LC120
	add	x0, x0, :lo12:.LC120
	bl	nand_print
	mov	w0, 1
	b	.L692
.L691:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 64]
	str	x0, [x29, 40]
.L690:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L693
	mov	w0, 0
.L692:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_build_all, .-nftl_build_all
	.align	2
	.global	nftl_build_one
	.type	nftl_build_one, %function
nftl_build_one:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 72]
	str	x0, [x29, 40]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	bne	.L696
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	xzr, [x0, 16]
	b	.L696
.L699:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bne	.L697
	ldr	x0, [x29, 40]
	bl	nftl_add
	cmp	x0, 0
	bne	.L697
	adrp	x0, .LC120
	add	x0, x0, :lo12:.LC120
	bl	nand_print
	mov	w0, 1
	b	.L698
.L697:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 64]
	str	x0, [x29, 40]
.L696:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L699
	mov	w0, 0
.L698:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_build_one, .-nftl_build_one
	.section	.rodata
	.align	3
.LC121:
	.string	"[NE]%s:malloc fail for nftl_blk\n"
	.align	3
.LC122:
	.string	"[NE]nftl_initialize failed\n"
	.align	3
.LC123:
	.string	"[ND]nftl_add ok\n"
	.text
	.align	2
	.global	nftl_add
	.type	nftl_add, %function
nftl_add:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 136
	bl	nftl_malloc
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L701
	adrp	x0, __func__.6563
	add	x1, x0, :lo12:__func__.6563
	adrp	x0, .LC121
	add	x0, x0, :lo12:.LC121
	bl	nand_print
	mov	x0, 0
	b	.L702
.L701:
	ldr	x0, [x29, 24]
	bl	build_nand_partition
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	nftl_initialize
	cmp	w0, 0
	beq	.L703
	adrp	x0, .LC122
	add	x0, x0, :lo12:.LC122
	bl	nand_print
	mov	x0, 0
	b	.L702
.L703:
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x1, [x29, 40]
	bl	add_nftl_blk_list
	ldr	x0, [x29, 40]
	bl	panic_recover_data
	adrp	x0, .LC123
	add	x0, x0, :lo12:.LC123
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
.L702:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_add, .-nftl_add
	.align	2
	.global	nand_ftl_exit
	.type	nand_ftl_exit, %function
nand_ftl_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	bl	del_last_nftl_blk
	str	x0, [x29, 24]
	b	.L705
.L706:
	ldr	x0, [x29, 24]
	bl	nftl_exit
	ldr	x0, [x29, 24]
	bl	nftl_free
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	bl	del_last_nftl_blk
	str	x0, [x29, 24]
.L705:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L706
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nand_ftl_exit, .-nand_ftl_exit
	.section	.rodata
	.align	3
.LC124:
	.string	"[ND]cap: 0x%08x.\n"
	.text
	.align	2
	.global	print_all_nftl_blk
	.type	print_all_nftl_blk, %function
print_all_nftl_blk:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [x29, 24]
	b	.L709
.L710:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	ldr	w1, [x0]
	adrp	x0, .LC124
	add	x0, x0, :lo12:.LC124
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
.L709:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L710
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	print_all_nftl_blk, .-print_all_nftl_blk
	.align	2
	.global	get_nftl_num
	.type	get_nftl_num, %function
get_nftl_num:
	sub	sp, sp, #16
	str	wzr, [sp, 12]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [sp]
	b	.L712
.L713:
	ldr	w0, [sp, 12]
	add	w0, w0, 1
	str	w0, [sp, 12]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	str	x0, [sp]
.L712:
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L713
	ldr	w0, [sp, 12]
	add	sp, sp, 16
	ret
	.size	get_nftl_num, .-get_nftl_num
	.align	2
	.global	get_nftl_cap
	.type	get_nftl_cap, %function
get_nftl_cap:
	sub	sp, sp, #16
	str	wzr, [sp, 12]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [sp]
	b	.L716
.L717:
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	ldr	w0, [x0]
	ldr	w1, [sp, 12]
	add	w0, w1, w0
	str	w0, [sp, 12]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	str	x0, [sp]
.L716:
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L717
	ldr	w0, [sp, 12]
	add	sp, sp, 16
	ret
	.size	get_nftl_cap, .-get_nftl_cap
	.align	2
	.global	get_first_nftl_cap
	.type	get_first_nftl_cap, %function
get_first_nftl_cap:
	sub	sp, sp, #16
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 16]
	ldr	w0, [x0]
	add	sp, sp, 16
	ret
	.size	get_first_nftl_cap, .-get_first_nftl_cap
	.section	.rodata
	.align	3
.LC125:
	.string	"[NE]parameter error %d,%d !\n"
	.text
	.align	2
	.global	nftl_read
	.type	nftl_read, %function
nftl_read:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	str	w1, [x29, 24]
	str	x2, [x29, 16]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	w0, [x29, 24]
	cmp	w0, 0
	bne	.L724
	mov	w0, 0
	b	.L723
.L725:
	ldr	x0, [x29, 40]
	ldr	w0, [x0]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L724
	adrp	x0, .LC125
	add	x0, x0, :lo12:.LC125
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	bl	nand_print
	mov	w0, 1
	b	.L723
.L724:
	ldr	x0, [x29, 40]
	ldr	w1, [x0]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bls	.L725
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 72]
	ldr	x3, [x29, 16]
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 40]
	blr	x4
.L723:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_read, .-nftl_read
	.align	2
	.global	nftl_write
	.type	nftl_write, %function
nftl_write:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	str	w1, [x29, 24]
	str	x2, [x29, 16]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	w0, [x29, 24]
	cmp	w0, 0
	bne	.L729
	mov	w0, 0
	b	.L728
.L730:
	ldr	x0, [x29, 40]
	ldr	w0, [x0]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L729
	adrp	x0, .LC125
	add	x0, x0, :lo12:.LC125
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	bl	nand_print
	mov	w0, 1
	b	.L728
.L729:
	ldr	x0, [x29, 40]
	ldr	w1, [x0]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bls	.L730
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 80]
	ldr	x3, [x29, 16]
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 40]
	blr	x4
.L728:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_write, .-nftl_write
	.align	2
	.global	nftl_discard
	.type	nftl_discard, %function
nftl_discard:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	str	w1, [x29, 24]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	w0, [x29, 24]
	cmp	w0, 0
	bne	.L734
	mov	w0, 0
	b	.L733
.L735:
	ldr	x0, [x29, 40]
	ldr	w0, [x0]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L734
	adrp	x0, .LC125
	add	x0, x0, :lo12:.LC125
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	bl	nand_print
	mov	w0, 1
	b	.L733
.L734:
	ldr	x0, [x29, 40]
	ldr	w1, [x0]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bls	.L735
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 96]
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 40]
	blr	x3
.L733:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_discard, .-nftl_discard
	.align	2
	.global	nftl_flush_write_cache
	.type	nftl_flush_write_cache, %function
nftl_flush_write_cache:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
	b	.L737
.L738:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 88]
	mov	w1, 65535
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
.L737:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L738
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_flush_write_cache, .-nftl_flush_write_cache
	.align	2
	.global	nftl_write_end
	.type	nftl_write_end, %function
nftl_write_end:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
	b	.L741
.L742:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	mov	w1, 8
	bl	fill_no_use_page_to_current_block
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
.L741:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L742
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_write_end, .-nftl_write_end
	.align	2
	.global	_nftl_nand_erase_superblk
	.type	_nftl_nand_erase_superblk, %function
_nftl_nand_erase_superblk:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 42]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 40]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 88]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 40
	blr	x2
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_erase_superblk, .-_nftl_nand_erase_superblk
	.align	2
	.global	_nftl_nand_read_page
	.type	_nftl_nand_read_page, %function
_nftl_nand_read_page:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x0, 80]
	add	x1, x1, 1
	str	x1, [x0, 80]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 34]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 32]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 36]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 48]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 96]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 32
	blr	x2
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmn	w0, #2
	bne	.L747
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 132]
	add	w1, w1, 1
	str	w1, [x0, 132]
.L747:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_read_page, .-_nftl_nand_read_page
	.align	2
	.global	_nftl_nand_write_page
	.type	_nftl_nand_write_page, %function
_nftl_nand_write_page:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x0, 88]
	add	x1, x1, 1
	str	x1, [x0, 88]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 34]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 32]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 36]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 48]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 104]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 32
	blr	x2
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L750
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 136]
	add	w1, w1, 1
	str	w1, [x0, 136]
.L750:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_write_page, .-_nftl_nand_write_page
	.align	2
	.global	_nftl_nand_is_blk_good
	.type	_nftl_nand_is_blk_good, %function
_nftl_nand_is_blk_good:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 42]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 40]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 112]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 40
	blr	x2
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_is_blk_good, .-_nftl_nand_is_blk_good
	.align	2
	.global	_nftl_nand_mark_bad_blk
	.type	_nftl_nand_mark_bad_blk, %function
_nftl_nand_mark_bad_blk:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 42]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 40]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 120]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 40
	blr	x2
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_mark_bad_blk, .-_nftl_nand_mark_bad_blk
	.local	gc_src_type
	.comm	gc_src_type,4,4
	.section	.rodata
	.align	3
.LC126:
	.string	"[NE]garbage_collect phy_block null\n"
	.align	3
.LC127:
	.string	"[ND]garbage too frequency! %d:%d\n"
	.align	3
.LC128:
	.string	"[NE]garbage_collect_first wrong fbn:%d\n"
	.text
	.align	2
	.global	garbage_collect
	.type	garbage_collect, %function
garbage_collect:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bhi	.L757
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 684]
.L757:
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 3
	str	w1, [x0]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	bne	.L758
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	cmp	w1, w0
	bcs	.L759
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 686]
	cmp	w0, 0
	bne	.L760
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L761
	adrp	x0, .LC126
	add	x0, x0, :lo12:.LC126
	bl	nand_print
	mov	w0, 1
	b	.L762
.L761:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	cmp	w1, w0
	bcc	.L763
	ldr	x0, [x29, 24]
	mov	w1, 4
	strh	w1, [x0, 688]
	b	.L764
.L763:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 2
	uxth	w0, w0
	cmp	w1, w0
	bcc	.L765
	ldr	x0, [x29, 24]
	mov	w1, 8
	strh	w1, [x0, 688]
	b	.L764
.L765:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 3
	uxth	w0, w0
	cmp	w1, w0
	bcc	.L766
	ldr	x0, [x29, 24]
	mov	w1, 16
	strh	w1, [x0, 688]
	b	.L764
.L766:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC127
	add	x0, x0, :lo12:.LC127
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 688]
.L764:
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 16]
	cmp	w1, w0
	bhi	.L760
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 688]
.L760:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 688]
	strh	w0, [x29, 38]
	ldrh	w1, [x29, 38]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L767
	ldrh	w2, [x29, 38]
	mov	x1, 0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	cmp	w0, 0
	beq	.L758
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC128
	add	x0, x0, :lo12:.LC128
	bl	nand_print
	mov	w0, 1
	b	.L762
.L767:
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	gc_win_block
	b	.L758
.L759:
	ldr	x0, [x29, 24]
	mov	w1, 1
	strh	w1, [x0, 684]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 686]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 688]
.L758:
	mov	w0, 0
.L762:
	ldp	x29, x30, [sp], 48
	ret
	.size	garbage_collect, .-garbage_collect
	.section	.rodata
	.align	3
.LC129:
	.string	"[NI]garbage_collect_first none!!\n"
	.align	3
.LC130:
	.string	"[ND]garbage_collect_first wrong!!\n"
	.align	3
.LC131:
	.string	"[ND]garbage_collect_first cannot find PHY_MAPPING_PAGE%d %d %d %d!!\n"
	.align	3
.LC132:
	.string	"[ND]garbage_collect_first cannot find PHY_MAPPING_PAGE%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x.\n"
	.align	3
.LC133:
	.string	"[ND][NE]garbage_collect_first rebuild %d,%d!!\n"
	.align	3
.LC134:
	.string	"[ND]garbage_collect_first PHY_MAPPING_PAGEecc err block:%d !!\n"
	.align	3
.LC135:
	.string	"[NE]garbage_collect_first invaild_page num error!!\n"
	.align	3
.LC136:
	.string	"[ND]garbage_collect_first read a logic page error%d %d %d %d spare data:\n"
	.align	3
.LC137:
	.string	"%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n"
	.align	3
.LC138:
	.string	"[NE]nand_write_logic_page_no_gc gc fail!!\n"
	.align	3
.LC139:
	.string	"[NE]erase ecc error block\n"
	.text
	.align	2
	.global	garbage_collect_first
	.type	garbage_collect_first, %function
garbage_collect_first:
	sub	sp, sp, #224
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	strh	w2, [x29, 30]
	str	wzr, [x29, 116]
	str	wzr, [x29, 132]
	str	wzr, [x29, 112]
	add	x0, x29, 72
	mov	w2, 16
	mov	w1, 90
	bl	nand_memset
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L770
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_invalid_page_list
	str	x0, [x29, 120]
	b	.L771
.L770:
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	is_current_blcok
	cmp	w0, 1
	bne	.L772
	mov	w0, 0
	b	.L797
.L772:
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_invalid_page_list_by_block
	str	x0, [x29, 120]
.L771:
	ldr	x0, [x29, 120]
	cmp	x0, 0
	bne	.L774
	adrp	x0, .LC129
	add	x0, x0, :lo12:.LC129
	bl	nand_print
	mov	w0, 8
	b	.L797
.L774:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 120]
	add	w1, w1, 1
	str	w1, [x0, 120]
	ldr	x0, [x29, 120]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 136]
	add	x0, x29, 72
	bl	is_phy_mapping_page
	cmp	w0, 1
	beq	.L775
	adrp	x0, .LC130
	add	x0, x0, :lo12:.LC130
	bl	nand_print_dbg
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w1, w0, #1
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 28]
	mov	w2, w0
	adrp	x0, .LC131
	add	x0, x0, :lo12:.LC131
	mov	w4, w2
	ldr	w3, [x29, 112]
	mov	w2, w1
	mov	w1, w5
	bl	nand_print_dbg
	ldrb	w0, [x29, 73]
	mov	w9, w0
	ldrb	w0, [x29, 74]
	mov	w10, w0
	ldrb	w0, [x29, 75]
	mov	w11, w0
	ldrb	w0, [x29, 76]
	mov	w12, w0
	ldrb	w0, [x29, 77]
	mov	w13, w0
	ldrb	w0, [x29, 78]
	mov	w14, w0
	ldrb	w0, [x29, 79]
	mov	w15, w0
	ldrb	w0, [x29, 80]
	mov	w1, w0
	ldrb	w0, [x29, 81]
	mov	w2, w0
	ldrb	w0, [x29, 82]
	mov	w3, w0
	ldrb	w0, [x29, 83]
	mov	w4, w0
	ldrb	w0, [x29, 84]
	mov	w5, w0
	ldrb	w0, [x29, 85]
	mov	w6, w0
	ldrb	w0, [x29, 86]
	mov	w7, w0
	ldrb	w0, [x29, 87]
	mov	w8, w0
	adrp	x0, .LC132
	add	x0, x0, :lo12:.LC132
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w15
	mov	w6, w14
	mov	w5, w13
	mov	w4, w12
	mov	w3, w11
	mov	w2, w10
	mov	w1, w9
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC133
	add	x0, x0, :lo12:.LC133
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 480]
	mov	x2, x0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	recover_block_phy_page_mapping
	mov	w0, 1
	str	w0, [x29, 132]
	b	.L776
.L775:
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L777
	ldr	w0, [x29, 136]
	cmp	w0, 10
	beq	.L777
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC134
	add	x0, x0, :lo12:.LC134
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 480]
	mov	x2, x0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	recover_block_phy_page_mapping
	mov	w0, 1
	str	w0, [x29, 132]
	b	.L776
.L777:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 480]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 488]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
.L776:
	ldr	x0, [x29, 40]
	add	x1, x0, 464
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w3, w0
	ldr	x2, [x29, 120]
	ldr	x0, [x29, 40]
	bl	check_invalid_page_right
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L778
	adrp	x0, .LC135
	add	x0, x0, :lo12:.LC135
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 132]
.L778:
	ldr	w0, [x29, 132]
	cmp	w0, 1
	bne	.L779
	mov	w0, -1
	strh	w0, [x29, 30]
.L779:
	str	wzr, [x29, 140]
	b	.L780
.L793:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 480]
	ldr	w0, [x29, 140]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmn	w0, #1
	beq	.L798
	ldr	w1, [x29, 108]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	cmp	x0, 0
	beq	.L799
	ldr	x0, [x29, 96]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	cmp	w1, w0
	bne	.L800
	ldr	x0, [x29, 96]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bne	.L801
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 124]
	add	w1, w1, 1
	str	w1, [x0, 124]
	ldr	x0, [x29, 120]
	ldrh	w1, [x0]
	ldr	w0, [x29, 140]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 520]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	str	wzr, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 1
	bhi	.L786
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 136]
	nop
.L786:
	add	x0, x29, 72
	bl	get_logic_page_from_oob
	str	w0, [x29, 116]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L787
	ldr	w0, [x29, 136]
	cmp	w0, 10
	bne	.L788
.L787:
	ldr	w1, [x29, 116]
	ldr	w0, [x29, 108]
	cmp	w1, w0
	beq	.L789
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC136
	add	x0, x0, :lo12:.LC136
	ldr	w4, [x29, 108]
	ldr	w3, [x29, 116]
	ldr	w2, [x29, 140]
	bl	nand_print_dbg
	ldrb	w0, [x29, 72]
	mov	w10, w0
	ldrb	w0, [x29, 73]
	mov	w11, w0
	ldrb	w0, [x29, 74]
	mov	w12, w0
	ldrb	w0, [x29, 75]
	mov	w13, w0
	ldrb	w0, [x29, 76]
	mov	w14, w0
	ldrb	w0, [x29, 77]
	mov	w15, w0
	ldrb	w0, [x29, 78]
	mov	w16, w0
	ldrb	w0, [x29, 79]
	mov	w1, w0
	ldrb	w0, [x29, 80]
	mov	w2, w0
	ldrb	w0, [x29, 81]
	mov	w3, w0
	ldrb	w0, [x29, 82]
	mov	w4, w0
	ldrb	w0, [x29, 83]
	mov	w5, w0
	ldrb	w0, [x29, 84]
	mov	w6, w0
	ldrb	w0, [x29, 85]
	mov	w7, w0
	ldrb	w0, [x29, 86]
	mov	w8, w0
	ldrb	w0, [x29, 87]
	mov	w9, w0
	adrp	x0, .LC137
	add	x0, x0, :lo12:.LC137
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print
	ldr	x0, [x29, 96]
	mov	w1, -1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 96]
	ldrh	w1, [x0]
	orr	w1, w1, 4095
	strh	w1, [x0]
	mov	w0, 1
	str	w0, [x29, 132]
	mov	w0, -1
	strh	w0, [x29, 30]
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 120]
	strh	w1, [x0, 4]
	b	.L802
.L789:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 520]
	mov	x2, x0
	ldr	w1, [x29, 108]
	ldr	x0, [x29, 40]
	bl	nand_write_logic_page_no_gc
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L802
	adrp	x0, .LC138
	add	x0, x0, :lo12:.LC138
	bl	nand_print
	mov	w0, 1
	b	.L797
.L788:
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 120]
	strh	w1, [x0, 4]
	mov	w0, 1
	str	w0, [x29, 132]
	mov	w0, -1
	strh	w0, [x29, 30]
	b	.L791
.L802:
	nop
.L791:
	ldrh	w0, [x29, 30]
	sub	w0, w0, #1
	strh	w0, [x29, 30]
	ldrh	w0, [x29, 30]
	cmp	w0, 0
	beq	.L803
	b	.L782
.L798:
	nop
	b	.L782
.L799:
	nop
	b	.L782
.L800:
	nop
	b	.L782
.L801:
	nop
.L782:
	ldr	w0, [x29, 140]
	add	w0, w0, 1
	str	w0, [x29, 140]
.L780:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bhi	.L793
	b	.L792
.L803:
	nop
.L792:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bne	.L794
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 686]
.L794:
	ldr	w0, [x29, 132]
	cmp	w0, 1
	bne	.L795
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bne	.L795
	adrp	x0, .LC139
	add	x0, x0, :lo12:.LC139
	bl	nand_print
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	erase_block
	b	.L796
.L795:
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	put_phy_block_to_invalid_page_list
	ldr	x0, [x29, 40]
	bl	adjust_invaild_list
.L796:
	mov	w0, 0
.L797:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 144
	ret
	.size	garbage_collect_first, .-garbage_collect_first
	.align	2
	.global	get_empty_gc_node
	.type	get_empty_gc_node, %function
get_empty_gc_node:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strh	wzr, [sp, 30]
	b	.L805
.L808:
	ldrh	w0, [sp, 30]
	ldr	x1, [sp, 8]
	sxtw	x0, w0
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 10
	ldrh	w0, [x0]
	cmp	w0, 0
	bne	.L806
	ldrh	w0, [sp, 30]
	sxtw	x0, w0
	lsl	x0, x0, 5
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	add	x0, x0, 8
	b	.L807
.L806:
	ldrh	w0, [sp, 30]
	add	w0, w0, 1
	strh	w0, [sp, 30]
.L805:
	ldrh	w0, [sp, 30]
	cmp	w0, 9
	bls	.L808
	mov	x0, 0
.L807:
	add	sp, sp, 32
	ret
	.size	get_empty_gc_node, .-get_empty_gc_node
	.align	2
	.global	search_gc_node
	.type	search_gc_node, %function
search_gc_node:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 328
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L810
.L813:
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 8]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L811
	ldr	x0, [sp, 24]
	b	.L812
.L811:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L810:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L813
	mov	x0, 0
.L812:
	add	sp, sp, 32
	ret
	.size	search_gc_node, .-search_gc_node
	.align	2
	.global	add_to_gc_list_tail
	.type	add_to_gc_list_tail, %function
add_to_gc_list_tail:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 328
	str	x0, [sp, 24]
	b	.L815
.L816:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L815:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L816
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0]
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	add_to_gc_list_tail, .-add_to_gc_list_tail
	.align	2
	.global	del_from_gc_list
	.type	del_from_gc_list, %function
del_from_gc_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 16]
	ldr	x0, [sp, 24]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L819
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
.L819:
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 24]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_gc_list, .-del_from_gc_list
	.align	2
	.global	add_prio_gc
	.type	add_prio_gc, %function
add_prio_gc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	strh	w2, [x29, 30]
	ldr	x0, [x29, 40]
	add	x0, x0, 696
	ldr	x1, [x29, 32]
	bl	search_gc_node
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L822
	mov	w0, 1
	b	.L823
.L822:
	ldr	x0, [x29, 40]
	add	x0, x0, 696
	bl	get_empty_gc_node
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L824
	mov	w0, 1
	b	.L823
.L824:
	ldr	x0, [x29, 56]
	ldr	x1, [x29, 32]
	str	x1, [x0, 8]
	ldr	x0, [x29, 56]
	ldrh	w1, [x29, 30]
	strh	w1, [x0, 2]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	add	x0, x0, 696
	ldr	x1, [x29, 56]
	bl	add_to_gc_list_tail
	mov	w0, 0
.L823:
	ldp	x29, x30, [sp], 64
	ret
	.size	add_prio_gc, .-add_prio_gc
	.section	.rodata
	.align	3
.LC140:
	.string	"[ND]no block to prio gc\n"
	.align	3
.LC141:
	.string	"[ND]no need gc:%d, type:%d\n"
	.align	3
.LC142:
	.string	"[NE]do_prio_gc garbage_collect_first fail:%d, type:%d\n"
	.align	3
.LC143:
	.string	"[ND]prio gc fail1:%d, type:%d\n"
	.align	3
.LC144:
	.string	"[ND]prio gc fail2:%d, type:%d\n"
	.text
	.align	2
	.global	do_prio_gc
	.type	do_prio_gc, %function
do_prio_gc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1040]
	cmp	x0, 0
	bne	.L826
	mov	w0, 0
	b	.L827
.L826:
	ldr	x0, [x29, 24]
	add	x2, x0, 696
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1040]
	mov	x1, x0
	mov	x0, x2
	bl	del_from_gc_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_current_blcok
	cmp	w0, 1
	bne	.L828
	ldr	x0, [x29, 24]
	add	x0, x0, 696
	ldr	x1, [x29, 56]
	bl	add_to_gc_list_tail
	mov	w0, 0
	b	.L827
.L828:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L829
	adrp	x0, .LC140
	add	x0, x0, :lo12:.LC140
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L827
.L829:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_invalid_list
	cmp	w0, 0
	bne	.L830
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC141
	add	x0, x0, :lo12:.LC141
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L827
.L830:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	strh	w1, [x0, 698]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	cmp	w0, 5
	bne	.L831
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	w2, 4
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	str	w0, [x29, 52]
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L827
.L831:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	w2, -1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L832
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC142
	add	x0, x0, :lo12:.LC142
	bl	nand_print
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 1
	b	.L827
.L832:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_free_list
	cmp	w0, 0
	bne	.L833
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC143
	add	x0, x0, :lo12:.LC143
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L827
.L833:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L834
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC144
	add	x0, x0, :lo12:.LC144
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L827
.L834:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 698]
	cmp	w0, 1
	bne	.L835
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L836
.L835:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 698]
	cmp	w0, 2
	bne	.L837
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L836
.L837:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 698]
	cmp	w0, 4
	bne	.L838
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L836
.L838:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
.L836:
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
.L827:
	ldp	x29, x30, [sp], 64
	ret
	.size	do_prio_gc, .-do_prio_gc
	.align	2
	.global	prio_gc_all
	.type	prio_gc_all, %function
prio_gc_all:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L840
.L841:
	ldr	x0, [x29, 24]
	bl	do_prio_gc
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L840:
	ldr	w0, [x29, 44]
	cmp	w0, 9
	bls	.L841
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	prio_gc_all, .-prio_gc_all
	.section	.rodata
	.align	3
.LC145:
	.string	"[ND]swl: static WL need to do(%d : %d)\n"
	.align	3
.LC146:
	.string	"[ND]swl: min_ec_blk_no = %d,\n"
	.align	3
.LC147:
	.string	"[ND]swl: zone->blocks = %d)\n"
	.text
	.align	2
	.global	check_static_wear_leveling
	.type	check_static_wear_leveling, %function
check_static_wear_leveling:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	xzr, [x29, 56]
	str	xzr, [x29, 48]
	mov	w0, -1
	strh	w0, [x29, 46]
	strh	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	cmp	x0, 0
	beq	.L844
	mov	w0, 1
	b	.L845
.L844:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	cmp	x0, 0
	beq	.L846
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 304]
	cmp	x0, 0
	bne	.L847
.L846:
	mov	w0, 1
	b	.L845
.L847:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w0, [x0, 8]
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 26]
	cmp	w0, w1
	bge	.L848
	mov	w0, 1
	b	.L845
.L848:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	get_block_max_erase_time_from_free_list
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	bne	.L849
	mov	w0, 1
	b	.L845
.L849:
	ldr	x0, [x29, 48]
	ldrh	w0, [x0, 6]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w0, w0
	ldrh	w1, [x29, 44]
	cmp	w1, w0
	bcs	.L850
	mov	w0, 1
	b	.L845
.L850:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w0, [x0, 8]
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 26]
	sub	w0, w0, w1
	mov	w2, w0
	mov	w1, w3
	ldr	x0, [x29, 24]
	bl	get_block_min_erase_time_from_invalid_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L851
	mov	w0, 1
	b	.L845
.L851:
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 6]
	strh	w0, [x29, 46]
	ldrh	w1, [x29, 44]
	ldrh	w0, [x29, 46]
	cmp	w1, w0
	bhi	.L852
	mov	w0, 1
	b	.L845
.L852:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	lsl	w2, w0, 1
	ldrh	w1, [x29, 44]
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	cmp	w2, w0
	bge	.L853
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 43691
	movk	w0, 0xaaaa, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
	b	.L854
.L853:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	ldrh	w0, [x29, 44]
	lsl	w0, w0, 1
	cmp	w1, w0
	bge	.L855
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
	b	.L854
.L855:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w2, w0
	ldrh	w1, [x29, 44]
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	cmp	w2, w0
	bge	.L856
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 18725
	movk	w0, 0x2492, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	sub	w1, w1, w0
	lsr	w1, w1, 1
	add	w0, w1, w0
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
	b	.L854
.L856:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
.L854:
	ldrh	w1, [x29, 44]
	ldrh	w0, [x29, 46]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 1058]
	cmp	w0, w1
	bge	.L857
	mov	w0, 1
	b	.L845
.L857:
	ldrh	w1, [x29, 44]
	ldrh	w2, [x29, 46]
	adrp	x0, .LC145
	add	x0, x0, :lo12:.LC145
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC146
	add	x0, x0, :lo12:.LC146
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC147
	add	x0, x0, :lo12:.LC147
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 56]
	str	x1, [x0, 1064]
	mov	w0, 0
.L845:
	ldp	x29, x30, [sp], 64
	ret
	.size	check_static_wear_leveling, .-check_static_wear_leveling
	.section	.rodata
	.align	3
.LC148:
	.string	"[ND]swl: the block(%d) for static WL is not in invalid_list\n"
	.align	3
.LC149:
	.string	"[ND]swl: done_static_wear_leveling(min_ec) block:%d, erase_count:%d\n"
	.align	3
.LC150:
	.string	"[NE]swl: garbage_collect_first fail:%d\n"
	.align	3
.LC151:
	.string	"[NE]swl: last->check fail:%d\n"
	.text
	.align	2
	.global	done_static_wear_leveling
	.type	done_static_wear_leveling, %function
done_static_wear_leveling:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	cmp	x0, 0
	bne	.L859
	mov	w0, 2
	b	.L860
.L859:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_current_blcok
	cmp	w0, 1
	bne	.L861
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 2
	b	.L860
.L861:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_invalid_list
	cmp	w0, 0
	bne	.L862
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC148
	add	x0, x0, :lo12:.LC148
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 2
	b	.L860
.L862:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC149
	add	x0, x0, :lo12:.LC149
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	mov	w1, 1
	strh	w1, [x0, 1062]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 128]
	add	w1, w1, 1
	str	w1, [x0, 128]
	ldr	x0, [x29, 24]
	bl	fill_no_use_data_to_current_block
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	w2, -1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L863
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC150
	add	x0, x0, :lo12:.LC150
	bl	nand_print
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 1
	b	.L860
.L863:
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 1062]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_free_list
	cmp	w0, 0
	bne	.L864
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC151
	add	x0, x0, :lo12:.LC151
	bl	nand_print
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 0
	b	.L860
.L864:
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 0
.L860:
	ldp	x29, x30, [sp], 48
	ret
	.size	done_static_wear_leveling, .-done_static_wear_leveling
	.align	2
	.global	do_static_wear_leveling
	.type	do_static_wear_leveling, %function
do_static_wear_leveling:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 6]
	cmp	w0, 0
	bne	.L866
	mov	w0, 0
	b	.L867
.L866:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	bne	.L868
	mov	w0, 1
	b	.L867
.L868:
	ldr	x0, [x29, 24]
	bl	check_static_wear_leveling
	cmp	w0, 0
	bne	.L869
	ldr	x0, [x29, 24]
	bl	done_static_wear_leveling
	b	.L867
.L869:
	mov	w0, 0
.L867:
	ldp	x29, x30, [sp], 32
	ret
	.size	do_static_wear_leveling, .-do_static_wear_leveling
	.align	2
	.global	prio_gc_one
	.type	prio_gc_one, %function
prio_gc_one:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	w2, [x29, 16]
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 40]
	ldr	w0, [x29, 16]
	cmp	w0, 1
	bne	.L871
	mov	w0, 1
	str	w0, [x29, 16]
	b	.L872
.L871:
	ldr	w0, [x29, 16]
	cmp	w0, 2
	bne	.L873
	mov	w0, 2
	str	w0, [x29, 16]
	b	.L872
.L873:
	ldr	w0, [x29, 16]
	cmp	w0, 3
	bne	.L874
	mov	w0, 3
	str	w0, [x29, 16]
	b	.L872
.L874:
	mov	w0, 3
	str	w0, [x29, 16]
.L872:
	ldr	w0, [x29, 16]
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	prio_gc_one, .-prio_gc_one
	.section	.rodata
	.align	3
.LC152:
	.string	"[ND]gc_one ok! from: %d [%d %d %d] @%s %d\n"
	.align	3
.LC153:
	.string	"[NE][NE]gc_one error!\n"
	.text
	.align	2
	.global	gc_one
	.type	gc_one, %function
gc_one:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	str	x0, [x29, 72]
	strh	w1, [x29, 70]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 352]
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	cmp	x0, 0
	bne	.L877
	mov	w0, 8
	b	.L878
.L877:
	ldr	x0, [x29, 88]
	ldrh	w0, [x0, 4]
	ldrh	w1, [x29, 70]
	cmp	w1, w0
	bcs	.L879
	mov	w2, -1
	mov	x1, 0
	ldr	x0, [x29, 72]
	bl	garbage_collect_first
	str	w0, [x29, 84]
	ldr	w0, [x29, 84]
	cmp	w0, 8
	bne	.L880
	mov	w0, 8
	b	.L878
.L880:
	ldr	w0, [x29, 84]
	cmp	w0, 0
	bne	.L881
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	ldr	w19, [x0]
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 30]
	mov	w20, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 680]
	mov	w21, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 682]
	mov	w22, w0
	bl	nand_get_cur_task_name
	mov	x23, x0
	bl	nand_get_cur_task_pid
	mov	w1, w0
	adrp	x0, .LC152
	add	x0, x0, :lo12:.LC152
	mov	w6, w1
	mov	x5, x23
	mov	w4, w22
	mov	w3, w21
	mov	w2, w20
	mov	w1, w19
	bl	nand_print_dbg
	mov	w0, 0
	b	.L878
.L881:
	adrp	x0, .LC153
	add	x0, x0, :lo12:.LC153
	bl	nand_print
	mov	w0, 1
	b	.L878
.L879:
	mov	w0, 8
.L878:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
	.size	gc_one, .-gc_one
	.align	2
	.global	gc_all_cnt
	.type	gc_all_cnt, %function
gc_all_cnt:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	w2, [x29, 16]
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 255
	str	w1, [x0]
	b	.L883
.L885:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 8
	beq	.L887
.L883:
	ldr	w0, [x29, 16]
	sub	w1, w0, #1
	str	w1, [x29, 16]
	cmp	w0, 0
	bne	.L885
	b	.L884
.L887:
	nop
.L884:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	gc_all_cnt, .-gc_all_cnt
	.align	2
	.global	gc_all
	.type	gc_all, %function
gc_all:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 255
	str	w1, [x0]
.L891:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L894
	b	.L891
.L894:
	nop
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	gc_all, .-gc_all
	.align	2
	.global	gc_all_enhance
	.type	gc_all_enhance, %function
gc_all_enhance:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, phyinfo
	add	x0, x0, :lo12:phyinfo
	str	x0, [x29, 56]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	str	w0, [x29, 52]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	ldr	w0, [x29, 52]
	cmp	w1, w0
	bls	.L896
	mov	w0, 0
	b	.L897
.L896:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, w0
	ldr	w0, [x29, 52]
	cmp	w1, w0
	bcs	.L898
	mov	w0, 0
	b	.L897
.L898:
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 4]
	ldr	x1, [x29, 56]
	ldrh	w1, [x1, 8]
	mul	w0, w0, w1
	uxtw	x0, w0
	str	x0, [x29, 40]
	ldr	x0, [x29, 56]
	ldrb	w0, [x0, 10]
	uxtb	x0, w0
	ldr	x1, [x29, 40]
	mul	x0, x1, x0
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	lsr	x0, x0, 11
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 128
	bls	.L899
	mov	w0, 0
	b	.L897
.L899:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	gc_all
.L897:
	ldp	x29, x30, [sp], 64
	ret
	.size	gc_all_enhance, .-gc_all_enhance
	.align	2
	.global	gc_all_base_on_free_blks
	.type	gc_all_base_on_free_blks, %function
gc_all_base_on_free_blks:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, phyinfo
	add	x0, x0, :lo12:phyinfo
	str	x0, [x29, 72]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bls	.L901
	mov	w0, 0
	b	.L902
.L901:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bcs	.L903
	mov	w0, 0
	b	.L902
.L903:
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 4]
	ldr	x1, [x29, 72]
	ldrh	w1, [x1, 8]
	mul	w0, w0, w1
	uxtw	x0, w0
	str	x0, [x29, 48]
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 10]
	uxtb	x0, w0
	ldr	x1, [x29, 48]
	mul	x0, x1, x0
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	lsr	x0, x0, 11
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 128
	bls	.L904
	mov	w0, 0
	b	.L902
.L904:
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	sub	w0, w0, #3
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	ldr	w0, [x29, 60]
	sub	w0, w0, w1
	ldr	w1, [x29, 44]
	mul	w0, w1, w0
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 680]
	sub	w0, w0, w1
	mov	w1, w0
	ldr	w0, [x29, 44]
	udiv	w0, w0, w1
	str	w0, [x29, 44]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	mov	w1, w0
	ldr	w0, [x29, 44]
	add	w0, w0, w1
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	uxth	w0, w0
	mov	w2, 3
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	gc_all_cnt
.L902:
	ldp	x29, x30, [sp], 80
	ret
	.size	gc_all_base_on_free_blks, .-gc_all_base_on_free_blks
	.section	.rodata
	.align	3
.LC154:
	.string	"[ND][NE] gc_win_block gc_one error \n"
	.text
	.align	2
	.global	gc_win_block
	.type	gc_win_block, %function
gc_win_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	wzr, [x29, 44]
	ldrh	w0, [x29, 22]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	mul	w0, w0, w1
	str	w0, [x29, 40]
	b	.L906
.L910:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	cmp	x0, 0
	bne	.L907
	mov	w0, 0
	b	.L908
.L907:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 44]
	add	w0, w0, w1
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	sub	w0, w0, #1
	str	w0, [x29, 44]
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	cmp	w0, 0
	beq	.L906
	ldr	w0, [x29, 36]
	cmp	w0, 1
	bne	.L909
	adrp	x0, .LC154
	add	x0, x0, :lo12:.LC154
	bl	nand_print_dbg
.L909:
	ldr	w0, [x29, 36]
	b	.L908
.L906:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 40]
	cmp	w1, w0
	blt	.L910
	mov	w0, 0
.L908:
	ldp	x29, x30, [sp], 48
	ret
	.size	gc_win_block, .-gc_win_block
	.align	2
	.global	get_total_invalid_page
	.type	get_total_invalid_page, %function
get_total_invalid_page:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	wzr, [sp, 20]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	b	.L912
.L913:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [sp, 20]
	add	w0, w0, w1
	str	w0, [sp, 20]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L912:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L913
	ldr	w0, [sp, 20]
	add	sp, sp, 32
	ret
	.size	get_total_invalid_page, .-get_total_invalid_page
	.align	2
	.global	is_current_blcok
	.type	is_current_blcok, %function
is_current_blcok:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	str	wzr, [sp, 28]
	ldr	x0, [sp, 8]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L916
	ldr	x0, [sp, 8]
	ldr	x1, [x0, 592]
	ldr	x0, [sp]
	cmp	x1, x0
	beq	.L917
	ldr	x0, [sp, 8]
	ldr	x1, [x0, 528]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L919
.L917:
	mov	w0, 1
	str	w0, [sp, 28]
	b	.L919
.L916:
	ldr	x0, [sp, 8]
	ldr	x1, [x0, 528]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L919
	mov	w0, 1
	str	w0, [sp, 28]
.L919:
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	is_current_blcok, .-is_current_blcok
	.align	2
	.global	static_wear_leveling
	.type	static_wear_leveling, %function
static_wear_leveling:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bhi	.L922
	mov	w0, 0
	b	.L923
.L922:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 668]
	cmp	w0, 19
	bls	.L924
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 180]
	add	w1, w1, 1
	str	w1, [x0, 180]
	ldr	x0, [x29, 24]
	bl	get_block_min_count_from_invalid_list
	str	x0, [x29, 40]
	mov	w2, 5
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	cmp	w0, 0
	bne	.L925
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 672]
	add	w1, w0, 4
	ldr	x0, [x29, 24]
	str	w1, [x0, 672]
.L925:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 672]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L924
	ldr	x0, [x29, 24]
	str	wzr, [x0, 668]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 672]
.L924:
	mov	w0, 0
.L923:
	ldp	x29, x30, [sp], 48
	ret
	.size	static_wear_leveling, .-static_wear_leveling
	.section	.rodata
	.align	3
.LC155:
	.string	"[ND]not power on gc [%d, %d, %d], [%d %d %d]!\n"
	.align	3
.LC156:
	.string	"[ND]emergency power on gc %d !\n"
	.align	3
.LC157:
	.string	"[ND]power on gc %d!\n"
	.text
	.align	2
	.global	power_on_gc
	.type	power_on_gc, %function
power_on_gc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bcc	.L927
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w7, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w8, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w4, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	udiv	w2, w0, w1
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 40]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	udiv	w1, w0, w1
	adrp	x0, .LC155
	add	x0, x0, :lo12:.LC155
	mov	w6, w1
	mov	w5, w2
	mov	w2, w8
	mov	w1, w7
	bl	nand_print_dbg
	mov	w0, 0
	b	.L928
.L927:
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 2
	str	w1, [x0]
	strh	wzr, [x29, 46]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #10
	strh	w0, [x29, 44]
	b	.L929
.L932:
	ldrh	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 8
	beq	.L937
	ldrh	w0, [x29, 46]
	add	w0, w0, 1
	strh	w0, [x29, 46]
.L929:
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bcc	.L932
	b	.L931
.L937:
	nop
.L931:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 3
	strh	w0, [x29, 44]
	b	.L933
.L936:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC156
	add	x0, x0, :lo12:.LC156
	bl	nand_print_dbg
	ldrh	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 8
	beq	.L938
	ldrh	w0, [x29, 46]
	add	w0, w0, 1
	strh	w0, [x29, 46]
.L933:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	cmp	w0, 4
	bls	.L936
	b	.L935
.L938:
	nop
.L935:
	ldrh	w1, [x29, 46]
	adrp	x0, .LC157
	add	x0, x0, :lo12:.LC157
	bl	nand_print_dbg
	mov	w0, 0
.L928:
	ldp	x29, x30, [sp], 48
	ret
	.size	power_on_gc, .-power_on_gc
	.align	2
	.global	all_time_gc
	.type	all_time_gc, %function
all_time_gc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #3
	strh	w0, [x29, 46]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	cmp	w1, w0
	bcs	.L940
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	beq	.L940
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 1
	str	w1, [x0]
	ldrh	w1, [x29, 46]
	ldr	x0, [x29, 24]
	bl	gc_one
.L940:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	all_time_gc, .-all_time_gc
	.section	.rodata
	.align	3
.LC158:
	.string	"[ND]%s %p %d %d %d %\n"
	.align	3
.LC159:
	.string	"[NI]shutdown_on_gc end! [%d %d %d]\n"
	.align	3
.LC160:
	.string	"[ND]shutdown_on_gc one ok! [%d %d %d]\n"
	.text
	.align	2
	.global	shutdown_on_gc
	.type	shutdown_on_gc, %function
shutdown_on_gc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 4
	str	w1, [x0]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w4, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w5, w0
	adrp	x0, __func__.6859
	add	x1, x0, :lo12:__func__.6859
	adrp	x0, .LC158
	add	x0, x0, :lo12:.LC158
	mov	w6, w5
	mov	w5, w4
	mov	w4, w3
	mov	w3, w2
	ldr	x2, [x29, 40]
	bl	nand_print_dbg
	b	.L943
.L946:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L944
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w3, w0
	adrp	x0, .LC159
	add	x0, x0, :lo12:.LC159
	bl	nand_print
	b	.L945
.L944:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	beq	.L943
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	gc_one
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w3, w0
	adrp	x0, .LC160
	add	x0, x0, :lo12:.LC160
	bl	nand_print_dbg
.L943:
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	cmp	w1, w0
	bls	.L946
.L945:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	shutdown_on_gc, .-shutdown_on_gc
	.section	.rodata
	.align	3
.LC161:
	.string	"%d %d %d %d %d %d/%d %d/%d\n"
	.align	3
.LC162:
	.string	"%08d:%04d "
	.text
	.align	2
	.global	nftl_get_gc_info
	.type	nftl_get_gc_info, %function
nftl_get_gc_info:
	sub	sp, sp, #128
	stp	x29, x30, [sp, 32]
	add	x29, sp, 32
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	wzr, [x29, 92]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	str	x0, [x29, 56]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 76]
	str	w0, [x29, 80]
	ldr	w0, [x29, 80]
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	str	w0, [x29, 84]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 352]
	str	x0, [x29, 64]
	b	.L949
.L951:
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	cmp	w1, w0
	bls	.L950
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 80]
	add	w0, w0, w1
	str	w0, [x29, 80]
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L950:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 76]
	add	w0, w0, w1
	str	w0, [x29, 76]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
.L949:
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L951
	ldrsw	x0, [x29, 92]
	ldr	x1, [x29, 32]
	add	x8, x1, x0
	ldr	w1, [x29, 28]
	ldr	w0, [x29, 92]
	sub	w0, w1, w0
	mov	w9, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 30]
	mov	w10, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 680]
	mov	w4, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 682]
	mov	w5, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w2, [x0, 120]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w3, [x0, 124]
	adrp	x0, .LC161
	add	x1, x0, :lo12:.LC161
	ldr	w0, [x29, 76]
	str	w0, [sp, 24]
	ldr	w0, [x29, 80]
	str	w0, [sp, 16]
	ldr	w0, [x29, 84]
	str	w0, [sp, 8]
	ldr	w0, [x29, 88]
	str	w0, [sp]
	mov	w7, w3
	mov	w6, w2
	mov	w3, w10
	mov	x2, x1
	mov	w1, w9
	mov	x0, x8
	bl	nand_snprint
	str	w0, [x29, 92]
	str	wzr, [x29, 88]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 352]
	str	x0, [x29, 64]
	b	.L952
.L955:
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L953
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	cmp	w0, 0
	beq	.L953
	ldrsw	x0, [x29, 92]
	ldr	x1, [x29, 32]
	add	x5, x1, x0
	ldr	w1, [x29, 28]
	ldr	w0, [x29, 92]
	sub	w0, w1, w0
	mov	w1, w0
	ldr	x0, [x29, 64]
	ldrh	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	mov	w3, w0
	adrp	x0, .LC162
	add	x0, x0, :lo12:.LC162
	mov	w4, w3
	mov	w3, w2
	mov	x2, x0
	mov	x0, x5
	bl	nand_snprint
	mov	w1, w0
	ldr	w0, [x29, 92]
	add	w0, w0, w1
	str	w0, [x29, 92]
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	and	w0, w0, 7
	cmp	w0, 0
	bne	.L954
	ldrsw	x0, [x29, 92]
	sub	x0, x0, #1
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	mov	w1, 10
	strb	w1, [x0]
.L954:
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
.L952:
	ldr	w1, [x29, 28]
	ldr	w0, [x29, 92]
	sub	w0, w1, w0
	cmp	w0, 1
	bne	.L955
.L953:
	ldrsw	x0, [x29, 92]
	sub	x0, x0, #1
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	mov	w1, 10
	strb	w1, [x0]
	ldr	w0, [x29, 92]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 96
	ret
	.size	nftl_get_gc_info, .-nftl_get_gc_info
	.align	2
	.global	_nftl_nand_copy_page
	.type	_nftl_nand_copy_page, %function
_nftl_nand_copy_page:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	x3, [x29, 32]
	strh	w4, [x29, 30]
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	add	x0, x29, 88
	mov	x5, 0
	mov	x4, 0
	mov	w3, 0
	ldrh	w2, [x29, 30]
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldrh	w1, [x0]
	add	x0, x29, 64
	mov	x5, 0
	mov	x4, 0
	mov	w3, 0
	ldrh	w2, [x29, 30]
	bl	set_physic_op_par
	str	wzr, [x29, 136]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 28]
	strh	w0, [x29, 92]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 28]
	strh	w0, [x29, 68]
	ldr	x0, [x29, 32]
	str	x0, [x29, 96]
	ldr	x0, [x29, 32]
	str	x0, [x29, 72]
	add	x0, x29, 112
	str	x0, [x29, 104]
	add	x0, x29, 112
	str	x0, [x29, 80]
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 88
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 140]
	ldr	w0, [x29, 140]
	cmp	w0, 10
	bne	.L958
	str	wzr, [x29, 140]
.L958:
	add	x0, x29, 112
	bl	get_special_data_from_oob
	str	w0, [x29, 132]
	ldr	x0, [x29, 48]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	add	x0, x29, 112
	mov	w4, w1
	mov	w3, w2
	ldr	w2, [x29, 132]
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	set_oob_special_page
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3472]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 128]
	ldr	w0, [x29, 128]
	lsl	w1, w0, 8
	ldr	w0, [x29, 140]
	orr	w0, w1, w0
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	ldp	x29, x30, [sp], 144
	ret
	.size	_nftl_nand_copy_page, .-_nftl_nand_copy_page
	.align	2
	.global	set_spare_data
	.type	set_spare_data, %function
set_spare_data:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	strb	w2, [sp, 3]
	strb	w3, [sp, 2]
	str	wzr, [sp, 28]
	b	.L961
.L962:
	ldrb	w1, [sp, 2]
	ldr	w0, [sp, 28]
	sub	w0, w1, w0
	str	w0, [sp, 24]
	ldr	w0, [sp, 24]
	sub	w0, w0, #1
	str	w0, [sp, 24]
	ldr	w0, [sp, 24]
	lsl	w0, w0, 3
	str	w0, [sp, 24]
	ldrb	w1, [sp, 3]
	ldr	w0, [sp, 28]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldr	w1, [sp, 24]
	ldr	w2, [sp, 4]
	lsr	w1, w2, w1
	uxtb	w1, w1
	strb	w1, [x0]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L961:
	ldrb	w1, [sp, 2]
	ldr	w0, [sp, 28]
	cmp	w1, w0
	bhi	.L962
	nop
	add	sp, sp, 32
	ret
	.size	set_spare_data, .-set_spare_data
	.align	2
	.global	get_spare_data
	.type	get_spare_data, %function
get_spare_data:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strb	w1, [sp, 7]
	strb	w2, [sp, 6]
	str	wzr, [sp, 24]
	str	wzr, [sp, 28]
	b	.L964
.L965:
	ldr	w0, [sp, 28]
	lsl	w0, w0, 8
	str	w0, [sp, 28]
	ldrb	w1, [sp, 7]
	ldr	w0, [sp, 24]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [sp, 28]
	orr	w0, w0, w1
	str	w0, [sp, 28]
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L964:
	ldrb	w1, [sp, 6]
	ldr	w0, [sp, 24]
	cmp	w1, w0
	bhi	.L965
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	get_spare_data, .-get_spare_data
	.align	2
	.global	set_oob_logic_page
	.type	set_oob_logic_page, %function
set_oob_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	strh	w4, [x29, 22]
	ldr	x0, [x29, 32]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 4
	mov	w2, 1
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	ldrh	w0, [x29, 22]
	mov	w3, 2
	mov	w2, 5
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 4
	mov	w2, 7
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldr	x1, [x29, 32]
	add	x1, x1, 1
	ldrb	w1, [x1]
	orr	w1, w1, -64
	uxtb	w1, w1
	strb	w1, [x0]
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	set_oob_logic_page, .-set_oob_logic_page
	.align	2
	.global	set_oob_special_page
	.type	set_oob_special_page, %function
set_oob_special_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	strh	w4, [x29, 22]
	ldr	x0, [x29, 32]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 4
	mov	w2, 1
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	ldrh	w0, [x29, 22]
	mov	w3, 2
	mov	w2, 5
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 4
	mov	w2, 7
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	set_oob_special_page, .-set_oob_special_page
	.align	2
	.global	set_no_use_page
	.type	set_no_use_page, %function
set_no_use_page:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 1
	mov	w2, 1
	mov	w1, 170
	ldr	x0, [x29, 16]
	bl	set_spare_data
	mov	w3, 1
	mov	w2, 2
	mov	w1, 187
	ldr	x0, [x29, 16]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	set_no_use_page, .-set_no_use_page
	.align	2
	.global	set_start_block_flag
	.type	set_start_block_flag, %function
set_start_block_flag:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	ldr	x0, [x29, 32]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 1
	mov	w2, 1
	mov	w1, 170
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 1
	mov	w2, 2
	mov	w1, 238
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 4
	mov	w2, 3
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	set_start_block_flag, .-set_start_block_flag
	.align	2
	.global	set_end_block_flag
	.type	set_end_block_flag, %function
set_end_block_flag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 1
	mov	w2, 1
	mov	w1, 170
	ldr	x0, [x29, 16]
	bl	set_spare_data
	mov	w3, 1
	mov	w2, 2
	mov	w1, 204
	ldr	x0, [x29, 16]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	set_end_block_flag, .-set_end_block_flag
	.align	2
	.global	get_bad_flag_from_oob
	.type	get_bad_flag_from_oob, %function
get_bad_flag_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L973
	mov	w0, 0
	b	.L974
.L973:
	mov	w0, 1
.L974:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_bad_flag_from_oob, .-get_bad_flag_from_oob
	.align	2
	.global	get_block_used_count_from_oob
	.type	get_block_used_count_from_oob, %function
get_block_used_count_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 7
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_block_used_count_from_oob, .-get_block_used_count_from_oob
	.align	2
	.global	get_logic_page_from_oob
	.type	get_logic_page_from_oob, %function
get_logic_page_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	and	w0, w0, 268435455
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_logic_page_from_oob, .-get_logic_page_from_oob
	.align	2
	.global	get_special_data_from_oob
	.type	get_special_data_from_oob, %function
get_special_data_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_special_data_from_oob, .-get_special_data_from_oob
	.align	2
	.global	get_erase_count_from_oob
	.type	get_erase_count_from_oob, %function
get_erase_count_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 2
	mov	w1, 5
	ldr	x0, [x29, 24]
	bl	get_spare_data
	strh	w0, [x29, 46]
	ldrh	w0, [x29, 46]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_erase_count_from_oob, .-get_erase_count_from_oob
	.align	2
	.global	is_ftl_start_flag_page
	.type	is_ftl_start_flag_page, %function
is_ftl_start_flag_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L984
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 238
	bne	.L984
	mov	w0, 1
	b	.L985
.L984:
	mov	w0, 0
.L985:
	add	sp, sp, 16
	ret
	.size	is_ftl_start_flag_page, .-is_ftl_start_flag_page
	.align	2
	.global	is_ftl_end_flag_page
	.type	is_ftl_end_flag_page, %function
is_ftl_end_flag_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L987
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 204
	bne	.L987
	mov	w0, 1
	b	.L988
.L987:
	mov	w0, 0
.L988:
	add	sp, sp, 16
	ret
	.size	is_ftl_end_flag_page, .-is_ftl_end_flag_page
	.align	2
	.global	is_ftl_special_data
	.type	is_ftl_special_data, %function
is_ftl_special_data:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L990
	mov	w0, 1
	b	.L991
.L990:
	mov	w0, 0
.L991:
	add	sp, sp, 16
	ret
	.size	is_ftl_special_data, .-is_ftl_special_data
	.align	2
	.global	is_ftl_logic_page_data
	.type	is_ftl_logic_page_data, %function
is_ftl_logic_page_data:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	and	w0, w0, 240
	cmp	w0, 192
	bne	.L993
	mov	w0, 1
	b	.L994
.L993:
	mov	w0, 0
.L994:
	add	sp, sp, 16
	ret
	.size	is_ftl_logic_page_data, .-is_ftl_logic_page_data
	.align	2
	.global	is_nouse_page
	.type	is_nouse_page, %function
is_nouse_page:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	wzr, [sp, 24]
	str	wzr, [sp, 28]
	b	.L996
.L998:
	ldr	w0, [sp, 28]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L997
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L997:
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L996:
	ldr	w0, [sp, 28]
	cmp	w0, 9
	bls	.L998
	ldr	w0, [sp, 24]
	cmp	w0, 8
	bls	.L999
	ldr	x0, [sp, 8]
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L999
	mov	w0, 1
	b	.L1000
.L999:
	mov	w0, 0
.L1000:
	add	sp, sp, 32
	ret
	.size	is_nouse_page, .-is_nouse_page
	.align	2
	.global	is_fill_page
	.type	is_fill_page, %function
is_fill_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	mov	w0, 1431655765
	cmp	w1, w0
	bne	.L1002
	mov	w0, 1
	b	.L1003
.L1002:
	mov	w0, 0
.L1003:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_fill_page, .-is_fill_page
	.align	2
	.global	is_phy_mapping_page
	.type	is_phy_mapping_page, %function
is_phy_mapping_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	mov	w0, -1431633921
	cmp	w1, w0
	bne	.L1005
	mov	w0, 1
	b	.L1006
.L1005:
	mov	w0, 0
.L1006:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_phy_mapping_page, .-is_phy_mapping_page
	.align	2
	.global	is_function_info_page
	.type	is_function_info_page, %function
is_function_info_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	and	w1, w0, -65536
	mov	w0, -1441726464
	cmp	w1, w0
	bne	.L1008
	mov	w0, 1
	b	.L1009
.L1008:
	mov	w0, 0
.L1009:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_function_info_page, .-is_function_info_page
	.align	2
	.global	is_power_down_info_page
	.type	is_power_down_info_page, %function
is_power_down_info_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	mov	w0, 13107
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L1011
	mov	w0, 1
	b	.L1012
.L1011:
	mov	w0, 0
.L1012:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_power_down_info_page, .-is_power_down_info_page
	.section	.rodata
	.align	3
.LC163:
	.string	"[NE]%s:logic_page:0x%x >= total_pages:0x%x\n"
	.text
	.align	2
	.global	get_logic_page_map
	.type	get_logic_page_map, %function
get_logic_page_map:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 36]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bhi	.L1014
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 36]
	adrp	x0, __func__.7007
	add	x1, x0, :lo12:__func__.7007
	adrp	x0, .LC163
	add	x0, x0, :lo12:.LC163
	mov	w3, w2
	ldr	w2, [x29, 20]
	bl	nand_print
	str	xzr, [x29, 48]
	ldr	x0, [x29, 48]
	b	.L1015
.L1014:
	mov	w0, 262144
	str	w0, [x29, 44]
	str	wzr, [x29, 60]
	b	.L1016
.L1017:
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	str	w0, [x29, 20]
.L1016:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bcs	.L1017
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	str	x0, [x29, 32]
	ldr	w0, [x29, 20]
	lsl	x0, x0, 2
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
.L1015:
	ldp	x29, x30, [sp], 64
	ret
	.size	get_logic_page_map, .-get_logic_page_map
	.section	.rodata
	.align	3
.LC164:
	.string	"[NE]%s:malloc fail for logic_page_map %d\n"
	.align	3
.LC165:
	.string	"[NE]%s: malloc fail for logic_page_map %d\n"
	.align	3
.LC166:
	.string	"[NE]zone_param_init malloc_logic_page_map error %d\n"
	.text
	.align	2
	.global	malloc_logic_page_map
	.type	malloc_logic_page_map, %function
malloc_logic_page_map:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	mov	w0, 262144
	str	w0, [x29, 40]
	str	wzr, [x29, 44]
	b	.L1019
.L1020:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	str	xzr, [x0, 8]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1019:
	ldr	w0, [x29, 44]
	cmp	w0, 24
	bls	.L1020
	str	wzr, [x29, 44]
	b	.L1021
.L1028:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bcc	.L1022
	ldr	w0, [x29, 40]
	lsl	w0, w0, 2
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	bl	nftl_malloc
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	str	x2, [x0, 8]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L1023
	adrp	x0, .LC164
	add	x0, x0, :lo12:.LC164
	ldr	w1, [x29, 44]
	bl	nand_print
	mov	w0, -2
	b	.L1024
.L1023:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 40]
	sub	w0, w1, w0
	str	w0, [x29, 20]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 36]
	mov	w1, -1
	bl	nand_memset
	b	.L1030
.L1022:
	ldr	w0, [x29, 20]
	lsl	w0, w0, 2
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	bl	nftl_malloc
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	str	x2, [x0, 8]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L1026
	adrp	x0, .LC165
	add	x0, x0, :lo12:.LC165
	ldr	w1, [x29, 44]
	bl	nand_print
	mov	w0, -2
	b	.L1024
.L1026:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 36]
	mov	w1, -1
	bl	nand_memset
	str	wzr, [x29, 20]
	b	.L1027
.L1030:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1021:
	ldr	w0, [x29, 44]
	cmp	w0, 24
	bls	.L1028
.L1027:
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L1029
	adrp	x0, .LC166
	add	x0, x0, :lo12:.LC166
	ldr	w1, [x29, 20]
	bl	nand_print
	mov	w0, -2
	b	.L1024
.L1029:
	mov	w0, 0
.L1024:
	ldp	x29, x30, [sp], 48
	ret
	.size	malloc_logic_page_map, .-malloc_logic_page_map
	.align	2
	.global	free_logic_page_map
	.type	free_logic_page_map, %function
free_logic_page_map:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	b	.L1032
.L1034:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cmp	x0, 0
	beq	.L1033
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	bl	nftl_free
.L1033:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1032:
	ldr	w0, [x29, 44]
	cmp	w0, 24
	bls	.L1034
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	free_logic_page_map, .-free_logic_page_map
	.align	2
	.global	set_physic_op_par
	.type	set_physic_op_par, %function
set_physic_op_par:
	sub	sp, sp, #32
	str	x0, [sp, 24]
	strh	w1, [sp, 22]
	strh	w2, [sp, 20]
	strh	w3, [sp, 18]
	str	x4, [sp, 8]
	str	x5, [sp]
	ldr	x0, [sp, 24]
	ldrh	w1, [sp, 20]
	strh	w1, [x0]
	ldr	x0, [sp, 24]
	ldrh	w1, [sp, 22]
	strh	w1, [x0, 2]
	ldr	x0, [sp, 24]
	ldrh	w1, [sp, 18]
	strh	w1, [x0, 4]
	ldr	x0, [sp, 24]
	ldr	x1, [sp, 8]
	str	x1, [x0, 8]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	nop
	add	sp, sp, 32
	ret
	.size	set_physic_op_par, .-set_physic_op_par
	.align	2
	.global	add_free_list
	.type	add_free_list, %function
add_free_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	b	.L1038
.L1041:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldrh	w1, [x0, 6]
	ldr	x0, [sp]
	ldrh	w0, [x0, 6]
	cmp	w1, w0
	bcc	.L1039
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [sp]
	str	x1, [x0, 40]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [sp]
	str	x1, [x0, 32]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 32]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 40]
	mov	w0, 0
	b	.L1040
.L1039:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1038:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	bne	.L1041
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 32]
	ldr	x0, [sp]
	str	xzr, [x0, 32]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 40]
	mov	w0, 0
.L1040:
	add	sp, sp, 32
	ret
	.size	add_free_list, .-add_free_list
	.align	2
	.global	del_free_list
	.type	del_free_list, %function
del_free_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	cmp	x0, 0
	beq	.L1043
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [sp, 8]
	str	x1, [x0, 304]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [sp, 8]
	str	x1, [x0, 304]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	beq	.L1043
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [sp, 8]
	add	x1, x1, 272
	str	x1, [x0, 40]
.L1043:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_free_list, .-del_free_list
	.align	2
	.global	del_free_list_by_block
	.type	del_free_list_by_block, %function
del_free_list_by_block:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1046
.L1050:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1047
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 32]
	str	x1, [x0, 32]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	beq	.L1048
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 40]
	str	x1, [x0, 40]
.L1048:
	ldr	x0, [sp, 24]
	str	xzr, [x0, 40]
	ldr	x0, [sp, 24]
	str	xzr, [x0, 32]
	b	.L1049
.L1047:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1046:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1050
.L1049:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_free_list_by_block, .-del_free_list_by_block
	.align	2
	.global	is_block_in_free_list
	.type	is_block_in_free_list, %function
is_block_in_free_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1053
.L1056:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1054
	mov	w0, 1
	b	.L1055
.L1054:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1053:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1056
	mov	w0, 0
.L1055:
	add	sp, sp, 32
	ret
	.size	is_block_in_free_list, .-is_block_in_free_list
	.section	.rodata
	.align	3
.LC167:
	.string	"[ND]free list: \n"
	.align	3
.LC168:
	.string	"[ND]block NO:%4d; erase:%3d; "
	.align	3
.LC169:
	.string	"[ND]count:%4d; "
	.align	3
.LC170:
	.string	"[ND]info:0x%x\n"
	.text
	.align	2
	.global	print_free_list
	.type	print_free_list, %function
print_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 272
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1062
	adrp	x0, .LC167
	add	x0, x0, :lo12:.LC167
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
	b	.L1060
.L1061:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC168
	add	x0, x0, :lo12:.LC168
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 8]
	adrp	x0, .LC169
	add	x0, x0, :lo12:.LC169
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	adrp	x0, .LC170
	add	x0, x0, :lo12:.LC170
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
.L1060:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1061
	b	.L1057
.L1062:
	nop
.L1057:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_free_list, .-print_free_list
	.align	2
	.global	add_block_invalid_list
	.type	add_block_invalid_list, %function
add_block_invalid_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	b	.L1064
.L1067:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldrh	w1, [x0, 4]
	ldr	x0, [sp]
	ldrh	w0, [x0, 4]
	cmp	w1, w0
	bhi	.L1065
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldr	x1, [sp]
	str	x1, [x0, 24]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 16]
	ldr	x0, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	mov	w0, 0
	b	.L1066
.L1065:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1064:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1067
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	mov	w0, 0
.L1066:
	add	sp, sp, 32
	ret
	.size	add_block_invalid_list, .-add_block_invalid_list
	.align	2
	.global	del_block_invalid_list
	.type	del_block_invalid_list, %function
del_block_invalid_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	cmp	x0, 0
	beq	.L1069
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 16]
	ldr	x0, [sp, 8]
	str	x1, [x0, 352]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1069
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 8]
	add	x1, x1, 336
	str	x1, [x0, 24]
.L1069:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_invalid_list, .-del_block_invalid_list
	.align	2
	.global	del_block_invalid_list_by_block
	.type	del_block_invalid_list_by_block, %function
del_block_invalid_list_by_block:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1072
.L1076:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1073
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 16]
	str	x1, [x0, 16]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1074
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 24]
	str	x1, [x0, 24]
.L1074:
	ldr	x0, [sp, 24]
	str	xzr, [x0, 24]
	ldr	x0, [sp, 24]
	str	xzr, [x0, 16]
	b	.L1075
.L1073:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1072:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1076
.L1075:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_invalid_list_by_block, .-del_block_invalid_list_by_block
	.align	2
	.global	is_block_in_invalid_list
	.type	is_block_in_invalid_list, %function
is_block_in_invalid_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1079
.L1082:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1080
	mov	w0, 1
	b	.L1081
.L1080:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1079:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1082
	mov	w0, 0
.L1081:
	add	sp, sp, 32
	ret
	.size	is_block_in_invalid_list, .-is_block_in_invalid_list
	.section	.rodata
	.align	3
.LC171:
	.string	"[ND]invalid_page_count list: \n"
	.align	3
.LC172:
	.string	"[ND]count:%4d; invalid:%3d;"
	.text
	.align	2
	.global	print_block_invalid_list
	.type	print_block_invalid_list, %function
print_block_invalid_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 336
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1088
	adrp	x0, .LC171
	add	x0, x0, :lo12:.LC171
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	b	.L1086
.L1087:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC168
	add	x0, x0, :lo12:.LC168
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC172
	add	x0, x0, :lo12:.LC172
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	adrp	x0, .LC170
	add	x0, x0, :lo12:.LC170
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
.L1086:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1087
	b	.L1083
.L1088:
	nop
.L1083:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_block_invalid_list, .-print_block_invalid_list
	.align	2
	.global	add_block_count_list
	.type	add_block_count_list, %function
add_block_count_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 400
	str	x0, [sp, 24]
	b	.L1090
.L1093:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	w1, [x0, 8]
	ldr	x0, [sp]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	blt	.L1091
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	x1, [sp]
	str	x1, [x0, 56]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 48]
	ldr	x0, [sp]
	str	x1, [x0, 48]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 48]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 56]
	mov	w0, 0
	b	.L1092
.L1091:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
.L1090:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	cmp	x0, 0
	bne	.L1093
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 48]
	ldr	x0, [sp]
	str	xzr, [x0, 48]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 56]
	mov	w0, 0
.L1092:
	add	sp, sp, 32
	ret
	.size	add_block_count_list, .-add_block_count_list
	.align	2
	.global	del_block_count_list
	.type	del_block_count_list, %function
del_block_count_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 400
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	cmp	x0, 0
	beq	.L1095
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	x1, [x0, 48]
	ldr	x0, [sp, 24]
	str	x1, [x0, 48]
.L1095:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_count_list, .-del_block_count_list
	.align	2
	.global	del_block_count_by_block
	.type	del_block_count_by_block, %function
del_block_count_by_block:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 400
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
	b	.L1098
.L1102:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1099
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 56]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 48]
	str	x1, [x0, 48]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	cmp	x0, 0
	beq	.L1100
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 56]
	str	x1, [x0, 56]
.L1100:
	ldr	x0, [sp, 24]
	str	xzr, [x0, 56]
	ldr	x0, [sp, 24]
	str	xzr, [x0, 48]
	b	.L1101
.L1099:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
.L1098:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1102
.L1101:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_count_by_block, .-del_block_count_by_block
	.section	.rodata
	.align	3
.LC173:
	.string	"[ND]block_used_count list: \n"
	.align	3
.LC174:
	.string	"[ND]block NO: %d "
	.align	3
.LC175:
	.string	"[ND]block_used_count: %d \n"
	.text
	.align	2
	.global	print_block_count_list
	.type	print_block_count_list, %function
print_block_count_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 400
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1109
	adrp	x0, .LC173
	add	x0, x0, :lo12:.LC173
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 48]
	str	x0, [x29, 40]
	b	.L1107
.L1108:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC174
	add	x0, x0, :lo12:.LC174
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 8]
	adrp	x0, .LC175
	add	x0, x0, :lo12:.LC175
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 48]
	str	x0, [x29, 40]
.L1107:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1108
	b	.L1104
.L1109:
	nop
.L1104:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_block_count_list, .-print_block_count_list
	.section	.rodata
	.align	3
.LC176:
	.string	"[ND]free_list error !\n"
	.text
	.align	2
	.global	put_phy_block_to_free_list
	.type	put_phy_block_to_free_list, %function
put_phy_block_to_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 40]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	add_free_list
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L1111
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 30]
	b	.L1112
.L1111:
	adrp	x0, .LC176
	add	x0, x0, :lo12:.LC176
	bl	nand_print_dbg
.L1112:
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	put_phy_block_to_free_list, .-put_phy_block_to_free_list
	.align	2
	.global	out_phy_block_from_free_list
	.type	out_phy_block_from_free_list, %function
out_phy_block_from_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	del_free_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1115
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	cmp	w0, 0
	beq	.L1115
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 30]
.L1115:
	ldr	x0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_free_list, .-out_phy_block_from_free_list
	.align	2
	.global	out_phy_block_from_free_list_by_erase_time
	.type	out_phy_block_from_free_list_by_erase_time, %function
out_phy_block_from_free_list_by_erase_time:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	xzr, [x29, 56]
	strh	wzr, [x29, 54]
	ldr	x0, [x29, 24]
	add	x0, x0, 272
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
	b	.L1118
.L1120:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	ldrh	w1, [x29, 22]
	cmp	w1, w0
	bls	.L1119
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	ldrh	w1, [x29, 54]
	cmp	w1, w0
	bhi	.L1119
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	strh	w0, [x29, 54]
	ldr	x0, [x29, 40]
	str	x0, [x29, 56]
.L1119:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
.L1118:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1120
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L1121
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 56]
.L1121:
	ldr	x0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	out_phy_block_from_free_list_by_erase_time, .-out_phy_block_from_free_list_by_erase_time
	.align	2
	.global	get_max_erase_time_from_free_list
	.type	get_max_erase_time_from_free_list, %function
get_max_erase_time_from_free_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	xzr, [sp, 40]
	strh	wzr, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1124
.L1126:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1125
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bhi	.L1125
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1125:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1124:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1126
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1127
	ldr	x0, [sp, 40]
	ldrh	w0, [x0, 6]
	b	.L1128
.L1127:
	mov	w0, 65535
.L1128:
	add	sp, sp, 48
	ret
	.size	get_max_erase_time_from_free_list, .-get_max_erase_time_from_free_list
	.align	2
	.global	get_block_max_erase_time_from_free_list
	.type	get_block_max_erase_time_from_free_list, %function
get_block_max_erase_time_from_free_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	xzr, [sp, 40]
	strh	wzr, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1130
.L1132:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1131
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bhi	.L1131
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1131:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1130:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1132
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1133
	ldr	x0, [sp, 40]
	b	.L1134
.L1133:
	mov	x0, 0
.L1134:
	add	sp, sp, 48
	ret
	.size	get_block_max_erase_time_from_free_list, .-get_block_max_erase_time_from_free_list
	.align	2
	.global	get_block_max_erase_time_from_invalid_list
	.type	get_block_max_erase_time_from_invalid_list, %function
get_block_max_erase_time_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	xzr, [sp, 40]
	strh	wzr, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1136
.L1138:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1137
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bhi	.L1137
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1137:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1136:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1138
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1139
	ldr	x0, [sp, 40]
	b	.L1140
.L1139:
	mov	x0, 0
.L1140:
	add	sp, sp, 48
	ret
	.size	get_block_max_erase_time_from_invalid_list, .-get_block_max_erase_time_from_invalid_list
	.align	2
	.global	get_block_min_erase_time_from_invalid_list
	.type	get_block_min_erase_time_from_invalid_list, %function
get_block_min_erase_time_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	w2, [sp]
	str	xzr, [sp, 40]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1142
.L1144:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1143
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [sp]
	cmp	w1, w0
	bcs	.L1143
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bcc	.L1143
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1143:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1142:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1144
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1145
	ldr	x0, [sp, 40]
	b	.L1146
.L1145:
	mov	x0, 0
.L1146:
	add	sp, sp, 48
	ret
	.size	get_block_min_erase_time_from_invalid_list, .-get_block_min_erase_time_from_invalid_list
	.align	2
	.global	get_block_max_count_from_invalid_list
	.type	get_block_max_count_from_invalid_list, %function
get_block_max_count_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	str	xzr, [sp, 40]
	str	wzr, [sp, 36]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1148
.L1150:
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [sp, 36]
	cmp	w1, w0
	bcc	.L1149
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	str	w0, [sp, 36]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1149:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1148:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1150
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1151
	ldr	x0, [sp, 40]
	b	.L1152
.L1151:
	mov	x0, 0
.L1152:
	add	sp, sp, 48
	ret
	.size	get_block_max_count_from_invalid_list, .-get_block_max_count_from_invalid_list
	.align	2
	.global	get_block_min_count_from_invalid_list
	.type	get_block_min_count_from_invalid_list, %function
get_block_min_count_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	str	xzr, [sp, 40]
	mov	w0, -1
	str	w0, [sp, 36]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1154
.L1156:
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [sp, 36]
	cmp	w1, w0
	bhi	.L1155
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	str	w0, [sp, 36]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1155:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1154:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1156
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1157
	ldr	x0, [sp, 40]
	b	.L1158
.L1157:
	mov	x0, 0
.L1158:
	add	sp, sp, 48
	ret
	.size	get_block_min_count_from_invalid_list, .-get_block_min_count_from_invalid_list
	.align	2
	.global	out_phy_block_from_free_list_by_block
	.type	out_phy_block_from_free_list_by_block, %function
out_phy_block_from_free_list_by_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	del_free_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1160
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	cmp	w0, 0
	beq	.L1161
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 30]
.L1161:
	ldr	x0, [x29, 40]
	b	.L1162
.L1160:
	ldr	x0, [x29, 40]
.L1162:
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_free_list_by_block, .-out_phy_block_from_free_list_by_block
	.section	.rodata
	.align	3
.LC177:
	.string	"[ND]add invalid_list error !\n"
	.text
	.align	2
	.global	put_phy_block_to_invalid_page_list
	.type	put_phy_block_to_invalid_page_list, %function
put_phy_block_to_invalid_page_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 24]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	add_block_invalid_list
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1164
	adrp	x0, .LC177
	add	x0, x0, :lo12:.LC177
	bl	nand_print_dbg
.L1164:
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	put_phy_block_to_invalid_page_list, .-put_phy_block_to_invalid_page_list
	.align	2
	.global	out_phy_block_from_invalid_page_list
	.type	out_phy_block_from_invalid_page_list, %function
out_phy_block_from_invalid_page_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	del_block_invalid_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1167
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	b	.L1168
.L1167:
	ldr	x0, [x29, 40]
.L1168:
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_invalid_page_list, .-out_phy_block_from_invalid_page_list
	.align	2
	.global	out_phy_block_from_invalid_page_list_by_block
	.type	out_phy_block_from_invalid_page_list_by_block, %function
out_phy_block_from_invalid_page_list_by_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	del_block_invalid_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1170
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	b	.L1171
.L1170:
	ldr	x0, [x29, 40]
.L1171:
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_invalid_page_list_by_block, .-out_phy_block_from_invalid_page_list_by_block
	.section	.rodata
	.align	3
.LC178:
	.string	"[ND]invalid_page_count more than pages per blk 0 %d %d!!\n"
	.align	3
.LC179:
	.string	"[ND]invalid_page_count more than pages per blk 1!!\n"
	.text
	.align	2
	.global	phy_block_from_invalid_page_incr
	.type	phy_block_from_invalid_page_incr, %function
phy_block_from_invalid_page_incr:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1173
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	bne	.L1173
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1174
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC178
	add	x0, x0, :lo12:.LC178
	bl	nand_print_dbg
.L1174:
	mov	w0, 0
	b	.L1175
.L1173:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1176
	adrp	x0, .LC179
	add	x0, x0, :lo12:.LC179
	bl	nand_print_dbg
.L1176:
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	add	x1, x0, 336
	ldr	x0, [x29, 40]
	cmp	x1, x0
	bne	.L1178
	mov	w0, 0
	b	.L1175
.L1181:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 24]
	ldr	x0, [x29, 32]
	str	x1, [x0, 24]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x1, [x29, 32]
	str	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	str	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1179
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	ldr	x1, [x29, 40]
	str	x1, [x0, 24]
.L1179:
	ldr	x0, [x29, 32]
	ldr	x1, [x29, 40]
	str	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x1, [x29, 32]
	str	x1, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	beq	.L1182
.L1178:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 4]
	cmp	w1, w0
	bcc	.L1181
	b	.L1180
.L1182:
	nop
.L1180:
	mov	w0, 0
.L1175:
	ldp	x29, x30, [sp], 48
	ret
	.size	phy_block_from_invalid_page_incr, .-phy_block_from_invalid_page_incr
	.align	2
	.global	adjust_invaild_list
	.type	adjust_invaild_list, %function
adjust_invaild_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	b	.L1184
.L1187:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bne	.L1189
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_invalid_page_list
	str	x0, [x29, 40]
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
.L1184:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	cmp	x0, 0
	bne	.L1187
	b	.L1186
.L1189:
	nop
.L1186:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	adjust_invaild_list, .-adjust_invaild_list
	.align	2
	.global	nftl_get_boot_cnt
	.type	nftl_get_boot_cnt, %function
nftl_get_boot_cnt:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 140]
	add	sp, sp, 32
	ret
	.size	nftl_get_boot_cnt, .-nftl_get_boot_cnt
	.align	2
	.global	nftl_get_zone_cap
	.type	nftl_get_zone_cap, %function
nftl_get_zone_cap:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 32]
	add	sp, sp, 32
	ret
	.size	nftl_get_zone_cap, .-nftl_get_zone_cap
	.align	2
	.global	nftl_set_zone_test
	.type	nftl_set_zone_test, %function
nftl_set_zone_test:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	w1, [x29, 20]
	str	w1, [x0, 656]
	ldr	x0, [x29, 40]
	bl	print_nftl_zone
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_set_zone_test, .-nftl_set_zone_test
	.align	2
	.global	nftl_get_zone_priv
	.type	nftl_get_zone_priv, %function
nftl_get_zone_priv:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0]
	add	sp, sp, 32
	ret
	.size	nftl_get_zone_priv, .-nftl_get_zone_priv
	.align	2
	.global	nftl_get_zone_write_cache_nums
	.type	nftl_get_zone_write_cache_nums, %function
nftl_get_zone_write_cache_nums:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 1076]
	add	sp, sp, 32
	ret
	.size	nftl_get_zone_write_cache_nums, .-nftl_get_zone_write_cache_nums
	.section	.rodata
	.align	3
.LC180:
	.string	"[ND]smart->version:%x;\n"
	.align	3
.LC181:
	.string	"[ND]smart->total_recv_read_sectors:%lld;\n"
	.align	3
.LC182:
	.string	"[ND]smart->total_recv_write_sectors:%lld;\n"
	.align	3
.LC183:
	.string	"[ND]smart->total_real_read_pages:%lld;\n"
	.align	3
.LC184:
	.string	"[ND]smart->total_real_write_pages:%lld;\n"
	.align	3
.LC185:
	.string	"[ND]smart->total_recv_discard_sectors:%lld;\n"
	.align	3
.LC186:
	.string	"[ND]smart->total_real_discard_sectors:%lld;\n"
	.align	3
.LC187:
	.string	"[ND]smart->total_recv_read_claim_pages:%d;\n"
	.align	3
.LC188:
	.string	"[ND]smart->total_gc_times:%d;\n"
	.align	3
.LC189:
	.string	"[ND]smart->total_gc_pages:%d;\n"
	.align	3
.LC190:
	.string	"[ND]smart->total_wl_times:%d;\n"
	.align	3
.LC191:
	.string	"[ND]smart->total_wl_gc_times:%d;\n"
	.align	3
.LC192:
	.string	"[ND]smart->total_real_read_error_pages:%d;\n"
	.align	3
.LC193:
	.string	"[ND]smart->total_real_write_error_pages:%d;\n"
	.align	3
.LC194:
	.string	"[ND]smart->total_normal_power_cycles:%x;\n"
	.align	3
.LC195:
	.string	"[ND]smart->total_unusual_power_cycles:%x;\n"
	.align	3
.LC196:
	.string	"[ND]smart->max_block_erase_times:%d;\n"
	.align	3
.LC197:
	.string	"[ND]smart->min_block_erase_times:%d;\n"
	.align	3
.LC198:
	.string	"[ND]smart->max_block_used_counter:%d;\n"
	.align	3
.LC199:
	.string	"[ND]smart->min_block_used_counter:%d;\n"
	.align	3
.LC200:
	.string	"[ND]smart->read_reclaim_utc:%d;\n"
	.align	3
.LC201:
	.string	"[ND]smart->cross_talk_times:%d;\n"
	.align	3
.LC202:
	.string	"[ND]smart->read_reclaim_zone_no:%d;\n"
	.align	3
.LC203:
	.string	"[ND]smart->read_reclaim_page_no:%d;\n"
	.text
	.align	2
	.global	print_smart
	.type	print_smart, %function
print_smart:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1203
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 112]
	adrp	x0, .LC180
	add	x0, x0, :lo12:.LC180
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 64]
	adrp	x0, .LC181
	add	x0, x0, :lo12:.LC181
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 72]
	adrp	x0, .LC182
	add	x0, x0, :lo12:.LC182
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 80]
	adrp	x0, .LC183
	add	x0, x0, :lo12:.LC183
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 88]
	adrp	x0, .LC184
	add	x0, x0, :lo12:.LC184
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 96]
	adrp	x0, .LC185
	add	x0, x0, :lo12:.LC185
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 104]
	adrp	x0, .LC186
	add	x0, x0, :lo12:.LC186
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 116]
	adrp	x0, .LC187
	add	x0, x0, :lo12:.LC187
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 120]
	adrp	x0, .LC188
	add	x0, x0, :lo12:.LC188
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 124]
	adrp	x0, .LC189
	add	x0, x0, :lo12:.LC189
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 128]
	adrp	x0, .LC190
	add	x0, x0, :lo12:.LC190
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 180]
	adrp	x0, .LC191
	add	x0, x0, :lo12:.LC191
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 132]
	adrp	x0, .LC192
	add	x0, x0, :lo12:.LC192
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 136]
	adrp	x0, .LC193
	add	x0, x0, :lo12:.LC193
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 140]
	adrp	x0, .LC194
	add	x0, x0, :lo12:.LC194
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 144]
	adrp	x0, .LC195
	add	x0, x0, :lo12:.LC195
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 148]
	adrp	x0, .LC196
	add	x0, x0, :lo12:.LC196
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 152]
	adrp	x0, .LC197
	add	x0, x0, :lo12:.LC197
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 156]
	adrp	x0, .LC198
	add	x0, x0, :lo12:.LC198
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 160]
	adrp	x0, .LC199
	add	x0, x0, :lo12:.LC199
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 164]
	adrp	x0, .LC200
	add	x0, x0, :lo12:.LC200
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 168]
	adrp	x0, .LC201
	add	x0, x0, :lo12:.LC201
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 172]
	adrp	x0, .LC202
	add	x0, x0, :lo12:.LC202
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 176]
	adrp	x0, .LC203
	add	x0, x0, :lo12:.LC203
	bl	nand_print_dbg
	b	.L1200
.L1203:
	nop
.L1200:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_smart, .-print_smart
	.section	.rodata
	.align	3
.LC204:
	.string	"%08x: "
	.text
	.align	2
	.global	dump_buf
	.type	dump_buf, %function
dump_buf:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 44]
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print
	str	wzr, [x29, 44]
	b	.L1205
.L1208:
	ldr	w0, [x29, 44]
	and	w0, w0, 15
	cmp	w0, 0
	bne	.L1206
	adrp	x0, .LC204
	add	x0, x0, :lo12:.LC204
	ldr	w1, [x29, 44]
	bl	nand_print
.L1206:
	ldrsw	x0, [x29, 44]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC81
	add	x0, x0, :lo12:.LC81
	bl	nand_print
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	and	w0, w0, 15
	cmp	w0, 0
	bne	.L1207
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print
.L1207:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1205:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L1208
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	dump_buf, .-dump_buf
	.section	.rodata
	.align	3
.LC205:
	.string	"[NE]exception case wb %04x %04x, len: %d\n"
	.text
	.align	2
	.type	crc_wrong, %function
crc_wrong:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	strh	wzr, [x29, 58]
	strh	wzr, [x29, 56]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	lsl	w0, w0, 9
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	bl	is_nouse_page
	cmp	w0, 0
	beq	.L1210
	mov	w0, 0
	b	.L1211
.L1210:
	ldr	x0, [x29, 24]
	bl	is_phy_mapping_page
	cmp	w0, 0
	beq	.L1212
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 560]
	str	w0, [x29, 60]
.L1212:
	ldr	w2, [x29, 60]
	ldr	x1, [x29, 32]
	mov	w0, -1
	bl	nand_crc16
	strh	w0, [x29, 58]
	ldr	x0, [x29, 24]
	add	x0, x0, 13
	ldrb	w0, [x0]
	sxth	w1, w0
	ldr	x0, [x29, 24]
	add	x0, x0, 12
	ldrb	w0, [x0]
	lsl	w0, w0, 8
	sxth	w0, w0
	orr	w0, w1, w0
	sxth	w0, w0
	strh	w0, [x29, 56]
	ldrh	w1, [x29, 58]
	ldrh	w0, [x29, 56]
	cmp	w1, w0
	beq	.L1213
	ldrh	w1, [x29, 58]
	ldrh	w2, [x29, 56]
	adrp	x0, .LC205
	add	x0, x0, :lo12:.LC205
	ldr	w3, [x29, 60]
	bl	nand_print
	mov	w0, 1
	b	.L1211
.L1213:
	mov	w0, 0
.L1211:
	ldp	x29, x30, [sp], 64
	ret
	.size	crc_wrong, .-crc_wrong
	.section	.rodata
	.align	3
.LC206:
	.string	"[NE]%s:malloc fail for temp_page_buf\n"
	.align	3
.LC207:
	.string	"[NE]%s:malloc fail for logic_page_buf\n"
	.align	3
.LC208:
	.string	"[NE]zone_param_init error2\n"
	.align	3
.LC209:
	.string	"[NE]%s:malloc fail for current_block.user_info.buf\n"
	.align	3
.LC210:
	.string	"[NE]%s:malloc fail for assist_block.user_info.buf\n"
	.align	3
.LC211:
	.string	"[NE]%s:malloc fail for zone_phy_page_map_for_gc.buf\n"
	.text
	.align	2
	.global	zone_param_init
	.type	zone_param_init, %function
zone_param_init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	strh	w1, [x29, 38]
	str	w2, [x29, 32]
	str	w3, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	cmp	x0, 0
	bne	.L1215
	adrp	x0, __func__.7328
	add	x1, x0, :lo12:__func__.7328
	adrp	x0, .LC206
	add	x0, x0, :lo12:.LC206
	bl	nand_print
	mov	w0, -2
	b	.L1216
.L1215:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 520]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 520]
	cmp	x0, 0
	bne	.L1217
	adrp	x0, __func__.7328
	add	x1, x0, :lo12:__func__.7328
	adrp	x0, .LC207
	add	x0, x0, :lo12:.LC207
	bl	nand_print
	mov	w0, -2
	b	.L1216
.L1217:
	ldr	x0, [x29, 40]
	ldr	w1, [x29, 32]
	str	w1, [x0, 32]
	ldr	x0, [x29, 40]
	ldr	w1, [x29, 28]
	str	w1, [x0, 40]
	ldr	x0, [x29, 40]
	ldrh	w1, [x29, 38]
	strh	w1, [x0, 44]
	ldrh	w1, [x29, 38]
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 48]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 26]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 28]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 30]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 56]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 56]
	ldr	w1, [x29, 56]
	ldr	x0, [x29, 40]
	bl	malloc_logic_page_map
	cmp	w0, 0
	beq	.L1218
	adrp	x0, .LC208
	add	x0, x0, :lo12:.LC208
	bl	nand_print
	mov	w0, -2
	b	.L1216
.L1218:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsl	w0, w0, 2
	mov	w1, w0
	ldr	x0, [x29, 40]
	str	w1, [x0, 560]
	ldr	x0, [x29, 40]
	mov	w1, 320
	str	w1, [x0, 576]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 560]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 576]
	add	w1, w1, w0
	ldr	x0, [x29, 40]
	str	w1, [x0, 544]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 544]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 536]
	cmp	x0, 0
	bne	.L1219
	adrp	x0, __func__.7328
	add	x1, x0, :lo12:__func__.7328
	adrp	x0, .LC209
	add	x0, x0, :lo12:.LC209
	bl	nand_print
	mov	w0, -2
	b	.L1216
.L1219:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 536]
	ldr	x0, [x29, 40]
	str	x1, [x0, 552]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 560]
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 568]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 568]
	ldr	x0, [x29, 40]
	str	x1, [x0, 3440]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 544]
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 560]
	ldr	x0, [x29, 40]
	str	w1, [x0, 624]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 576]
	ldr	x0, [x29, 40]
	str	w1, [x0, 576]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 544]
	ldr	x0, [x29, 40]
	str	w1, [x0, 608]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 608]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 600]
	cmp	x0, 0
	bne	.L1220
	adrp	x0, __func__.7328
	add	x1, x0, :lo12:__func__.7328
	adrp	x0, .LC210
	add	x0, x0, :lo12:.LC210
	bl	nand_print
	mov	w0, -2
	b	.L1216
.L1220:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 600]
	ldr	x0, [x29, 40]
	str	x1, [x0, 616]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 624]
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 632]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 608]
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 560]
	ldr	x0, [x29, 40]
	str	w1, [x0, 488]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 576]
	ldr	x0, [x29, 40]
	str	w1, [x0, 504]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 544]
	ldr	x0, [x29, 40]
	str	w1, [x0, 472]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 472]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 464]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 464]
	cmp	x0, 0
	bne	.L1221
	adrp	x0, __func__.7328
	add	x1, x0, :lo12:__func__.7328
	adrp	x0, .LC211
	add	x0, x0, :lo12:.LC211
	bl	nand_print
	mov	w0, -2
	b	.L1216
.L1221:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 464]
	ldr	x0, [x29, 40]
	str	x1, [x0, 480]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 464]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 488]
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 496]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 656]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 676]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 660]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 664]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 668]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 672]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	lsr	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 1056]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 43691
	movk	w0, 0xaaaa, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 1058]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 1060]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 1062]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1064]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 528]
	ldr	x0, [x29, 40]
	mov	w1, -1
	strh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	ldr	x0, [x29, 40]
	mov	w1, -1
	strh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 304]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 312]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 288]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 296]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 320]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 328]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 368]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 376]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 352]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 360]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 384]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 392]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 432]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 440]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 416]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 424]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 448]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 456]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 696]
	str	wzr, [x29, 60]
	b	.L1222
.L1223:
	ldr	w0, [x29, 60]
	uxth	w2, w0
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 704
	mov	w1, w2
	strh	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 706
	strh	wzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 712
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 16
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 24
	str	xzr, [x0]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1222:
	ldr	w0, [x29, 60]
	cmp	w0, 9
	bls	.L1223
	ldr	x0, [x29, 40]
	mov	w1, 255
	strh	w1, [x0, 1024]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 1026]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1032]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1040]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1048]
	mov	w0, 0
.L1216:
	ldp	x29, x30, [sp], 64
	ret
	.size	zone_param_init, .-zone_param_init
	.align	2
	.global	zone_param_exit
	.type	zone_param_exit, %function
zone_param_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	free_logic_page_map
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 464]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 536]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 600]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 520]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	zone_param_exit, .-zone_param_exit
	.section	.rodata
	.align	3
.LC212:
	.string	"[NE]mtd size changed !\n"
	.text
	.align	2
	.global	is_no_use_device
	.type	is_no_use_device, %function
is_no_use_device:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 48]
	str	x0, [x29, 80]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	beq	.L1227
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 32]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 40]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 92]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	b	.L1228
.L1227:
	mov	w0, -1
	str	w0, [x29, 92]
.L1228:
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 26]
	b	.L1229
.L1237:
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 80]
	str	x1, [x0, 56]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1230
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	add	x0, x29, 56
	bl	is_ftl_start_flag_page
	cmp	w0, 1
	bne	.L1231
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1232
	ldr	w0, [x29, 76]
	cmp	w0, 10
	bne	.L1231
.L1232:
	add	x0, x29, 56
	mov	w2, 4
	mov	w1, 3
	bl	get_spare_data
	str	w0, [x29, 72]
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	beq	.L1233
	adrp	x0, .LC212
	add	x0, x0, :lo12:.LC212
	bl	nand_print
	mov	w0, 1
	b	.L1238
.L1233:
	ldr	x0, [x29, 80]
	add	x1, x0, 64
	ldr	x0, [x29, 24]
	str	x1, [x0, 48]
	mov	w0, 0
	b	.L1238
.L1231:
	mov	w0, 1
	b	.L1238
.L1230:
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 80]
	bl	is_last_phy_block
	cmp	w0, 1
	beq	.L1239
	ldr	x0, [x29, 80]
	add	x0, x0, 64
	str	x0, [x29, 80]
.L1229:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bcc	.L1237
	b	.L1236
.L1239:
	nop
.L1236:
	mov	w0, 1
.L1238:
	ldp	x29, x30, [sp], 96
	ret
	.size	is_no_use_device, .-is_no_use_device
	.align	2
	.global	get_vaild_blocks
	.type	get_vaild_blocks, %function
get_vaild_blocks:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 20]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	x0, [x29, 64]
	ldr	w1, [x29, 16]
	ldr	w0, [x29, 20]
	sub	w0, w1, w0
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	str	w0, [x29, 76]
	ldr	w0, [x29, 20]
	str	w0, [x29, 72]
	b	.L1241
.L1246:
	ldr	x0, [x29, 64]
	ldrh	w0, [x0]
	strh	w0, [x29, 34]
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 159
	bne	.L1242
	ldr	w0, [x29, 76]
	sub	w0, w0, #1
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	bne	.L1242
	mov	w0, 0
	b	.L1247
.L1242:
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 64]
	bl	is_last_phy_block
	cmp	w0, 1
	beq	.L1248
	ldr	x0, [x29, 64]
	add	x0, x0, 64
	str	x0, [x29, 64]
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L1241:
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bcc	.L1246
	b	.L1245
.L1248:
	nop
.L1245:
	ldr	w0, [x29, 76]
.L1247:
	ldp	x29, x30, [sp], 80
	ret
	.size	get_vaild_blocks, .-get_vaild_blocks
	.section	.rodata
	.align	3
.LC213:
	.string	"[ND]build_zone_list_first.\n"
	.align	3
.LC214:
	.string	"[NE]no block %d\n"
	.text
	.align	2
	.global	build_zone_list_first
	.type	build_zone_list_first, %function
build_zone_list_first:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	adrp	x0, .LC213
	add	x0, x0, :lo12:.LC213
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 48]
	str	x0, [x29, 80]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	beq	.L1250
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 32]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 40]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 92]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	b	.L1251
.L1250:
	mov	w0, -1
	str	w0, [x29, 92]
.L1251:
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 26]
	b	.L1252
.L1261:
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 80]
	str	x1, [x0, 56]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1253
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1254
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L1258
.L1254:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	cmp	w0, 1
	bne	.L1258
	add	x0, x29, 56
	mov	w2, 16
	mov	w1, -1
	bl	nand_memset
	add	x0, x29, 56
	ldr	w2, [x29, 20]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_start_block_flag
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	str	wzr, [x29, 88]
	b	.L1256
.L1257:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldrh	w0, [x29, 32]
	add	w0, w0, 1
	uxth	w0, w0
	strh	w0, [x29, 32]
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L1256:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 88]
	cmp	w1, w0
	bhi	.L1257
	ldr	x0, [x29, 80]
	add	x1, x0, 64
	ldr	x0, [x29, 24]
	str	x1, [x0, 48]
	b	.L1258
.L1253:
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
.L1258:
	ldr	x0, [x29, 80]
	mov	w1, -1
	strh	w1, [x0, 4]
	ldr	x0, [x29, 80]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 80]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 80]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 80]
	str	x1, [x0, 56]
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 80]
	bl	is_last_phy_block
	cmp	w0, 1
	beq	.L1275
	ldr	x0, [x29, 80]
	add	x0, x0, 64
	str	x0, [x29, 80]
.L1252:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bcc	.L1261
	b	.L1260
.L1275:
	nop
.L1260:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	cmp	w0, 1
	bls	.L1262
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	w0, [x29, 92]
	cmn	w0, #1
	bne	.L1265
	b	.L1274
.L1262:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC214
	add	x0, x0, :lo12:.LC214
	bl	nand_print
	mov	w0, 1
	b	.L1273
.L1274:
	mov	w0, 0
	b	.L1273
.L1265:
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 80]
	bl	is_last_phy_block
	cmp	w0, 1
	bne	.L1267
	mov	w0, 0
	b	.L1273
.L1272:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1268
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1269
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L1270
.L1269:
	add	x0, x29, 56
	mov	w2, 16
	mov	w1, -1
	bl	nand_memset
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_end_block_flag
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L1271
.L1268:
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
.L1270:
	ldr	x0, [x29, 80]
	add	x0, x0, 64
	str	x0, [x29, 80]
.L1267:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcc	.L1272
.L1271:
	mov	w0, 0
.L1273:
	ldp	x29, x30, [sp], 96
	ret
	.size	build_zone_list_first, .-build_zone_list_first
	.section	.rodata
	.align	3
.LC215:
	.string	"[ND]first\n"
	.align	3
.LC216:
	.string	"[NE]init_zone_after_first_scan error\n"
	.align	3
.LC217:
	.string	"[NE]second_scan_all_blocks error\n"
	.align	3
.LC218:
	.string	"[NI]nftl ok!\n"
	.text
	.align	2
	.global	build_zone_list
	.type	build_zone_list, %function
build_zone_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC215
	add	x0, x0, :lo12:.LC215
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	first_scan_all_blocks
	cmp	w0, 0
	beq	.L1277
	mov	w0, 1
	b	.L1278
.L1277:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	init_zone_after_first_scan
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1279
	adrp	x0, .LC216
	add	x0, x0, :lo12:.LC216
	bl	nand_print
	mov	w0, 1
	b	.L1278
.L1279:
	ldr	x0, [x29, 24]
	bl	second_scan_all_blocks
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1280
	adrp	x0, .LC217
	add	x0, x0, :lo12:.LC217
	bl	nand_print
	mov	w0, 1
	b	.L1278
.L1280:
	ldr	x0, [x29, 24]
	bl	power_on_gc
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	bl	print_nftl_zone
	adrp	x0, .LC218
	add	x0, x0, :lo12:.LC218
	bl	nand_print
	ldr	x0, [x29, 24]
	bl	panic_backup_block
	mov	w0, 0
.L1278:
	ldp	x29, x30, [sp], 48
	ret
	.size	build_zone_list, .-build_zone_list
	.section	.rodata
	.align	3
.LC219:
	.string	"[ND]fsab\n"
	.align	3
.LC220:
	.string	"[NI]ftl end block! %d!\n"
	.align	3
.LC221:
	.string	"[ND]READ_RECLAIM!\n"
	.align	3
.LC222:
	.string	"[NI]start page , do nothing\n"
	.align	3
.LC223:
	.string	"[ND]fill block %d\n"
	.align	3
.LC224:
	.string	"[NI]b@%d p@0 unknow spare data:%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x.\n"
	.align	3
.LC225:
	.string	"[NE]nftl bad block: %d\n"
	.align	3
.LC226:
	.string	"[NE]nftl b@%d p@0 ecc error\n"
	.align	3
.LC227:
	.string	"[NE]b@%d last page ok !\n"
	.align	3
.LC228:
	.string	"[NI]READ_RECLAIM!\n"
	.align	3
.LC229:
	.string	"[NE]last page blank ! %d!\n"
	.align	3
.LC230:
	.string	"[NE]last page unkown ! %d!\n"
	.align	3
.LC231:
	.string	"[NE]b@%d first page last page ecc error\n"
	.align	3
.LC232:
	.string	"[NE]b@%d last page spare data %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n"
	.align	3
.LC233:
	.string	"[NI]nftl bad block: %d\n"
	.text
	.align	2
	.global	first_scan_all_blocks
	.type	first_scan_all_blocks, %function
first_scan_all_blocks:
	sub	sp, sp, #208
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 24]
	str	wzr, [x29, 124]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 48]
	str	x0, [x29, 112]
	str	xzr, [x29, 104]
	adrp	x0, .LC2
	add	x1, x0, :lo12:.LC2
	add	x0, x29, 48
	ldr	w2, [x1]
	str	w2, [x0]
	ldr	w1, [x1, 1]
	str	w1, [x0, 1]
	mov	w0, 47
	strb	w0, [x29, 40]
	strb	wzr, [x29, 41]
	adrp	x0, mod
	add	x1, x0, :lo12:mod
	add	x0, x29, 40
	mov	x2, x1
	mov	x1, x0
	mov	x0, 0
	bl	nand_fdt_get_prop_string_by_path
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1282
	mov	w0, -1
	b	.L1313
.L1282:
	add	x0, x29, 48
	mov	x2, 3
	mov	x1, x0
	ldr	x0, [x29, 104]
	bl	nand_memcmp
	cmp	w0, 0
	beq	.L1284
	mov	w0, -1
	b	.L1313
.L1284:
	adrp	x0, .LC219
	add	x0, x0, :lo12:.LC219
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 26]
	b	.L1285
.L1312:
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	add	x2, x29, 80
	add	x0, x29, 56
	mov	x5, x2
	mov	x4, 0
	mov	w3, 0
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1287
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 100]
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 112]
	str	x1, [x0, 56]
	add	x0, x29, 80
	bl	get_block_used_count_from_oob
	str	w0, [x29, 96]
	add	x0, x29, 80
	bl	is_ftl_end_flag_page
	cmp	w0, 1
	bne	.L1288
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC220
	add	x0, x0, :lo12:.LC220
	bl	nand_print
	b	.L1289
.L1288:
	add	x0, x29, 80
	bl	is_panic_data_page
	cmp	w0, 1
	bne	.L1290
	ldrb	w1, [x29, 83]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	cmp	w1, w0
	bne	.L1311
	add	x0, x29, 80
	ldr	x1, [x29, 112]
	bl	panic_recover_info
	b	.L1311
.L1290:
	add	x0, x29, 80
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1293
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 4]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
	b	.L1311
.L1293:
	add	x0, x29, 80
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	beq	.L1294
	add	x0, x29, 80
	bl	is_function_info_page
	cmp	w0, 1
	bne	.L1295
.L1294:
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1296
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1295
.L1296:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	mov	w1, 1
	strh	w1, [x0, 2]
	add	x0, x29, 80
	bl	get_erase_count_from_oob
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 124]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 4]
	ldr	w1, [x29, 96]
	ldr	x0, [x29, 112]
	str	w1, [x0, 8]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 56]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_block_count_list
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1314
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 4]
	cmp	w0, 0
	beq	.L1314
	mov	w2, 1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	adrp	x0, .LC221
	add	x0, x0, :lo12:.LC221
	bl	nand_print_dbg
	b	.L1314
.L1295:
	add	x0, x29, 80
	bl	is_ftl_start_flag_page
	cmp	w0, 1
	bne	.L1298
	adrp	x0, .LC222
	add	x0, x0, :lo12:.LC222
	bl	nand_print
	b	.L1311
.L1298:
	add	x0, x29, 80
	bl	is_fill_page
	cmp	w0, 1
	bne	.L1299
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 2]
	add	x0, x29, 80
	bl	get_erase_count_from_oob
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 124]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 20]
	ldr	x0, [x29, 112]
	strh	w1, [x0, 4]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC223
	add	x0, x0, :lo12:.LC223
	bl	nand_print_dbg
	b	.L1311
.L1299:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w11, w0
	ldrb	w0, [x29, 80]
	mov	w12, w0
	ldrb	w0, [x29, 81]
	mov	w13, w0
	ldrb	w0, [x29, 82]
	mov	w14, w0
	ldrb	w0, [x29, 83]
	mov	w15, w0
	ldrb	w0, [x29, 84]
	mov	w16, w0
	ldrb	w0, [x29, 85]
	mov	w17, w0
	ldrb	w0, [x29, 86]
	mov	w1, w0
	ldrb	w0, [x29, 87]
	mov	w2, w0
	ldrb	w0, [x29, 88]
	mov	w3, w0
	ldrb	w0, [x29, 89]
	mov	w4, w0
	ldrb	w0, [x29, 90]
	mov	w5, w0
	ldrb	w0, [x29, 91]
	mov	w6, w0
	ldrb	w0, [x29, 92]
	mov	w7, w0
	ldrb	w0, [x29, 93]
	mov	w8, w0
	ldrb	w0, [x29, 94]
	mov	w9, w0
	ldrb	w0, [x29, 95]
	mov	w10, w0
	adrp	x0, .LC224
	add	x0, x0, :lo12:.LC224
	str	w10, [sp, 72]
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w17
	mov	w6, w16
	mov	w5, w15
	mov	w4, w14
	mov	w3, w13
	mov	w2, w12
	mov	w1, w11
	bl	nand_print
	ldr	w0, [x29, 124]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1300
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1301
.L1300:
	ldrb	w0, [x29, 80]
	cmp	w0, 255
	beq	.L1302
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC225
	add	x0, x0, :lo12:.LC225
	bl	nand_print
	ldr	x0, [x29, 112]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 28]
	b	.L1315
.L1302:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	erase_block
	cmp	w0, 0
	bne	.L1315
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1315
.L1301:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC226
	add	x0, x0, :lo12:.LC226
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	add	x3, x29, 80
	add	x0, x29, 56
	mov	x5, x3
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1304
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1305
.L1304:
	add	x0, x29, 80
	bl	is_phy_mapping_page
	cmp	w0, 1
	bne	.L1306
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC227
	add	x0, x0, :lo12:.LC227
	bl	nand_print
	add	x0, x29, 80
	bl	get_block_used_count_from_oob
	str	w0, [x29, 96]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	mov	w1, 1
	strh	w1, [x0, 2]
	add	x0, x29, 80
	bl	get_erase_count_from_oob
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 124]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 4]
	ldr	w1, [x29, 96]
	ldr	x0, [x29, 112]
	str	w1, [x0, 8]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 56]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_block_count_list
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1316
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 4]
	cmp	w0, 0
	beq	.L1316
	mov	w2, 1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	adrp	x0, .LC228
	add	x0, x0, :lo12:.LC228
	bl	nand_print
	b	.L1316
.L1306:
	add	x0, x29, 80
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1309
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC229
	add	x0, x0, :lo12:.LC229
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	erase_block
	cmp	w0, 0
	bne	.L1316
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1316
.L1309:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC230
	add	x0, x0, :lo12:.LC230
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	erase_block
	cmp	w0, 0
	bne	.L1316
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1316
.L1305:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC231
	add	x0, x0, :lo12:.LC231
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w11, w0
	ldrb	w0, [x29, 80]
	mov	w12, w0
	ldrb	w0, [x29, 81]
	mov	w13, w0
	ldrb	w0, [x29, 82]
	mov	w14, w0
	ldrb	w0, [x29, 83]
	mov	w15, w0
	ldrb	w0, [x29, 84]
	mov	w16, w0
	ldrb	w0, [x29, 85]
	mov	w17, w0
	ldrb	w0, [x29, 86]
	mov	w1, w0
	ldrb	w0, [x29, 87]
	mov	w2, w0
	ldrb	w0, [x29, 88]
	mov	w3, w0
	ldrb	w0, [x29, 89]
	mov	w4, w0
	ldrb	w0, [x29, 90]
	mov	w5, w0
	ldrb	w0, [x29, 91]
	mov	w6, w0
	ldrb	w0, [x29, 92]
	mov	w7, w0
	ldrb	w0, [x29, 93]
	mov	w8, w0
	ldrb	w0, [x29, 94]
	mov	w9, w0
	ldrb	w0, [x29, 95]
	mov	w10, w0
	adrp	x0, .LC232
	add	x0, x0, :lo12:.LC232
	str	w10, [sp, 72]
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w17
	mov	w6, w16
	mov	w5, w15
	mov	w4, w14
	mov	w3, w13
	mov	w2, w12
	mov	w1, w11
	bl	nand_print
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	torture_block_and_mark
	cmp	w0, 0
	bne	.L1311
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1311
.L1287:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC233
	add	x0, x0, :lo12:.LC233
	bl	nand_print
	ldr	x0, [x29, 112]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 28]
	b	.L1311
.L1314:
	nop
	b	.L1311
.L1315:
	nop
	b	.L1311
.L1316:
	nop
.L1311:
	ldr	x0, [x29, 112]
	add	x0, x0, 64
	str	x0, [x29, 112]
.L1285:
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcc	.L1312
.L1289:
	mov	w0, 0
.L1313:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 128
	ret
	.size	first_scan_all_blocks, .-first_scan_all_blocks
	.section	.rodata
	.align	3
.LC2:
	.byte	115
	.byte	117
	.byte	110
	.byte	120
	.byte	105
	.text
	.section	.rodata
	.align	3
.LC234:
	.string	"[ND]before second %d %d.\n"
	.align	3
.LC235:
	.string	"[NE]only have blocks:%u,logic cap need blocks:%u this zone not enough data block!!\n"
	.align	3
.LC236:
	.string	"[NE]this zone not enough free block %d , %d!!\n"
	.text
	.align	2
	.global	init_zone_after_first_scan
	.type	init_zone_after_first_scan, %function
init_zone_after_first_scan:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 44]
	udiv	w0, w0, w1
	str	w0, [x29, 44]
	adrp	x0, .LC234
	add	x0, x0, :lo12:.LC234
	ldr	w2, [x29, 44]
	ldr	w1, [x29, 20]
	bl	nand_print_dbg
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L1318
	adrp	x0, .LC235
	add	x0, x0, :lo12:.LC235
	ldr	w2, [x29, 44]
	ldr	w1, [x29, 20]
	bl	nand_print
	mov	w0, 1
	b	.L1319
.L1318:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	mul	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	mul	w1, w0, w1
	ldr	x0, [x29, 24]
	str	w1, [x0, 40]
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3448]
	ldrh	w1, [x1, 12]
	sub	w1, w1, #1
	cmp	w0, w1
	bcs	.L1320
	adrp	x0, .LC236
	add	x0, x0, :lo12:.LC236
	ldr	w2, [x29, 44]
	ldr	w1, [x29, 20]
	bl	nand_print
	mov	w0, 1
	b	.L1319
.L1320:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w1, w1, w0
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 680]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 680]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 14]
	cmp	w1, w0
	bcs	.L1321
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w1, [x0, 14]
	ldr	x0, [x29, 24]
	strh	w1, [x0, 680]
.L1321:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3448]
	ldrh	w1, [x1, 18]
	mul	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3448]
	ldrh	w1, [x1, 20]
	udiv	w0, w0, w1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 682]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	add	w0, w0, 2
	cmp	w1, w0
	bge	.L1322
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	add	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 682]
.L1322:
	ldr	x0, [x29, 24]
	mov	w1, 1
	strh	w1, [x0, 684]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 686]
	mov	w0, 0
.L1319:
	ldp	x29, x30, [sp], 48
	ret
	.size	init_zone_after_first_scan, .-init_zone_after_first_scan
	.section	.rodata
	.align	3
.LC237:
	.string	"[NI]NAND_EVENT: 2 current used block found %d,%d!\n"
	.align	3
.LC238:
	.string	"[NE]no crosstalk:2 block used count diff %d,%d %d,%d !\n"
	.align	3
.LC239:
	.string	"[NE]NAND_EVENT: muti current used block found1 %d %d!\n"
	.align	3
.LC240:
	.string	"[NE]NAND_EVENT: muti current used block found2 %d %d!\n"
	.align	3
.LC241:
	.string	"[NE]NAND_EVENT: muti current used block found3 %d %d!\n"
	.align	3
.LC242:
	.string	"[NE]NAND_EVENT: last page spare data %x,%x,%x,%x,%x,%x,%x %x %x %x %x %x %x %x %x %x!!\n"
	.align	3
.LC243:
	.string	"[NI]zone@%d b@%d last page unkown ecc ok.\n"
	.align	3
.LC244:
	.string	"[NE]zone@%d b@%d last page ecc error.\n"
	.align	3
.LC245:
	.string	"[NE]something strange %d,%d!!\n"
	.align	3
.LC246:
	.string	"[NI]all block full!!\n"
	.align	3
.LC247:
	.string	"[ND]corss talk rebuild 0 %d %d!!\n"
	.align	3
.LC248:
	.string	"[NE]1st current block:%d 1st current block uc:%d muti current used block 1st.\n"
	.align	3
.LC249:
	.string	"[ND]2nd current block:%d 2nd current block uc:%d muti current used block 2nd.\n"
	.align	3
.LC250:
	.string	"[NE]something is error here 7!\n"
	.align	3
.LC251:
	.string	"[NE]p_phy_block_info_last %d %d.\n"
	.align	3
.LC252:
	.string	"[NE]get_current_used_block fail.\n"
	.align	3
.LC253:
	.string	"[ND]free block nums is %d, aec: %d, iec: %d, a_buc: %d, i_buc: %d!\n"
	.text
	.align	2
	.global	second_scan_all_blocks
	.type	second_scan_all_blocks, %function
second_scan_all_blocks:
	sub	sp, sp, #256
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 24]
	str	wzr, [x29, 160]
	str	xzr, [x29, 112]
	str	xzr, [x29, 104]
	str	xzr, [x29, 128]
	str	xzr, [x29, 136]
	str	xzr, [x29, 120]
	str	wzr, [x29, 100]
	adrp	x0, g_nreg_base
	add	x0, x0, :lo12:g_nreg_base
	ldr	x0, [x0]
	str	x0, [x29, 88]
	mov	w0, 188
	str	w0, [x29, 84]
	mov	w0, 1
	str	w0, [x29, 80]
	mov	w0, 2
	str	w0, [x29, 76]
	mov	w0, 1
	movk	w0, 0x3, lsl 16
	str	w0, [x29, 72]
	ldr	w0, [x29, 84]
	lsl	x0, x0, 2
	ldr	x1, [x29, 88]
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 100]
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 72]
	cmp	w1, w0
	beq	.L1324
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 80]
	cmp	w1, w0
	beq	.L1324
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	beq	.L1324
	mov	w0, -1
	b	.L1366
.L1324:
	mov	w0, -1
	str	w0, [x29, 172]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w1, w0, w1
	ldr	x0, [x29, 24]
	str	w1, [x0, 36]
	mov	w0, -1
	str	w0, [x29, 164]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	str	x0, [x29, 152]
	b	.L1326
.L1327:
	ldr	x0, [x29, 152]
	ldr	w0, [x0, 8]
	str	w0, [x29, 164]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 152]
	ldr	w1, [x1, 8]
	str	w1, [x0, 156]
	ldr	x0, [x29, 152]
	ldr	x0, [x0, 48]
	str	x0, [x29, 152]
.L1326:
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1327
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	cmp	x0, 0
	beq	.L1328
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 448]
	ldr	w1, [x1, 8]
	str	w1, [x0, 160]
.L1328:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	str	x0, [x29, 152]
	b	.L1329
.L1347:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	and	w0, w0, 511
	cmp	w0, 0
	beq	.L1330
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	lsr	w0, w0, 9
	add	w0, w0, 1
	str	w0, [x29, 160]
	b	.L1331
.L1330:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	lsr	w0, w0, 9
	str	w0, [x29, 160]
.L1331:
	ldr	x0, [x29, 152]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	w0, [x29, 160]
	uxth	w3, w0
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 56
	add	x0, x29, 32
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 168]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1332
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	add	x1, x29, 56
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	crc_wrong
	cmp	w0, 0
	beq	.L1332
	mov	w0, -2
	str	w0, [x29, 168]
.L1332:
	ldr	x0, [x29, 152]
	ldr	w0, [x0, 8]
	str	w0, [x29, 172]
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 676]
	cmp	w1, w0
	bls	.L1333
	ldr	x0, [x29, 152]
	ldrh	w1, [x0, 6]
	mov	w0, 49999
	cmp	w1, w0
	bhi	.L1333
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 24]
	str	w1, [x0, 676]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	w1, [x1, 676]
	str	w1, [x0, 148]
.L1333:
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 152]
	cmp	w1, w0
	bcs	.L1334
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 152]
	ldrh	w1, [x1, 6]
	str	w1, [x0, 152]
.L1334:
	add	x0, x29, 56
	bl	is_phy_mapping_page
	cmp	w0, 1
	bne	.L1335
	ldr	w0, [x29, 168]
	cmp	w0, 0
	beq	.L1336
	ldr	w0, [x29, 168]
	cmp	w0, 10
	bne	.L1335
.L1336:
	ldr	x0, [x29, 152]
	str	x0, [x29, 128]
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 152]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	ldr	x0, [x29, 152]
	str	x0, [x29, 120]
	b	.L1337
.L1335:
	add	x0, x29, 56
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1338
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L1339
	ldr	x0, [x29, 152]
	str	x0, [x29, 112]
	b	.L1337
.L1339:
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1341
	ldr	x0, [x29, 152]
	str	x0, [x29, 104]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC237
	add	x0, x0, :lo12:.LC237
	bl	nand_print
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	bne	.L1337
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	beq	.L1337
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 104]
	ldr	w2, [x0, 8]
	adrp	x0, .LC238
	add	x0, x0, :lo12:.LC238
	mov	w4, w2
	mov	w3, w1
	mov	w2, w6
	mov	w1, w5
	bl	nand_print
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	ldr	x0, [x29, 104]
	str	x0, [x29, 112]
	str	xzr, [x29, 104]
	b	.L1337
.L1341:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC239
	add	x0, x0, :lo12:.LC239
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 8]
	adrp	x0, .LC240
	add	x0, x0, :lo12:.LC240
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 152]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC241
	add	x0, x0, :lo12:.LC241
	ldr	w2, [x29, 172]
	bl	nand_print
	ldr	x0, [x29, 112]
	str	x0, [x29, 144]
	ldr	x0, [x29, 104]
	str	x0, [x29, 112]
	ldr	x0, [x29, 152]
	str	x0, [x29, 104]
	mov	w2, 3
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1337
.L1338:
	ldrb	w0, [x29, 56]
	mov	w10, w0
	ldrb	w0, [x29, 57]
	mov	w11, w0
	ldrb	w0, [x29, 58]
	mov	w12, w0
	ldrb	w0, [x29, 59]
	mov	w13, w0
	ldrb	w0, [x29, 60]
	mov	w14, w0
	ldrb	w0, [x29, 61]
	mov	w15, w0
	ldrb	w0, [x29, 62]
	mov	w16, w0
	ldrb	w0, [x29, 63]
	mov	w1, w0
	ldrb	w0, [x29, 64]
	mov	w2, w0
	ldrb	w0, [x29, 65]
	mov	w3, w0
	ldrb	w0, [x29, 66]
	mov	w4, w0
	ldrb	w0, [x29, 67]
	mov	w5, w0
	ldrb	w0, [x29, 68]
	mov	w6, w0
	ldrb	w0, [x29, 69]
	mov	w7, w0
	ldrb	w0, [x29, 70]
	mov	w8, w0
	ldrb	w0, [x29, 71]
	mov	w9, w0
	adrp	x0, .LC242
	add	x0, x0, :lo12:.LC242
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print
	ldr	w0, [x29, 168]
	cmp	w0, 0
	beq	.L1343
	ldr	w0, [x29, 168]
	cmp	w0, 10
	bne	.L1344
.L1343:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	x0, [x29, 152]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC243
	add	x0, x0, :lo12:.LC243
	bl	nand_print
	b	.L1345
.L1344:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	x0, [x29, 152]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC244
	add	x0, x0, :lo12:.LC244
	bl	nand_print
.L1345:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1346
	ldr	w1, [x29, 164]
	ldr	w0, [x29, 172]
	cmp	w1, w0
	bne	.L1346
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	erase_block
	ldr	x0, [x29, 152]
	str	x0, [x29, 136]
	b	.L1337
.L1346:
	ldr	x0, [x29, 152]
	str	x0, [x29, 128]
	ldr	x0, [x29, 152]
	mov	w1, 170
	strh	w1, [x0, 2]
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 152]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	recover_block_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
.L1337:
	ldr	x0, [x29, 152]
	ldr	x0, [x0, 48]
	str	x0, [x29, 152]
.L1329:
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1347
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 24]
	bl	init_smart_info
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	str	x0, [x29, 144]
	b	.L1348
.L1350:
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 112]
	cmp	x1, x0
	beq	.L1349
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 104]
	cmp	x1, x0
	beq	.L1349
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 136]
	cmp	x1, x0
	beq	.L1349
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_invalid_page_list
.L1349:
	ldr	x0, [x29, 144]
	ldr	x0, [x0, 48]
	str	x0, [x29, 144]
.L1348:
	ldr	x0, [x29, 144]
	cmp	x0, 0
	bne	.L1350
	ldr	x0, [x29, 24]
	bl	adjust_invaild_list
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1351
	ldr	x0, [x29, 112]
	cmp	x0, 0
	beq	.L1351
	ldr	x0, [x29, 128]
	ldr	w0, [x0, 8]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	beq	.L1351
	ldr	x0, [x29, 128]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 112]
	ldr	w2, [x0, 8]
	adrp	x0, .LC245
	add	x0, x0, :lo12:.LC245
	bl	nand_print
.L1351:
	ldr	x0, [x29, 24]
	str	xzr, [x0, 528]
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L1352
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1352
	adrp	x0, .LC246
	add	x0, x0, :lo12:.LC246
	bl	nand_print
	str	xzr, [x29, 152]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1353
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1367
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1367
.L1353:
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1367
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 2]
	cmp	w0, 170
	bne	.L1367
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1367
.L1352:
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1357
	ldr	x0, [x29, 112]
	str	x0, [x29, 152]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1358
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC247
	add	x0, x0, :lo12:.LC247
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1359
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 2]
	cmp	w0, 170
	bne	.L1359
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
.L1359:
	mov	x2, 0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1356
	mov	w0, 1
	b	.L1366
.L1358:
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1361
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 2]
	cmp	w0, 170
	bne	.L1361
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
.L1361:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	cmp	w0, 0
	bne	.L1362
	ldr	x0, [x29, 112]
	str	x0, [x29, 152]
	b	.L1356
.L1362:
	mov	x2, 0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	no_cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	b	.L1356
.L1357:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1363
	ldr	x2, [x29, 104]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1356
	mov	w0, 1
	b	.L1366
.L1363:
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	bne	.L1364
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC248
	add	x0, x0, :lo12:.LC248
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 8]
	adrp	x0, .LC249
	add	x0, x0, :lo12:.LC249
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x2, [x29, 104]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	no_cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	b	.L1356
.L1364:
	adrp	x0, .LC250
	add	x0, x0, :lo12:.LC250
	bl	nand_print
	ldr	x0, [x29, 128]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 128]
	ldr	w1, [x0, 8]
	adrp	x0, .LC251
	add	x0, x0, :lo12:.LC251
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC248
	add	x0, x0, :lo12:.LC248
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 8]
	adrp	x0, .LC249
	add	x0, x0, :lo12:.LC249
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_block_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 104]
	ldr	x0, [x29, 24]
	bl	recover_block_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 104]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 104]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1356
.L1367:
	nop
.L1356:
	ldr	w2, [x29, 172]
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	get_current_used_block
	str	w0, [x29, 168]
	ldr	w0, [x29, 168]
	cmp	w0, 0
	beq	.L1365
	adrp	x0, .LC252
	add	x0, x0, :lo12:.LC252
	bl	nand_print
	mov	w0, 1
	b	.L1366
.L1365:
	ldr	x0, [x29, 24]
	bl	adjust_invaild_list
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 24]
	str	x1, [x0, 64]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w6, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 148]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w2, [x0, 152]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w3, [x0, 156]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w4, [x0, 160]
	adrp	x0, .LC253
	add	x0, x0, :lo12:.LC253
	mov	w5, w4
	mov	w4, w3
	mov	w3, w2
	mov	w2, w1
	mov	w1, w6
	bl	nand_print_dbg
	mov	w0, 0
.L1366:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 176
	ret
	.size	second_scan_all_blocks, .-second_scan_all_blocks
	.align	2
	.global	check_bit_nums
	.type	check_bit_nums, %function
check_bit_nums:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	wzr, [sp, 20]
	str	wzr, [sp, 28]
	b	.L1369
.L1373:
	mov	w0, 1
	strb	w0, [sp, 19]
	str	wzr, [sp, 24]
	b	.L1370
.L1372:
	ldrsw	x0, [sp, 28]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w1, [x0]
	ldrb	w0, [sp, 19]
	and	w0, w1, w0
	uxtb	w0, w0
	cmp	w0, 0
	beq	.L1371
	ldr	w0, [sp, 20]
	add	w0, w0, 1
	str	w0, [sp, 20]
.L1371:
	ldrb	w0, [sp, 19]
	ubfiz	w0, w0, 1, 7
	strb	w0, [sp, 19]
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L1370:
	ldr	w0, [sp, 24]
	cmp	w0, 7
	ble	.L1372
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1369:
	ldr	w1, [sp, 28]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	blt	.L1373
	ldr	w0, [sp, 20]
	add	sp, sp, 32
	ret
	.size	check_bit_nums, .-check_bit_nums
	.align	2
	.global	is_all_bit_ff
	.type	is_all_bit_ff, %function
is_all_bit_ff:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	w0, [x29, 20]
	lsl	w0, w0, 3
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 16]
	sub	w0, w1, w0
	str	w0, [x29, 44]
	ldr	w1, [x29, 20]
	ldr	x0, [x29, 24]
	bl	check_bit_nums
	str	w0, [x29, 40]
	ldr	w1, [x29, 40]
	ldr	w0, [x29, 44]
	cmp	w1, w0
	blt	.L1376
	mov	w0, 1
	b	.L1377
.L1376:
	mov	w0, 0
.L1377:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_all_bit_ff, .-is_all_bit_ff
	.align	2
	.global	is_all_byte_ff
	.type	is_all_byte_ff, %function
is_all_byte_ff:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	w2, [sp]
	str	wzr, [sp, 28]
	ldr	w1, [sp, 4]
	ldr	w0, [sp]
	sub	w0, w1, w0
	str	w0, [sp, 20]
	str	wzr, [sp, 24]
	b	.L1379
.L1381:
	ldrsw	x0, [sp, 24]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L1380
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1380:
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L1379:
	ldr	w1, [sp, 24]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	blt	.L1381
	ldr	w1, [sp, 28]
	ldr	w0, [sp, 20]
	cmp	w1, w0
	blt	.L1382
	mov	w0, 1
	b	.L1383
.L1382:
	mov	w0, 0
.L1383:
	add	sp, sp, 32
	ret
	.size	is_all_byte_ff, .-is_all_byte_ff
	.section	.rodata
	.align	3
.LC254:
	.string	"[NE]current_block ecc error %d!\n"
	.align	3
.LC255:
	.string	"[NE]error! no free block!\n"
	.align	3
.LC256:
	.string	"[ND]recover %d %d\n"
	.align	3
.LC257:
	.string	"[NE]page used:%d invalid page count:%d error\n"
	.align	3
.LC258:
	.string	"[NE]new_block_init_for_write error here!\n"
	.align	3
.LC259:
	.string	"[NE]something is error here 10!\n"
	.align	3
.LC260:
	.string	"[NE]something is error here 11!\n"
	.align	3
.LC261:
	.string	"[NE]assist block page used:%d current block page used:%d something is error here 12.\n"
	.align	3
.LC262:
	.string	"[NE]assist block page used:%d current block page used:%d something is error here 17.\n"
	.align	3
.LC263:
	.string	"[NE]something is error here 16!\n"
	.align	3
.LC264:
	.string	"[ND]get a new free block\n"
	.align	3
.LC265:
	.string	"[NE]no free block to use!\n"
	.text
	.align	2
	.global	get_current_used_block
	.type	get_current_used_block, %function
get_current_used_block:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	wzr, [x29, 56]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	cmp	x0, 0
	beq	.L1385
	mov	w0, 0
	b	.L1386
.L1385:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1387
.L1388:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	recover_phy_page_mapping
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 584]
	mov	w0, 65535
	cmp	w1, w0
	bne	.L1389
	mov	w0, 1
	str	w0, [x29, 56]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC254
	add	x0, x0, :lo12:.LC254
	bl	nand_print
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	current_block_ecc_error
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1390
	b	.L1388
.L1390:
	adrp	x0, .LC255
	add	x0, x0, :lo12:.LC255
	bl	nand_print
	mov	w0, 1
	b	.L1386
.L1389:
	ldr	x0, [x29, 40]
	ldr	x1, [x29, 32]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w2, w0
	adrp	x0, .LC256
	add	x0, x0, :lo12:.LC256
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 552]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w3, w0
	ldr	x0, [x29, 40]
	bl	recover_logic_page_mapping
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0, 4]
	cmp	w1, w0
	bcs	.L1391
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC257
	add	x0, x0, :lo12:.LC257
	bl	nand_print
.L1391:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1392
.L1393:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	cmp	x0, 0
	bne	.L1394
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	cmp	x0, 0
	bne	.L1395
	mov	w0, 1
	b	.L1386
.L1395:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 648]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 528]
	ldr	w1, [x1, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1396
	adrp	x0, .LC258
	add	x0, x0, :lo12:.LC258
	bl	nand_print
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1393
.L1396:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	str	w0, [x29, 52]
	str	wzr, [x29, 60]
	b	.L1397
.L1399:
	ldr	x0, [x29, 40]
	ldr	x5, [x0, 3480]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	ldr	w3, [x29, 60]
	uxth	w3, w3
	mov	w4, w3
	mov	x3, x0
	ldr	x0, [x29, 40]
	blr	x5
	str	w0, [x29, 48]
	ldr	w0, [x29, 48]
	cmp	w0, 0
	beq	.L1398
	adrp	x0, .LC259
	add	x0, x0, :lo12:.LC259
	bl	nand_print
.L1398:
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1397:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 52]
	cmp	w1, w0
	bcc	.L1399
.L1394:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 616]
	mov	x2, x0
	ldr	x0, [x29, 40]
	bl	recover_phy_page_mapping
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	mov	w0, 65535
	cmp	w1, w0
	bne	.L1400
	adrp	x0, .LC260
	add	x0, x0, :lo12:.LC260
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1393
.L1400:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 528]
	ldr	w1, [x1, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 616]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 648]
	mov	w3, w0
	ldr	x0, [x29, 40]
	bl	recover_logic_page_mapping
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	cmp	w1, w0
	bls	.L1401
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 648]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w2, w0
	adrp	x0, .LC261
	add	x0, x0, :lo12:.LC261
	bl	nand_print
	ldr	w0, [x29, 56]
	cmp	w0, 0
	beq	.L1402
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	strh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 544]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1393
.L1402:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 648]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w2, w0
	adrp	x0, .LC262
	add	x0, x0, :lo12:.LC262
	bl	nand_print
	b	.L1392
.L1401:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	cmp	w1, w0
	bcs	.L1392
	adrp	x0, .LC263
	add	x0, x0, :lo12:.LC263
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1393
.L1387:
	adrp	x0, .LC264
	add	x0, x0, :lo12:.LC264
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 304]
	cmp	x0, 0
	beq	.L1403
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	cmp	x0, 0
	bne	.L1404
	mov	w0, 1
	b	.L1386
.L1404:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 584]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldr	w1, [x29, 28]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1405
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	cmp	x0, 0
	bne	.L1406
	mov	w0, 1
	b	.L1386
.L1406:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 648]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 528]
	ldr	w1, [x1, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
	b	.L1392
.L1405:
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1392
.L1403:
	adrp	x0, .LC265
	add	x0, x0, :lo12:.LC265
	bl	nand_print
	ldr	x0, [x29, 40]
	bl	print_free_list
	ldr	x0, [x29, 40]
	bl	print_block_invalid_list
	mov	w0, 1
	b	.L1386
.L1392:
	mov	w0, 0
.L1386:
	ldp	x29, x30, [sp], 64
	ret
	.size	get_current_used_block, .-get_current_used_block
	.section	.rodata
	.align	3
.LC266:
	.string	"[NE]sb@%d sp@%d recover_phy_page_mapping no used page\n"
	.align	3
.LC267:
	.string	"[NE]sb@%d sp@%d power off ecc error1.\n"
	.align	3
.LC268:
	.string	"[NE]invalid logic_page block:%d page:%d,logic_page 0x%x.\n"
	.align	3
.LC269:
	.string	"[NE]rppm invalid page %d!!\n"
	.align	3
.LC270:
	.string	"[NI]power_down page %d!!\n"
	.align	3
.LC271:
	.string	"[NE]recover_phy_page_mapping unkown page data %d!!\n"
	.align	3
.LC272:
	.string	"[NE]recover_phy_page_mapping ecc error block:%d;page:%d.\n"
	.text
	.align	2
	.global	recover_phy_page_mapping
	.type	recover_phy_page_mapping, %function
recover_phy_page_mapping:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 560]
	mov	w2, w0
	mov	w1, -1
	ldr	x0, [x29, 40]
	bl	nand_memset
	str	wzr, [x29, 108]
	b	.L1409
.L1424:
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	x4, [x0, 512]
	add	x5, x29, 88
	add	x0, x29, 64
	bl	set_physic_op_par
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 104]
	add	x0, x29, 88
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1410
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1411
	ldr	w0, [x29, 104]
	cmp	w0, 10
	bne	.L1410
.L1411:
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC266
	add	x0, x0, :lo12:.LC266
	ldr	w2, [x29, 108]
	bl	nand_print
	b	.L1412
.L1410:
	add	x0, x29, 88
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1413
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1414
	ldr	w0, [x29, 104]
	cmp	w0, 10
	bne	.L1417
.L1414:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 512]
	add	x1, x29, 88
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1416
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC267
	add	x0, x0, :lo12:.LC267
	ldr	w2, [x29, 108]
	bl	nand_print
	mov	w0, -2
	str	w0, [x29, 104]
	b	.L1417
.L1416:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x19, x1, x0
	add	x0, x29, 88
	bl	get_logic_page_from_oob
	str	w0, [x19]
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 36]
	cmp	w1, w0
	bcc	.L1417
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w4, w0
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	adrp	x0, .LC268
	add	x0, x0, :lo12:.LC268
	mov	w3, w1
	ldr	w2, [x29, 108]
	mov	w1, w4
	bl	nand_print
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0]
	b	.L1417
.L1413:
	add	x0, x29, 88
	bl	is_ftl_special_data
	cmp	w0, 1
	bne	.L1418
	adrp	x0, .LC269
	add	x0, x0, :lo12:.LC269
	ldr	w1, [x29, 108]
	bl	nand_print
	b	.L1417
.L1418:
	add	x0, x29, 88
	bl	is_power_down_info_page
	cmp	w0, 1
	bne	.L1419
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1420
	ldr	w0, [x29, 104]
	cmp	w0, 10
	bne	.L1417
.L1420:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x19, x1, x0
	add	x0, x29, 88
	bl	get_special_data_from_oob
	str	w0, [x19]
	adrp	x0, .LC270
	add	x0, x0, :lo12:.LC270
	ldr	w1, [x29, 108]
	bl	nand_print
	b	.L1417
.L1419:
	adrp	x0, .LC271
	add	x0, x0, :lo12:.LC271
	ldr	w1, [x29, 108]
	bl	nand_print
.L1417:
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1422
	ldr	w0, [x29, 104]
	cmp	w0, 10
	beq	.L1422
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC272
	add	x0, x0, :lo12:.LC272
	ldr	w2, [x29, 108]
	bl	nand_print
	mov	w0, 65535
	b	.L1426
.L1422:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1409:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1424
.L1412:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w1, w0, #2
	ldr	w0, [x29, 108]
	cmp	w1, w0
	ldr	w0, [x29, 108]
.L1426:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 112
	ret
	.size	recover_phy_page_mapping, .-recover_phy_page_mapping
	.section	.rodata
	.align	3
.LC273:
	.string	"[NE]ret:%d read fail %d %d,read one more time!!\n"
	.align	3
.LC274:
	.string	"[NE]ret:%d read fail %d %d,%d times,don't read more!!\n"
	.align	3
.LC275:
	.string	"[NE]power off ecc error 3!!\n"
	.align	3
.LC276:
	.string	"[NE]recover sb@%d sp@%d phy page mapping error.\n"
	.text
	.align	2
	.global	recover_block_phy_page_mapping
	.type	recover_block_phy_page_mapping, %function
recover_block_phy_page_mapping:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	wzr, [x29, 116]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 560]
	mov	w2, w0
	mov	w1, -1
	ldr	x0, [x29, 40]
	bl	nand_memset
	str	wzr, [x29, 124]
	b	.L1428
.L1442:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1429
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	x4, [x0, 512]
	add	x5, x29, 96
	add	x0, x29, 72
	bl	set_physic_op_par
	b	.L1430
.L1429:
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	add	x3, x29, 96
	add	x0, x29, 72
	mov	x5, x3
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
.L1430:
	str	wzr, [x29, 116]
	b	.L1431
.L1436:
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 72
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 120]
	ldr	w0, [x29, 120]
	cmp	w0, 0
	bge	.L1444
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC273
	add	x0, x0, :lo12:.LC273
	ldr	w3, [x29, 124]
	mov	w2, w1
	ldr	w1, [x29, 120]
	bl	nand_print
	ldr	w0, [x29, 116]
	cmp	w0, 1
	bne	.L1435
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC274
	add	x0, x0, :lo12:.LC274
	ldr	w4, [x29, 116]
	ldr	w3, [x29, 124]
	mov	w2, w1
	ldr	w1, [x29, 120]
	bl	nand_print
.L1435:
	ldr	w0, [x29, 116]
	add	w0, w0, 1
	str	w0, [x29, 116]
.L1431:
	ldr	w0, [x29, 116]
	cmp	w0, 1
	ble	.L1436
	b	.L1434
.L1444:
	nop
.L1434:
	add	x0, x29, 96
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1437
	ldr	w0, [x29, 120]
	cmp	w0, 0
	beq	.L1438
	ldr	w0, [x29, 120]
	cmp	w0, 10
	bne	.L1441
.L1438:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 512]
	add	x1, x29, 96
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1440
	adrp	x0, .LC275
	add	x0, x0, :lo12:.LC275
	bl	nand_print
	b	.L1441
.L1440:
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x19, x1, x0
	add	x0, x29, 96
	bl	get_logic_page_from_oob
	str	w0, [x19]
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 36]
	cmp	w1, w0
	bcc	.L1441
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w4, w0
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	adrp	x0, .LC268
	add	x0, x0, :lo12:.LC268
	mov	w3, w1
	ldr	w2, [x29, 124]
	mov	w1, w4
	bl	nand_print
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0]
	b	.L1441
.L1437:
	add	x0, x29, 96
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1441
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC276
	add	x0, x0, :lo12:.LC276
	ldr	w2, [x29, 124]
	bl	nand_print
.L1441:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1428:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w1, w0, #1
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bgt	.L1442
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 128
	ret
	.size	recover_block_phy_page_mapping, .-recover_block_phy_page_mapping
	.section	.rodata
	.align	3
.LC277:
	.string	"[NI]logic_page_map is NULL\n"
	.align	3
.LC278:
	.string	"[NE]invalid_page_count more than pages_per_blk :%d\n"
	.align	3
.LC279:
	.string	"[NE]invalid_page data block:%d page:%d,logic_page 0x%x!!\n"
	.align	3
.LC280:
	.string	"[NE]build invalid_page_count more than pages_per_blk!!\n"
	.text
	.align	2
	.global	recover_logic_page_mapping
	.type	recover_logic_page_mapping, %function
recover_logic_page_mapping:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 76]
	b	.L1446
.L1454:
	ldr	w0, [x29, 76]
	lsl	x0, x0, 2
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 72]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 36]
	ldr	w0, [x29, 72]
	cmp	w1, w0
	bls	.L1447
	ldr	w1, [x29, 72]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L1448
	adrp	x0, .LC277
	add	x0, x0, :lo12:.LC277
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 4]
	b	.L1449
.L1448:
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L1450
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 56]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 32]
	cmp	x1, x0
	beq	.L1451
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 40]
	bl	phy_block_from_invalid_page_incr
	b	.L1452
.L1451:
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 56]
	strh	w1, [x0, 4]
.L1452:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1450
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC278
	add	x0, x0, :lo12:.LC278
	bl	nand_print
.L1450:
	ldr	w0, [x29, 76]
	uxth	w0, w0
	and	w0, w0, 4095
	uxth	w2, w0
	ldr	x1, [x29, 64]
	ldrh	w0, [x1]
	bfi	w0, w2, 0, 12
	strh	w0, [x1]
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 64]
	strh	w1, [x0, 2]
	b	.L1449
.L1447:
	ldr	w0, [x29, 72]
	cmn	w0, #1
	beq	.L1453
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC279
	add	x0, x0, :lo12:.LC279
	ldr	w3, [x29, 72]
	ldr	w2, [x29, 76]
	bl	nand_print
.L1453:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1449
	adrp	x0, .LC280
	add	x0, x0, :lo12:.LC280
	bl	nand_print
.L1449:
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
.L1446:
	ldr	w1, [x29, 76]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L1454
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.size	recover_logic_page_mapping, .-recover_logic_page_mapping
	.section	.rodata
	.align	3
.LC281:
	.string	"[NE]NAND EVENT:do_write_error erase error1!\n"
	.align	3
.LC282:
	.string	"[NE]NAND EVENT:do_write_error erase error2!\n"
	.text
	.align	2
	.global	do_write_error_in_build_list
	.type	do_write_error_in_build_list, %function
do_write_error_in_build_list:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	strh	w3, [x29, 22]
	ldr	x0, [x29, 24]
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 80]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1457
	adrp	x0, .LC281
	add	x0, x0, :lo12:.LC281
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	mov	w0, 1
	b	.L1463
.L1457:
	str	wzr, [x29, 92]
	b	.L1459
.L1461:
	ldr	x0, [x29, 40]
	ldr	x5, [x0, 3480]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	ldr	w1, [x29, 92]
	uxth	w1, w1
	mov	w4, w1
	mov	x3, x0
	ldr	x2, [x29, 80]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	blr	x5
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1460
	adrp	x0, .LC282
	add	x0, x0, :lo12:.LC282
	bl	nand_print
.L1460:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L1459:
	ldrh	w1, [x29, 22]
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bhi	.L1461
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldrh	w0, [x29, 22]
	cmp	w0, 0
	bne	.L1462
	ldr	x0, [x29, 40]
	ldr	x1, [x29, 24]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 584]
.L1462:
	mov	w0, 0
.L1463:
	ldp	x29, x30, [sp], 96
	ret
	.size	do_write_error_in_build_list, .-do_write_error_in_build_list
	.section	.rodata
	.align	3
.LC283:
	.string	"%s %d:fail 1\n"
	.align	3
.LC284:
	.string	"%s %d:fail 2\n"
	.align	3
.LC285:
	.string	"%s %d:fail 3\n"
	.align	3
.LC286:
	.string	"%s %d:fail 4\n"
	.align	3
.LC287:
	.string	"[NE]pages_per_blk: %d, %p, %p, %p, %p\n"
	.text
	.align	2
	.global	init_nrc
	.type	init_nrc, %function
init_nrc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 44]
	str	xzr, [x29, 32]
	mov	w0, 32
	bl	nftl_malloc
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1465
	adrp	x0, __func__.7542
	add	x1, x0, :lo12:__func__.7542
	adrp	x0, .LC283
	add	x0, x0, :lo12:.LC283
	mov	w2, 1408
	bl	nand_print
	mov	w0, -1
	b	.L1466
.L1465:
	mov	w2, 32
	mov	w1, 0
	ldr	x0, [x29, 32]
	bl	nand_memset
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w1, w0, 64
	ldr	w0, [x29, 44]
	mul	w0, w1, w0
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1467
	adrp	x0, __func__.7542
	add	x1, x0, :lo12:__func__.7542
	adrp	x0, .LC284
	add	x0, x0, :lo12:.LC284
	mov	w2, 1417
	bl	nand_print
	b	.L1468
.L1467:
	ldrsw	x0, [x29, 44]
	lsl	w0, w0, 2
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	bne	.L1469
	adrp	x0, __func__.7542
	add	x1, x0, :lo12:__func__.7542
	adrp	x0, .LC285
	add	x0, x0, :lo12:.LC285
	mov	w2, 1422
	bl	nand_print
	b	.L1468
.L1469:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldrsw	x1, [x29, 44]
	lsl	w1, w1, 2
	mov	w2, w1
	mov	w1, -1
	bl	nand_memset
	ldr	w0, [x29, 44]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L1470
	adrp	x0, __func__.7542
	add	x1, x0, :lo12:__func__.7542
	adrp	x0, .LC286
	add	x0, x0, :lo12:.LC286
	mov	w2, 1428
	bl	nand_print
	b	.L1468
.L1470:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 44]
	mov	w2, w1
	mov	w1, 0
	bl	nand_memset
	ldr	x0, [x29, 16]
	ldr	x1, [x29, 32]
	str	x1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w6, w0
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x2, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x3, [x0, 8]
	adrp	x0, .LC287
	add	x0, x0, :lo12:.LC287
	mov	x5, x3
	mov	x4, x2
	mov	x3, x1
	ldr	x2, [x29, 32]
	mov	w1, w6
	bl	nand_print
	mov	w0, 0
	b	.L1466
.L1468:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1471
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	bl	nftl_free
.L1471:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	beq	.L1472
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	bl	nftl_free
.L1472:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	beq	.L1473
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	bl	nftl_free
.L1473:
	ldr	x0, [x29, 32]
	bl	nftl_free
	ldr	x0, [x29, 16]
	str	xzr, [x0]
	mov	w0, -1
.L1466:
	ldp	x29, x30, [sp], 48
	ret
	.size	init_nrc, .-init_nrc
	.align	2
	.global	free_nrc
	.type	free_nrc, %function
free_nrc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1475
	mov	w0, 0
	b	.L1476
.L1475:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	nftl_free
	ldr	x0, [x29, 24]
	bl	nftl_free
	mov	w0, 0
.L1476:
	ldp	x29, x30, [sp], 32
	ret
	.size	free_nrc, .-free_nrc
	.section	.rodata
	.align	3
.LC288:
	.string	"[NE]cross_talk_rebuild_current_block fail 1\n"
	.align	3
.LC289:
	.string	"[NE]cross_talk_rebuild_current_block!! %d,%d,%d!\n"
	.align	3
.LC290:
	.string	"[NE]cross_talk_rebuild_current_block fail 2\n"
	.align	3
.LC291:
	.string	"[NE]cross_talk_rebuild_current_block!! %d,%d,%d,%d,%d,%d!\n"
	.align	3
.LC292:
	.string	"[NE]cross_talk_rebuild_current_block fail 3\n"
	.align	3
.LC293:
	.string	"[NE]corss talk rebuild 1 %d %d %d!\n"
	.align	3
.LC294:
	.string	"[NE]corss talk rebuild 2 %d %d %d!\n"
	.align	3
.LC295:
	.string	"[NE]get free block fail!\n"
	.align	3
.LC296:
	.string	"[NE]something is error here 14!\n"
	.align	3
.LC297:
	.string	"[NE]page lost %d %d\n"
	.text
	.align	2
	.global	cross_talk_rebuild_current_block
	.type	cross_talk_rebuild_current_block, %function
cross_talk_rebuild_current_block:
	stp	x29, x30, [sp, -176]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	wzr, [x29, 160]
	str	xzr, [x29, 56]
	str	xzr, [x29, 144]
	str	xzr, [x29, 136]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	init_nrc
	cmp	w0, 0
	beq	.L1478
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC288
	add	x0, x0, :lo12:.LC288
	bl	nand_print
	mov	x0, 0
	b	.L1496
.L1478:
	ldr	x1, [x29, 56]
	add	x0, x29, 112
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	get_used_page_num
	str	w0, [x29, 132]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1480
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	adrp	x0, .LC289
	add	x0, x0, :lo12:.LC289
	mov	w3, w1
	ldr	w2, [x29, 132]
	mov	w1, w4
	bl	nand_print
	ldr	x0, [x29, 32]
	str	x0, [x29, 152]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 164]
	ldr	x0, [x29, 56]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1481
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC290
	add	x0, x0, :lo12:.LC290
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1496
.L1481:
	ldr	w0, [x29, 112]
	cmp	w0, 0
	beq	.L1483
	ldr	w0, [x29, 164]
	add	w0, w0, 20
	str	w0, [x29, 164]
	b	.L1483
.L1480:
	ldr	x1, [x29, 56]
	add	x0, x29, 108
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_used_page_num
	str	w0, [x29, 128]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 8]
	adrp	x0, .LC291
	add	x0, x0, :lo12:.LC291
	mov	w6, w2
	ldr	w5, [x29, 128]
	mov	w4, w3
	mov	w3, w1
	ldr	w2, [x29, 132]
	mov	w1, w7
	bl	nand_print
	ldr	x0, [x29, 56]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1484
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC292
	add	x0, x0, :lo12:.LC292
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1496
.L1484:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 132]
	cmp	w1, w0
	bcc	.L1485
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 164]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 160]
	b	.L1486
.L1485:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 164]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 160]
.L1486:
	ldr	w0, [x29, 112]
	cmp	w0, 0
	bne	.L1487
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L1488
.L1487:
	ldr	w0, [x29, 164]
	add	w0, w0, 20
	str	w0, [x29, 164]
	ldr	w0, [x29, 160]
	add	w0, w0, 20
	str	w0, [x29, 160]
.L1488:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 132]
	cmp	w1, w0
	bls	.L1489
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	adrp	x0, .LC293
	add	x0, x0, :lo12:.LC293
	mov	w3, w1
	ldr	w2, [x29, 128]
	ldr	w1, [x29, 132]
	bl	nand_print
	ldr	w0, [x29, 164]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 24]
	str	x0, [x29, 152]
	b	.L1483
.L1489:
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	adrp	x0, .LC294
	add	x0, x0, :lo12:.LC294
	mov	w3, w1
	ldr	w2, [x29, 128]
	ldr	w1, [x29, 132]
	bl	nand_print
	ldr	w0, [x29, 164]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 32]
	str	x0, [x29, 152]
.L1483:
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 120]
	ldr	x0, [x29, 120]
	cmp	x0, 0
	bne	.L1490
	adrp	x0, .LC295
	add	x0, x0, :lo12:.LC295
	bl	nand_print
	mov	x0, 0
	b	.L1496
.L1490:
	ldr	x0, [x29, 152]
	ldr	w0, [x0, 8]
	add	w1, w0, 1
	ldr	x0, [x29, 120]
	str	w1, [x0, 8]
	ldr	x0, [x29, 120]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1491
	adrp	x0, .LC296
	add	x0, x0, :lo12:.LC296
	bl	nand_print
	b	.L1483
.L1491:
	str	wzr, [x29, 172]
	str	wzr, [x29, 168]
	b	.L1492
.L1495:
	ldr	x0, [x29, 56]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 172]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	beq	.L1493
	ldr	x0, [x29, 56]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 172]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 136]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 136]
	add	x0, x1, x0
	str	x0, [x29, 144]
	ldr	x0, [x29, 120]
	ldrh	w1, [x0]
	ldr	w0, [x29, 168]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 64
	ldr	x5, [x29, 144]
	ldr	x4, [x29, 136]
	bl	set_physic_op_par
	ldr	x0, [x29, 144]
	bl	get_special_data_from_oob
	str	w0, [x29, 116]
	ldr	x0, [x29, 120]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 6]
	mov	w4, w0
	mov	w3, w1
	ldr	w2, [x29, 116]
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	w0, [x29, 168]
	add	w0, w0, 1
	str	w0, [x29, 168]
	b	.L1494
.L1493:
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	adrp	x0, .LC297
	add	x0, x0, :lo12:.LC297
	mov	w2, w1
	ldr	w1, [x29, 172]
	bl	nand_print
.L1494:
	ldr	w0, [x29, 172]
	add	w0, w0, 1
	str	w0, [x29, 172]
.L1492:
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	ldr	w0, [x29, 172]
	cmp	w1, w0
	bhi	.L1495
	ldr	w0, [x29, 160]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 56]
	bl	free_nrc
	ldr	x0, [x29, 120]
.L1496:
	ldp	x29, x30, [sp], 176
	ret
	.size	cross_talk_rebuild_current_block, .-cross_talk_rebuild_current_block
	.section	.rodata
	.align	3
.LC298:
	.string	"[NI]nctrcb1 %d,%d,%d!\n"
	.align	3
.LC299:
	.string	"[NE]no_cross_talk_rebuild_current_block fail 2\n"
	.align	3
.LC300:
	.string	"[NI]nctrcb2 %d,%d,%d,%d,%d,%d!\n"
	.align	3
.LC301:
	.string	"[NE]no_cross_talk_rebuild_current_block fail 4\n"
	.align	3
.LC302:
	.string	"[NE]no_cross_talk_rebuild_current_block erase block fail %d!\n"
	.text
	.align	2
	.global	no_cross_talk_rebuild_current_block
	.type	no_cross_talk_rebuild_current_block, %function
no_cross_talk_rebuild_current_block:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	xzr, [x29, 152]
	str	xzr, [x29, 144]
	str	xzr, [x29, 64]
	str	xzr, [x29, 56]
	str	xzr, [x29, 160]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	init_nrc
	cmp	w0, 0
	beq	.L1498
	ldr	x0, [x29, 64]
	bl	free_nrc
	adrp	x0, .LC288
	add	x0, x0, :lo12:.LC288
	bl	nand_print
	mov	x0, 0
	b	.L1516
.L1498:
	ldr	x1, [x29, 64]
	add	x0, x29, 124
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	get_used_page_num_no_crosstalk
	str	w0, [x29, 140]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1500
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	adrp	x0, .LC298
	add	x0, x0, :lo12:.LC298
	mov	w3, w1
	ldr	w2, [x29, 140]
	mov	w1, w4
	bl	nand_print
	ldr	x0, [x29, 32]
	str	x0, [x29, 176]
	ldr	x0, [x29, 64]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1501
	ldr	x0, [x29, 64]
	bl	free_nrc
	adrp	x0, .LC299
	add	x0, x0, :lo12:.LC299
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1516
.L1501:
	ldr	x0, [x29, 64]
	str	x0, [x29, 160]
.L1502:
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 168]
	ldr	x0, [x29, 168]
	cmp	x0, 0
	bne	.L1503
	adrp	x0, .LC295
	add	x0, x0, :lo12:.LC295
	bl	nand_print
	mov	x0, 0
	b	.L1516
.L1503:
	ldr	x0, [x29, 176]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 168]
	str	w1, [x0, 8]
	ldr	x0, [x29, 168]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 168]
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1504
	adrp	x0, .LC296
	add	x0, x0, :lo12:.LC296
	bl	nand_print
	b	.L1502
.L1500:
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	init_nrc
	cmp	w0, 0
	beq	.L1505
	ldr	x0, [x29, 64]
	bl	free_nrc
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC292
	add	x0, x0, :lo12:.LC292
	bl	nand_print
	mov	x0, 0
	b	.L1516
.L1505:
	ldr	x1, [x29, 56]
	add	x0, x29, 120
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_used_page_num_no_crosstalk
	str	w0, [x29, 136]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 8]
	adrp	x0, .LC300
	add	x0, x0, :lo12:.LC300
	mov	w6, w2
	ldr	w5, [x29, 136]
	mov	w4, w3
	mov	w3, w1
	ldr	w2, [x29, 140]
	mov	w1, w7
	bl	nand_print
	ldr	x0, [x29, 56]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1506
	ldr	x0, [x29, 64]
	bl	free_nrc
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC301
	add	x0, x0, :lo12:.LC301
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1516
.L1506:
	ldr	w1, [x29, 136]
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bls	.L1507
	ldr	x0, [x29, 24]
	str	x0, [x29, 176]
	ldr	x0, [x29, 32]
	str	x0, [x29, 168]
	ldr	x0, [x29, 56]
	str	x0, [x29, 160]
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	adrp	x0, .LC293
	add	x0, x0, :lo12:.LC293
	mov	w3, w1
	ldr	w2, [x29, 136]
	ldr	w1, [x29, 140]
	bl	nand_print
	b	.L1508
.L1507:
	ldr	x0, [x29, 32]
	str	x0, [x29, 176]
	ldr	x0, [x29, 24]
	str	x0, [x29, 168]
	ldr	x0, [x29, 64]
	str	x0, [x29, 160]
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	adrp	x0, .LC294
	add	x0, x0, :lo12:.LC294
	mov	w3, w1
	ldr	w2, [x29, 136]
	ldr	w1, [x29, 140]
	bl	nand_print
.L1508:
	ldr	x0, [x29, 168]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 96
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 96
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 132]
	ldr	w0, [x29, 132]
	cmp	w0, 0
	beq	.L1509
	ldr	x0, [x29, 168]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC302
	add	x0, x0, :lo12:.LC302
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 96
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	x0, [x29, 176]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 176]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 28]
.L1510:
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 168]
	ldr	x0, [x29, 168]
	cmp	x0, 0
	bne	.L1511
	adrp	x0, .LC295
	add	x0, x0, :lo12:.LC295
	bl	nand_print
	mov	x0, 0
	b	.L1516
.L1511:
	ldr	x0, [x29, 168]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 168]
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1509
	adrp	x0, .LC296
	add	x0, x0, :lo12:.LC296
	bl	nand_print
	b	.L1510
.L1509:
	ldr	x0, [x29, 176]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 168]
	str	w1, [x0, 8]
.L1504:
	str	wzr, [x29, 188]
	str	wzr, [x29, 184]
	b	.L1512
.L1515:
	ldr	x0, [x29, 160]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 188]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	beq	.L1513
	ldr	x0, [x29, 160]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 188]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 144]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 144]
	add	x0, x1, x0
	str	x0, [x29, 152]
	ldr	x0, [x29, 168]
	ldrh	w1, [x0]
	ldr	w0, [x29, 184]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 72
	ldr	x5, [x29, 152]
	ldr	x4, [x29, 144]
	bl	set_physic_op_par
	ldr	x0, [x29, 152]
	bl	get_special_data_from_oob
	str	w0, [x29, 128]
	ldr	x0, [x29, 168]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	x0, [x29, 168]
	ldrh	w0, [x0, 6]
	mov	w4, w0
	mov	w3, w1
	ldr	w2, [x29, 128]
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 72
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	w0, [x29, 184]
	add	w0, w0, 1
	str	w0, [x29, 184]
	b	.L1514
.L1513:
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	adrp	x0, .LC297
	add	x0, x0, :lo12:.LC297
	mov	w2, w1
	ldr	w1, [x29, 188]
	bl	nand_print
.L1514:
	ldr	w0, [x29, 188]
	add	w0, w0, 1
	str	w0, [x29, 188]
.L1512:
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	ldr	w0, [x29, 188]
	cmp	w1, w0
	bhi	.L1515
	ldr	x0, [x29, 176]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 176]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 64]
	bl	free_nrc
	ldr	x0, [x29, 56]
	bl	free_nrc
	ldr	x0, [x29, 168]
.L1516:
	ldp	x29, x30, [sp], 192
	ret
	.size	no_cross_talk_rebuild_current_block, .-no_cross_talk_rebuild_current_block
	.section	.rodata
	.align	3
.LC303:
	.string	"[NI]erase_block block %d!\n"
	.align	3
.LC304:
	.string	"[NE]erase_block block fail %d!\n"
	.text
	.align	2
	.global	erase_block
	.type	erase_block, %function
erase_block:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	strh	w2, [x29, 30]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC303
	add	x0, x0, :lo12:.LC303
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1518
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC304
	add	x0, x0, :lo12:.LC304
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 32]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 28]
	b	.L1519
.L1518:
	ldr	x0, [x29, 32]
	ldrh	w1, [x29, 30]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 32]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	put_phy_block_to_free_list
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	mark_free_block
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 20]
	ldr	x0, [x29, 32]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 2]
.L1519:
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	erase_block, .-erase_block
	.align	2
	.global	nftl_check_pattern
	.type	nftl_check_pattern, %function
nftl_check_pattern:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strb	w1, [sp, 7]
	str	w2, [sp]
	str	wzr, [sp, 28]
	b	.L1522
.L1525:
	ldrsw	x0, [sp, 28]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	ldrb	w1, [sp, 7]
	cmp	w1, w0
	beq	.L1523
	mov	w0, 1
	b	.L1524
.L1523:
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1522:
	ldr	w1, [sp, 28]
	ldr	w0, [sp]
	cmp	w1, w0
	blt	.L1525
	mov	w0, 0
.L1524:
	add	sp, sp, 32
	ret
	.size	nftl_check_pattern, .-nftl_check_pattern
	.data
	.align	3
	.type	patterns, %object
	.size	patterns, 1
patterns:
	.byte	-91
	.section	.rodata
	.align	3
.LC305:
	.string	"[NI]torture b@%d.\n"
	.align	3
.LC306:
	.string	"[NE]%s write b@%d p@%d err\n"
	.align	3
.LC307:
	.string	"[NE]%s read b@%d p@%d err\n"
	.align	3
.LC308:
	.string	"[NE]%s b@%d p@%d patt %02x checking main data fail\n"
	.align	3
.LC309:
	.string	"[NE]%s b@%d p@%d patt %02x checking spare data fail\n"
	.align	3
.LC310:
	.string	"[NE][NE]erase_block block fail %d!\n"
	.text
	.align	2
	.global	torture_block_and_mark
	.type	torture_block_and_mark, %function
torture_block_and_mark:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 92]
	str	wzr, [x29, 88]
	str	wzr, [x29, 80]
	str	wzr, [x29, 84]
	mov	w0, 3
	str	w0, [x29, 76]
	str	xzr, [x29, 56]
	str	wzr, [x29, 64]
	strh	wzr, [x29, 68]
	strb	wzr, [x29, 70]
	mov	w0, 1
	str	w0, [x29, 80]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC305
	add	x0, x0, :lo12:.LC305
	bl	nand_print
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 32
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1527
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC304
	add	x0, x0, :lo12:.LC304
	bl	nand_print
	b	.L1528
.L1527:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 6]
	str	wzr, [x29, 88]
	b	.L1529
.L1538:
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	mov	x2, x0
	mov	x0, x3
	bl	memset
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w1, w0
	add	x0, x29, 56
	mov	x2, 15
	bl	memset
	str	wzr, [x29, 84]
	b	.L1530
.L1532:
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 84]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 56
	add	x0, x29, 32
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1531
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, __func__.7632
	add	x1, x0, :lo12:__func__.7632
	adrp	x0, .LC306
	add	x0, x0, :lo12:.LC306
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1528
.L1531:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L1530:
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	blt	.L1532
	str	wzr, [x29, 84]
	b	.L1533
.L1537:
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	mov	x2, x0
	mov	w1, 0
	mov	x0, x3
	bl	memset
	add	x0, x29, 56
	mov	x2, 15
	mov	w1, 0
	bl	memset
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 84]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 56
	add	x0, x29, 32
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1534
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, __func__.7632
	add	x1, x0, :lo12:__func__.7632
	adrp	x0, .LC307
	add	x0, x0, :lo12:.LC307
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1528
.L1534:
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w1, [x1, x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	x0, x3
	bl	nftl_check_pattern
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1535
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w3, w0
	adrp	x0, __func__.7632
	add	x1, x0, :lo12:__func__.7632
	adrp	x0, .LC308
	add	x0, x0, :lo12:.LC308
	mov	w4, w3
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1528
.L1535:
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w1, [x1, x0]
	add	x0, x29, 56
	mov	w2, 15
	bl	nftl_check_pattern
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1536
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w3, w0
	adrp	x0, __func__.7632
	add	x1, x0, :lo12:__func__.7632
	adrp	x0, .LC309
	add	x0, x0, :lo12:.LC309
	mov	w4, w3
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1528
.L1536:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L1533:
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	blt	.L1537
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L1529:
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 80]
	cmp	w1, w0
	blt	.L1538
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 32
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1539
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC310
	add	x0, x0, :lo12:.LC310
	bl	nand_print
	b	.L1528
.L1539:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 16]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	mark_free_block
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 20]
	ldr	x0, [x29, 16]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	strh	wzr, [x0, 2]
	mov	w0, 0
	b	.L1541
.L1528:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 16]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 28]
	ldr	w0, [x29, 92]
.L1541:
	ldp	x29, x30, [sp], 96
	ret
	.size	torture_block_and_mark, .-torture_block_and_mark
	.align	2
	.global	mark_free_block
	.type	mark_free_block, %function
mark_free_block:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	add	x0, x29, 40
	mov	w2, 16
	mov	w1, -91
	bl	nand_memset
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 40
	add	x0, x29, 56
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 40
	add	x0, x29, 56
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 1
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #2
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 40
	add	x0, x29, 56
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 40
	add	x0, x29, 56
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.size	mark_free_block, .-mark_free_block
	.align	2
	.global	is_last_phy_block
	.type	is_last_phy_block, %function
is_last_phy_block:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L1545
	mov	w0, 1
	b	.L1546
.L1545:
	mov	w0, 0
.L1546:
	add	sp, sp, 16
	ret
	.size	is_last_phy_block, .-is_last_phy_block
	.align	2
	.global	is_phy_block_valid
	.type	is_phy_block_valid, %function
is_phy_block_valid:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	ldrh	w1, [x0]
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcs	.L1548
	mov	w0, 1
	b	.L1549
.L1548:
	mov	w0, 0
.L1549:
	add	sp, sp, 16
	ret
	.size	is_phy_block_valid, .-is_phy_block_valid
	.align	2
	.global	get_phy_block_addr
	.type	get_phy_block_addr, %function
get_phy_block_addr:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldrh	w0, [sp, 6]
	lsl	x0, x0, 6
	add	x0, x1, x0
	add	sp, sp, 16
	ret
	.size	get_phy_block_addr, .-get_phy_block_addr
	.section	.rodata
	.align	3
.LC311:
	.string	"[NE]get_used_page_num ecc error block:%d page:%d!\n"
	.align	3
.LC312:
	.string	"[NE]power off ecc error 4!!\n"
	.align	3
.LC313:
	.string	"[NE]sb@%d sp@%d get_used_page_num used spare data %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n"
	.text
	.align	2
	.global	get_used_page_num
	.type	get_used_page_num, %function
get_used_page_num:
	sub	sp, sp, #224
	stp	x29, x30, [sp, 96]
	add	x29, sp, 96
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	x3, [x29, 16]
	ldr	x0, [x29, 24]
	str	wzr, [x0]
	str	wzr, [x29, 120]
	str	wzr, [x29, 124]
	b	.L1553
.L1565:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	beq	.L1554
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L1554
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 124]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 112]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 112]
	add	x0, x1, x0
	str	x0, [x29, 104]
	b	.L1555
.L1554:
	str	xzr, [x29, 112]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	cmp	w0, 8
	bne	.L1556
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	str	x0, [x29, 112]
.L1556:
	add	x0, x29, 80
	str	x0, [x29, 104]
.L1555:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1557
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	ldr	x4, [x29, 112]
	bl	set_physic_op_par
	b	.L1558
.L1557:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
.L1558:
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1559
	ldr	w0, [x29, 100]
	cmp	w0, 10
	beq	.L1559
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC311
	add	x0, x0, :lo12:.LC311
	ldr	w2, [x29, 124]
	bl	nand_print
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	add	w1, w0, 1
	ldr	x0, [x29, 24]
	str	w1, [x0]
.L1559:
	ldr	x0, [x29, 104]
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1576
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1562
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1563
.L1562:
	ldr	x0, [x29, 112]
	cmp	x0, 0
	beq	.L1563
	ldr	x2, [x29, 104]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 40]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1564
	adrp	x0, .LC312
	add	x0, x0, :lo12:.LC312
	bl	nand_print
	b	.L1563
.L1564:
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	mov	w1, 1
	strb	w1, [x0]
	ldr	x0, [x29, 16]
	ldr	w1, [x0]
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	cmp	w1, w0
	bcs	.L1563
	ldr	w0, [x29, 124]
	add	w1, w0, 1
	ldr	x0, [x29, 16]
	str	w1, [x0]
.L1563:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1553:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bhi	.L1565
	b	.L1561
.L1576:
	nop
.L1561:
	ldr	w0, [x29, 124]
	str	w0, [x29, 96]
	b	.L1566
.L1574:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	beq	.L1567
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L1567
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 124]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 112]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 112]
	add	x0, x1, x0
	str	x0, [x29, 104]
	b	.L1568
.L1567:
	str	xzr, [x29, 112]
	add	x0, x29, 80
	str	x0, [x29, 104]
.L1568:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1569
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	ldr	x4, [x29, 112]
	bl	set_physic_op_par
	b	.L1570
.L1569:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
.L1570:
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	x0, [x29, 104]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1571
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	str	w0, [x29, 120]
	b	.L1572
.L1571:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w12, w0
	ldr	x0, [x29, 104]
	ldrb	w0, [x0]
	mov	w13, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w14, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w15, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w16, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w17, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 5
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 6
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 7
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 8
	ldrb	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 9
	ldrb	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 10
	ldrb	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 11
	ldrb	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 12
	ldrb	w0, [x0]
	mov	w8, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 13
	ldrb	w0, [x0]
	mov	w9, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 14
	ldrb	w0, [x0]
	mov	w10, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 15
	ldrb	w0, [x0]
	mov	w11, w0
	adrp	x0, .LC313
	add	x0, x0, :lo12:.LC313
	str	w11, [sp, 80]
	str	w10, [sp, 72]
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w17
	mov	w6, w16
	mov	w5, w15
	mov	w4, w14
	mov	w3, w13
	ldr	w2, [x29, 124]
	mov	w1, w12
	bl	nand_print
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1573
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1572
.L1573:
	ldr	x0, [x29, 112]
	cmp	x0, 0
	beq	.L1572
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	mov	w1, 1
	strb	w1, [x0]
	ldr	x0, [x29, 16]
	ldr	w1, [x0]
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	cmp	w1, w0
	bcs	.L1572
	ldr	w0, [x29, 124]
	add	w1, w0, 1
	ldr	x0, [x29, 16]
	str	w1, [x0]
.L1572:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1566:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bhi	.L1574
	ldr	w0, [x29, 96]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 128
	ret
	.size	get_used_page_num, .-get_used_page_num
	.section	.rodata
	.align	3
.LC314:
	.string	"[NE]gupnnc ecc err b:%d p:%d!\n"
	.align	3
.LC315:
	.string	"[NE]gupnnc power off ecc err!!\n"
	.align	3
.LC316:
	.string	"[NE]cross talk block:%d total_good_num %d good_page_num: %d!\n"
	.text
	.align	2
	.global	get_used_page_num_no_crosstalk
	.type	get_used_page_num_no_crosstalk, %function
get_used_page_num_no_crosstalk:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	x3, [x29, 32]
	mov	w0, -1
	str	w0, [x29, 132]
	str	wzr, [x29, 128]
	str	wzr, [x29, 124]
	ldr	x0, [x29, 40]
	str	wzr, [x0]
	str	wzr, [x29, 156]
	b	.L1578
.L1589:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1579
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 156]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L1579
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 156]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 144]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 144]
	add	x0, x1, x0
	str	x0, [x29, 136]
	b	.L1580
.L1579:
	str	xzr, [x29, 144]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	cmp	w0, 4
	bne	.L1581
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 512]
	str	x0, [x29, 144]
.L1581:
	add	x0, x29, 96
	str	x0, [x29, 136]
.L1580:
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 156]
	uxth	w2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 72
	ldr	x5, [x29, 136]
	ldr	x4, [x29, 144]
	bl	set_physic_op_par
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 72
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 120]
	ldr	w0, [x29, 120]
	cmp	w0, 0
	beq	.L1582
	ldr	w0, [x29, 120]
	cmp	w0, 10
	beq	.L1582
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC314
	add	x0, x0, :lo12:.LC314
	ldr	w2, [x29, 156]
	bl	nand_print
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1584
	ldr	w0, [x29, 156]
	str	w0, [x29, 132]
	b	.L1584
.L1582:
	ldr	x0, [x29, 136]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1585
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1586
	ldr	w0, [x29, 156]
	str	w0, [x29, 132]
.L1586:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
	ldr	w0, [x29, 124]
	cmp	w0, 3
	bne	.L1584
	b	.L1587
.L1585:
	ldr	x2, [x29, 136]
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 56]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1588
	adrp	x0, .LC315
	add	x0, x0, :lo12:.LC315
	bl	nand_print
	b	.L1584
.L1588:
	ldr	w0, [x29, 128]
	add	w0, w0, 1
	str	w0, [x29, 128]
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1584
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 156]
	add	x0, x1, x0
	mov	w1, 1
	strb	w1, [x0]
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 24]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 2
	add	x19, x1, x0
	ldr	x0, [x29, 136]
	bl	get_logic_page_from_oob
	str	w0, [x19]
	ldr	x0, [x29, 32]
	ldr	w1, [x0]
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	cmp	w1, w0
	bcs	.L1584
	ldr	w0, [x29, 156]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0]
.L1584:
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	str	w0, [x29, 156]
.L1578:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 156]
	cmp	w1, w0
	bhi	.L1589
.L1587:
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1590
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	str	w0, [x29, 132]
	b	.L1591
.L1590:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 132]
	cmp	w1, w0
	beq	.L1591
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC316
	add	x0, x0, :lo12:.LC316
	ldr	w3, [x29, 132]
	ldr	w2, [x29, 128]
	bl	nand_print
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 168]
	add	w1, w1, 1
	str	w1, [x0, 168]
	str	wzr, [x29, 156]
	b	.L1592
.L1594:
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 156]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	beq	.L1593
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 156]
	mul	w0, w2, w0
	uxtw	x2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	add	x0, x2, x0
	add	x0, x1, x0
	str	x0, [x29, 136]
	ldr	x0, [x29, 136]
	add	x0, x0, 14
	ldr	x1, [x29, 56]
	ldr	x1, [x1, 3440]
	ldr	w1, [x1, 168]
	uxtb	w1, w1
	strb	w1, [x0]
.L1593:
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	str	w0, [x29, 156]
.L1592:
	ldr	x0, [x29, 32]
	ldr	w1, [x0]
	ldr	w0, [x29, 156]
	cmp	w1, w0
	bhi	.L1594
.L1591:
	ldr	w0, [x29, 132]
	str	w0, [x29, 116]
	ldr	w0, [x29, 116]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 160
	ret
	.size	get_used_page_num_no_crosstalk, .-get_used_page_num_no_crosstalk
	.align	2
	.global	check_cross_talk
	.type	check_cross_talk, %function
check_cross_talk:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	str	x0, [x29, 96]
	str	wzr, [x29, 108]
	b	.L1597
.L1600:
	str	xzr, [x29, 96]
	add	x0, x29, 64
	str	x0, [x29, 88]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 40
	ldr	x5, [x29, 88]
	ldr	x4, [x29, 96]
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 84]
	ldr	w0, [x29, 84]
	cmp	w0, 0
	bne	.L1598
	ldr	x0, [x29, 88]
	add	x0, x0, 14
	ldrb	w0, [x0]
	cmp	w0, 99
	bhi	.L1598
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 168]
	ldr	x1, [x29, 88]
	add	x1, x1, 14
	ldrb	w1, [x1]
	cmp	w0, w1
	bhi	.L1598
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 88]
	add	x1, x1, 14
	ldrb	w1, [x1]
	str	w1, [x0, 168]
	b	.L1599
.L1598:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1597:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bhi	.L1600
.L1599:
	mov	w0, 0
	ldp	x29, x30, [sp], 112
	ret
	.size	check_cross_talk, .-check_cross_talk
	.align	2
	.global	init_smart_info
	.type	init_smart_info, %function
init_smart_info:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	mov	w2, 320
	mov	w1, 0
	bl	nand_memset
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	mov	w1, 43521
	movk	w1, 0xaaaa, lsl 16
	str	w1, [x0, 112]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 140]
	add	w1, w1, 1
	str	w1, [x0, 140]
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L1603
	mov	w0, 0
	b	.L1609
.L1603:
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 104
	add	x0, x29, 80
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 80
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 124]
	add	x0, x29, 104
	bl	is_phy_mapping_page
	cmp	w0, 1
	bne	.L1605
	ldr	w0, [x29, 124]
	cmp	w0, 0
	beq	.L1606
	ldr	w0, [x29, 124]
	cmp	w0, 10
	bne	.L1605
.L1606:
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	ldr	w1, [x0, 112]
	mov	w0, 43521
	movk	w0, 0xaaaa, lsl 16
	cmp	w1, w0
	bne	.L1605
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 64]
	mov	w2, 320
	bl	nand_memcpy
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 140]
	add	w1, w1, 1
	str	w1, [x0, 140]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 164]
	cmn	w0, #1
	bne	.L1607
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 164]
.L1607:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 172]
	cmn	w0, #1
	bne	.L1608
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 172]
.L1608:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 176]
	cmn	w0, #1
	bne	.L1605
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 176]
.L1605:
	ldr	x0, [x29, 24]
	bl	print_smart
	mov	w0, 0
.L1609:
	ldp	x29, x30, [sp], 128
	ret
	.size	init_smart_info, .-init_smart_info
	.align	2
	.global	get_used_block_count
	.type	get_used_block_count, %function
get_used_block_count:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	add	x2, x29, 32
	add	x0, x29, 48
	mov	x5, x2
	mov	x4, 0
	mov	w3, 0
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	add	x0, x29, 32
	bl	get_block_used_count_from_oob
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	get_used_block_count, .-get_used_block_count
	.section	.rodata
	.align	3
.LC317:
	.string	"[NE]current_block_ecc_error no free block!!!\n"
	.align	3
.LC318:
	.string	"[NE]do current_block_ecc_error %d!!\n"
	.align	3
.LC319:
	.string	"[NE]power off ecc error 2.\n"
	.align	3
.LC320:
	.string	"[NE]current_block_ecc ok page %d %d!!\n"
	.align	3
.LC321:
	.string	"[NE]current_block_ecc_error page %d!!\n"
	.align	3
.LC322:
	.string	"[NE]current_block_ecc_error invalid page data %d!!\n"
	.align	3
.LC323:
	.string	"[NE]current_block_ecc_error unkown page data %d!!\n"
	.align	3
.LC324:
	.string	"[ND]do current_block_ecc_error end %d %d!!\n"
	.text
	.align	2
	.global	current_block_ecc_error
	.type	current_block_ecc_error, %function
current_block_ecc_error:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
.L1613:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 112]
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L1614
	adrp	x0, .LC317
	add	x0, x0, :lo12:.LC317
	bl	nand_print
	mov	x0, 0
	b	.L1629
.L1614:
	ldr	x0, [x29, 16]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 112]
	str	w1, [x0, 8]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1616
	adrp	x0, .LC263
	add	x0, x0, :lo12:.LC263
	bl	nand_print
	b	.L1613
.L1616:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC318
	add	x0, x0, :lo12:.LC318
	bl	nand_print
	str	wzr, [x29, 124]
	str	wzr, [x29, 120]
	b	.L1617
.L1626:
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 88
	add	x0, x29, 64
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 108]
	add	x0, x29, 88
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1618
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L1619
	ldr	w0, [x29, 108]
	cmp	w0, 10
	bne	.L1620
.L1619:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	add	x1, x29, 88
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1621
	adrp	x0, .LC319
	add	x0, x0, :lo12:.LC319
	bl	nand_print
	b	.L1624
.L1621:
	adrp	x0, .LC320
	add	x0, x0, :lo12:.LC320
	ldr	w2, [x29, 108]
	ldr	w1, [x29, 124]
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	ldr	w0, [x29, 120]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 88
	add	x0, x29, 40
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	str	w0, [x29, 120]
	b	.L1624
.L1620:
	adrp	x0, .LC321
	add	x0, x0, :lo12:.LC321
	ldr	w1, [x29, 124]
	bl	nand_print
	b	.L1624
.L1618:
	add	x0, x29, 88
	bl	is_ftl_special_data
	cmp	w0, 1
	bne	.L1625
	adrp	x0, .LC322
	add	x0, x0, :lo12:.LC322
	ldr	w1, [x29, 124]
	bl	nand_print
	b	.L1624
.L1625:
	add	x0, x29, 88
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1624
	adrp	x0, .LC323
	add	x0, x0, :lo12:.LC323
	ldr	w1, [x29, 124]
	bl	nand_print
.L1624:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1617:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bgt	.L1626
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1627
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L1628
.L1627:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	erase_block
.L1628:
	adrp	x0, .LC324
	add	x0, x0, :lo12:.LC324
	ldr	w2, [x29, 120]
	ldr	w1, [x29, 124]
	bl	nand_print_dbg
	ldr	x0, [x29, 112]
.L1629:
	ldp	x29, x30, [sp], 128
	ret
	.size	current_block_ecc_error, .-current_block_ecc_error
	.section	.rodata
	.align	3
.LC325:
	.string	"[NE]do block_last_page_ecc_error!!\n"
	.align	3
.LC326:
	.string	"[NE]%s:malloc fail 1!\n"
	.align	3
.LC327:
	.string	"[NE]%s:malloc fail 2!\n"
	.align	3
.LC328:
	.string	"[NE]current_block_ecc_error invalid page data!!\n"
	.align	3
.LC329:
	.string	"[NE]current_block_ecc_error unkown page data!!\n"
	.align	3
.LC330:
	.string	"[NE]do block_last_page_ecc_error end!!\n"
	.text
	.align	2
	.global	block_last_page_ecc_error
	.type	block_last_page_ecc_error, %function
block_last_page_ecc_error:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	adrp	x0, .LC325
	add	x0, x0, :lo12:.LC325
	bl	nand_print
	str	wzr, [x29, 104]
	mov	w0, 8192
	bl	nftl_malloc
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	cmp	x0, 0
	bne	.L1631
	adrp	x0, __func__.7756
	add	x1, x0, :lo12:__func__.7756
	adrp	x0, .LC326
	add	x0, x0, :lo12:.LC326
	bl	nand_print
.L1631:
	str	wzr, [x29, 108]
	b	.L1632
.L1634:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x19, x1, x0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	str	x0, [x19]
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L1633
	adrp	x0, __func__.7756
	add	x1, x0, :lo12:__func__.7756
	adrp	x0, .LC327
	add	x0, x0, :lo12:.LC327
	bl	nand_print
.L1633:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1632:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1634
	str	wzr, [x29, 108]
	b	.L1635
.L1645:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 92]
	add	x0, x29, 72
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1654
	add	x0, x29, 72
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1638
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1639
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L1641
.L1639:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x3, [x0]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	w0, [x29, 104]
	add	w0, w0, 1
	str	w0, [x29, 104]
	b	.L1641
.L1638:
	add	x0, x29, 72
	bl	is_ftl_special_data
	cmp	w0, 1
	bne	.L1642
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1643
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L1644
.L1643:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x3, [x0]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	w0, [x29, 104]
	add	w0, w0, 1
	str	w0, [x29, 104]
.L1644:
	adrp	x0, .LC328
	add	x0, x0, :lo12:.LC328
	bl	nand_print
	b	.L1641
.L1642:
	adrp	x0, .LC329
	add	x0, x0, :lo12:.LC329
	bl	nand_print
.L1641:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1635:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1645
	b	.L1637
.L1654:
	nop
.L1637:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1646
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1647
	adrp	x0, .LC317
	add	x0, x0, :lo12:.LC317
	bl	nand_print
	str	xzr, [x29, 32]
	b	.L1648
.L1647:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
.L1646:
	str	wzr, [x29, 108]
	b	.L1649
.L1650:
	ldr	x0, [x29, 32]
	ldrh	w6, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x1, [x0]
	add	x4, x29, 72
	add	x0, x29, 48
	mov	x5, x4
	mov	x4, x1
	mov	w1, w6
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1649:
	ldr	w1, [x29, 108]
	ldr	w0, [x29, 104]
	cmp	w1, w0
	blt	.L1650
.L1648:
	str	wzr, [x29, 108]
	b	.L1651
.L1652:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x0, [x0]
	bl	nftl_free
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1651:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1652
	ldr	x0, [x29, 96]
	bl	nftl_free
	adrp	x0, .LC330
	add	x0, x0, :lo12:.LC330
	bl	nand_print
	ldr	x0, [x29, 32]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 112
	ret
	.size	block_last_page_ecc_error, .-block_last_page_ecc_error
	.section	.rodata
	.align	3
.LC331:
	.string	"[NE]adjust free list fail!!\n"
	.align	3
.LC332:
	.string	"[NI]adjust free list : %d %d!!\n"
	.text
	.align	2
	.global	adjust_free_list
	.type	adjust_free_list, %function
adjust_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1656
	adrp	x0, .LC331
	add	x0, x0, :lo12:.LC331
	bl	nand_print
.L1656:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 40]
	str	x1, [x0, 64]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC332
	add	x0, x0, :lo12:.LC332
	bl	nand_print
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	adjust_free_list, .-adjust_free_list
	.align	2
	.global	nftl_get_bad_block_cnt
	.type	nftl_get_bad_block_cnt, %function
nftl_get_bad_block_cnt:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 24]
	ldrh	w0, [x0, 28]
	add	sp, sp, 16
	ret
	.size	nftl_get_bad_block_cnt, .-nftl_get_bad_block_cnt
	.section	.rodata
	.align	3
.LC333:
	.string	"[ND]nand3.x \n"
	.align	3
.LC334:
	.string	"[ND]zone->nand_chip->blk_per_chip: %d \n"
	.align	3
.LC335:
	.string	"[ND]zone->nand_chip->bytes_per_page: %d \n"
	.align	3
.LC336:
	.string	"[ND]zone->nand_chip->pages_per_blk: %d \n"
	.align	3
.LC337:
	.string	"[ND]zone->nand_chip->max_erase_times: %d \n"
	.align	3
.LC338:
	.string	"[ND]zone->nand_chip->support_read_reclaim: %d \n"
	.align	3
.LC339:
	.string	"[ND]zone->test: %d \n"
	.align	3
.LC340:
	.string	"[ND]zone->zone_no: %d \n"
	.align	3
.LC341:
	.string	"[ND]zone->zone_attr: %d \n"
	.align	3
.LC342:
	.string	"[ND]zone->blocks: %d \n"
	.align	3
.LC343:
	.string	"[ND]zone->bad_block: %d \n"
	.align	3
.LC344:
	.string	"[ND]zone->logic_cap_in_sects: %d \n"
	.align	3
.LC345:
	.string	"[ND]zone->backup_cap_in_sects: %d \n"
	.align	3
.LC346:
	.string	"[ND]zone->free_block_num: %d \n"
	.align	3
.LC347:
	.string	"[ND]zone->gc_strategy.start_gc_free_blocks: %d \n"
	.align	3
.LC348:
	.string	"[ND]zone->gc_strategy.stop_gc_free_blocks: %d \n"
	.align	3
.LC349:
	.string	"[ND]zone->gc_strategy.gc_page: %d \n"
	.align	3
.LC350:
	.string	"[ND]zone->gc_strategy.process: %d \n"
	.align	3
.LC351:
	.string	"[ND]zone->prio_gc.prio_type : %d \n"
	.align	3
.LC352:
	.string	"[ND]zone->zone_start_phy_block->phy_block.Block_NO: %d \n"
	.align	3
.LC353:
	.string	"[ND]zone->zone_end_phy_block->phy_block.Block_NO: %d \n"
	.align	3
.LC354:
	.string	"[ND]zone->zone_phy_page_map_for_gc: %x \n"
	.align	3
.LC355:
	.string	"[ND]zone->current_block.user_info: %x \n"
	.align	3
.LC356:
	.string	"[ND]zone->current_block.block_info: %x \n"
	.align	3
.LC357:
	.string	"[ND]zone->current_block.block_info->phy_block.Block_NO: %d \n"
	.align	3
.LC358:
	.string	"[ND]zone->current_block.page_used: %d \n"
	.align	3
.LC359:
	.string	"[ND]zone->current_block.block_info->block_used_count: %d \n"
	.align	3
.LC360:
	.string	"[ND]zone->recv_free_block: %d %d!!\n"
	.align	3
.LC361:
	.string	"[ND]zone->assist_block.user_info: %x \n"
	.align	3
.LC362:
	.string	"[ND]zone->assist_block.block_info: %x \n"
	.align	3
.LC363:
	.string	"[ND]zone->assist_block.block_info->phy_block.Block_NO: %d \n"
	.align	3
.LC364:
	.string	"[ND]zone->assist_block.page_used: %d \n"
	.align	3
.LC365:
	.string	"[ND]zone->assist_block.block_info->block_used_count: %d \n"
	.align	3
.LC366:
	.string	"[ND]zone->read_reclaim_complete: %x \n"
	.align	3
.LC367:
	.string	"[ND]zone->temp_page_buf: %x \n"
	.align	3
.LC368:
	.string	"[ND]zone->max_erase_num: %d \n"
	.align	3
.LC369:
	.string	"[ND]zone->cache.cache_totals: %x \n"
	.align	3
.LC370:
	.string	"[ND]zone->wl_gc_count: %d \n"
	.align	3
.LC371:
	.string	"[ND]zone->wl_gc_page_num: %d \n"
	.align	3
.LC372:
	.string	"[ND]zone->cfg->nftl_dont_use_cache: %x \n"
	.align	3
.LC373:
	.string	"[ND]zone->cfg->nftl_use_cache_sort: %x \n"
	.align	3
.LC374:
	.string	"[ND]zone->cfg->nftl_support_gc_read_reclaim: %x \n"
	.align	3
.LC375:
	.string	"[ND]zone->cfg->nftl_support_wear_leveling: %x \n"
	.align	3
.LC376:
	.string	"[ND]zone->cfg->nftl_need_erase: %x \n"
	.align	3
.LC377:
	.string	"[ND]zone->cfg->nftl_min_free_block_num: %x \n"
	.align	3
.LC378:
	.string	"[ND]zone->cfg->nftl_gc_threshold_free_block_num: %x \n"
	.align	3
.LC379:
	.string	"[ND]zone->cfg->nftl_min_free_block: %x \n"
	.align	3
.LC380:
	.string	"[ND]zone->cfg->nftl_gc_threshold_ratio_numerator: %x \n"
	.align	3
.LC381:
	.string	"[ND]zone->cfg->nftl_gc_threshold_ratio_denominator: %x \n"
	.align	3
.LC382:
	.string	"[ND]zone->cfg->nftl_max_cache_num: %d \n"
	.align	3
.LC383:
	.string	"[ND]zone->cfg->nftl_max_cache_write_num: %d \n"
	.align	3
.LC384:
	.string	"[ND]zone->cfg->nftl_cross_talk: %x \n"
	.align	3
.LC385:
	.string	"[ND]zone->cfg->nftl_read_claim_interval: %d \n"
	.text
	.align	2
	.global	print_nftl_zone
	.type	print_nftl_zone, %function
print_nftl_zone:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1664
	adrp	x0, .LC333
	add	x0, x0, :lo12:.LC333
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC334
	add	x0, x0, :lo12:.LC334
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC335
	add	x0, x0, :lo12:.LC335
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	adrp	x0, .LC336
	add	x0, x0, :lo12:.LC336
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	adrp	x0, .LC337
	add	x0, x0, :lo12:.LC337
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC338
	add	x0, x0, :lo12:.LC338
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 656]
	adrp	x0, .LC339
	add	x0, x0, :lo12:.LC339
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC340
	add	x0, x0, :lo12:.LC340
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 25]
	mov	w1, w0
	adrp	x0, .LC341
	add	x0, x0, :lo12:.LC341
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC342
	add	x0, x0, :lo12:.LC342
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 28]
	mov	w1, w0
	adrp	x0, .LC343
	add	x0, x0, :lo12:.LC343
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 32]
	adrp	x0, .LC344
	add	x0, x0, :lo12:.LC344
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 40]
	adrp	x0, .LC345
	add	x0, x0, :lo12:.LC345
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC346
	add	x0, x0, :lo12:.LC346
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	adrp	x0, .LC347
	add	x0, x0, :lo12:.LC347
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	adrp	x0, .LC348
	add	x0, x0, :lo12:.LC348
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 688]
	mov	w1, w0
	adrp	x0, .LC349
	add	x0, x0, :lo12:.LC349
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 684]
	mov	w1, w0
	adrp	x0, .LC350
	add	x0, x0, :lo12:.LC350
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 698]
	mov	w1, w0
	adrp	x0, .LC351
	add	x0, x0, :lo12:.LC351
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC352
	add	x0, x0, :lo12:.LC352
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 56]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC353
	add	x0, x0, :lo12:.LC353
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	add	x0, x0, 512
	sub	x1, x0, #48
	add	x0, x29, 16
	ldp	x2, x3, [x1]
	stp	x2, x3, [x0]
	ldp	x2, x3, [x1, 16]
	stp	x2, x3, [x0, 16]
	ldp	x1, x2, [x1, 32]
	stp	x1, x2, [x0, 32]
	add	x1, x29, 16
	adrp	x0, .LC354
	add	x0, x0, :lo12:.LC354
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	add	x0, x0, 512
	add	x1, x0, 24
	add	x0, x29, 16
	ldp	x2, x3, [x1]
	stp	x2, x3, [x0]
	ldp	x2, x3, [x1, 16]
	stp	x2, x3, [x0, 16]
	ldp	x1, x2, [x1, 32]
	stp	x1, x2, [x0, 32]
	add	x1, x29, 16
	adrp	x0, .LC355
	add	x0, x0, :lo12:.LC355
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 528]
	adrp	x0, .LC356
	add	x0, x0, :lo12:.LC356
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC357
	add	x0, x0, :lo12:.LC357
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 584]
	mov	w1, w0
	adrp	x0, .LC358
	add	x0, x0, :lo12:.LC358
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 528]
	ldr	w1, [x0, 8]
	adrp	x0, .LC359
	add	x0, x0, :lo12:.LC359
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 64]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 64]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC360
	add	x0, x0, :lo12:.LC360
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1663
	ldr	x0, [x29, 72]
	add	x0, x0, 512
	add	x1, x0, 88
	add	x0, x29, 16
	ldp	x2, x3, [x1]
	stp	x2, x3, [x0]
	ldp	x2, x3, [x1, 16]
	stp	x2, x3, [x0, 16]
	ldp	x1, x2, [x1, 32]
	stp	x1, x2, [x0, 32]
	add	x1, x29, 16
	adrp	x0, .LC361
	add	x0, x0, :lo12:.LC361
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 592]
	adrp	x0, .LC362
	add	x0, x0, :lo12:.LC362
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 592]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC363
	add	x0, x0, :lo12:.LC363
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 648]
	mov	w1, w0
	adrp	x0, .LC364
	add	x0, x0, :lo12:.LC364
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 592]
	ldr	w1, [x0, 8]
	adrp	x0, .LC365
	add	x0, x0, :lo12:.LC365
	bl	nand_print_dbg
.L1663:
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 660]
	adrp	x0, .LC366
	add	x0, x0, :lo12:.LC366
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 512]
	adrp	x0, .LC367
	add	x0, x0, :lo12:.LC367
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 676]
	adrp	x0, .LC368
	add	x0, x0, :lo12:.LC368
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 1072]
	mov	w1, w0
	adrp	x0, .LC369
	add	x0, x0, :lo12:.LC369
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 668]
	adrp	x0, .LC370
	add	x0, x0, :lo12:.LC370
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 672]
	adrp	x0, .LC371
	add	x0, x0, :lo12:.LC371
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC372
	add	x0, x0, :lo12:.LC372
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	adrp	x0, .LC373
	add	x0, x0, :lo12:.LC373
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	adrp	x0, .LC374
	add	x0, x0, :lo12:.LC374
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	adrp	x0, .LC375
	add	x0, x0, :lo12:.LC375
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 10]
	mov	w1, w0
	adrp	x0, .LC376
	add	x0, x0, :lo12:.LC376
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 12]
	mov	w1, w0
	adrp	x0, .LC377
	add	x0, x0, :lo12:.LC377
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 14]
	mov	w1, w0
	adrp	x0, .LC378
	add	x0, x0, :lo12:.LC378
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	adrp	x0, .LC379
	add	x0, x0, :lo12:.LC379
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 18]
	mov	w1, w0
	adrp	x0, .LC380
	add	x0, x0, :lo12:.LC380
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	adrp	x0, .LC381
	add	x0, x0, :lo12:.LC381
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 22]
	mov	w1, w0
	adrp	x0, .LC382
	add	x0, x0, :lo12:.LC382
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC383
	add	x0, x0, :lo12:.LC383
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC384
	add	x0, x0, :lo12:.LC384
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldr	w1, [x0, 32]
	adrp	x0, .LC385
	add	x0, x0, :lo12:.LC385
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	bl	print_smart
	b	.L1660
.L1664:
	nop
.L1660:
	ldp	x29, x30, [sp], 80
	ret
	.size	print_nftl_zone, .-print_nftl_zone
	.section	.rodata
	.align	3
.LC386:
	.string	"[NE]%s:malloc 4096 fail!\n"
	.align	3
.LC387:
	.string	"[ND]spare: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x"
	.align	3
.LC388:
	.string	"[ND]buf:%x %x %x %x %x %x %x %x %x %x %x.\n"
	.text
	.align	2
	.global	nand_dbg_phy_read
	.type	nand_dbg_phy_read, %function
nand_dbg_phy_read:
	sub	sp, sp, #144
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	strh	w2, [x29, 26]
	mov	w0, 4096
	bl	nftl_malloc
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1666
	adrp	x0, __func__.7788
	add	x1, x0, :lo12:__func__.7788
	adrp	x0, .LC386
	add	x0, x0, :lo12:.LC386
	bl	nand_print
.L1666:
	add	x0, x29, 32
	mov	x5, x0
	ldr	x4, [x29, 56]
	mov	w3, 8
	ldrh	w2, [x29, 26]
	ldrh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	bl	page_read
	str	w0, [x29, 52]
	ldrb	w0, [x29, 32]
	mov	w10, w0
	ldrb	w0, [x29, 33]
	mov	w11, w0
	ldrb	w0, [x29, 34]
	mov	w12, w0
	ldrb	w0, [x29, 35]
	mov	w13, w0
	ldrb	w0, [x29, 36]
	mov	w14, w0
	ldrb	w0, [x29, 37]
	mov	w15, w0
	ldrb	w0, [x29, 38]
	mov	w16, w0
	ldrb	w0, [x29, 39]
	mov	w1, w0
	ldrb	w0, [x29, 40]
	mov	w2, w0
	ldrb	w0, [x29, 41]
	mov	w3, w0
	ldrb	w0, [x29, 42]
	mov	w4, w0
	ldrb	w0, [x29, 43]
	mov	w5, w0
	ldrb	w0, [x29, 44]
	mov	w6, w0
	ldrb	w0, [x29, 45]
	mov	w7, w0
	ldrb	w0, [x29, 46]
	mov	w8, w0
	ldrb	w0, [x29, 47]
	mov	w9, w0
	adrp	x0, .LC387
	add	x0, x0, :lo12:.LC387
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	ldrb	w0, [x0]
	mov	w8, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w9, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w10, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w11, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 5
	ldrb	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 6
	ldrb	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 7
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 8
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 9
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 10
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC388
	add	x0, x0, :lo12:.LC388
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w4, w11
	mov	w3, w10
	mov	w2, w9
	mov	w1, w8
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	bl	nftl_free
	ldr	w0, [x29, 52]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_dbg_phy_read, .-nand_dbg_phy_read
	.align	2
	.global	nand_dbg_zone_phy_read
	.type	nand_dbg_zone_phy_read, %function
nand_dbg_zone_phy_read:
	sub	sp, sp, #176
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	strh	w2, [x29, 20]
	mov	w0, 4096
	bl	nftl_malloc
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	cmp	x0, 0
	bne	.L1669
	adrp	x0, __func__.7799
	add	x1, x0, :lo12:__func__.7799
	adrp	x0, .LC386
	add	x0, x0, :lo12:.LC386
	bl	nand_print
.L1669:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	add	x2, x29, 32
	add	x0, x29, 48
	mov	x5, x2
	ldr	x4, [x29, 88]
	mov	w3, 8
	ldrh	w2, [x29, 20]
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldrb	w0, [x29, 32]
	mov	w10, w0
	ldrb	w0, [x29, 33]
	mov	w11, w0
	ldrb	w0, [x29, 34]
	mov	w12, w0
	ldrb	w0, [x29, 35]
	mov	w13, w0
	ldrb	w0, [x29, 36]
	mov	w14, w0
	ldrb	w0, [x29, 37]
	mov	w15, w0
	ldrb	w0, [x29, 38]
	mov	w16, w0
	ldrb	w0, [x29, 39]
	mov	w1, w0
	ldrb	w0, [x29, 40]
	mov	w2, w0
	ldrb	w0, [x29, 41]
	mov	w3, w0
	ldrb	w0, [x29, 42]
	mov	w4, w0
	ldrb	w0, [x29, 43]
	mov	w5, w0
	ldrb	w0, [x29, 44]
	mov	w6, w0
	ldrb	w0, [x29, 45]
	mov	w7, w0
	ldrb	w0, [x29, 46]
	mov	w8, w0
	ldrb	w0, [x29, 47]
	mov	w9, w0
	adrp	x0, .LC387
	add	x0, x0, :lo12:.LC387
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print_dbg
	ldr	x0, [x29, 88]
	ldrb	w0, [x0]
	mov	w8, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w9, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w10, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w11, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 5
	ldrb	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 6
	ldrb	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 7
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 8
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 9
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 10
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC388
	add	x0, x0, :lo12:.LC388
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w4, w11
	mov	w3, w10
	mov	w2, w9
	mov	w1, w8
	bl	nand_print_dbg
	ldr	x0, [x29, 88]
	bl	nftl_free
	ldr	w0, [x29, 76]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_dbg_zone_phy_read, .-nand_dbg_zone_phy_read
	.section	.rodata
	.align	3
.LC389:
	.string	"[NE]%s:malloc 8192 fail!\n"
	.text
	.align	2
	.global	nand_dbg_zone_phy_write
	.type	nand_dbg_zone_phy_write, %function
nand_dbg_zone_phy_write:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	strh	w2, [x29, 20]
	mov	w0, 8192
	bl	nftl_malloc
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	cmp	x0, 0
	bne	.L1672
	adrp	x0, __func__.7810
	add	x1, x0, :lo12:__func__.7810
	adrp	x0, .LC389
	add	x0, x0, :lo12:.LC389
	bl	nand_print
.L1672:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 80]
	mov	w2, 4096
	mov	w1, -91
	ldr	x0, [x29, 88]
	bl	nand_memset
	mov	w0, -1
	strb	w0, [x29, 32]
	mov	w0, -1
	strb	w0, [x29, 33]
	mov	w0, -1
	strb	w0, [x29, 34]
	mov	w0, -1
	strb	w0, [x29, 35]
	mov	w0, -1
	strb	w0, [x29, 36]
	mov	w0, -91
	strb	w0, [x29, 37]
	mov	w0, -91
	strb	w0, [x29, 38]
	mov	w0, -1
	strb	w0, [x29, 39]
	mov	w0, -1
	strb	w0, [x29, 40]
	mov	w0, -1
	strb	w0, [x29, 41]
	mov	w0, -1
	strb	w0, [x29, 42]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x3, x29, 32
	add	x0, x29, 48
	mov	x5, x3
	ldr	x4, [x29, 88]
	mov	w3, w2
	ldrh	w2, [x29, 20]
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 88]
	bl	nftl_free
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_dbg_zone_phy_write, .-nand_dbg_zone_phy_write
	.align	2
	.global	nand_dbg_phy_write
	.type	nand_dbg_phy_write, %function
nand_dbg_phy_write:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	strh	w2, [x29, 26]
	mov	w0, 8192
	bl	nftl_malloc
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1675
	adrp	x0, __func__.7819
	add	x1, x0, :lo12:__func__.7819
	adrp	x0, .LC389
	add	x0, x0, :lo12:.LC389
	bl	nand_print
.L1675:
	mov	w2, 4096
	mov	w1, -91
	ldr	x0, [x29, 56]
	bl	nand_memset
	mov	w0, -1
	strb	w0, [x29, 32]
	mov	w0, -1
	strb	w0, [x29, 33]
	mov	w0, -1
	strb	w0, [x29, 34]
	mov	w0, -1
	strb	w0, [x29, 35]
	mov	w0, -1
	strb	w0, [x29, 36]
	mov	w0, -91
	strb	w0, [x29, 37]
	mov	w0, -91
	strb	w0, [x29, 38]
	mov	w0, -1
	strb	w0, [x29, 39]
	mov	w0, -1
	strb	w0, [x29, 40]
	mov	w0, -1
	strb	w0, [x29, 41]
	mov	w0, -1
	strb	w0, [x29, 42]
	add	x0, x29, 32
	mov	x5, x0
	ldr	x4, [x29, 56]
	mov	w3, 8
	ldrh	w2, [x29, 26]
	ldrh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	bl	page_write
	str	w0, [x29, 52]
	ldr	x0, [x29, 56]
	bl	nftl_free
	ldr	w0, [x29, 52]
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_dbg_phy_write, .-nand_dbg_phy_write
	.align	2
	.global	nand_dbg_zone_erase
	.type	nand_dbg_zone_erase, %function
nand_dbg_zone_erase:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	strh	w2, [x29, 20]
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 40]
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 40]
	ldrh	w2, [x29, 20]
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_dbg_zone_erase, .-nand_dbg_zone_erase
	.align	2
	.global	nand_dbg_phy_erase
	.type	nand_dbg_phy_erase, %function
nand_dbg_phy_erase:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	ldrh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	bl	block_erase
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_dbg_phy_erase, .-nand_dbg_phy_erase
	.align	2
	.global	nand_dbg_single_phy_erase
	.type	nand_dbg_single_phy_erase, %function
nand_dbg_single_phy_erase:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	ldrh	w1, [x29, 28]
	bl	nand_physic_erase_block
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_dbg_single_phy_erase, .-nand_dbg_single_phy_erase
	.align	2
	.global	clean_block_list
	.type	clean_block_list, %function
clean_block_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
.L1686:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1684
	b	.L1685
.L1684:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 40]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 56]
	b	.L1686
.L1685:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_invalid_page_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1691
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 40]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 56]
	b	.L1685
.L1691:
	nop
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	clean_block_list, .-clean_block_list
	.section	.rodata
	.align	3
.LC390:
	.string	"[ND]zoneNO:%d total_pages: %d\n"
	.align	3
.LC391:
	.string	"[ND]nand_memset bytes: %d\n"
	.text
	.align	2
	.global	nand_clean_zone_table2
	.type	nand_clean_zone_table2, %function
nand_clean_zone_table2:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 56]
	ldr	w0, [x29, 56]
	lsl	w0, w0, 2
	str	w0, [x29, 52]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC390
	add	x0, x0, :lo12:.LC390
	ldr	w2, [x29, 56]
	bl	nand_print_dbg
	ldr	w0, [x29, 52]
	lsr	w0, w0, 20
	str	w0, [x29, 48]
	str	wzr, [x29, 60]
	b	.L1693
.L1694:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	mov	w2, 1048576
	mov	w1, -1
	bl	nand_memset
	adrp	x0, .LC391
	add	x0, x0, :lo12:.LC391
	mov	w1, 1048576
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1693:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bcc	.L1694
	ldr	w0, [x29, 52]
	and	w0, w0, 1048575
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1695
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 44]
	mov	w1, -1
	bl	nand_memset
	adrp	x0, .LC391
	add	x0, x0, :lo12:.LC391
	ldr	w1, [x29, 44]
	bl	nand_print_dbg
.L1695:
	ldr	x0, [x29, 24]
	str	xzr, [x0, 528]
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 584]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 592]
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 648]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 304]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 312]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 288]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 296]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 320]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 328]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 368]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 376]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 352]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 360]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 384]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 392]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 432]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 440]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 416]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 424]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 448]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 456]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 28]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 30]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 696]
	str	wzr, [x29, 60]
	b	.L1696
.L1697:
	ldr	w0, [x29, 60]
	uxth	w2, w0
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 704
	mov	w1, w2
	strh	w1, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 706
	strh	wzr, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 712
	str	xzr, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 16
	str	xzr, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 24
	str	xzr, [x0]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1696:
	ldr	w0, [x29, 60]
	cmp	w0, 9
	bls	.L1697
	ldr	x0, [x29, 24]
	mov	w1, 255
	strh	w1, [x0, 1024]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 1026]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1032]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1040]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1048]
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_clean_zone_table2, .-nand_clean_zone_table2
	.align	2
	.global	nand_clean_zone_table
	.type	nand_clean_zone_table, %function
nand_clean_zone_table:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	mov	w1, 1000
	ldr	x0, [x29, 40]
	bl	__nand_flush_write_cache
	ldr	x0, [x29, 40]
	bl	clean_block_list
	ldr	x0, [x29, 40]
	bl	nand_clean_zone_table2
	ldr	x0, [x29, 40]
	bl	nftl_cache_exit
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_clean_zone_table, .-nand_clean_zone_table
	.section	.rodata
	.align	3
.LC392:
	.string	"[ND]build_zone_list start\n"
	.align	3
.LC393:
	.string	"[ND]nftl_cache_init start\n"
	.align	3
.LC394:
	.string	"[ND]=================================2=======================================\n"
	.align	3
.LC395:
	.string	"[ND]nand_find_zone_table end\n"
	.text
	.align	2
	.global	nand_find_zone_table
	.type	nand_find_zone_table, %function
nand_find_zone_table:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	clean_block_list
	ldr	x0, [x29, 40]
	bl	nand_clean_zone_table2
	adrp	x0, .LC392
	add	x0, x0, :lo12:.LC392
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	build_zone_list
	adrp	x0, .LC393
	add	x0, x0, :lo12:.LC393
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	nftl_cache_init
	adrp	x0, .LC394
	add	x0, x0, :lo12:.LC394
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	print_block_invalid_list
	ldr	x0, [x29, 40]
	bl	print_free_list
	adrp	x0, .LC394
	add	x0, x0, :lo12:.LC394
	bl	nand_print_dbg
	adrp	x0, .LC395
	add	x0, x0, :lo12:.LC395
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_find_zone_table, .-nand_find_zone_table
	.section	.rodata
	.align	3
.LC396:
	.string	"[NE][NE]%s:malloc 512 fail!\n"
	.text
	.align	2
	.global	write_data_to_nand
	.type	write_data_to_nand, %function
write_data_to_nand:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 512
	bl	nftl_malloc
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1704
	adrp	x0, __func__.7872
	add	x1, x0, :lo12:__func__.7872
	adrp	x0, .LC396
	add	x0, x0, :lo12:.LC396
	bl	nand_print
.L1704:
	mov	w2, 512
	mov	w1, -86
	ldr	x0, [x29, 32]
	bl	nand_memset
	str	wzr, [x29, 44]
	b	.L1705
.L1706:
	ldr	w0, [x29, 44]
	ldr	x3, [x29, 32]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	__nand_write
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1705:
	ldr	w1, [x29, 44]
	mov	w0, 38911
	cmp	w1, w0
	ble	.L1706
	ldr	x0, [x29, 32]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	write_data_to_nand, .-write_data_to_nand
	.section	.rodata
	.align	3
.LC397:
	.string	"[ND]==========================\n"
	.text
	.align	2
	.global	nand_check_table
	.type	nand_check_table, %function
nand_check_table:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	mov	w1, 1000
	ldr	x0, [x29, 40]
	bl	__nand_flush_write_cache
	adrp	x0, .LC397
	add	x0, x0, :lo12:.LC397
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	print_nftl_zone
	adrp	x0, .LC397
	add	x0, x0, :lo12:.LC397
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	nand_clean_zone_table
	adrp	x0, .LC397
	add	x0, x0, :lo12:.LC397
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	nand_find_zone_table
	ldr	x0, [x29, 40]
	bl	write_data_to_nand
	adrp	x0, .LC397
	add	x0, x0, :lo12:.LC397
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_check_table, .-nand_check_table
	.section	.rodata
	.align	3
.LC398:
	.string	"%.2x "
	.text
	.align	2
	.type	_DumpHexData, %function
_DumpHexData:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	str	wzr, [x29, 36]
	b	.L1711
.L1713:
	ldr	x0, [x29, 40]
	add	x1, x0, 1
	str	x1, [x29, 40]
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC398
	add	x0, x0, :lo12:.LC398
	bl	nand_print
	ldr	w0, [x29, 36]
	negs	w1, w0
	and	w0, w0, 15
	and	w1, w1, 15
	csneg	w0, w0, w1, mi
	cmp	w0, 15
	bne	.L1712
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print
.L1712:
	ldr	w0, [x29, 36]
	add	w0, w0, 1
	str	w0, [x29, 36]
.L1711:
	ldr	w1, [x29, 36]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	blt	.L1713
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	bl	nand_print
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	_DumpHexData, .-_DumpHexData
	.section	.rodata
	.align	3
.LC399:
	.string	"[NE]exception case bad block\n"
	.align	3
.LC400:
	.string	"[NE]exception case wbc\n"
	.align	3
.LC401:
	.string	"[NE]exception case gd\n"
	.align	3
.LC402:
	.string	"[NE]%s %d, flag = %d\n"
	.align	3
.LC403:
	.string	"[NE]OOB :"
	.text
	.align	2
	.global	nand_power_off_ecc_error
	.type	nand_power_off_ecc_error, %function
nand_power_off_ecc_error:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0]
	cmp	w0, 255
	beq	.L1716
	adrp	x0, .LC399
	add	x0, x0, :lo12:.LC399
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 60]
	b	.L1717
.L1716:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1718
	ldr	x2, [x29, 24]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	crc_wrong
	cmp	w0, 0
	beq	.L1717
	adrp	x0, .LC400
	add	x0, x0, :lo12:.LC400
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 60]
	b	.L1717
.L1718:
	ldr	x0, [x29, 24]
	add	x0, x0, 11
	ldrb	w0, [x0]
	cmp	w0, 165
	bne	.L1719
	ldr	x0, [x29, 24]
	add	x0, x0, 15
	ldrb	w0, [x0]
	cmp	w0, 165
	beq	.L1717
.L1719:
	adrp	x0, .LC401
	add	x0, x0, :lo12:.LC401
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 60]
	nop
.L1717:
	ldr	w0, [x29, 60]
	cmp	w0, 1
	bne	.L1720
	adrp	x0, __FUNCTION__.7896
	add	x1, x0, :lo12:__FUNCTION__.7896
	adrp	x0, .LC402
	add	x0, x0, :lo12:.LC402
	ldr	w3, [x29, 60]
	mov	w2, 3046
	bl	nand_print
	adrp	x0, .LC403
	add	x0, x0, :lo12:.LC403
	bl	nand_print
	mov	w1, 16
	ldr	x0, [x29, 24]
	bl	_DumpHexData
.L1720:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_power_off_ecc_error, .-nand_power_off_ecc_error
	.section	.rodata
	.align	3
.LC404:
	.string	"[NE]%s: malloc fail for zone\n"
	.align	3
.LC405:
	.string	"[NE]%s: malloc fail for nand_chip\n"
	.align	3
.LC406:
	.string	"[NE]%s: malloc fail for nand_block_info\n"
	.align	3
.LC407:
	.string	"[NE]not enough blocks %d!\n"
	.align	3
.LC408:
	.string	"[ND]nftl start:%d,%d\n"
	.align	3
.LC409:
	.string	"[NE]zone_param_init fail!\n"
	.align	3
.LC410:
	.string	"[NE]build_zone_list_first fail!\n"
	.align	3
.LC411:
	.string	"[NE]build_zone_list fail!\n"
	.align	3
.LC412:
	.string	"[NI]try again\n"
	.text
	.align	2
	.global	nftl_start
	.type	nftl_start, %function
nftl_start:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 88]
	str	x1, [x29, 80]
	str	x2, [x29, 72]
	str	x3, [x29, 64]
	str	x4, [x29, 56]
	str	w5, [x29, 52]
	str	w6, [x29, 48]
	str	w7, [x29, 44]
	str	wzr, [x29, 152]
	ldr	x0, [x29, 64]
	lsr	x0, x0, 9
	str	x0, [x29, 64]
	ldr	x0, [x29, 56]
	lsr	x0, x0, 9
	str	x0, [x29, 56]
	ldr	w0, [x29, 52]
	lsr	w0, w0, 9
	str	w0, [x29, 52]
	ldr	w0, [x29, 48]
	lsr	w0, w0, 9
	str	w0, [x29, 48]
	ldr	x0, [x29, 64]
	str	w0, [x29, 148]
	ldr	x0, [x29, 56]
	str	w0, [x29, 144]
	ldr	w0, [x29, 52]
	str	w0, [x29, 140]
	ldr	w0, [x29, 48]
	str	w0, [x29, 136]
	mov	w0, 3528
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 72]
	str	x1, [x0]
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	str	x0, [x29, 128]
	ldr	x0, [x29, 128]
	cmp	x0, 0
	bne	.L1723
	adrp	x0, __func__.7922
	add	x1, x0, :lo12:__func__.7922
	adrp	x0, .LC404
	add	x0, x0, :lo12:.LC404
	bl	nand_print
	mov	w0, -2
	b	.L1724
.L1723:
	ldr	w0, [x29, 152]
	add	w0, w0, 1
	str	w0, [x29, 152]
	mov	w2, 3528
	mov	w1, 0
	ldr	x0, [x29, 128]
	bl	nand_memset
	ldr	x0, [x29, 128]
	ldr	x1, [x29, 88]
	str	x1, [x0]
	ldr	x0, [x29, 128]
	ldr	x1, [x29, 80]
	str	x1, [x0, 3448]
	mov	w0, 40
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 128]
	str	x1, [x0, 8]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L1725
	adrp	x0, __func__.7922
	add	x1, x0, :lo12:__func__.7922
	adrp	x0, .LC405
	add	x0, x0, :lo12:.LC405
	bl	nand_print
	mov	w0, -2
	b	.L1724
.L1725:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	mov	w2, 8
	mov	w1, 0
	bl	nand_memset
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	mov	x3, x0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	add	x0, x0, 16
	mov	w2, 8
	mov	x1, x0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 136]
	uxth	w1, w1
	ubfiz	w1, w1, 9, 7
	uxth	w1, w1
	strh	w1, [x0, 26]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 44]
	uxtb	w1, w1
	strb	w1, [x0, 10]
	ldr	w1, [x29, 144]
	ldr	w0, [x29, 140]
	udiv	w0, w1, w0
	str	w0, [x29, 124]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 12]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bcc	.L1726
	mov	w0, -1
	b	.L1724
.L1726:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 124]
	uxth	w1, w1
	strh	w1, [x0, 24]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 140]
	ldr	w1, [x29, 136]
	udiv	w1, w2, w1
	uxth	w1, w1
	strh	w1, [x0, 20]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 136]
	uxtb	w1, w1
	strb	w1, [x0, 8]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x29, 128]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	uxth	w1, w1
	strh	w1, [x0, 28]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	mov	w1, 3000
	strh	w1, [x0, 16]
	ldr	w0, [x29, 124]
	str	w0, [x29, 120]
	ldr	x0, [x29, 128]
	ldr	x19, [x0, 8]
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	lsl	w0, w0, 6
	bl	nftl_malloc
	str	x0, [x19, 32]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	bne	.L1727
	adrp	x0, __func__.7922
	add	x1, x0, :lo12:__func__.7922
	adrp	x0, .LC406
	add	x0, x0, :lo12:.LC406
	bl	nand_print
	mov	w0, -2
	b	.L1724
.L1727:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x3, [x0, 32]
	ldr	w0, [x29, 120]
	lsl	w0, w0, 6
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	str	wzr, [x29, 156]
	b	.L1728
.L1729:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	ldr	w1, [x29, 156]
	uxth	w1, w1
	strh	w1, [x0]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 16]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 24]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 32]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 40]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 48]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 56]
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	str	w0, [x29, 156]
.L1728:
	ldr	w0, [x29, 120]
	add	w1, w0, 1
	ldr	w0, [x29, 156]
	cmp	w1, w0
	bhi	.L1729
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	bl	nftl_ops_init
	ldr	w1, [x29, 148]
	ldr	w0, [x29, 140]
	udiv	w0, w1, w0
	str	w0, [x29, 116]
	ldr	w0, [x29, 124]
	uxth	w1, w0
	ldr	w0, [x29, 116]
	uxth	w0, w0
	sub	w0, w1, w0
	uxth	w1, w0
	ldr	x0, [x29, 128]
	strh	w1, [x0, 30]
	ldr	x0, [x29, 128]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 12]
	cmp	w1, w0
	bcs	.L1730
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC407
	add	x0, x0, :lo12:.LC407
	bl	nand_print
	mov	w0, -2
	b	.L1724
.L1730:
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC408
	add	x0, x0, :lo12:.LC408
	mov	w2, w1
	ldr	w1, [x29, 124]
	bl	nand_print_dbg
	ldr	w0, [x29, 140]
	str	w0, [x29, 112]
	ldr	x0, [x29, 72]
	ldr	x4, [x0]
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	w0, [x29, 124]
	sub	w1, w0, w1
	ldr	w0, [x29, 112]
	mul	w1, w1, w0
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w2, w0
	ldr	w0, [x29, 112]
	mul	w0, w2, w0
	mov	w3, w0
	mov	w2, w1
	mov	w1, 0
	mov	x0, x4
	bl	zone_param_init
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L1731
	adrp	x0, .LC409
	add	x0, x0, :lo12:.LC409
	bl	nand_print
	ldr	w0, [x29, 108]
	b	.L1724
.L1731:
	ldr	w0, [x29, 168]
	uxtb	w1, w0
	ldr	x0, [x29, 128]
	strb	w1, [x0, 24]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 26]
	cmp	w0, 0
	beq	.L1732
	ldr	x0, [x29, 128]
	ldrb	w0, [x0, 25]
	orr	w0, w0, 1
	uxtb	w1, w0
	ldr	x0, [x29, 128]
	strb	w1, [x0, 25]
.L1732:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 10]
	cmp	w0, 0
	beq	.L1733
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	ldr	w1, [x29, 124]
	bl	is_no_use_device
	cmp	w0, 1
	bne	.L1733
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	ldr	w1, [x29, 124]
	bl	build_zone_list_first
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L1733
	adrp	x0, .LC410
	add	x0, x0, :lo12:.LC410
	bl	nand_print
	ldr	w0, [x29, 108]
	b	.L1724
.L1733:
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	bl	build_zone_list
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L1734
	adrp	x0, .LC411
	add	x0, x0, :lo12:.LC411
	bl	nand_print
	ldr	w0, [x29, 152]
	cmp	w0, 1
	bhi	.L1735
	adrp	x0, .LC412
	add	x0, x0, :lo12:.LC412
	bl	nand_print
	b	.L1723
.L1735:
	ldr	w0, [x29, 108]
	b	.L1724
.L1734:
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	bl	nftl_cache_init
	mov	w0, 0
.L1724:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 160
	ret
	.size	nftl_start, .-nftl_start
	.align	2
	.global	nftl_zone_release
	.type	nftl_zone_release, %function
nftl_zone_release:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	nftl_cache_exit
	ldr	x0, [x29, 24]
	bl	zone_param_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldr	x0, [x0, 32]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	bl	nftl_free
	ldr	x0, [x29, 24]
	bl	nftl_free
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_zone_release, .-nftl_zone_release
	.section	.rodata
	.align	3
	.type	nand_crc16_table, %object
	.size	nand_crc16_table, 512
nand_crc16_table:
	.hword	0
	.hword	-16191
	.hword	-15999
	.hword	320
	.hword	-15615
	.hword	960
	.hword	640
	.hword	-15807
	.hword	-14847
	.hword	1728
	.hword	1920
	.hword	-14527
	.hword	1280
	.hword	-14911
	.hword	-15231
	.hword	1088
	.hword	-13311
	.hword	3264
	.hword	3456
	.hword	-12991
	.hword	3840
	.hword	-12351
	.hword	-12671
	.hword	3648
	.hword	2560
	.hword	-13631
	.hword	-13439
	.hword	2880
	.hword	-14079
	.hword	2496
	.hword	2176
	.hword	-14271
	.hword	-10239
	.hword	6336
	.hword	6528
	.hword	-9919
	.hword	6912
	.hword	-9279
	.hword	-9599
	.hword	6720
	.hword	7680
	.hword	-8511
	.hword	-8319
	.hword	8000
	.hword	-8959
	.hword	7616
	.hword	7296
	.hword	-9151
	.hword	5120
	.hword	-11071
	.hword	-10879
	.hword	5440
	.hword	-10495
	.hword	6080
	.hword	5760
	.hword	-10687
	.hword	-11775
	.hword	4800
	.hword	4992
	.hword	-11455
	.hword	4352
	.hword	-11839
	.hword	-12159
	.hword	4160
	.hword	-4095
	.hword	12480
	.hword	12672
	.hword	-3775
	.hword	13056
	.hword	-3135
	.hword	-3455
	.hword	12864
	.hword	13824
	.hword	-2367
	.hword	-2175
	.hword	14144
	.hword	-2815
	.hword	13760
	.hword	13440
	.hword	-3007
	.hword	15360
	.hword	-831
	.hword	-639
	.hword	15680
	.hword	-255
	.hword	16320
	.hword	16000
	.hword	-447
	.hword	-1535
	.hword	15040
	.hword	15232
	.hword	-1215
	.hword	14592
	.hword	-1599
	.hword	-1919
	.hword	14400
	.hword	10240
	.hword	-5951
	.hword	-5759
	.hword	10560
	.hword	-5375
	.hword	11200
	.hword	10880
	.hword	-5567
	.hword	-4607
	.hword	11968
	.hword	12160
	.hword	-4287
	.hword	11520
	.hword	-4671
	.hword	-4991
	.hword	11328
	.hword	-7167
	.hword	9408
	.hword	9600
	.hword	-6847
	.hword	9984
	.hword	-6207
	.hword	-6527
	.hword	9792
	.hword	8704
	.hword	-7487
	.hword	-7295
	.hword	9024
	.hword	-7935
	.hword	8640
	.hword	8320
	.hword	-8127
	.hword	-24575
	.hword	24768
	.hword	24960
	.hword	-24255
	.hword	25344
	.hword	-23615
	.hword	-23935
	.hword	25152
	.hword	26112
	.hword	-22847
	.hword	-22655
	.hword	26432
	.hword	-23295
	.hword	26048
	.hword	25728
	.hword	-23487
	.hword	27648
	.hword	-21311
	.hword	-21119
	.hword	27968
	.hword	-20735
	.hword	28608
	.hword	28288
	.hword	-20927
	.hword	-22015
	.hword	27328
	.hword	27520
	.hword	-21695
	.hword	26880
	.hword	-22079
	.hword	-22399
	.hword	26688
	.hword	30720
	.hword	-18239
	.hword	-18047
	.hword	31040
	.hword	-17663
	.hword	31680
	.hword	31360
	.hword	-17855
	.hword	-16895
	.hword	32448
	.hword	32640
	.hword	-16575
	.hword	32000
	.hword	-16959
	.hword	-17279
	.hword	31808
	.hword	-19455
	.hword	29888
	.hword	30080
	.hword	-19135
	.hword	30464
	.hword	-18495
	.hword	-18815
	.hword	30272
	.hword	29184
	.hword	-19775
	.hword	-19583
	.hword	29504
	.hword	-20223
	.hword	29120
	.hword	28800
	.hword	-20415
	.hword	20480
	.hword	-28479
	.hword	-28287
	.hword	20800
	.hword	-27903
	.hword	21440
	.hword	21120
	.hword	-28095
	.hword	-27135
	.hword	22208
	.hword	22400
	.hword	-26815
	.hword	21760
	.hword	-27199
	.hword	-27519
	.hword	21568
	.hword	-25599
	.hword	23744
	.hword	23936
	.hword	-25279
	.hword	24320
	.hword	-24639
	.hword	-24959
	.hword	24128
	.hword	23040
	.hword	-25919
	.hword	-25727
	.hword	23360
	.hword	-26367
	.hword	22976
	.hword	22656
	.hword	-26559
	.hword	-30719
	.hword	18624
	.hword	18816
	.hword	-30399
	.hword	19200
	.hword	-29759
	.hword	-30079
	.hword	19008
	.hword	19968
	.hword	-28991
	.hword	-28799
	.hword	20288
	.hword	-29439
	.hword	19904
	.hword	19584
	.hword	-29631
	.hword	17408
	.hword	-31551
	.hword	-31359
	.hword	17728
	.hword	-30975
	.hword	18368
	.hword	18048
	.hword	-31167
	.hword	-32255
	.hword	17088
	.hword	17280
	.hword	-31935
	.hword	16640
	.hword	-32319
	.hword	-32639
	.hword	16448
	.text
	.align	2
	.type	nand_crc16_byte, %function
nand_crc16_byte:
	sub	sp, sp, #16
	strh	w0, [sp, 14]
	strb	w1, [sp, 13]
	ldrh	w0, [sp, 14]
	lsr	w0, w0, 8
	uxth	w1, w0
	ldrh	w2, [sp, 14]
	ldrb	w0, [sp, 13]
	eor	w0, w2, w0
	and	w2, w0, 255
	adrp	x0, nand_crc16_table
	add	x0, x0, :lo12:nand_crc16_table
	sxtw	x2, w2
	ldrh	w0, [x0, x2, lsl 1]
	eor	w0, w1, w0
	uxth	w0, w0
	add	sp, sp, 16
	ret
	.size	nand_crc16_byte, .-nand_crc16_byte
	.align	2
	.global	nand_crc16
	.type	nand_crc16, %function
nand_crc16:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	str	x1, [x29, 16]
	str	w2, [x29, 24]
	b	.L1740
.L1741:
	ldr	x0, [x29, 16]
	add	x1, x0, 1
	str	x1, [x29, 16]
	ldrb	w0, [x0]
	mov	w1, w0
	ldrh	w0, [x29, 30]
	bl	nand_crc16_byte
	strh	w0, [x29, 30]
.L1740:
	ldr	w0, [x29, 24]
	sub	w1, w0, #1
	str	w1, [x29, 24]
	cmp	w0, 0
	bne	.L1741
	ldrh	w0, [x29, 30]
	ldp	x29, x30, [sp], 32
	ret
	.size	nand_crc16, .-nand_crc16
	.section	.rodata
	.align	3
.LC413:
	.string	"[NE]error nand_write_logic_page_no_gc page_no:%u\n"
	.text
	.align	2
	.global	_nftl_nand_write_logic_page
	.type	_nftl_nand_write_logic_page, %function
_nftl_nand_write_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	ldr	x2, [x29, 24]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	nand_write_logic_page_no_gc
	cmp	w0, 0
	beq	.L1744
	adrp	x0, .LC413
	add	x0, x0, :lo12:.LC413
	ldr	w1, [x29, 36]
	bl	nand_print
	mov	w0, 1
	b	.L1745
.L1744:
	ldr	x0, [x29, 40]
	bl	garbage_collect
	ldr	x0, [x29, 40]
	bl	all_time_gc
	ldr	x0, [x29, 40]
	bl	do_prio_gc
	mov	w0, 0
.L1745:
	ldp	x29, x30, [sp], 48
	ret
	.size	_nftl_nand_write_logic_page, .-_nftl_nand_write_logic_page
	.section	.rodata
	.align	3
.LC414:
	.string	"[ND]logic_page_map_temp NULL!!\n"
	.align	3
.LC415:
	.string	"[NE]read a page: %d READ_RECLAIM!\n"
	.text
	.align	2
	.global	_nftl_nand_read_logic_page
	.type	_nftl_nand_read_logic_page, %function
_nftl_nand_read_logic_page:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1747
	adrp	x0, .LC414
	add	x0, x0, :lo12:.LC414
	bl	nand_print_dbg
	mov	w0, 1
	b	.L1751
.L1747:
	ldr	x0, [x29, 104]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	bne	.L1749
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	nand_memset
	mov	w0, 0
	b	.L1751
.L1749:
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 664]
	uxtb	w0, w0
	and	w0, w0, 1
	uxtb	w2, w0
	ldr	x1, [x29, 104]
	ldrb	w0, [x1, 1]
	bfi	w0, w2, 4, 1
	strb	w0, [x1, 1]
	ldr	x0, [x29, 104]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w6, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x3, x29, 72
	add	x0, x29, 48
	mov	x5, x3
	ldr	x4, [x29, 24]
	mov	w3, w2
	mov	w2, w6
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmn	w0, #2
	bne	.L1750
	ldr	x0, [x29, 104]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 88]
	mov	w2, 1
	ldr	x1, [x29, 88]
	ldr	x0, [x29, 40]
	bl	add_prio_gc
	adrp	x0, .LC415
	add	x0, x0, :lo12:.LC415
	ldr	w1, [x29, 36]
	bl	nand_print
.L1750:
	ldr	x0, [x29, 40]
	bl	do_prio_gc
	ldr	w0, [x29, 100]
.L1751:
	ldp	x29, x30, [sp], 112
	ret
	.size	_nftl_nand_read_logic_page, .-_nftl_nand_read_logic_page
	.align	2
	.global	_nftl_discard_logic_page
	.type	_nftl_discard_logic_page, %function
_nftl_discard_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	mov	x3, 0
	ldr	w2, [x29, 20]
	mov	w1, 4369
	movk	w1, 0xaa11, lsl 16
	ldr	x0, [x29, 24]
	bl	write_data_to_page
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	_nftl_discard_logic_page, .-_nftl_discard_logic_page
	.section	.rodata
	.align	3
.LC416:
	.string	"[NE]no free block\n"
	.align	3
.LC417:
	.string	"[NE]do_write_error erase error sb:%d!\n"
	.align	3
.LC418:
	.string	"[NE]do_write_error sb@%d nftl_nand_copy_page error!\n"
	.text
	.align	2
	.global	do_write_error
	.type	do_write_error, %function
do_write_error:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
.L1755:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	cmp	x0, 0
	bne	.L1756
	adrp	x0, .LC416
	add	x0, x0, :lo12:.LC416
	bl	nand_print
	mov	w0, 1
	b	.L1768
.L1756:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 40
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 80]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1758
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 80]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC417
	add	x0, x0, :lo12:.LC417
	bl	nand_print
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	lsr	w0, w0, 2
	uxth	w0, w0
	cmp	w1, w0
	bls	.L1759
	b	.L1755
.L1759:
	adrp	x0, .LC416
	add	x0, x0, :lo12:.LC416
	bl	nand_print
	mov	w0, 1
	b	.L1768
.L1758:
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	add	w1, w0, 1
	ldr	x0, [x29, 80]
	str	w1, [x0, 8]
	str	wzr, [x29, 92]
	b	.L1760
.L1764:
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 24]
	ldr	w0, [x29, 92]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 72]
	ldr	w0, [x29, 72]
	cmn	w0, #1
	beq	.L1769
	ldr	x0, [x29, 24]
	ldr	x5, [x0, 3480]
	ldr	x0, [x29, 16]
	ldr	x1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	ldr	w2, [x29, 92]
	uxth	w2, w2
	mov	w4, w2
	mov	x3, x0
	ldr	x2, [x29, 80]
	ldr	x0, [x29, 24]
	blr	x5
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1762
	ldr	x0, [x29, 80]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC418
	add	x0, x0, :lo12:.LC418
	bl	nand_print
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 40
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L1755
.L1762:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L1760:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bhi	.L1764
	b	.L1763
.L1769:
	nop
.L1763:
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 80]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 40
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	x1, [x0]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 24]
	bl	erase_block
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 80]
	strh	w1, [x0, 2]
	ldr	x0, [x29, 16]
	ldr	x1, [x29, 80]
	str	x1, [x0]
	str	wzr, [x29, 92]
	b	.L1765
.L1767:
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 24]
	ldr	w0, [x29, 92]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 72]
	ldr	w0, [x29, 72]
	cmn	w0, #1
	beq	.L1766
	ldr	w1, [x29, 72]
	ldr	x0, [x29, 24]
	bl	get_logic_page_map
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L1766
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 64]
	strh	w1, [x0, 2]
	ldr	w0, [x29, 92]
	uxth	w0, w0
	and	w0, w0, 4095
	uxth	w2, w0
	ldr	x1, [x29, 64]
	ldrh	w0, [x1]
	bfi	w0, w2, 0, 12
	strh	w0, [x1]
.L1766:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L1765:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bhi	.L1767
	mov	w0, 0
.L1768:
	ldp	x29, x30, [sp], 96
	ret
	.size	do_write_error, .-do_write_error
	.section	.rodata
	.align	3
.LC419:
	.string	"[NE]do after write error1!\n"
	.align	3
.LC420:
	.string	"[NE]do_before_write error!\n"
	.align	3
.LC421:
	.string	"[NE]write data to page error!\n"
	.align	3
.LC422:
	.string	"[NE]do after write error2!\n"
	.text
	.align	2
	.global	nand_write_data_page
	.type	nand_write_data_page, %function
nand_write_data_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	current_block_at_last_page
	cmp	w0, 1
	bne	.L1771
	ldr	x0, [x29, 40]
	bl	do_after_write
	cmp	w0, 0
	beq	.L1771
	adrp	x0, .LC419
	add	x0, x0, :lo12:.LC419
	bl	nand_print
	mov	w0, 1
	b	.L1772
.L1771:
	ldr	x0, [x29, 40]
	bl	do_before_write
	cmp	w0, 0
	beq	.L1773
	adrp	x0, .LC420
	add	x0, x0, :lo12:.LC420
	bl	nand_print
	mov	w0, 1
	b	.L1772
.L1773:
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	write_data_to_page
	cmp	w0, 0
	beq	.L1774
	adrp	x0, .LC421
	add	x0, x0, :lo12:.LC421
	bl	nand_print
	mov	w0, 1
	b	.L1772
.L1774:
	ldr	x0, [x29, 40]
	bl	do_after_write
	cmp	w0, 0
	beq	.L1775
	adrp	x0, .LC422
	add	x0, x0, :lo12:.LC422
	bl	nand_print
	mov	w0, 1
	b	.L1772
.L1775:
	mov	w0, 0
.L1772:
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_write_data_page, .-nand_write_data_page
	.align	2
	.global	nand_write_logic_page_no_gc
	.type	nand_write_logic_page_no_gc, %function
nand_write_logic_page_no_gc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 36]
	mov	w1, 192
	ldr	x0, [x29, 40]
	bl	nand_write_data_page
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_write_logic_page_no_gc, .-nand_write_logic_page_no_gc
	.section	.rodata
	.align	3
.LC423:
	.string	"[NE]get_new_current_block error3!\n"
	.align	3
.LC424:
	.string	"[NE]get_new_current_block error4!\n"
	.text
	.align	2
	.global	do_before_write
	.type	do_before_write, %function
do_before_write:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	b	.L1779
.L1784:
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 24]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1790
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1781
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
.L1781:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L1779
	adrp	x0, .LC423
	add	x0, x0, :lo12:.LC423
	bl	nand_print
	mov	w0, 1
	b	.L1782
.L1779:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_first_page
	cmp	w0, 1
	beq	.L1784
	b	.L1783
.L1790:
	nop
.L1783:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1785
	b	.L1786
.L1789:
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 592]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 24]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1791
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 592]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1788
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 592]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
.L1788:
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L1786
	adrp	x0, .LC424
	add	x0, x0, :lo12:.LC424
	bl	nand_print
	mov	w0, 1
	b	.L1782
.L1786:
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_first_page
	cmp	w0, 0
	bne	.L1789
	b	.L1785
.L1791:
	nop
.L1785:
	mov	w0, 0
.L1782:
	ldp	x29, x30, [sp], 32
	ret
	.size	do_before_write, .-do_before_write
	.section	.rodata
	.align	3
.LC425:
	.string	"[NE]write_phy_page_map_to_current_block error!\n"
	.align	3
.LC426:
	.string	"[NE]get_new_current_block error1!\n"
	.align	3
.LC427:
	.string	"[NE]get_new_current_block error2!\n"
	.text
	.align	2
	.global	do_after_write
	.type	do_after_write, %function
do_after_write:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_last_page
	cmp	w0, 1
	bne	.L1793
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L1794
	adrp	x0, .LC425
	add	x0, x0, :lo12:.LC425
	bl	nand_print
	mov	w0, 1
	b	.L1795
.L1794:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1796
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L1796
	adrp	x0, .LC425
	add	x0, x0, :lo12:.LC425
	bl	nand_print
.L1796:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L1797
	adrp	x0, .LC426
	add	x0, x0, :lo12:.LC426
	bl	nand_print
	mov	w0, 1
	b	.L1795
.L1797:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1798
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L1798
	adrp	x0, .LC427
	add	x0, x0, :lo12:.LC427
	bl	nand_print
	mov	w0, 1
	b	.L1795
.L1798:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 668]
	add	w1, w0, 1
	ldr	x0, [x29, 24]
	str	w1, [x0, 668]
.L1793:
	mov	w0, 0
.L1795:
	ldp	x29, x30, [sp], 32
	ret
	.size	do_after_write, .-do_after_write
	.section	.rodata
	.align	3
.LC428:
	.string	"[NE]write logic data to current block error\n"
	.text
	.align	2
	.global	write_data_to_page
	.type	write_data_to_page, %function
write_data_to_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	add	x0, x0, 528
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	write_logic_data_to_current_block
	cmp	w0, 0
	beq	.L1800
	adrp	x0, .LC428
	add	x0, x0, :lo12:.LC428
	bl	nand_print
	mov	w0, 1
	b	.L1801
.L1800:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1802
	ldr	x0, [x29, 40]
	add	x0, x0, 592
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	write_logic_data_to_current_block
	cmp	w0, 0
	beq	.L1802
	adrp	x0, .LC428
	add	x0, x0, :lo12:.LC428
	bl	nand_print
	mov	w0, 1
	b	.L1801
.L1802:
	mov	w0, 0
.L1801:
	ldp	x29, x30, [sp], 48
	ret
	.size	write_data_to_page, .-write_data_to_page
	.section	.rodata
	.align	3
.LC429:
	.string	"[ND]last block can not used to logic page %d!!\n"
	.align	3
.LC430:
	.string	"[ND]last block can not used to power down page %d!!\n"
	.align	3
.LC431:
	.string	"[ND]write_phy_data_to_current_block write error %d!\n"
	.align	3
.LC432:
	.string	"[NE]do_write_error error\n"
	.text
	.align	2
	.global	write_phy_data_to_current_block
	.type	write_phy_data_to_current_block, %function
write_phy_data_to_current_block:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	w4, [x29, 16]
	strh	wzr, [x29, 104]
	strh	wzr, [x29, 106]
	add	x0, x29, 80
	mov	w2, 16
	mov	w1, -91
	bl	nand_memset
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 106]
	ldr	w1, [x29, 16]
	mov	w0, -1431633921
	cmp	w1, w0
	bne	.L1804
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 560]
	strh	w0, [x29, 106]
.L1804:
	ldrh	w0, [x29, 106]
	mov	w2, w0
	ldr	x1, [x29, 24]
	mov	w0, -1
	bl	nand_crc16
	strh	w0, [x29, 104]
	ldrh	w0, [x29, 104]
	lsr	w0, w0, 8
	uxth	w0, w0
	uxtb	w0, w0
	strb	w0, [x29, 92]
	ldrh	w0, [x29, 104]
	uxtb	w0, w0
	strb	w0, [x29, 93]
	str	wzr, [x29, 108]
.L1805:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 32]
	ldrh	w2, [x0, 56]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x4, x29, 80
	add	x0, x29, 56
	mov	x5, x4
	ldr	x4, [x29, 24]
	bl	set_physic_op_par
	ldr	w1, [x29, 16]
	mov	w0, -1431633921
	cmp	w1, w0
	bne	.L1806
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 6]
	add	x0, x29, 80
	mov	w4, w1
	mov	w3, w2
	mov	w2, -1431633921
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
	b	.L1807
.L1806:
	ldr	w0, [x29, 16]
	cmp	w0, 192
	bne	.L1808
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L1809
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC429
	add	x0, x0, :lo12:.LC429
	bl	nand_print_dbg
.L1809:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 6]
	add	x0, x29, 80
	mov	w4, w1
	mov	w3, w2
	ldr	w2, [x29, 20]
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_oob_logic_page
	b	.L1807
.L1808:
	ldr	w1, [x29, 16]
	mov	w0, 13107
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L1807
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L1810
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC430
	add	x0, x0, :lo12:.LC430
	bl	nand_print_dbg
.L1810:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 6]
	add	x0, x29, 80
	mov	w4, w1
	mov	w3, w2
	mov	w2, 13107
	movk	w2, 0xaa11, lsl 16
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
.L1807:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1811
	ldr	w0, [x29, 108]
	cmp	w0, 1
	bgt	.L1811
	ldrh	w0, [x29, 58]
	mov	w1, w0
	adrp	x0, .LC431
	add	x0, x0, :lo12:.LC431
	bl	nand_print_dbg
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	do_write_error
	cmp	w0, 0
	bne	.L1812
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
	b	.L1805
.L1812:
	adrp	x0, .LC432
	add	x0, x0, :lo12:.LC432
	bl	nand_print
	mov	w0, 1
	b	.L1814
.L1811:
	mov	w0, 0
.L1814:
	ldp	x29, x30, [sp], 112
	ret
	.size	write_phy_data_to_current_block, .-write_phy_data_to_current_block
	.section	.rodata
	.align	3
.LC433:
	.string	"[NE]write_phy_data_to_current_block error1\n"
	.text
	.align	2
	.global	write_phy_page_map_to_current_block
	.type	write_phy_page_map_to_current_block, %function
write_phy_page_map_to_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	add	x1, x0, 592
	ldr	x0, [x29, 16]
	cmp	x1, x0
	bne	.L1816
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 632]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 568]
	mov	w2, 320
	mov	x1, x0
	mov	x0, x3
	bl	nand_memcpy
.L1816:
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	mov	w4, -1431633921
	mov	w3, 0
	mov	x2, x0
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	write_phy_data_to_current_block
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1817
	adrp	x0, .LC433
	add	x0, x0, :lo12:.LC433
	bl	nand_print
	ldr	w0, [x29, 44]
	b	.L1818
.L1817:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 56]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0, 4]
	add	w1, w1, 1
	uxth	w1, w1
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	add	x1, x0, 8
	ldr	x0, [x29, 16]
	ldr	x2, [x0]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	mov	w3, w0
	ldr	x0, [x29, 24]
	bl	check_invalid_page_right
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 56]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bne	.L1819
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_invalid_page_list
	ldr	x0, [x29, 24]
	bl	adjust_invaild_list
.L1819:
	mov	w0, 0
.L1818:
	ldp	x29, x30, [sp], 48
	ret
	.size	write_phy_page_map_to_current_block, .-write_phy_page_map_to_current_block
	.section	.rodata
	.align	3
.LC434:
	.string	"[ND]logic_page_map NULL!!\n"
	.align	3
.LC435:
	.string	"[ND]invalid_page_count more than pages_per_blk at write logic data to current block:%d!!\n"
	.align	3
.LC436:
	.string	"[NE]write_phy_data_to_current_block error2\n"
	.align	3
.LC437:
	.string	"[NE]invalid_page_count more than pages_per_blk at write logic data to current block:%d!!\n"
	.align	3
.LC438:
	.string	"[NE]write phy data to current block error2\n"
	.text
	.align	2
	.global	write_logic_data_to_current_block
	.type	write_logic_data_to_current_block, %function
write_logic_data_to_current_block:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	str	x4, [x29, 16]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 28]
	cmp	w0, 192
	bne	.L1821
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1822
	adrp	x0, .LC434
	add	x0, x0, :lo12:.LC434
	bl	nand_print_dbg
	mov	w0, 1
	b	.L1823
.L1822:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L1824
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 64]
	ldr	x0, [x29, 32]
	ldr	x1, [x0]
	ldr	x0, [x29, 64]
	cmp	x1, x0
	bne	.L1825
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1824
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	adrp	x0, .LC435
	add	x0, x0, :lo12:.LC435
	bl	nand_print_dbg
	b	.L1824
.L1825:
	mov	w0, 1
	str	w0, [x29, 76]
.L1824:
	mov	w4, 192
	ldr	w3, [x29, 24]
	ldr	x2, [x29, 16]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	write_phy_data_to_current_block
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L1827
	adrp	x0, .LC436
	add	x0, x0, :lo12:.LC436
	bl	nand_print
	ldr	w0, [x29, 52]
	b	.L1823
.L1827:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 56]
	strh	w1, [x0, 2]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	and	w0, w0, 4095
	uxth	w2, w0
	ldr	x1, [x29, 56]
	ldrh	w0, [x1]
	bfi	w0, w2, 0, 12
	strh	w0, [x1]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 664]
	cmp	w0, 0
	cset	w0, eq
	uxtb	w2, w0
	ldr	x1, [x29, 56]
	ldrb	w0, [x1, 1]
	bfi	w0, w2, 4, 1
	strb	w0, [x1, 1]
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 24]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	uxth	x0, w0
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w1, [x29, 24]
	str	w1, [x0]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 56]
	b	.L1828
.L1821:
	ldr	w1, [x29, 28]
	mov	w0, 4369
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L1829
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1830
	adrp	x0, .LC434
	add	x0, x0, :lo12:.LC434
	bl	nand_print_dbg
	mov	w0, 1
	b	.L1823
.L1830:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L1831
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 64]
	ldr	x0, [x29, 32]
	ldr	x1, [x0]
	ldr	x0, [x29, 64]
	cmp	x1, x0
	bne	.L1832
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 24]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	uxth	x0, w0
	lsl	x0, x0, 2
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1831
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	adrp	x0, .LC437
	add	x0, x0, :lo12:.LC437
	bl	nand_print
	b	.L1831
.L1832:
	mov	w0, 1
	str	w0, [x29, 76]
.L1831:
	ldr	x0, [x29, 56]
	mov	w1, -1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 56]
	ldrh	w1, [x0]
	orr	w1, w1, 4095
	strh	w1, [x0]
	ldr	x0, [x29, 56]
	ldrb	w1, [x0, 1]
	orr	w1, w1, 16
	strb	w1, [x0, 1]
	b	.L1828
.L1829:
	ldr	w1, [x29, 28]
	mov	w0, 13107
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L1828
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 4]
	add	w1, w1, 1
	uxth	w1, w1
	strh	w1, [x0, 4]
	mov	w4, 13107
	movk	w4, 0xaa11, lsl 16
	ldr	w3, [x29, 24]
	ldr	x2, [x29, 16]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	write_phy_data_to_current_block
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L1834
	adrp	x0, .LC438
	add	x0, x0, :lo12:.LC438
	bl	nand_print
	ldr	w0, [x29, 52]
	b	.L1823
.L1834:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 56]
.L1828:
	ldr	w0, [x29, 76]
	cmp	w0, 1
	bne	.L1835
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 40]
	bl	phy_block_from_invalid_page_incr
.L1835:
	ldr	x0, [x29, 40]
	bl	adjust_invaild_list
	mov	w0, 0
.L1823:
	ldp	x29, x30, [sp], 80
	ret
	.size	write_logic_data_to_current_block, .-write_logic_data_to_current_block
	.section	.rodata
	.align	3
.LC439:
	.string	"[ND]swl: new_cur_blk->alloc ok(max_ec): block_no = %d; ec = %d; \n"
	.align	3
.LC440:
	.string	"[NE]no free block!!!!\n"
	.text
	.align	2
	.global	get_new_current_block
	.type	get_new_current_block, %function
get_new_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	str	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x3, [x0, 24]
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 32]
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 1062]
	cmp	w0, 0
	beq	.L1837
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_erase_time
	mov	x1, x0
	ldr	x0, [x29, 16]
	str	x1, [x0]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	cmp	x0, 0
	beq	.L1839
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC439
	add	x0, x0, :lo12:.LC439
	bl	nand_print_dbg
	b	.L1839
.L1837:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 16]
	str	x1, [x0]
.L1839:
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L1840
	adrp	x0, .LC440
	add	x0, x0, :lo12:.LC440
	bl	nand_print
	mov	w0, 1
	b	.L1841
.L1840:
	ldr	x0, [x29, 16]
	strh	wzr, [x0, 56]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w1, [x29, 44]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1842
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w1, [x0, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
.L1842:
	mov	w0, 0
.L1841:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_new_current_block, .-get_new_current_block
	.section	.rodata
	.align	3
.LC441:
	.string	"[NE]new_block_init_for_write erase error!\n"
	.text
	.align	2
	.global	new_block_init_for_write
	.type	new_block_init_for_write, %function
new_block_init_for_write:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 676]
	cmp	w1, w0
	bls	.L1844
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 6]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	cmp	w1, w0
	bcs	.L1844
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 40]
	str	w1, [x0, 676]
.L1844:
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	str	w1, [x0, 8]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1845
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	adrp	x0, .LC441
	add	x0, x0, :lo12:.LC441
	bl	nand_print
	mov	w0, 1
	b	.L1847
.L1845:
	mov	w0, 0
.L1847:
	ldp	x29, x30, [sp], 80
	ret
	.size	new_block_init_for_write, .-new_block_init_for_write
	.section	.rodata
	.align	3
.LC442:
	.string	"[NE]invaild_page num error:[%d] [%d]  used_page:[%d]!!\n"
	.text
	.align	2
	.global	check_invalid_page_right
	.type	check_invalid_page_right, %function
check_invalid_page_right:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	strh	w3, [x29, 22]
	str	wzr, [x29, 72]
	str	wzr, [x29, 76]
	b	.L1849
.L1855:
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	w0, [x29, 76]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 68]
	ldr	w0, [x29, 68]
	cmn	w0, #1
	beq	.L1850
	ldr	w1, [x29, 68]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1851
	adrp	x0, .LC414
	add	x0, x0, :lo12:.LC414
	bl	nand_print_dbg
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
	b	.L1852
.L1851:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	cmp	w1, w0
	bne	.L1853
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w1, w0
	ldr	w0, [x29, 76]
	cmp	w1, w0
	beq	.L1852
.L1853:
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
	b	.L1852
.L1850:
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L1852:
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
.L1849:
	ldrh	w1, [x29, 22]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	bhi	.L1855
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 72]
	cmp	w1, w0
	beq	.L1856
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	ldrh	w1, [x29, 22]
	adrp	x0, .LC442
	add	x0, x0, :lo12:.LC442
	mov	w3, w1
	ldr	w1, [x29, 72]
	bl	nand_print
	mov	w0, 1
	b	.L1857
.L1856:
	mov	w0, 0
.L1857:
	ldp	x29, x30, [sp], 80
	ret
	.size	check_invalid_page_right, .-check_invalid_page_right
	.align	2
	.global	nftl_ops_init
	.type	nftl_ops_init, %function
nftl_ops_init:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_erase_superblk
	add	x1, x1, :lo12:_nftl_nand_erase_superblk
	str	x1, [x0, 3456]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_read_page
	add	x1, x1, :lo12:_nftl_nand_read_page
	str	x1, [x0, 3464]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_write_page
	add	x1, x1, :lo12:_nftl_nand_write_page
	str	x1, [x0, 3472]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_is_blk_good
	add	x1, x1, :lo12:_nftl_nand_is_blk_good
	str	x1, [x0, 3488]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_mark_bad_blk
	add	x1, x1, :lo12:_nftl_nand_mark_bad_blk
	str	x1, [x0, 3496]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_copy_page
	add	x1, x1, :lo12:_nftl_nand_copy_page
	str	x1, [x0, 3480]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_read_logic_page
	add	x1, x1, :lo12:_nftl_nand_read_logic_page
	str	x1, [x0, 3512]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_write_logic_page
	add	x1, x1, :lo12:_nftl_nand_write_logic_page
	str	x1, [x0, 3504]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_discard_logic_page
	add	x1, x1, :lo12:_nftl_discard_logic_page
	str	x1, [x0, 3520]
	nop
	add	sp, sp, 16
	ret
	.size	nftl_ops_init, .-nftl_ops_init
	.align	2
	.global	current_block_at_last_page
	.type	current_block_at_last_page, %function
current_block_at_last_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L1861
	mov	w0, 1
	b	.L1862
.L1861:
	mov	w0, 0
.L1862:
	add	sp, sp, 16
	ret
	.size	current_block_at_last_page, .-current_block_at_last_page
	.align	2
	.global	current_block_at_first_page
	.type	current_block_at_first_page, %function
current_block_at_first_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldrh	w0, [x0, 56]
	cmp	w0, 0
	bne	.L1864
	mov	w0, 1
	b	.L1865
.L1864:
	mov	w0, 0
.L1865:
	add	sp, sp, 16
	ret
	.size	current_block_at_first_page, .-current_block_at_first_page
	.align	2
	.global	fill_no_use_data_to_page
	.type	fill_no_use_data_to_page, %function
fill_no_use_data_to_page:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_last_page
	cmp	w0, 0
	beq	.L1867
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L1868
	adrp	x0, .LC425
	add	x0, x0, :lo12:.LC425
	bl	nand_print
	mov	w0, 1
	b	.L1869
.L1868:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1870
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L1870
	adrp	x0, .LC425
	add	x0, x0, :lo12:.LC425
	bl	nand_print
.L1870:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L1871
	adrp	x0, .LC426
	add	x0, x0, :lo12:.LC426
	bl	nand_print
	mov	w0, 1
	b	.L1869
.L1871:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1872
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L1872
	adrp	x0, .LC427
	add	x0, x0, :lo12:.LC427
	bl	nand_print
	mov	w0, 1
	b	.L1869
.L1872:
	mov	w0, 0
	b	.L1869
.L1867:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_first_page
	cmp	w0, 0
	beq	.L1873
	mov	w0, 0
	b	.L1869
.L1873:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L1874
	adrp	x0, .LC425
	add	x0, x0, :lo12:.LC425
	bl	nand_print
.L1874:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1875
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L1875
	adrp	x0, .LC425
	add	x0, x0, :lo12:.LC425
	bl	nand_print
.L1875:
	mov	w0, 8
.L1869:
	ldp	x29, x30, [sp], 32
	ret
	.size	fill_no_use_data_to_page, .-fill_no_use_data_to_page
	.align	2
	.global	fill_no_use_data_to_current_block
	.type	fill_no_use_data_to_current_block, %function
fill_no_use_data_to_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	b	.L1877
.L1880:
	ldr	x0, [x29, 24]
	bl	fill_no_use_data_to_page
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L1878
	mov	w0, 0
	b	.L1879
.L1878:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1877:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L1880
	mov	w0, 1
.L1879:
	ldp	x29, x30, [sp], 48
	ret
	.size	fill_no_use_data_to_current_block, .-fill_no_use_data_to_current_block
	.align	2
	.global	fill_no_use_page_to_current_block
	.type	fill_no_use_page_to_current_block, %function
fill_no_use_page_to_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 44]
	b	.L1882
.L1885:
	ldr	x0, [x29, 24]
	bl	fill_no_use_data_to_page
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L1883
	mov	w0, 0
	b	.L1884
.L1883:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1882:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	blt	.L1885
	mov	w0, 1
.L1884:
	ldp	x29, x30, [sp], 48
	ret
	.size	fill_no_use_page_to_current_block, .-fill_no_use_page_to_current_block
	.align	2
	.global	nand_before_shutdown
	.type	nand_before_shutdown, %function
nand_before_shutdown:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w1, 8
	ldr	x0, [x29, 24]
	bl	fill_no_use_page_to_current_block
	ldp	x29, x30, [sp], 32
	ret
	.size	nand_before_shutdown, .-nand_before_shutdown
	.section	.rodata
	.align	3
.LC443:
	.string	"[NE]%s:malloc fail for cache_buf!\n"
	.text
	.align	2
	.global	nftl_sector_cache_init
	.type	nftl_sector_cache_init, %function
nftl_sector_cache_init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 2880
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	mov	w1, 16
	strh	w1, [x0]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 8]
	mov	w0, 8192
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 8]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L1889
	adrp	x0, __func__.8106
	add	x1, x0, :lo12:__func__.8106
	adrp	x0, .LC443
	add	x0, x0, :lo12:.LC443
	bl	nand_print
	mov	w0, 1
	b	.L1890
.L1889:
	ldr	x0, [x29, 40]
	str	xzr, [x0, 544]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 552]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	str	wzr, [x29, 60]
	b	.L1891
.L1892:
	ldr	w0, [x29, 60]
	uxth	w2, w0
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 16
	mov	w1, w2
	strh	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 18
	mov	w1, 255
	strh	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 20
	mov	w1, -1
	str	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 1
	lsl	x0, x0, 5
	add	x0, x1, x0
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 1
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 8
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 24
	ldr	x1, [x29, 48]
	str	x1, [x0]
	ldr	x0, [x29, 48]
	add	x0, x0, 512
	str	x0, [x29, 48]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1891:
	ldr	w0, [x29, 60]
	cmp	w0, 15
	bls	.L1892
	mov	w0, 0
.L1890:
	ldp	x29, x30, [sp], 64
	ret
	.size	nftl_sector_cache_init, .-nftl_sector_cache_init
	.align	2
	.global	nftl_sector_cache_exit
	.type	nftl_sector_cache_exit, %function
nftl_sector_cache_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 2880
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_sector_cache_exit, .-nftl_sector_cache_exit
	.align	2
	.global	search_from_sector_cache_write_list
	.type	search_from_sector_cache_write_list, %function
search_from_sector_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	ldr	x0, [sp, 8]
	add	x0, x0, 528
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1896
.L1899:
	ldr	x0, [sp, 24]
	ldr	w1, [x0, 4]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	bne	.L1897
	ldr	x0, [sp, 24]
	b	.L1898
.L1897:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1896:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1899
	mov	x0, 0
.L1898:
	add	sp, sp, 32
	ret
	.size	search_from_sector_cache_write_list, .-search_from_sector_cache_write_list
	.align	2
	.global	get_empty_sector_cahce_node
	.type	get_empty_sector_cahce_node, %function
get_empty_sector_cahce_node:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strh	wzr, [sp, 30]
	b	.L1901
.L1904:
	ldrh	w0, [sp, 30]
	ldr	x1, [sp, 8]
	sxtw	x0, w0
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 18
	ldrh	w0, [x0]
	cmp	w0, 255
	bne	.L1902
	ldrh	w0, [sp, 30]
	sxtw	x0, w0
	lsl	x0, x0, 5
	add	x0, x0, 16
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	b	.L1903
.L1902:
	ldrh	w0, [sp, 30]
	add	w0, w0, 1
	strh	w0, [sp, 30]
.L1901:
	ldrh	w0, [sp, 30]
	cmp	w0, 15
	bls	.L1904
	mov	x0, 0
.L1903:
	add	sp, sp, 32
	ret
	.size	get_empty_sector_cahce_node, .-get_empty_sector_cahce_node
	.align	2
	.global	add_to_sector_cache_write_list_tail
	.type	add_to_sector_cache_write_list_tail, %function
add_to_sector_cache_write_list_tail:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 528
	str	x0, [sp, 24]
	b	.L1906
.L1907:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1906:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1907
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	ldr	x0, [sp]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 2]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 2]
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	add_to_sector_cache_write_list_tail, .-add_to_sector_cache_write_list_tail
	.align	2
	.global	del_from_sector_cache_write_list
	.type	del_from_sector_cache_write_list, %function
del_from_sector_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 16]
	ldr	x0, [sp, 24]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1910
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
.L1910:
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 24]
	ldr	x0, [sp]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 2]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 2]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_sector_cache_write_list, .-del_from_sector_cache_write_list
	.align	2
	.global	__nand_sector_read
	.type	__nand_sector_read, %function
__nand_sector_read:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 36]
	str	w0, [x29, 68]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	str	wzr, [x29, 72]
	b	.L1913
.L1914:
	ldr	x2, [x29, 56]
	ldr	w1, [x29, 68]
	ldr	x0, [x29, 40]
	bl	nand_sector_cache_read
	mov	w1, w0
	ldr	w0, [x29, 76]
	orr	w0, w0, w1
	str	w0, [x29, 76]
	ldr	w0, [x29, 68]
	add	w0, w0, 1
	str	w0, [x29, 68]
	ldr	x0, [x29, 56]
	add	x0, x0, 512
	str	x0, [x29, 56]
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L1913:
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 32]
	cmp	w1, w0
	bcc	.L1914
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	__nand_sector_read, .-__nand_sector_read
	.align	2
	.global	__nand_sector_write
	.type	__nand_sector_write, %function
__nand_sector_write:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 36]
	str	w0, [x29, 68]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	str	wzr, [x29, 72]
	b	.L1917
.L1918:
	ldr	x2, [x29, 56]
	ldr	w1, [x29, 68]
	ldr	x0, [x29, 40]
	bl	nand_sector_cache_write
	mov	w1, w0
	ldr	w0, [x29, 76]
	orr	w0, w0, w1
	str	w0, [x29, 76]
	ldr	w0, [x29, 68]
	add	w0, w0, 1
	str	w0, [x29, 68]
	ldr	x0, [x29, 56]
	add	x0, x0, 512
	str	x0, [x29, 56]
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L1917:
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 32]
	cmp	w1, w0
	bcc	.L1918
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	__nand_sector_write, .-__nand_sector_write
	.section	.rodata
	.align	3
.LC444:
	.string	"[NE]nand_sector_cache_read 1!\n"
	.text
	.align	2
	.global	nand_sector_cache_read
	.type	nand_sector_cache_read, %function
nand_sector_cache_read:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	mov	w0, 512
	str	w0, [x29, 76]
	ldr	x0, [x29, 40]
	add	x0, x0, 2880
	str	x0, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 64]
	bl	search_from_sector_cache_write_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L1921
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 76]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w0, 0
	b	.L1922
.L1921:
	ldr	x3, [x29, 24]
	mov	w2, 1
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	__nand_read
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L1923
	adrp	x0, .LC444
	add	x0, x0, :lo12:.LC444
	bl	nand_print
	mov	w0, 1
	b	.L1922
.L1923:
	ldr	w0, [x29, 52]
.L1922:
	ldp	x29, x30, [sp], 80
	ret
	.size	nand_sector_cache_read, .-nand_sector_cache_read
	.section	.rodata
	.align	3
.LC445:
	.string	"[NE]nand_sector_cache_write 1!\n"
	.align	3
.LC446:
	.string	"[NE]nand_sector_cache_write 2!\n"
	.text
	.align	2
	.global	nand_sector_cache_write
	.type	nand_sector_cache_write, %function
nand_sector_cache_write:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	str	wzr, [x29, 68]
	strh	wzr, [x29, 66]
	ldr	x0, [x29, 40]
	add	x0, x0, 2880
	str	x0, [x29, 56]
	mov	w0, 512
	strh	w0, [x29, 66]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 56]
	bl	search_from_sector_cache_write_list
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	beq	.L1925
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 56]
	bl	del_from_sector_cache_write_list
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w1, [x29, 66]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 56]
	bl	add_to_sector_cache_write_list_tail
	mov	w0, 0
	b	.L1926
.L1925:
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	cmp	w0, 7
	bls	.L1927
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 544]
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	del_from_sector_cache_write_list
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	mov	x3, x0
	mov	w2, 1
	ldr	x0, [x29, 40]
	bl	__nand_write
	str	w0, [x29, 68]
	ldr	w0, [x29, 68]
	cmp	w0, 0
	beq	.L1928
	adrp	x0, .LC445
	add	x0, x0, :lo12:.LC445
	bl	nand_print
	mov	w0, 1
	b	.L1926
.L1927:
	ldr	x0, [x29, 56]
	bl	get_empty_sector_cahce_node
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	bne	.L1928
	adrp	x0, .LC446
	add	x0, x0, :lo12:.LC446
	bl	nand_print
	mov	w0, 1
	b	.L1926
.L1928:
	ldr	x0, [x29, 72]
	ldr	w1, [x29, 36]
	str	w1, [x0, 4]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w1, [x29, 66]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 56]
	bl	add_to_sector_cache_write_list_tail
	mov	w0, 0
.L1926:
	ldp	x29, x30, [sp], 80
	ret
	.size	nand_sector_cache_write, .-nand_sector_cache_write
	.section	.rodata
	.align	3
.LC447:
	.string	"[NE]nand_flush_write_cache write error\n"
	.text
	.align	2
	.global	__nand_flush_sector_write_cache
	.type	__nand_flush_sector_write_cache, %function
__nand_flush_sector_write_cache:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	add	x0, x0, 2880
	str	x0, [x29, 48]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 48]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	beq	.L1930
	mov	w0, 1
	str	w0, [x29, 44]
	b	.L1932
.L1930:
	str	wzr, [x29, 44]
	b	.L1932
.L1934:
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 544]
	mov	x1, x0
	ldr	x0, [x29, 48]
	bl	del_from_sector_cache_write_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	mov	x3, x0
	mov	w2, 1
	ldr	x0, [x29, 24]
	bl	__nand_write
	str	w0, [x29, 60]
	ldr	w0, [x29, 20]
	sub	w0, w0, #1
	str	w0, [x29, 20]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L1938
.L1932:
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 544]
	cmp	x0, 0
	bne	.L1934
	b	.L1933
.L1938:
	nop
.L1933:
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L1937
	adrp	x0, .LC447
	add	x0, x0, :lo12:.LC447
	bl	nand_print
.L1937:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	__nand_flush_sector_write_cache, .-__nand_flush_sector_write_cache
	.section	.rodata
	.align	3
.LC448:
	.string	"[NE]%s:malloc fail for cache_node!\n"
	.align	3
.LC449:
	.string	"[NE]%s:malloc fail for cache_page_buf!\n"
	.text
	.align	2
	.global	nftl_cache_init
	.type	nftl_cache_init, %function
nftl_cache_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w1, [x0, 22]
	ldr	x0, [x29, 32]
	strh	w1, [x0]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1712]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1720]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1728]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1736]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1768]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1776]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1784]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1792]
	str	wzr, [x29, 44]
	b	.L1940
.L1943:
	ldr	w0, [x29, 44]
	uxth	w3, w0
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 8
	mov	w1, w3
	strh	w1, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 10
	mov	w1, 255
	strh	w1, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 16
	mov	w1, -1
	str	w1, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 32
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 40
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 48
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 56
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 12
	strh	wzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 14
	strh	wzr, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x3, x0
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 24
	str	x3, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 24
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L1941
	adrp	x0, __func__.8204
	add	x1, x0, :lo12:__func__.8204
	adrp	x0, .LC448
	add	x0, x0, :lo12:.LC448
	bl	nand_print
	mov	w0, 1
	b	.L1942
.L1941:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1940:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L1943
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 1800]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 1800]
	cmp	x0, 0
	bne	.L1944
	adrp	x0, __func__.8204
	add	x1, x0, :lo12:__func__.8204
	adrp	x0, .LC449
	add	x0, x0, :lo12:.LC449
	bl	nand_print
	mov	w0, 1
	b	.L1942
.L1944:
	mov	w0, 0
.L1942:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_cache_init, .-nftl_cache_init
	.align	2
	.global	nftl_cache_exit
	.type	nftl_cache_exit, %function
nftl_cache_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 32]
	str	wzr, [x29, 44]
	b	.L1946
.L1947:
	ldr	x2, [x29, 32]
	ldrsw	x1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 24
	ldr	x0, [x0]
	bl	nftl_free
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1946:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L1947
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 1800]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_cache_exit, .-nftl_cache_exit
	.align	2
	.global	nftl_cache_clean
	.type	nftl_cache_clean, %function
nftl_cache_clean:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1072
	str	x0, [sp, 16]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 3448]
	ldrh	w1, [x0, 22]
	ldr	x0, [sp, 16]
	strh	w1, [x0]
	ldr	x0, [sp, 16]
	strh	wzr, [x0, 2]
	ldr	x0, [sp, 16]
	strh	wzr, [x0, 4]
	ldr	x0, [sp, 16]
	strh	wzr, [x0, 6]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1712]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1720]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1728]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1736]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1768]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1776]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1784]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1792]
	str	wzr, [sp, 28]
	b	.L1950
.L1951:
	ldr	w0, [sp, 28]
	uxth	w3, w0
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 8
	mov	w1, w3
	strh	w1, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 10
	mov	w1, 255
	strh	w1, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 16
	mov	w1, -1
	str	w1, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 32
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 40
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 48
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 56
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 12
	strh	wzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 14
	strh	wzr, [x0]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1950:
	ldr	x0, [sp, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [sp, 28]
	cmp	w1, w0
	bhi	.L1951
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	nftl_cache_clean, .-nftl_cache_clean
	.align	2
	.global	search_from_cache_read_list
	.type	search_from_cache_read_list, %function
search_from_cache_read_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	ldr	x0, [sp, 8]
	add	x0, x0, 1688
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
	b	.L1954
.L1957:
	ldr	x0, [sp, 24]
	ldr	w1, [x0, 8]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	bne	.L1955
	ldr	x0, [sp, 24]
	b	.L1956
.L1955:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
.L1954:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1957
	mov	x0, 0
.L1956:
	add	sp, sp, 32
	ret
	.size	search_from_cache_read_list, .-search_from_cache_read_list
	.align	2
	.global	search_from_cache_write_list
	.type	search_from_cache_write_list, %function
search_from_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	ldr	x0, [sp, 8]
	add	x0, x0, 1744
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	str	x0, [sp, 24]
	b	.L1959
.L1962:
	ldr	x0, [sp, 24]
	ldr	w1, [x0, 8]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	bne	.L1960
	ldr	x0, [sp, 24]
	b	.L1961
.L1960:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	str	x0, [sp, 24]
.L1959:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1962
	mov	x0, 0
.L1961:
	add	sp, sp, 32
	ret
	.size	search_from_cache_write_list, .-search_from_cache_write_list
	.align	2
	.global	add_to_cache_read_list_tail
	.type	add_to_cache_read_list_tail, %function
add_to_cache_read_list_tail:
	sub	sp, sp, #48
	str	x0, [sp, 24]
	str	x1, [sp, 16]
	str	w2, [sp, 12]
	ldr	x0, [sp, 24]
	add	x0, x0, 1688
	str	x0, [sp, 40]
	b	.L1964
.L1965:
	ldr	x0, [sp, 40]
	ldr	x0, [x0, 24]
	str	x0, [sp, 40]
.L1964:
	ldr	x0, [sp, 40]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	bne	.L1965
	ldr	x0, [sp, 40]
	ldr	x1, [sp, 16]
	str	x1, [x0, 24]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 24]
	ldr	x0, [sp, 16]
	ldr	x1, [sp, 40]
	str	x1, [x0, 32]
	ldr	w0, [sp, 12]
	uxth	w1, w0
	ldr	x0, [sp, 16]
	strh	w1, [x0, 2]
	ldr	w0, [sp, 12]
	cmp	w0, 0
	bne	.L1966
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 2]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 24]
	strh	w1, [x0, 2]
	b	.L1967
.L1966:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 24]
	strh	w1, [x0, 6]
.L1967:
	mov	w0, 0
	add	sp, sp, 48
	ret
	.size	add_to_cache_read_list_tail, .-add_to_cache_read_list_tail
	.align	2
	.global	add_to_cache_write_list_tail
	.type	add_to_cache_write_list_tail, %function
add_to_cache_write_list_tail:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 1744
	str	x0, [sp, 24]
	b	.L1970
.L1971:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	str	x0, [sp, 24]
.L1970:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	cmp	x0, 0
	bne	.L1971
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 40]
	ldr	x0, [sp]
	str	xzr, [x0, 40]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 48]
	ldr	x0, [sp]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 4]
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	add_to_cache_write_list_tail, .-add_to_cache_write_list_tail
	.align	2
	.global	del_from_cache_read_list
	.type	del_from_cache_read_list, %function
del_from_cache_read_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 24]
	ldr	x0, [sp, 24]
	str	x1, [x0, 24]
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	beq	.L1974
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	ldr	x1, [sp, 24]
	str	x1, [x0, 32]
.L1974:
	ldr	x0, [sp]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L1975
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 2]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 2]
	b	.L1976
.L1975:
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 6]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 6]
.L1976:
	ldr	x0, [sp]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [sp]
	str	xzr, [x0, 24]
	ldr	x0, [sp]
	str	xzr, [x0, 32]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_cache_read_list, .-del_from_cache_read_list
	.align	2
	.global	del_from_cache_write_list
	.type	del_from_cache_write_list, %function
del_from_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 40]
	ldr	x0, [sp, 24]
	str	x1, [x0, 40]
	ldr	x0, [sp]
	ldr	x0, [x0, 40]
	cmp	x0, 0
	beq	.L1979
	ldr	x0, [sp]
	ldr	x0, [x0, 40]
	ldr	x1, [sp, 24]
	str	x1, [x0, 48]
.L1979:
	ldr	x0, [sp]
	str	xzr, [x0, 40]
	ldr	x0, [sp]
	str	xzr, [x0, 48]
	ldr	x0, [sp]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 4]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 4]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_cache_write_list, .-del_from_cache_write_list
	.align	2
	.global	get_empty_cahce_node
	.type	get_empty_cahce_node, %function
get_empty_cahce_node:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	xzr, [x29, 32]
	strh	wzr, [x29, 46]
	b	.L1982
.L1985:
	ldrh	w0, [x29, 46]
	ldr	x2, [x29, 24]
	sxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 10
	ldrh	w0, [x0]
	cmp	w0, 255
	bne	.L1983
	ldrh	w0, [x29, 46]
	sxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	add	x0, x0, 8
	b	.L1984
.L1983:
	ldrh	w0, [x29, 46]
	add	w0, w0, 1
	strh	w0, [x29, 46]
.L1982:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	ldrh	w1, [x29, 46]
	cmp	w1, w0
	bcc	.L1985
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	cmp	w0, 9
	bls	.L1986
	ldr	x0, [x29, 24]
	add	x0, x0, 1688
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
	b	.L1987
.L1989:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 2]
	cmp	w0, 2
	bne	.L1988
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 24]
	bl	del_from_cache_read_list
	b	.L1984
.L1988:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
.L1987:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1989
	b	.L1990
.L1986:
	ldr	x0, [x29, 24]
	add	x0, x0, 1688
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
	b	.L1991
.L1993:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L1992
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 24]
	bl	del_from_cache_read_list
	b	.L1984
.L1992:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
.L1991:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1993
.L1990:
	mov	x0, 0
.L1984:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_empty_cahce_node, .-get_empty_cahce_node
	.section	.rodata
	.align	3
.LC450:
	.string	"[ND]read sector:0x%x,len:0x%x\n"
	.text
	.align	2
	.global	__nand_read
	.type	__nand_read, %function
__nand_read:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 656]
	cmp	w0, 3
	bne	.L1995
	adrp	x0, .LC450
	add	x0, x0, :lo12:.LC450
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	nand_print_dbg
.L1995:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 64]
	ldr	w1, [x29, 32]
	add	x1, x2, x1
	str	x1, [x0, 64]
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	ldr	x1, [x29, 40]
	mov	w0, 0
	bl	nand_op
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	__nand_read, .-__nand_read
	.section	.rodata
	.align	3
.LC451:
	.string	"[ND]write sector:0x%x,len:0x%x\n"
	.text
	.align	2
	.global	__nand_write
	.type	__nand_write, %function
__nand_write:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 656]
	cmp	w0, 3
	bne	.L1998
	adrp	x0, .LC451
	add	x0, x0, :lo12:.LC451
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	nand_print_dbg
.L1998:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 72]
	ldr	w1, [x29, 32]
	add	x1, x2, x1
	str	x1, [x0, 72]
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	ldr	x1, [x29, 40]
	mov	w0, 1
	bl	nand_op
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	__nand_write, .-__nand_write
	.section	.rodata
	.align	3
.LC452:
	.string	"[ND]dicard sector:0x%x,len:0x%x\n"
	.text
	.align	2
	.global	__nand_discard
	.type	__nand_discard, %function
__nand_discard:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 3
	bne	.L2001
	adrp	x0, .LC452
	add	x0, x0, :lo12:.LC452
	ldr	w2, [x29, 16]
	ldr	w1, [x29, 20]
	bl	nand_print_dbg
.L2001:
	ldr	w2, [x29, 16]
	ldr	w1, [x29, 20]
	ldr	x0, [x29, 24]
	bl	nand_discard
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	__nand_discard, .-__nand_discard
	.section	.rodata
	.align	3
.LC453:
	.string	"[NE]paramter error. beyond reality capacity\n"
	.text
	.align	2
	.global	nand_op
	.type	nand_op, %function
nand_op:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	strb	w0, [x29, 47]
	str	x1, [x29, 32]
	str	w2, [x29, 40]
	str	w3, [x29, 28]
	str	x4, [x29, 16]
	str	wzr, [x29, 72]
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L2004
	mov	w0, 0
	b	.L2005
.L2004:
	ldr	w1, [x29, 40]
	ldr	w0, [x29, 28]
	add	w1, w1, w0
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bls	.L2006
	adrp	x0, .LC453
	add	x0, x0, :lo12:.LC453
	bl	nand_print
	mov	w0, 1
	b	.L2005
.L2006:
	ldrb	w0, [x29, 47]
	cmp	w0, 0
	bne	.L2007
	adrp	x0, nand_cache_read
	add	x0, x0, :lo12:nand_cache_read
	str	x0, [x29, 64]
	b	.L2008
.L2007:
	adrp	x0, nand_cache_write
	add	x0, x0, :lo12:nand_cache_write
	str	x0, [x29, 64]
.L2008:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 40]
	udiv	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	str	w0, [x29, 76]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 40]
	udiv	w2, w0, w1
	mul	w1, w2, w1
	sub	w0, w0, w1
	strh	w0, [x29, 58]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldrh	w0, [x29, 58]
	sub	w0, w1, w0
	strh	w0, [x29, 94]
	ldrh	w1, [x29, 94]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L2009
	ldr	w0, [x29, 28]
	strh	w0, [x29, 94]
	str	wzr, [x29, 28]
	b	.L2010
.L2009:
	ldrh	w0, [x29, 94]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
.L2010:
	ldr	x0, [x29, 16]
	str	x0, [x29, 80]
	ldrh	w0, [x29, 94]
	strh	w0, [x29, 92]
	ldrh	w0, [x29, 58]
	lsl	w0, w0, 8
	sxth	w1, w0
	ldrsh	w0, [x29, 94]
	orr	w0, w1, w0
	sxth	w0, w0
	strh	w0, [x29, 56]
	ldr	x4, [x29, 64]
	ldr	x3, [x29, 80]
	ldrh	w2, [x29, 56]
	ldr	w1, [x29, 76]
	ldr	x0, [x29, 32]
	blr	x4
	str	w0, [x29, 72]
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L2012
	ldr	w0, [x29, 72]
	b	.L2005
.L2013:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	strh	w0, [x29, 94]
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
	ldrh	w0, [x29, 92]
	lsl	w0, w0, 9
	sxtw	x0, w0
	ldr	x1, [x29, 80]
	add	x0, x1, x0
	str	x0, [x29, 80]
	ldrh	w0, [x29, 94]
	strh	w0, [x29, 56]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 28]
	sub	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	strh	w0, [x29, 92]
	ldr	x4, [x29, 64]
	ldr	x3, [x29, 80]
	ldrh	w2, [x29, 56]
	ldr	w1, [x29, 76]
	ldr	x0, [x29, 32]
	blr	x4
	mov	w1, w0
	ldr	w0, [x29, 72]
	orr	w0, w0, w1
	str	w0, [x29, 72]
.L2012:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L2013
	ldr	w0, [x29, 28]
	strh	w0, [x29, 94]
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
	ldrh	w0, [x29, 92]
	lsl	w0, w0, 9
	sxtw	x0, w0
	ldr	x1, [x29, 80]
	add	x0, x1, x0
	str	x0, [x29, 80]
	ldrh	w0, [x29, 94]
	strh	w0, [x29, 56]
	ldr	x4, [x29, 64]
	ldr	x3, [x29, 80]
	ldrh	w2, [x29, 56]
	ldr	w1, [x29, 76]
	ldr	x0, [x29, 32]
	blr	x4
	mov	w1, w0
	ldr	w0, [x29, 72]
	orr	w0, w0, w1
	str	w0, [x29, 72]
	ldr	w0, [x29, 72]
.L2005:
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_op, .-nand_op
	.section	.rodata
	.align	3
.LC454:
	.string	"[NI]nand_cache_read error buf is  NULL!\n"
	.align	3
.LC455:
	.string	"[NE]error1 node %d,%d,%d!\n"
	.text
	.align	2
	.global	nand_cache_read
	.type	nand_cache_read, %function
nand_cache_read:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	strh	w2, [x29, 34]
	str	x3, [x29, 24]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L2015
	adrp	x0, .LC454
	add	x0, x0, :lo12:.LC454
	bl	nand_print
	mov	w0, 0
	b	.L2016
.L2015:
	str	wzr, [x29, 92]
	ldrh	w0, [x29, 34]
	lsr	w0, w0, 8
	uxth	w0, w0
	str	w0, [x29, 88]
	ldrh	w0, [x29, 34]
	and	w0, w0, 255
	str	w0, [x29, 84]
	ldr	w0, [x29, 88]
	lsl	w0, w0, 9
	str	w0, [x29, 88]
	ldr	w0, [x29, 84]
	lsl	w0, w0, 9
	str	w0, [x29, 84]
	ldr	x0, [x29, 40]
	add	x0, x0, 1072
	str	x0, [x29, 72]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	cmp	w0, 0
	beq	.L2017
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 24]
	str	x0, [x29, 64]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 92]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L2018
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 116]
	add	w1, w1, 1
	str	w1, [x0, 116]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	wzr, [x29, 92]
.L2018:
	ldr	w0, [x29, 92]
	b	.L2016
.L2017:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 72]
	bl	search_from_cache_write_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L2019
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 4]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 54]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 6]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 52]
	ldrh	w1, [x29, 54]
	ldrh	w0, [x29, 52]
	add	w0, w1, w0
	mov	w1, w0
	ldr	w0, [x29, 88]
	cmp	w1, w0
	bcc	.L2020
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 84]
	add	w0, w1, w0
	ldrh	w2, [x29, 54]
	ldrh	w1, [x29, 52]
	add	w1, w2, w1
	cmp	w0, w1
	bhi	.L2020
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w0, 0
	b	.L2016
.L2020:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 1800]
	mov	x2, x0
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 92]
	ldrh	w0, [x29, 52]
	ldr	x1, [x29, 64]
	add	x3, x1, x0
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 1800]
	ldrh	w0, [x29, 52]
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	ldrh	w0, [x29, 52]
	sub	w0, w2, w0
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 56]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w0, 0
	b	.L2016
.L2019:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 72]
	bl	search_from_cache_read_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L2021
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L2022
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 72]
	bl	del_from_cache_read_list
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w2, 0
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 72]
	bl	add_to_cache_read_list_tail
	b	.L2023
.L2022:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
.L2023:
	mov	w0, 0
	b	.L2016
.L2021:
	ldr	x0, [x29, 72]
	bl	get_empty_cahce_node
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L2024
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 6]
	mov	w3, w0
	adrp	x0, .LC455
	add	x0, x0, :lo12:.LC455
	bl	nand_print
	mov	w0, 1
	b	.L2016
.L2024:
	ldr	x0, [x29, 56]
	ldr	w1, [x29, 36]
	str	w1, [x0, 8]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 92]
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 56]
	strh	w1, [x0, 6]
	mov	w2, 0
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 72]
	bl	add_to_cache_read_list_tail
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L2025
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 116]
	add	w1, w1, 1
	str	w1, [x0, 116]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	wzr, [x29, 92]
.L2025:
	ldr	w0, [x29, 92]
.L2016:
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_cache_read, .-nand_cache_read
	.section	.rodata
	.align	3
.LC456:
	.string	"[NE]error1 bitmap %x!\n"
	.align	3
.LC457:
	.string	"[NE]ECC_LIMIT happened 12! page:%d!\n"
	.align	3
.LC458:
	.string	"[NE]error1 flush write cache to nand\n"
	.align	3
.LC459:
	.string	"[NE]error2 flush write cache to nand\n"
	.align	3
.LC460:
	.string	"[NI]ECC_LIMIT happened 11! page:%d!\n"
	.text
	.align	2
	.global	nand_cache_write
	.type	nand_cache_write, %function
nand_cache_write:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	strh	w2, [x29, 34]
	str	x3, [x29, 24]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L2027
	mov	w0, 0
	b	.L2028
.L2027:
	ldrh	w0, [x29, 34]
	lsr	w0, w0, 8
	strh	w0, [x29, 94]
	ldrh	w0, [x29, 34]
	and	w0, w0, 255
	strh	w0, [x29, 92]
	ldrh	w0, [x29, 94]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 94]
	ldrh	w0, [x29, 92]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 92]
	str	wzr, [x29, 88]
	ldr	x0, [x29, 40]
	add	x0, x0, 1072
	str	x0, [x29, 80]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	cmp	w0, 0
	beq	.L2029
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 24]
	str	x0, [x29, 72]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 72]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 72]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	b	.L2028
.L2029:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L2030
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	ldrh	w1, [x29, 34]
	cmp	w1, w0
	bne	.L2030
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 26]
	cmp	w0, 0
	bne	.L2030
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_write_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L2031
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_write_list
.L2031:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_read_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L2032
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_read_list
.L2032:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 24]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	cmp	w0, 0
	beq	.L2033
	ldrh	w1, [x29, 34]
	adrp	x0, .LC456
	add	x0, x0, :lo12:.LC456
	bl	nand_print
	mov	w0, 1
	b	.L2028
.L2033:
	mov	w0, 0
	b	.L2028
.L2030:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_write_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L2034
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_write_list
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 72]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 6]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 62]
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 6]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	cmp	w1, w0
	beq	.L2035
	ldrh	w1, [x29, 94]
	ldrh	w0, [x29, 62]
	cmp	w1, w0
	bne	.L2036
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 6]
	ldrh	w0, [x29, 92]
	lsr	w0, w0, 9
	uxth	w0, w0
	add	w0, w1, w0
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	b	.L2035
.L2036:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1800]
	mov	x2, x0
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldrh	w0, [x29, 62]
	ldr	x1, [x29, 72]
	add	x3, x1, x0
	ldr	x0, [x29, 80]
	ldr	x1, [x0, 1800]
	ldrh	w0, [x29, 62]
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	ldrh	w0, [x29, 62]
	sub	w0, w2, w0
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 88]
	cmp	w0, 10
	bne	.L2035
	adrp	x0, .LC457
	add	x0, x0, :lo12:.LC457
	ldr	w1, [x29, 36]
	bl	nand_print
.L2035:
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	add_to_cache_write_list_tail
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_read_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L2037
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_read_list
.L2037:
	mov	w0, 0
	b	.L2028
.L2034:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_read_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L2040
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 72]
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x0, [x29, 64]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_read_list
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	add_to_cache_write_list_tail
	ldr	x0, [x29, 80]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcc	.L2039
	mov	w1, 0
	ldr	x0, [x29, 40]
	bl	flush_write_cache_to_nand
	cmp	w0, 0
	beq	.L2039
	adrp	x0, .LC458
	add	x0, x0, :lo12:.LC458
	bl	nand_print
	mov	w0, 1
	b	.L2028
.L2039:
	mov	w0, 0
	b	.L2028
.L2041:
	mov	w1, 0
	ldr	x0, [x29, 40]
	bl	flush_write_cache_to_nand
	cmp	w0, 0
	beq	.L2040
	adrp	x0, .LC459
	add	x0, x0, :lo12:.LC459
	bl	nand_print
	mov	w0, 1
	b	.L2028
.L2040:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcs	.L2041
	ldr	x0, [x29, 80]
	bl	get_empty_cahce_node
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L2042
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	mov	w3, w0
	adrp	x0, .LC455
	add	x0, x0, :lo12:.LC455
	bl	nand_print
	mov	w0, 1
	b	.L2028
.L2042:
	ldr	x0, [x29, 64]
	ldr	w1, [x29, 36]
	str	w1, [x0, 8]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 72]
	ldrh	w0, [x29, 94]
	cmp	w0, 0
	bne	.L2043
	ldr	x0, [x29, 64]
	strh	wzr, [x0, 4]
	ldrh	w0, [x29, 92]
	lsr	w0, w0, 9
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	b	.L2044
.L2043:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 72]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	cmp	w0, 10
	bne	.L2045
	adrp	x0, .LC460
	add	x0, x0, :lo12:.LC460
	ldr	w1, [x29, 36]
	bl	nand_print
.L2045:
	ldr	x0, [x29, 64]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
.L2044:
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	add_to_cache_write_list_tail
	mov	w0, 0
.L2028:
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_cache_write, .-nand_cache_write
	.section	.rodata
	.align	3
.LC461:
	.string	"[NE]flush write cache to nand no cache!\n"
	.align	3
.LC462:
	.string	"[NE]ECC_LIMIT happened 10! page:%d cache: %d!\n"
	.align	3
.LC463:
	.string	"[NE]flush write cache to nand error!\n"
	.text
	.align	2
	.global	flush_write_cache_to_nand
	.type	flush_write_cache_to_nand, %function
flush_write_cache_to_nand:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	xzr, [x29, 96]
	mov	w0, 1
	strb	w0, [x29, 111]
	mov	w0, 44
	strb	w0, [x29, 48]
	mov	w0, 100
	strb	w0, [x29, 49]
	mov	w0, 68
	strb	w0, [x29, 50]
	mov	w0, 50
	strb	w0, [x29, 51]
	mov	w0, -91
	strb	w0, [x29, 52]
	mov	w0, -1
	strb	w0, [x29, 53]
	mov	w0, -1
	strb	w0, [x29, 54]
	mov	w0, -1
	strb	w0, [x29, 55]
	mov	w0, 44
	strb	w0, [x29, 40]
	mov	w0, -124
	strb	w0, [x29, 41]
	mov	w0, 68
	strb	w0, [x29, 42]
	mov	w0, 50
	strb	w0, [x29, 43]
	mov	w0, -86
	strb	w0, [x29, 44]
	mov	w0, 4
	strb	w0, [x29, 45]
	strb	wzr, [x29, 46]
	strb	wzr, [x29, 47]
	str	wzr, [x29, 92]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1784]
	cmp	x0, 0
	bne	.L2047
	adrp	x0, .LC461
	add	x0, x0, :lo12:.LC461
	bl	nand_print
	mov	w0, 0
	b	.L2054
.L2047:
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1784]
	mov	x1, x0
	ldr	x0, [x29, 80]
	bl	del_from_cache_write_list
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	bne	.L2049
	mov	w0, 0
	b	.L2054
.L2049:
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	x0, [x29, 72]
	ldrh	w1, [x0, 6]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	cmp	w1, w0
	beq	.L2050
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 3512]
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1800]
	mov	x2, x0
	ldr	x0, [x29, 24]
	blr	x3
	str	w0, [x29, 92]
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 6]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 62]
	ldrh	w0, [x29, 62]
	ldr	x1, [x29, 64]
	add	x3, x1, x0
	ldr	x0, [x29, 80]
	ldr	x1, [x0, 1800]
	ldrh	w0, [x29, 62]
	add	x1, x1, x0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	ldrh	w0, [x29, 62]
	sub	w0, w2, w0
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L2050
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC462
	add	x0, x0, :lo12:.LC462
	bl	nand_print
.L2050:
	ldr	x0, [x29, 72]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 72]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 3504]
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 8]
	ldr	x2, [x29, 64]
	mov	w1, w0
	ldr	x0, [x29, 24]
	blr	x3
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L2051
	adrp	x0, .LC463
	add	x0, x0, :lo12:.LC463
	bl	nand_print
.L2051:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 96]
	add	x0, x29, 48
	mov	x2, 8
	mov	x1, x0
	ldr	x0, [x29, 96]
	bl	nand_memcmp
	strb	w0, [x29, 111]
	ldrb	w0, [x29, 111]
	cmp	w0, 0
	beq	.L2052
	add	x0, x29, 40
	mov	x2, 8
	mov	x1, x0
	ldr	x0, [x29, 96]
	bl	nand_memcmp
	strb	w0, [x29, 111]
.L2052:
	ldrb	w0, [x29, 111]
	cmp	w0, 0
	bne	.L2053
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 584]
	cmp	w0, 0
	beq	.L2053
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 584]
	sub	w0, w0, #1
	bl	nand_phy_get_page_type
	cmp	w0, 2
	bne	.L2053
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 3504]
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 8]
	ldr	x2, [x29, 64]
	mov	w1, w0
	ldr	x0, [x29, 24]
	blr	x3
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L2053
	adrp	x0, .LC463
	add	x0, x0, :lo12:.LC463
	bl	nand_print
.L2053:
	mov	w2, 2
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 80]
	bl	add_to_cache_read_list_tail
	mov	w0, 0
.L2054:
	ldp	x29, x30, [sp], 112
	ret
	.size	flush_write_cache_to_nand, .-flush_write_cache_to_nand
	.section	.rodata
	.align	3
.LC464:
	.string	"[ND]flush cache %d %d!\n"
	.text
	.align	2
	.global	__nand_flush_write_cache
	.type	__nand_flush_write_cache, %function
__nand_flush_write_cache:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 32]
	str	wzr, [x29, 44]
	b	.L2056
.L2058:
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	flush_write_cache_to_nand
	mov	w1, w0
	ldr	w0, [x29, 44]
	orr	w0, w0, w1
	str	w0, [x29, 44]
	ldr	w0, [x29, 20]
	sub	w0, w0, #1
	str	w0, [x29, 20]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L2062
.L2056:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 1784]
	cmp	x0, 0
	bne	.L2058
	b	.L2057
.L2062:
	nop
.L2057:
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L2059
	adrp	x0, .LC447
	add	x0, x0, :lo12:.LC447
	bl	nand_print
.L2059:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L2060
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L2060
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC464
	add	x0, x0, :lo12:.LC464
	ldr	w2, [x29, 40]
	bl	nand_print_dbg
.L2060:
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	__nand_flush_write_cache, .-__nand_flush_write_cache
	.section	.rodata
	.align	3
.LC465:
	.string	"[NE]nand_discard paramter error!\n"
	.text
	.align	2
	.global	nand_discard
	.type	nand_discard, %function
nand_discard:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	str	wzr, [x29, 56]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 48]
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 16]
	add	w1, w1, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bls	.L2064
	adrp	x0, .LC465
	add	x0, x0, :lo12:.LC465
	bl	nand_print
	mov	w0, 1
	b	.L2065
.L2064:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 96]
	ldr	w1, [x29, 16]
	add	x1, x2, x1
	str	x1, [x0, 96]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 20]
	udiv	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 20]
	udiv	w2, w0, w1
	mul	w1, w2, w1
	sub	w0, w0, w1
	strh	w0, [x29, 46]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldrh	w0, [x29, 46]
	sub	w0, w1, w0
	strh	w0, [x29, 44]
	ldrh	w0, [x29, 46]
	cmp	w0, 0
	beq	.L2066
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
	ldrh	w1, [x29, 44]
	ldr	w0, [x29, 16]
	cmp	w1, w0
	bls	.L2067
	str	wzr, [x29, 16]
	b	.L2069
.L2067:
	ldrh	w0, [x29, 44]
	ldr	w1, [x29, 16]
	sub	w0, w1, w0
	str	w0, [x29, 16]
	b	.L2069
.L2066:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 16]
	cmp	w1, w0
	bls	.L2069
	str	wzr, [x29, 16]
.L2069:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 16]
	udiv	w0, w0, w1
	str	w0, [x29, 16]
	ldr	w0, [x29, 16]
	cmp	w0, 0
	bne	.L2071
	mov	w0, 0
	b	.L2065
.L2074:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	cmp	w0, 0
	bne	.L2072
	ldr	w1, [x29, 60]
	ldr	x0, [x29, 48]
	bl	search_from_cache_write_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L2073
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 48]
	bl	del_from_cache_write_list
.L2073:
	ldr	w1, [x29, 60]
	ldr	x0, [x29, 48]
	bl	search_from_cache_read_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L2072
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 48]
	bl	del_from_cache_read_list
.L2072:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3520]
	ldr	w1, [x29, 60]
	ldr	x0, [x29, 24]
	blr	x2
	ldr	w0, [x29, 16]
	sub	w0, w0, #1
	str	w0, [x29, 16]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 104]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	uxtb	x1, w1
	add	x1, x2, x1
	str	x1, [x0, 104]
.L2071:
	ldr	w0, [x29, 16]
	cmp	w0, 0
	bne	.L2074
	ldr	w0, [x29, 56]
.L2065:
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_discard, .-nand_discard
	.align	2
	.global	__shutdown_op
	.type	__shutdown_op, %function
__shutdown_op:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	wzr, [sp, 28]
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	__shutdown_op, .-__shutdown_op
	.section	.rodata
	.align	3
.LC466:
	.string	"[NI]page_map NULL!!\n"
	.align	3
.LC467:
	.string	"[ND]%4d:<%4d %3d><%d>\n"
	.text
	.align	2
	.global	print_logic_page_map
	.type	print_logic_page_map, %function
print_logic_page_map:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 40]
	str	wzr, [x29, 44]
	b	.L2078
.L2081:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	get_logic_page_map
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L2079
	adrp	x0, .LC466
	add	x0, x0, :lo12:.LC466
	bl	nand_print
	b	.L2080
.L2079:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L2080
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 1]
	ubfx	x0, x0, 4, 1
	uxtb	w0, w0
	mov	w3, w0
	adrp	x0, .LC467
	add	x0, x0, :lo12:.LC467
	mov	w4, w3
	mov	w3, w2
	mov	w2, w1
	ldr	w1, [x29, 44]
	bl	nand_print_dbg
.L2080:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L2078:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bcc	.L2081
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	print_logic_page_map, .-print_logic_page_map
	.align	2
	.global	get_last_nftl
	.type	get_last_nftl, %function
get_last_nftl:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 8]
	str	x0, [sp, 16]
	b	.L2084
.L2085:
	ldr	x0, [sp, 24]
	str	x0, [sp, 16]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L2084:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L2085
	ldr	x0, [sp, 16]
	add	sp, sp, 32
	ret
	.size	get_last_nftl, .-get_last_nftl
	.align	2
	.global	get_nftl_need_read_claim
	.type	get_nftl_need_read_claim, %function
get_nftl_need_read_claim:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	str	wzr, [x29, 52]
	ldr	x0, [x29, 24]
	bl	get_last_nftl
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 172]
	str	w0, [x29, 52]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	b	.L2088
.L2091:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	w0, [x29, 52]
	cmp	w1, w0
	beq	.L2093
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
.L2088:
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L2091
	b	.L2090
.L2093:
	nop
.L2090:
	ldr	x0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	get_nftl_need_read_claim, .-get_nftl_need_read_claim
	.align	2
	.global	get_nftl_read_claim_page_no
	.type	get_nftl_read_claim_page_no, %function
get_nftl_read_claim_page_no:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_last_nftl
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 176]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_nftl_read_claim_page_no, .-get_nftl_read_claim_page_no
	.align	2
	.global	clean_read_reclaim_complete_flag
	.type	clean_read_reclaim_complete_flag, %function
clean_read_reclaim_complete_flag:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	b	.L2097
.L2098:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 24]
	ldr	w1, [x1, 664]
	cmp	w1, 0
	cset	w1, eq
	uxtb	w1, w1
	str	w1, [x0, 664]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L2097:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L2098
	nop
	add	sp, sp, 32
	ret
	.size	clean_read_reclaim_complete_flag, .-clean_read_reclaim_complete_flag
	.align	2
	.global	set_nftl_read_claim_complete
	.type	set_nftl_read_claim_complete, %function
set_nftl_read_claim_complete:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 24]
	mov	w1, 1
	str	w1, [x0, 660]
	ldr	x0, [x29, 24]
	bl	get_last_nftl
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L2100
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 172]
	ldr	x0, [x29, 24]
	bl	clean_read_reclaim_complete_flag
	b	.L2101
.L2100:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 16]
	ldr	x1, [x1, 24]
	ldrb	w1, [x1, 24]
	add	w1, w1, 1
	str	w1, [x0, 172]
.L2101:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 176]
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	set_nftl_read_claim_complete, .-set_nftl_read_claim_complete
	.align	2
	.global	set_nftl_read_claim_flag
	.type	set_nftl_read_claim_flag, %function
set_nftl_read_claim_flag:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	w2, [sp]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	b	.L2104
.L2105:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [sp, 4]
	str	w1, [x0, 172]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [sp]
	str	w1, [x0, 176]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L2104:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L2105
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	set_nftl_read_claim_flag, .-set_nftl_read_claim_flag
	.align	2
	.global	get_cold_logic_page
	.type	get_cold_logic_page, %function
get_cold_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	w0, [x29, 20]
	str	w0, [x29, 44]
	b	.L2108
.L2112:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	get_logic_page_map
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L2109
	adrp	x0, .LC466
	add	x0, x0, :lo12:.LC466
	bl	nand_print
	b	.L2110
.L2109:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L2110
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 1]
	ubfx	x0, x0, 4, 1
	uxtb	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 664]
	cmp	w1, w0
	beq	.L2110
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 664]
	uxtb	w0, w0
	and	w0, w0, 1
	uxtb	w2, w0
	ldr	x1, [x29, 32]
	ldrb	w0, [x1, 1]
	bfi	w0, w2, 4, 1
	strb	w0, [x1, 1]
	ldr	w0, [x29, 44]
	b	.L2111
.L2110:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L2108:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 16]
	cmp	w1, w0
	bcc	.L2112
	mov	w0, -1
.L2111:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_cold_logic_page, .-get_cold_logic_page
	.align	2
	.global	read_reclaim
	.type	read_reclaim, %function
read_reclaim:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	bl	get_nftl_read_claim_page_no
	str	w0, [x29, 56]
	ldr	x0, [x29, 32]
	ldr	x3, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldr	w0, [x0, 36]
	mov	w2, w0
	ldr	w1, [x29, 56]
	mov	x0, x3
	bl	get_cold_logic_page
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmn	w0, #1
	bne	.L2114
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	set_nftl_read_claim_complete
	str	w0, [x29, 60]
	b	.L2115
.L2114:
	ldr	x0, [x29, 32]
	ldr	x4, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	ldr	x3, [x29, 24]
	mov	w2, w0
	ldr	w1, [x29, 52]
	mov	x0, x4
	bl	nand_cache_read
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldrb	w0, [x0, 24]
	ldr	w2, [x29, 52]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_nftl_read_claim_flag
.L2115:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	read_reclaim, .-read_reclaim
	.local	panic_info
	.comm	panic_info,40,8
	.local	panic_part
	.comm	panic_part,16,8
	.local	nand_on_panic
	.comm	nand_on_panic,4,4
	.local	panic_enable
	.comm	panic_enable,4,4
	.align	2
	.type	panic_mark_panic, %function
panic_mark_panic:
	adrp	x0, nand_on_panic
	add	x0, x0, :lo12:nand_on_panic
	mov	w1, 1
	str	w1, [x0]
	nop
	ret
	.size	panic_mark_panic, .-panic_mark_panic
	.align	2
	.global	is_on_panic
	.type	is_on_panic, %function
is_on_panic:
	adrp	x0, nand_on_panic
	add	x0, x0, :lo12:nand_on_panic
	ldr	w0, [x0]
	cmp	w0, 0
	cset	w0, ne
	uxtb	w0, w0
	ret
	.size	is_on_panic, .-is_on_panic
	.align	2
	.global	panic_mark_enable
	.type	panic_mark_enable, %function
panic_mark_enable:
	adrp	x0, panic_enable
	add	x0, x0, :lo12:panic_enable
	mov	w1, 1
	str	w1, [x0]
	nop
	ret
	.size	panic_mark_enable, .-panic_mark_enable
	.align	2
	.global	is_panic_enable
	.type	is_panic_enable, %function
is_panic_enable:
	adrp	x0, panic_enable
	add	x0, x0, :lo12:panic_enable
	ldr	w0, [x0]
	cmp	w0, 0
	cset	w0, ne
	uxtb	w0, w0
	ret
	.size	is_panic_enable, .-is_panic_enable
	.section	.rodata
	.align	3
.LC468:
	.string	"[ND] panic part %s off %u size %u\n"
	.text
	.align	2
	.global	panic_init_part
	.type	panic_init_part, %function
panic_init_part:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	beq	.L2124
	ldr	w0, [x29, 16]
	cmp	w0, 0
	bne	.L2125
.L2124:
	mov	w0, -1
	b	.L2126
.L2125:
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	w0, [x0, 12]
	cmp	w0, 0
	beq	.L2127
	mov	w0, -1
	b	.L2126
.L2127:
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	x1, [x29, 24]
	str	x1, [x0]
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	w1, [x29, 20]
	str	w1, [x0, 8]
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	w1, [x29, 16]
	str	w1, [x0, 12]
	bl	panic_mark_enable
	adrp	x0, .LC468
	add	x0, x0, :lo12:.LC468
	ldr	w3, [x29, 16]
	ldr	w2, [x29, 20]
	ldr	x1, [x29, 24]
	bl	nand_print
	mov	w0, 0
.L2126:
	ldp	x29, x30, [sp], 32
	ret
	.size	panic_init_part, .-panic_init_part
	.section	.rodata
	.align	3
.LC469:
	.string	"[ND] reserved panic block before %d!\n"
	.align	3
.LC470:
	.string	"[NE] reserved panic block %d!\n"
	.align	3
.LC471:
	.string	"[NE] no reserved panic block!!! quit!\n"
	.text
	.align	2
	.global	panic_backup_block
	.type	panic_backup_block, %function
panic_backup_block:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	bl	is_panic_enable
	cmp	w0, 0
	bne	.L2129
	mov	w0, 0
	b	.L2130
.L2129:
	str	wzr, [x29, 60]
	b	.L2131
.L2135:
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	ldrsw	x1, [x29, 60]
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	beq	.L2132
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC469
	add	x0, x0, :lo12:.LC469
	bl	nand_print
	mov	w0, 0
	b	.L2130
.L2132:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	beq	.L2133
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC470
	add	x0, x0, :lo12:.LC470
	bl	nand_print
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	ldrsw	x1, [x29, 60]
	ldr	x2, [x29, 48]
	str	x2, [x0, x1, lsl 3]
	b	.L2136
.L2133:
	adrp	x0, .LC471
	add	x0, x0, :lo12:.LC471
	bl	nand_print
	mov	w0, -1
	b	.L2130
.L2136:
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L2131:
	ldr	w0, [x29, 60]
	cmp	w0, 2
	ble	.L2135
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, w0
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	str	w1, [x0, 24]
	mov	w0, 0
.L2130:
	ldp	x29, x30, [sp], 64
	ret
	.size	panic_backup_block, .-panic_backup_block
	.align	2
	.global	panic_read
	.type	panic_read, %function
panic_read:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	bl	is_panic_enable
	cmp	w0, 0
	bne	.L2138
	mov	w0, -1
	b	.L2139
.L2138:
	bl	panic_mark_panic
	b	.L2140
.L2141:
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 72]
	ldr	x3, [x29, 56]
	mov	w2, 32768
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x4
	ldr	x0, [x29, 56]
	add	x0, x0, 32768
	str	x0, [x29, 56]
	ldr	w0, [x29, 36]
	add	w0, w0, 32768
	str	w0, [x29, 36]
	ldr	w0, [x29, 32]
	sub	w0, w0, #32768
	str	w0, [x29, 32]
.L2140:
	ldr	w1, [x29, 32]
	mov	w0, 32767
	cmp	w1, w0
	bhi	.L2141
	ldr	w0, [x29, 32]
	cmp	w0, 0
	beq	.L2142
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 72]
	ldr	x3, [x29, 56]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x4
.L2142:
	mov	w0, 0
.L2139:
	ldp	x29, x30, [sp], 64
	ret
	.size	panic_read, .-panic_read
	.section	.rodata
	.align	3
.LC472:
	.string	"[ND] write panic data : logic addr: %d, sectors %d, pages %d, blocks %d\n"
	.align	3
.LC473:
	.string	"[NE] too much data for panic-write, %d sectors left over\n"
	.align	3
.LC474:
	.string	"[ND] write panic data block: %d page %d !\n"
	.align	3
.LC475:
	.string	"[ND] write dummy data block: %d page %d !\n"
	.text
	.align	2
	.global	panic_write
	.type	panic_write, %function
panic_write:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	str	x0, [x29, 112]
	bl	is_panic_enable
	cmp	w0, 0
	bne	.L2144
	mov	w0, -1
	b	.L2155
.L2144:
	bl	panic_mark_panic
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	ldr	x0, [x0, 8]
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	add	w0, w1, w0
	sub	w0, w0, #1
	ldr	x1, [x29, 96]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 124]
	ldr	x0, [x29, 96]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	add	w0, w1, w0
	sub	w0, w0, #1
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	adrp	x0, .LC472
	add	x0, x0, :lo12:.LC472
	ldr	w4, [x29, 92]
	ldr	w3, [x29, 124]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	nand_print
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w1, w0, #3
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bcs	.L2146
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w0, w0, #3
	ldr	x1, [x29, 96]
	ldrb	w1, [x1, 8]
	mul	w1, w0, w1
	adrp	x0, .LC473
	add	x0, x0, :lo12:.LC473
	bl	nand_print
	mov	w0, -1
	b	.L2155
.L2146:
	add	x0, x29, 72
	mov	w2, 16
	mov	w1, -91
	bl	nand_memset
	ldr	x0, [x29, 24]
	str	x0, [x29, 128]
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 112]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 136]
	b	.L2147
.L2150:
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L2148
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 28]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 28]
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 112]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 136]
	ldr	x0, [x29, 112]
	str	wzr, [x0, 32]
.L2148:
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	bne	.L2149
	ldr	x0, [x29, 136]
	ldrh	w1, [x0]
	ldr	x0, [x29, 96]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 104]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 104]
	blr	x2
	ldr	x0, [x29, 136]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 136]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 136]
	strh	wzr, [x0, 4]
.L2149:
	ldr	x0, [x29, 136]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 32]
	adrp	x0, .LC474
	add	x0, x0, :lo12:.LC474
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	mov	w0, -1
	strb	w0, [x29, 72]
	mov	w0, -86
	strb	w0, [x29, 73]
	mov	w0, 99
	strb	w0, [x29, 74]
	ldr	x0, [x29, 104]
	ldrb	w0, [x0, 24]
	strb	w0, [x29, 75]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 28]
	uxtb	w0, w0
	strb	w0, [x29, 76]
	ldr	x0, [x29, 136]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	add	x0, x29, 72
	mov	w3, 2
	mov	w2, 5
	bl	set_spare_data
	add	x0, x29, 72
	mov	w3, 4
	mov	w2, 7
	ldr	w1, [x29, 36]
	bl	set_spare_data
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	cmp	w1, w0
	csel	w1, w1, w0, ls
	add	x0, x29, 72
	mov	w3, 4
	mov	w2, 11
	bl	set_spare_data
	ldr	x0, [x29, 136]
	ldrh	w1, [x0]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	uxth	w2, w0
	ldr	x0, [x29, 96]
	ldrh	w3, [x0, 28]
	add	x4, x29, 72
	add	x0, x29, 48
	mov	x5, x4
	ldr	x4, [x29, 128]
	bl	set_physic_op_par
	ldr	x0, [x29, 104]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 104]
	blr	x2
	ldr	x0, [x29, 96]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 128]
	add	x0, x1, x0
	str	x0, [x29, 128]
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	cmp	w1, w0
	csel	w0, w1, w0, ls
	ldr	w1, [x29, 36]
	add	w0, w1, w0
	str	w0, [x29, 36]
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	cmp	w1, w0
	csel	w0, w1, w0, ls
	ldr	w1, [x29, 32]
	sub	w0, w1, w0
	str	w0, [x29, 32]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w1, w0, #1
	ldr	x0, [x29, 112]
	str	w1, [x0, 24]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 32]
	ldr	w0, [x29, 124]
	sub	w0, w0, #1
	str	w0, [x29, 124]
.L2147:
	ldr	w0, [x29, 124]
	cmp	w0, 0
	bne	.L2150
	mov	w0, 98
	strb	w0, [x29, 74]
	str	wzr, [x29, 120]
	b	.L2151
.L2154:
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	beq	.L2152
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcs	.L2152
	ldr	x0, [x29, 136]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 32]
	adrp	x0, .LC475
	add	x0, x0, :lo12:.LC475
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 136]
	ldrh	w1, [x0]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	uxth	w2, w0
	ldr	x0, [x29, 96]
	ldrh	w3, [x0, 28]
	add	x4, x29, 72
	add	x0, x29, 48
	mov	x5, x4
	ldr	x4, [x29, 128]
	bl	set_physic_op_par
	ldr	x0, [x29, 104]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 104]
	blr	x2
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 32]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w1, w0, #1
	ldr	x0, [x29, 112]
	str	w1, [x0, 24]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L2153
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 28]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 28]
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 112]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 136]
	ldr	x0, [x29, 112]
	str	wzr, [x0, 32]
.L2153:
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	str	w0, [x29, 120]
.L2151:
	ldr	w0, [x29, 120]
	cmp	w0, 2
	bls	.L2154
.L2152:
	mov	w0, 0
.L2155:
	ldp	x29, x30, [sp], 144
	ret
	.size	panic_write, .-panic_write
	.align	2
	.global	is_panic_data_page
	.type	is_panic_data_page, %function
is_panic_data_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L2157
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 99
	bne	.L2157
	mov	w0, 1
	b	.L2158
.L2157:
	mov	w0, 0
.L2158:
	add	sp, sp, 16
	ret
	.size	is_panic_data_page, .-is_panic_data_page
	.align	2
	.global	is_panic_dummy_page
	.type	is_panic_dummy_page, %function
is_panic_dummy_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L2161
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 98
	bne	.L2161
	mov	w0, 1
	b	.L2162
.L2161:
	mov	w0, 0
.L2162:
	add	sp, sp, 16
	ret
	.size	is_panic_dummy_page, .-is_panic_dummy_page
	.section	.rodata
	.align	3
.LC476:
	.string	"[ND] find panic block : %d\n"
	.align	3
.LC477:
	.string	"[NE] invalid panic block index %d\n"
	.text
	.align	2
	.global	panic_recover_info
	.type	panic_recover_info, %function
panic_recover_info:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	add	x0, x0, 4
	ldrb	w0, [x0]
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 2
	bhi	.L2165
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC476
	add	x0, x0, :lo12:.LC476
	bl	nand_print
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	ldr	w1, [x29, 44]
	ldr	x2, [x29, 16]
	str	x2, [x0, x1, lsl 3]
	mov	w0, 0
	b	.L2166
.L2165:
	adrp	x0, .LC477
	add	x0, x0, :lo12:.LC477
	ldr	w1, [x29, 44]
	bl	nand_print
	mov	w0, -1
.L2166:
	ldp	x29, x30, [sp], 48
	ret
	.size	panic_recover_info, .-panic_recover_info
	.section	.rodata
	.align	3
.LC478:
	.string	"[ND] not find panic data, no need to recovery!\n"
	.align	3
.LC479:
	.string	"[ND] recovery panic data start!\n"
	.align	3
.LC480:
	.string	"[NE] %s(%d): malloc failed\n"
	.align	3
.LC481:
	.string	"[ND] ignore blk %d page %d as dummy page\n"
	.align	3
.LC482:
	.string	"[ND] exit recovery panic block\n"
	.align	3
.LC483:
	.string	"[ND] invalid blk %d page %d, finish recovery\n"
	.align	3
.LC484:
	.string	"[ND] move blk %d page %d to logic addr %d\n"
	.text
	.align	2
	.global	panic_recover_data
	.type	panic_recover_data, %function
panic_recover_data:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	str	x0, [x29, 120]
	ldr	x0, [x29, 120]
	ldr	x0, [x0, 8]
	str	x0, [x29, 112]
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	str	x0, [x29, 104]
	str	xzr, [x29, 96]
	ldr	x0, [x29, 104]
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L2168
	adrp	x0, .LC478
	add	x0, x0, :lo12:.LC478
	bl	nand_print
	mov	w0, 0
	b	.L2169
.L2168:
	adrp	x0, .LC479
	add	x0, x0, :lo12:.LC479
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 26]
	bl	nand_malloc
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	cmp	x0, 0
	bne	.L2170
	adrp	x0, __func__.8557
	add	x1, x0, :lo12:__func__.8557
	adrp	x0, .LC480
	add	x0, x0, :lo12:.LC480
	mov	w2, 308
	bl	nand_print
	mov	w0, -1
	b	.L2169
.L2170:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	w1, 0
	ldr	x0, [x29, 96]
	bl	nand_memset
	ldr	x0, [x29, 104]
	str	wzr, [x0, 28]
	b	.L2171
.L2180:
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 104]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 88]
	ldr	x0, [x29, 104]
	str	wzr, [x0, 32]
	b	.L2172
.L2179:
	ldr	x0, [x29, 88]
	ldrh	w1, [x0]
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	uxth	w2, w0
	ldr	x0, [x29, 112]
	ldrh	w3, [x0, 28]
	add	x4, x29, 40
	add	x0, x29, 56
	mov	x5, x4
	ldr	x4, [x29, 96]
	bl	set_physic_op_par
	ldr	x0, [x29, 120]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 120]
	blr	x2
	add	x0, x29, 40
	bl	is_panic_dummy_page
	cmp	w0, 0
	beq	.L2173
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC481
	add	x0, x0, :lo12:.LC481
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	b	.L2174
.L2173:
	add	x0, x29, 40
	bl	is_panic_data_page
	cmp	w0, 0
	bne	.L2175
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 0
	bne	.L2176
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	bne	.L2176
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC482
	add	x0, x0, :lo12:.LC482
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	b	.L2177
.L2176:
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC483
	add	x0, x0, :lo12:.LC483
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	b	.L2181
.L2175:
	add	x0, x29, 40
	mov	w2, 4
	mov	w1, 7
	bl	get_spare_data
	str	w0, [x29, 84]
	add	x0, x29, 40
	mov	w2, 4
	mov	w1, 11
	bl	get_spare_data
	str	w0, [x29, 80]
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC484
	add	x0, x0, :lo12:.LC484
	ldr	w3, [x29, 84]
	mov	w2, w1
	mov	w1, w4
	bl	nand_print
	ldr	x2, [x29, 96]
	ldr	w1, [x29, 80]
	ldr	w0, [x29, 84]
	bl	nftl_write
.L2174:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	add	w1, w0, 1
	ldr	x0, [x29, 104]
	str	w1, [x0, 32]
.L2172:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 112]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L2179
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	add	w1, w0, 1
	ldr	x0, [x29, 104]
	str	w1, [x0, 28]
.L2171:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 2
	bls	.L2180
.L2178:
	b	.L2181
.L2182:
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 104]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 88]
	ldr	x0, [x29, 120]
	bl	erase_block
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	sub	w1, w0, #1
	ldr	x0, [x29, 104]
	str	w1, [x0, 28]
.L2181:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 0
	bge	.L2182
.L2177:
	ldr	x0, [x29, 96]
	bl	nand_free
	mov	w0, 0
.L2169:
	ldp	x29, x30, [sp], 128
	ret
	.size	panic_recover_data, .-panic_recover_data
	.section	.rodata
	.align	3
	.type	__func__.4479, %object
	.size	__func__.4479, 33
__func__.4479:
	.string	"samsung_set_readretry_K9GCGD8U0F"
	.align	3
	.type	__func__.6341, %object
	.size	__func__.6341, 25
__func__.6341:
	.string	"sandisk_A19_special_exit"
	.align	3
	.type	__func__.6431, %object
	.size	__func__.6431, 16
__func__.6431:
	.string	"nftl_initialize"
	.align	3
	.type	__func__.6563, %object
	.size	__func__.6563, 9
__func__.6563:
	.string	"nftl_add"
	.align	3
	.type	__func__.6859, %object
	.size	__func__.6859, 15
__func__.6859:
	.string	"shutdown_on_gc"
	.align	3
	.type	__func__.7007, %object
	.size	__func__.7007, 19
__func__.7007:
	.string	"get_logic_page_map"
	.align	3
	.type	__func__.7328, %object
	.size	__func__.7328, 16
__func__.7328:
	.string	"zone_param_init"
	.align	3
	.type	__func__.7542, %object
	.size	__func__.7542, 9
__func__.7542:
	.string	"init_nrc"
	.align	3
	.type	__func__.7632, %object
	.size	__func__.7632, 23
__func__.7632:
	.string	"torture_block_and_mark"
	.align	3
	.type	__func__.7756, %object
	.size	__func__.7756, 26
__func__.7756:
	.string	"block_last_page_ecc_error"
	.align	3
	.type	__func__.7788, %object
	.size	__func__.7788, 18
__func__.7788:
	.string	"nand_dbg_phy_read"
	.align	3
	.type	__func__.7799, %object
	.size	__func__.7799, 23
__func__.7799:
	.string	"nand_dbg_zone_phy_read"
	.align	3
	.type	__func__.7810, %object
	.size	__func__.7810, 24
__func__.7810:
	.string	"nand_dbg_zone_phy_write"
	.align	3
	.type	__func__.7819, %object
	.size	__func__.7819, 19
__func__.7819:
	.string	"nand_dbg_phy_write"
	.align	3
	.type	__func__.7872, %object
	.size	__func__.7872, 19
__func__.7872:
	.string	"write_data_to_nand"
	.align	3
	.type	__FUNCTION__.7896, %object
	.size	__FUNCTION__.7896, 25
__FUNCTION__.7896:
	.string	"nand_power_off_ecc_error"
	.align	3
	.type	__func__.7922, %object
	.size	__func__.7922, 11
__func__.7922:
	.string	"nftl_start"
	.align	3
	.type	__func__.8106, %object
	.size	__func__.8106, 23
__func__.8106:
	.string	"nftl_sector_cache_init"
	.align	3
	.type	__func__.8204, %object
	.size	__func__.8204, 16
__func__.8204:
	.string	"nftl_cache_init"
	.align	3
	.type	__func__.8557, %object
	.size	__func__.8557, 19
__func__.8557:
	.string	"panic_recover_data"
	.ident	"GCC: (Linaro GCC 5.3-2016.05) 5.3.1 20160412"
	.section	.note.GNU-stack,"",@progbits
