/*FIRcirc_ext.cmd Linker command file for external memory*/

MEMORY
{
  IVECS:     org =          0h,  len =      0x220
  IRAM:      org =  0x00000220,  len = 0x0002FFFF
  SRAM_EXT1: org =  0x80000000,  len = 0x00000110
  SRAM_EXT2: org =  0x80000110,  len = 0x00100000
  FLASH:     org =  0x90000000,  len = 0x00020000                     
}

SECTIONS
{
  circdata :> SRAM_EXT1 /*buffer in external mem*/
  .vecs    :> IVECS	/*Created in vectors file*/
  .text    :> IRAM	/*Created by C Compiler*/
  .bss     :> IRAM
  .cinit   :> IRAM
  .stack   :> IRAM
  .sysmem  :> IRAM
  .const   :> IRAM
  .switch  :> IRAM
  .far     :> IRAM
  .cio     :> IRAM
  .csldata :> IRAM
}