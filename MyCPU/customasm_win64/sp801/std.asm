#once
#include "sp801.asm"

#bank mem
std_mem:	
#res 16
#bank prog

;preserve A,X,Y
;payx


;void printhex(uint8_t a)

;void memcpy(char* s : std_mem+0, char* d : yx, uint8_t length : a)
memcpy:
	.s = std_mem + 0
	.d = std_mem + 2
	.len = std_mem + 4

	styx [.d]
	sta [.len]
	.loop:
		ldyx [.s]
		lda [yx]
		inc X
		bcc .b1
		inc Y
		.b1:
		styx [.s]
		ldyx [.d]
		sta [yx]
		inc X
		bcc .b2
		inc Y
		.b2:
		styx [.d]
		ldx [.len]
		dec X
		stx [.len]
		bzc .loop
	ret
	
;void clearscreen()
clearscreen:
	;uint8_t i = 20;
	;uint8_t c = ' ';
	ldx 0x20
	lda " "
	.loop:
		dec x
		stx [display]
		sta [display]
	bne .loop
	ret

;void print(char* cstring : yx)
print:
	.index = std_mem
	;uint8_t index = 0;
	lda 0x00
	sta [.index]
	;while(true) {
	.loop:
		;if(*cstring == 0) return;
		lda [yx]
		cmp "\n"
		bne .b2
			lda [.index]
			add 15
			and 0b00010000
			sta [.index]
			jmp .increment
		.b2:
		cmp 0
		bne .b3
			ret
		.b3:
		;display(index);
		;display(*cstring);
		push x
		ldx [.index]
		stx [display]
		sta [display]
		;index++;
		inc x
		stx [.index]
		pop x
		;cstring++;
		.increment:
		inc x
		bcc .b1
			inc y
		.b1:
	;}
	jmp .loop

HEX_DIGITS:
#d "0123456789ABCDEF"
;void printByteHex(unsigned char v : a, unsigned char i : y) //i -> i + 2
printByteHex:
	.data = std_mem
	sta [.data]
	and 0xF0
	shr a
	shr a
	shr a
	shr a
	xta
	lda [x~ HEX_DIGITS]
	xty
	inc y
	stx [display]
	sta [display]
	lda [.data]
	and 0x0F
	xta
	lda [x~ HEX_DIGITS]
	xty
	inc y
	stx [display]
	sta [display]
	ret
;void printShortHex(unsigned short v : yx)
printShortHex:
	.data = std_mem+1
	styx [.data]
	lda [.data+1]
	ldy 0
	call printByteHex
	lda [.data]
	call printByteHex
	ret

;void printLongHex(const unsigned long* v : yx)
printLongHex:
	.ptr = std_mem+1
	styx [.ptr]
	lda [yx]
	ldy 6
	call printByteHex

	ldyx [.ptr]
	inc X
	bcc .b1
		inc Y
	.b1:
	styx [.ptr]
	lda [yx]
	ldy 4
	call printByteHex

	ldyx [.ptr]
	inc X
	bcc .b2
		inc Y
	.b2:
	styx [.ptr]
	lda [yx]
	ldy 2
	call printByteHex

	ldyx [.ptr]
	inc X
	bcc .b3
		inc Y
	.b3:
	styx [.ptr]
	lda [yx]
	ldy 0
	call printByteHex
	ret