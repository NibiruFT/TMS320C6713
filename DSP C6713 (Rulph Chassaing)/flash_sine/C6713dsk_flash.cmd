/*C6713dsk_flash.cmd  Linker command file for flash*/

MEMORY
{
  VECS:	    org = 0h,  len =      0x200 
  BOOT:     org = 0x00000200, len = 0x200  													
  IRAM:     org = 0x00000400, len = 0x0002FB00
  SRAM_EXT: org = 0x80000000, len = 0x01000000
  FLASH:    org = 0x90000000, len = 0x00020000                     
}

SECTIONS
{
  "bootload" :> BOOT  /*boot section.Runs when C6713 starts*/
  .vecs      :> VECS		
  .text      :> IRAM		
  .bss       :> IRAM
  .cinit     :> IRAM
  .stack     :> IRAM
  .sysmem    :> IRAM
  .const     :> IRAM
  .switch    :> IRAM
  .far     :> IRAM
  .cio     :> IRAM
  .csldata :> IRAM

}