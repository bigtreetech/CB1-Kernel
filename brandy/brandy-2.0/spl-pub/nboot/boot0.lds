OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
 . = 0x20000;
 . = ALIGN(4);
 .head :
 {
  main/boot0_head.o (.rodata)
 }
    . = ALIGN(1);
 .text :
 {
  /home/lodge/Allwinner-H616/brandy/brandy-2.0/spl-pub/arch/arm/cpu/armv7/boot0_entry.o (.text)
  *(.text)
 }
 . = ALIGN(16);
   .rodata : { *(.rodata) }
 . = ALIGN(16);
   .data : { *(.data) }
 . = ALIGN(4);
 .bss :
 {
 __bss_start = .;
  *(.bss)
 }
 . = ALIGN(4);
 __bss_end = .;
 _end = .;
 ASSERT(. <= (0x20000 + 0x10000), "boot0 image has exceeded its limit.")
}
