*vecs_int_flash.asm Vector file. Setup interrupt vectors and boot section for flash

FLASH_START   .equ    0x90000400        ;flash start address
CODE_START    .equ    0x00000400        ;start of non boot code
CODE_SIZE     .equ    0x00003000        ;application code size in byte

   .global _vectors  ;Global symbols defined here/exported out of this file
   .global _c_int00
   .global _vector1
   .global _vector2
   .global _vector3
   .global _vector4
   .global _vector5
   .global _vector6
   .global _vector7
   .global _vector8
   .global _vector9	  
   .global _vector10 
   .global _c_int11   
   .global _vector12  
   .global _vector13   
   .global _vector14
   .global _vector15

   .ref _c_int00	;Global symbols referenced in this file but defined somewhere else 

VEC_ENTRY .macro addr  ;macro instantiates one entry in interrupt service table
    STW   B0,*--B15
    MVKL  addr,B0
    MVKH  addr,B0
    B     B0
    LDW   *B15++,B0
    NOP   2
    NOP   
    NOP   
   .endm

_vec_dummy:  ;dummy interrupt service routine used to initialize the IST
  B    B3
  NOP  5

* Actual interrupt service table (IST). Properly aligned and located in 
* subsection .text:vecs. If this section is not explicitly specified in 
* linker command file, it will default and link into .text section. 
* Remember to set the ISTP register to point to this table.

 .sect ".vecs"
 .align 1024

_vectors:
_vector0:   VEC_ENTRY _boot_start ;RESET to start of boot loader 
					    ;to copy code from FLASH
_vector1:   VEC_ENTRY _vec_dummy  ;NMI
_vector2:   VEC_ENTRY _vec_dummy  ;RSVD
_vector3:   VEC_ENTRY _vec_dummy
_vector4:   VEC_ENTRY _vec_dummy
_vector5:   VEC_ENTRY _vec_dummy
_vector6:   VEC_ENTRY _vec_dummy
_vector7:   VEC_ENTRY _vec_dummy
_vector8:   VEC_ENTRY _vec_dummy
_vector9:   VEC_ENTRY _vec_dummy
_vector10:  VEC_ENTRY _vec_dummy
_vector11:  VEC_ENTRY _c_int11
_vector12:  VEC_ENTRY _vec_dummy
_vector13:  VEC_ENTRY _vec_dummy
_vector14:  VEC_ENTRY _vec_dummy
_vector15:  VEC_ENTRY _vec_dummy

	.sect "bootload"
_boot_start:
    	mvkl  FLASH_START,B4 		;flash start address ->B4
    	mvkh  FLASH_START,B4   
      	
	mvkl  CODE_START,A4 		;apps code start address ->A4
	mvkh  CODE_START,A4
	zero  A1
	
_boot_loop1:
	ldb   *B4++,B5       		;flash read
	mvkl  CODE_SIZE-4,B6 		;B6 = BOOT_SIZE -1024
	add   1,A1,A1          		;A1+=1,inc outer counter
 ||   mvkh  CODE_SIZE-4,B6
	cmplt  A1,B6,B0
	nop    
	stb   B5,*A4++
	[B0]  b     _boot_loop1
	nop   5
	
	mvkl .S2 _c_int00, B0
	mvkh .S2 _c_int00, B0
	B    .S2 B0
	nop   5	
