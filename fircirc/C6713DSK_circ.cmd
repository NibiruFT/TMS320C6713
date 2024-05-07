/*C6713dsk_circ.cmd Linker command file with section for circular buffer*/

MEMORY
{
  IVECS:      org =          0h,  len =      0x220
  IRAM:       org =  0x00000220,  len = 0x0002FFFF
  SRAM_EXT:   org =  0x80000000,  len = 0x01000000
  FLASH:      org =  0x90000000,  len = 0x00020000                     
}

SECTIONS
{
  circdata :> IRAM		/*aligned buffer section */
  .vecs    :> IVECS		/*Created in vectors file*/
  .text    :> IRAM		/*Created by C Compiler*/
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