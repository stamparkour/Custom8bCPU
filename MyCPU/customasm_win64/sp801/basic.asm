#include "sp801.asm"
#include "std.asm"

entry:
	ldyx message2
	styx std_mem+0
	ldyx vmsg
	styx std_mem+2
	ldx 2
	call memcpy
	sti
	lda 3
	sta [var]
	ldyx message
	call print
	jmp $
interrupt:
	ldyx vmsg
	call print
	
	brk
	lda [var]
	inc a
	sta [var] 
	add "0"
	sta [vmsg]
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