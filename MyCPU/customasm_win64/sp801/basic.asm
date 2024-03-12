#include "sp801.asm"
#include "std.asm"

entry:
	sti
	lda 3
	sta [var]
	ldyx message
	call print
	jmp $
interrupt:
	ldyx message2
	call print
	
	brk
	lda [var]
	inc a
	sta [var] 
	add "0"
	sta [message2]
	iret

message:
#d "cheese?\nY|N", 0x00
message2:
#d "0", 0x00

#bank mem

vmsg:
#res 2
var:
#res 1