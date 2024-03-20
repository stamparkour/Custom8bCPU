#include "sp801.asm"
#include "std.asm"

;requests:
;ret to call
;int to push
;nop to jmp
;sty, stx, sta extra space - xty, ytx
;ldyx and styx reserved

entry:
	lda 3
	sta [var]
	ldyx message
	call print
	sti
	jmp $
interrupt:
	call clearscreen
	dbs
	ldy 0
	lda [test]
	inc a
	sta [test]
	call printByteHex
	dbc
	iret

message:
#d "cheese?\nY|N", 0x00
message2:
#d "0 is a god\nbe", 0x00
.length = $-message2

#bank mem

var:
#res 1
test:
#res 2