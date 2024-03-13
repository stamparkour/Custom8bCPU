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
		decx
		sta [x~ display]
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
		;display[index] = *cstring;
		push x
		ldx [.index]
		sta [x~ display]
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