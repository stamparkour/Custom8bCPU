#once

seg_prog = 0x0000
seg_vec = 0x7FF0
seg_mem = 0x8000

seg_prog_size = 0x7FF0
seg_vec_size = 0x0010
seg_mem_size = 0x8000

#bankdef prog {
	#labelalign 8
	#bits 8
	#addr seg_prog
	#outp 8 * seg_prog
	#size seg_prog_size
	#fill
}

#bankdef vec {
	#labelalign 8
	#bits 8
	#addr seg_vec
	#outp 8 * seg_vec
	#size seg_vec_size
	#fill
}

#bankdef mem {
	#labelalign 8
	#bits 8
	#addr seg_mem
	#size seg_mem_size
}


display = 0xFEE0

#bank vec
#d16 le(0x0000`16) 	;0x7FF0
#d16 le(0x0000`16) 	;0x7FF2
#d16 le(0x0000`16) 	;0x7FF4
#d16 le(0x0000`16) 	;0x7FF6
#d16 le(0x0000`16) 	;0x7FF8
#d16 le(0x0000`16) 	;0x7FFA
#d16 le(interrupt`16) 	;0x7FFC
#d16 le(entry`16) 		;0x7FFE

#bank prog

#ruledef {
	nop => 0x00
	brk => 0x01
	jmp {addr : u16} => 0x40 @ le(addr)
	jmp x~ {addr : u16} => 0x80 @ le(addr)
	jmp yx => 0xC0
	bzs {addr : u16} => 0x41 @ le(addr)
	bzs x~ {addr : u16} => 0x81 @ le(addr)
	bzs yx => 0xC1
	be {addr : u16} => 0x41 @ le(addr)
	be x~ {addr : u16} => 0x81 @ le(addr)
	be yx => 0xC1
	bzc {addr : u16} => 0x42 @ le(addr)
	bzc x~ {addr : u16} => 0x82 @ le(addr)
	bzc yx => 0xC2
	bne {addr : u16} => 0x42 @ le(addr)
	bne x~ {addr : u16} => 0x82 @ le(addr)
	bne yx => 0xC2
	bcs {addr : u16} => 0x43 @ le(addr)
	bcs x~ {addr : u16} => 0x83 @ le(addr)
	bcs yx => 0xC3
	bl {addr : u16} => 0x43 @ le(addr)
	bl x~ {addr : u16} => 0x83 @ le(addr)
	bl yx => 0xC3
	bcc {addr : u16} => 0x44 @ le(addr)
	bcc x~ {addr : u16} => 0x84 @ le(addr)
	bcc yx => 0xC4
	bge {addr : u16} => 0x44 @ le(addr)
	bge x~ {addr : u16} => 0x84 @ le(addr)
	bge yx => 0xC4
	bg {addr : u16} => 0x45 @ le(addr)
	bg x~ {addr : u16} => 0x85 @ le(addr)
	bg yx => 0xC5
	ble {addr : u16} => 0x46 @ le(addr)
	ble x~ {addr : u16} => 0x86 @ le(addr)
	ble yx => 0xC6
	ret => 0x06
	call {addr : u16} => 0x47 @ le(addr)
	call x~ {addr : u16} => 0x87 @ le(addr)
	call yx => 0xC7
	lda {const : i8} => 0x08 @ le(const)
	lda [{addr : u16}] => 0x48 @ le(addr)
	lda [x~ {addr : u16}] => 0x88 @ le(addr)
	lda [yx] => 0xC8
	ldx {const : i8} => 0x09 @ le(const)
	ldx [{addr : u16}] => 0x49 @ le(addr)
	ldx [x~ {addr : u16}] => 0x89 @ le(addr)
	ldx [yx] => 0xC9
	ldy {const : i8} => 0x0A @ le(const)
	ldy [{addr : u16}] => 0x4A @ le(addr)
	ldy [x~ {addr : u16}] => 0x8A @ le(addr)
	ldy [yx] => 0xCA
	sta [{addr : u16}] => 0x4B @ le(addr)
	sta [x~ {addr : u16}] => 0x8B @ le(addr)
	sta [yx] => 0xCB
	stx [{addr : u16}] => 0x4C @ le(addr)
	stx [x~ {addr : u16}] => 0x8C @ le(addr)
	stx [yx] => 0xCC
	sty [{addr : u16}] => 0x4D @ le(addr)
	sty [x~ {addr : u16}] => 0x8D @ le(addr)
	sty [yx] => 0xCD
	inc a => 0x0E
	inc x => 0x4E
	inc y => 0x8E
	atx => 0xCE
	dec a => 0x0F
	dec x => 0x4F
	dec y => 0x8F
	aty => 0xCF
	add {const : i8} => 0x10 @ le(const)
	add [{addr : u16}] => 0x50 @ le(addr)
	add [x~ {addr : u16}] => 0x90 @ le(addr)
	add [yx] => 0xD0
	sub {const : i8} => 0x11 @ le(const)
	sub [{addr : u16}] => 0x51 @ le(addr)
	sub [x~ {addr : u16}] => 0x91 @ le(addr)
	sub [yx] => 0xD1
	addc {const : i8} => 0x12 @ le(const)
	addc [{addr : u16}] => 0x52 @ le(addr)
	addc [x~ {addr : u16}] => 0x92 @ le(addr)
	addc [yx] => 0xD2
	subc {const : i8} => 0x13 @ le(const)
	subc [{addr : u16}] => 0x53 @ le(addr)
	subc [x~ {addr : u16}] => 0x93 @ le(addr)
	subc [yx] => 0xD3
	and {const : i8} => 0x14 @ le(const)
	and [{addr : u16}] => 0x54 @ le(addr)
	and [x~ {addr : u16}] => 0x94 @ le(addr)
	and [yx] => 0xD4
	or {const : i8} => 0x15 @ le(const)
	or [{addr : u16}] => 0x55 @ le(addr)
	or [x~ {addr : u16}] => 0x95 @ le(addr)
	or [yx] => 0xD5
	xor {const : i8} => 0x16 @ le(const)
	xor [{addr : u16}] => 0x56 @ le(addr)
	xor [x~ {addr : u16}] => 0x96 @ le(addr)
	xor [yx] => 0xD6
	cmp {const : i8} => 0x17 @ le(const)
	cmp [{addr : u16}] => 0x57 @ le(addr)
	cmp [x~ {addr : u16}] => 0x97 @ le(addr)
	cmp [yx] => 0xD7
	shl a => 0x18
	shl x => 0x58
	shl y => 0x98
	xta => 0xD8
	shr a => 0x19
	shr x => 0x59
	shr y => 0x99
	yta => 0xD9
	push a => 0x1A
	push x => 0x5A
	push y => 0x9A
	int => 0xDA
	pop a => 0x1B
	pop x => 0x5B
	pop y => 0x9B
	iret => 0xDB
	stc => 0x1C
	stz => 0x5C
	stbcd => 0x9C
	sti => 0xDC
	clc => 0x1D
	clz => 0x5D
	clbcd => 0x9D
	cli => 0xDD
	ldyx {const : i8} => 0x0A @ le(const)
	ldyx [{addr : u16}] => 0x4A @ le(addr)
	ldyx [x~ {addr : u16}] => 0x8A @ le(addr)
	ldyx [yx] => 0xCA
	stax [{addr : u16}] => 0x4B @ le(addr)
	stax [x~ {addr : u16}] => 0x8B @ le(addr)
	stax [yx] => 0xCB

	ldyx {const : i16} => 0x0A @ le(const[15:8]) @ 0x09 @ le(const[7:0])
	ldyx [{addr : u16}] => 0x4A @ le((addr+1)`16) @ 0x49 @ le(addr`16)
	ldyx [x~ {addr : u16}] => 0x8A @ le((addr + 1)`16) @ 0x89 @ le(addr`16)
	styx [{addr : u16}] => 0x4D @ le((addr+1)`16) @ 0x4C @ le(addr`16)
	styx [x~ {addr : u16}] => 0x8D @ le((addr + 1)`16) @ 0x8C @ le(addr`16)
}