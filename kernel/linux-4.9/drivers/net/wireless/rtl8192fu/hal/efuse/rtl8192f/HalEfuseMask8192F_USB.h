/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/


#if (RTL8192F_SUPPORT == 1)


/******************************************************************************
*                           MUSB.TXT
******************************************************************************/

u16 EFUSE_GetArrayLen_MP_8192F_MUSB(void);

void EFUSE_GetMaskArray_MP_8192F_MUSB(u8 *Array);

BOOLEAN EFUSE_IsAddressMasked_MP_8192F_MUSB( /* TC: Test Chip, MP: MP Chip*/
		u16  Offset
);

#else
#define EFUSE_GetArrayLen_MP_8192F_MUSB()  	0
#define EFUSE_GetMaskArray_MP_8192F_MUSB(_Array)		0
#define EFUSE_IsAddressMasked_MP_8192F_MUSB(_Offset)	FALSE

#endif /* end of EFUSE_SUPPORT*/

/*8192F has no TestChip, this is for Compile*/
#define EFUSE_GetArrayLen_TC_8192F_MUSB() 	0
#define EFUSE_GetMaskArray_TC_8192F_MUSB(_Array)		0
#define EFUSE_IsAddressMasked_TC_8192F_MUSB(_Offset)	FALSE
