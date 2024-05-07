;vectors_dotp4a.asm Vector file for dotp4a project

        .ref      init      ;starting addr in init file
        .sect 	"vectors" ;in section vectors
rst:    mvkl .s2	init,b0   ;init addr 16 LSB -->B0
        mvkh .s2	init,b0   ;init addr 16 MSB -->B0
        b	 	b0        ;branch to addr init
        nop		5
