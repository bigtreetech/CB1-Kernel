/*
 * (C) Copyright 2019
 * frank@allwinnertech.com
 */

#ifndef __PRIVATE_OPENSBI__
#define __PRIVATE_OPENSBI__

/******************************************************************************/
/*               the control information stored in file head                  */
/******************************************************************************/
struct private_opensbi_head {
	unsigned int  jump_instruction;	/* jumping to real code */
	unsigned char magic[8];			/* ="opensbi" */
	unsigned int  dtb_base;			/* the address of dtb base*/
	unsigned int  uboot_base;		/* the address of uboot base*/
	unsigned int  res3;
	unsigned int  res4;
	unsigned char res5[8];
	unsigned char res6[8];
	unsigned int opensbi_base;		/* the address of opensbi base*/
};
#endif /* __PRIVATE_OPENSBI__ */
