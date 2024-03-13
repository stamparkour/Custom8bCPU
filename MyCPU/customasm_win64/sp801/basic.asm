#include "sp801.asm"
#include "std.asm"

;requests:
;ret to call
;int to push
;nop to jmp
;sty, stx, sta extra space - xty, ytx
;ldyx and styx reserved

entry:
	ldyx message2
	styx [std_mem+0]
	ldyx vmsg
	styx [std_mem+2]
	ldx message2.length
	call memcpy
	sti
	lda 3
	sta [var]
	ldyx message
	call print
	jmp $
interrupt:
	call clearscreen

	ldyx vmsg
	call print
	
	brk
	lda [var]
	inc a
	cmp 10
	bne .b1
		lda 0
	.b1:
	sta [var] 
	add "0"
	sta [vmsg]
	iret

message:
#d "cheese?\nY|N", 0x00
message2:
#d "0 is a god\nbe", 0x00
.length = $-message2

#bank mem

vmsg:
#res message2.length
var:
#res 1