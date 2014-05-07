MEMORY { .sram : ORIGIN = 0x402F0400, LENGTH = (101 * 1024) }
MEMORY { .sdram : ORIGIN = 0x80000000, LENGTH = 0x80000 }
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
 .text :
 {
  __start = .;
  arch/arm/cpu/armv7/start.o (.text)
  *(.text*)
 } >.sram
 . = ALIGN(4);
 .rodata : { *(SORT_BY_ALIGNMENT(.rodata*)) } >.sram
 . = ALIGN(4);
 .data : { *(SORT_BY_ALIGNMENT(.data*)) } >.sram
 .u_boot_list : {
_u_boot_list__start = .;
_u_boot_list_cmd__start = .;
*(SORT(.u_boot_list.cmd.*));
_u_boot_list_cmd__end = .;
_u_boot_list_env_clbk__start = .;
*(SORT(.u_boot_list.env_clbk.*));
_u_boot_list_env_clbk__end = .;
*(SORT(.u_boot_list.*));
_u_boot_list__end = .;
 }
 . = ALIGN(4);
 __image_copy_end = .;
 _end = .;
 .bss :
 {
  . = ALIGN(4);
  __bss_start = .;
  *(.bss*)
  . = ALIGN(4);
  __bss_end__ = .;
 } >.sdram
}
