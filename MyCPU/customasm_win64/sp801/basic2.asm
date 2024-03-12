#include "sp801.asm"

;uint32_t A = B = C = 0;
	lda 0
	ldx 12

	clearLoop:
	sta [x~ A]
	dec x
	bzc clearLoop

	;A = 1;
	lda 1
	sta [A]

	;while(true) {
	whileTrueLoop:
		;	C = A;
		lda [A+0]
		sta [C+0]
		lda [A+1]
		sta [C+1]
		lda [A+2]
		sta [C+2]
		lda [A+3]
		sta [C+3]
		;	A = B + A;
		lda [B+0]
		add [A+0]
		sta [A+0] 
		lda [B+1]
		addc [A+1]
		sta [A+1] 
		lda [B+2]
		addc [A+2]
		sta [A+2] 
		lda [B+3]
		addc [A+3]
		sta [A+3] 
		;	B = C;
		lda [C+0]
		sta [B+0]
		lda [C+1]
		sta [B+1]
		lda [C+2]
		sta [B+2]
		lda [C+3]
		sta [B+3]
		;print(
		;}
	jmp whileTrueLoop

message:
#d "hello world", 0x00

#bank ram
A:	
#res 4
B:	
#res 4
C:	
#res 4
#bank rom


#include "std.asm"