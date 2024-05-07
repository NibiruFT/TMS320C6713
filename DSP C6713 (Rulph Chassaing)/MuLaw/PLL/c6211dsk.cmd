/*C6211dsk.cmd Linker command file*/

MEMORY
{
  VECS:       org=          0h,  len=       0x220
  IRAM:       org = 0x00000220,  len = 0x0000FE00                           
  SDRAM: 	  org = 0x80000000,  len = 0x01000000	
}
SECTIONS
{
  sin8000_table :> SDRAM

  vectors  :> VECS		/*Created in vectors.asm */
  .text    :> IRAM		/*Created by C Compiler  */
  .bss     :> IRAM
   coeffs  :> IRAM
  .cinit   :> IRAM
  .stack   :> IRAM
  .sysmem  :> IRAM
  .const   :> IRAM
  .switch  :> IRAM
  .far     :> IRAM
  .cio     :> IRAM
}
