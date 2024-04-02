#include "sp801.asm"
#include "std.asm"

;requests:
;ret to call
;int to push
;nop to jmp
;sty, stx, sta extra space - xty, ytx
;ldyx and styx reserved

entry:
	ldyx 1
	styx [B]
	ldyx 0
	styx [B+2]
	sti
loop:
	ldyx A
	call printLongHex
	stbcd
	lda [A]
	add [B]
	sta [C]
	lda [A+1]
	addc [B+1]
	sta [C+1]
	lda [A+2]
	addc [B+2]
	sta [C+2]
	lda [A+3]
	addc [B+3]
	sta [C+3]
	clbcd
	ldyx [B]
	styx [A]
	ldyx [B+2]
	styx [A+2]
	ldyx [C]
	styx [B]
	ldyx [C+2]
	styx [B+2]

	ldy 4
	.b2:
	ldx 255
	.b1:
	dec x
	bzc .b1
	dec y 
	bzc .b2

	jmp loop
interrupt:
	;call clearscreen
	iret

message:
#d "cheese?\nY|N", 0x00
message2:
#d "0 is a god\nbe", 0x00
.length = $-message2

#bank mem

var:
#res 1
A:
#res 4
B:
#res 4
C:
#res 4