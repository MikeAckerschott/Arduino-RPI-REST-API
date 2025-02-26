
build\nano_blink.elf:     file format elf32-avr


Disassembly of section .text:

00000000 <__vectors>:
   0:	0c 94 34 00 	jmp	0x68	; 0x68 <__ctors_end>
   4:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
   8:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
   c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  10:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  14:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  18:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  1c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  20:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  24:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  28:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  2c:	0c 94 40 00 	jmp	0x80	; 0x80 <__vector_11>
  30:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  34:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  38:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  3c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  40:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  44:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  48:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  4c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  50:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  54:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  58:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  5c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  60:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  64:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>

00000068 <__ctors_end>:
  68:	11 24       	eor	r1, r1
  6a:	1f be       	out	0x3f, r1	; 63
  6c:	cf ef       	ldi	r28, 0xFF	; 255
  6e:	d8 e0       	ldi	r29, 0x08	; 8
  70:	de bf       	out	0x3e, r29	; 62
  72:	cd bf       	out	0x3d, r28	; 61
  74:	0e 94 58 00 	call	0xb0	; 0xb0 <main>
  78:	0c 94 79 00 	jmp	0xf2	; 0xf2 <_exit>

0000007c <__bad_interrupt>:
  7c:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

00000080 <__vector_11>:
  80:	1f 92       	push	r1
  82:	0f 92       	push	r0
  84:	0f b6       	in	r0, 0x3f	; 63
  86:	0f 92       	push	r0
  88:	11 24       	eor	r1, r1
  8a:	8f 93       	push	r24
  8c:	9f 93       	push	r25
  8e:	00 00       	nop
  90:	85 b1       	in	r24, 0x05	; 5
  92:	91 e0       	ldi	r25, 0x01	; 1
  94:	89 27       	eor	r24, r25
  96:	85 b9       	out	0x05, r24	; 5
  98:	85 b1       	in	r24, 0x05	; 5
  9a:	92 e0       	ldi	r25, 0x02	; 2
  9c:	89 27       	eor	r24, r25
  9e:	85 b9       	out	0x05, r24	; 5
  a0:	00 00       	nop
  a2:	9f 91       	pop	r25
  a4:	8f 91       	pop	r24
  a6:	0f 90       	pop	r0
  a8:	0f be       	out	0x3f, r0	; 63
  aa:	0f 90       	pop	r0
  ac:	1f 90       	pop	r1
  ae:	18 95       	reti

000000b0 <main>:
  b0:	84 b1       	in	r24, 0x04	; 4
  b2:	83 60       	ori	r24, 0x03	; 3
  b4:	84 b9       	out	0x04, r24	; 4
  b6:	85 b1       	in	r24, 0x05	; 5
  b8:	8c 7f       	andi	r24, 0xFC	; 252
  ba:	85 b9       	out	0x05, r24	; 5
  bc:	85 b1       	in	r24, 0x05	; 5
  be:	92 e0       	ldi	r25, 0x02	; 2
  c0:	89 27       	eor	r24, r25
  c2:	85 b9       	out	0x05, r24	; 5
  c4:	80 91 81 00 	lds	r24, 0x0081	; 0x800081 <__DATA_REGION_ORIGIN__+0x21>
  c8:	88 60       	ori	r24, 0x08	; 8
  ca:	80 93 81 00 	sts	0x0081, r24	; 0x800081 <__DATA_REGION_ORIGIN__+0x21>
  ce:	88 e0       	ldi	r24, 0x08	; 8
  d0:	9d e3       	ldi	r25, 0x3D	; 61
  d2:	90 93 89 00 	sts	0x0089, r25	; 0x800089 <__DATA_REGION_ORIGIN__+0x29>
  d6:	80 93 88 00 	sts	0x0088, r24	; 0x800088 <__DATA_REGION_ORIGIN__+0x28>
  da:	80 91 6f 00 	lds	r24, 0x006F	; 0x80006f <__DATA_REGION_ORIGIN__+0xf>
  de:	82 60       	ori	r24, 0x02	; 2
  e0:	80 93 6f 00 	sts	0x006F, r24	; 0x80006f <__DATA_REGION_ORIGIN__+0xf>
  e4:	80 91 81 00 	lds	r24, 0x0081	; 0x800081 <__DATA_REGION_ORIGIN__+0x21>
  e8:	85 60       	ori	r24, 0x05	; 5
  ea:	80 93 81 00 	sts	0x0081, r24	; 0x800081 <__DATA_REGION_ORIGIN__+0x21>
  ee:	78 94       	sei
  f0:	ff cf       	rjmp	.-2      	; 0xf0 <main+0x40>

000000f2 <_exit>:
  f2:	f8 94       	cli

000000f4 <__stop_program>:
  f4:	ff cf       	rjmp	.-2      	; 0xf4 <__stop_program>
