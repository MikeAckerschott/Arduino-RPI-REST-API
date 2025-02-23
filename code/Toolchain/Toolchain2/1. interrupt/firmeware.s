
./.pio/build/uno/firmware.elf:     file format elf32-avr


Disassembly of section .text:

00000000 <__vectors>:
   0:	0c 94 34 00 	jmp	0x68	; 0x68 <__ctors_end>
   4:	0c 94 48 00 	jmp	0x90	; 0x90 <__vector_1>
   8:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
   c:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  10:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  14:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  18:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  1c:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  20:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  24:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  28:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  2c:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  30:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  34:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  38:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  3c:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  40:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  44:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  48:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  4c:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  50:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  54:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  58:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  5c:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  60:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>
  64:	0c 94 46 00 	jmp	0x8c	; 0x8c <__bad_interrupt>

00000068 <__ctors_end>:
  68:	11 24       	eor	r1, r1
  6a:	1f be       	out	0x3f, r1	; 63
  6c:	cf ef       	ldi	r28, 0xFF	; 255
  6e:	d8 e0       	ldi	r29, 0x08	; 8
  70:	de bf       	out	0x3e, r29	; 62
  72:	cd bf       	out	0x3d, r28	; 61

00000074 <__do_clear_bss>:
  74:	21 e0       	ldi	r18, 0x01	; 1
  76:	a0 e0       	ldi	r26, 0x00	; 0
  78:	b1 e0       	ldi	r27, 0x01	; 1
  7a:	01 c0       	rjmp	.+2      	; 0x7e <.do_clear_bss_start>

0000007c <.do_clear_bss_loop>:
  7c:	1d 92       	st	X+, r1

0000007e <.do_clear_bss_start>:
  7e:	a1 30       	cpi	r26, 0x01	; 1
  80:	b2 07       	cpc	r27, r18
  82:	e1 f7       	brne	.-8      	; 0x7c <.do_clear_bss_loop>
  84:	0e 94 68 00 	call	0xd0	; 0xd0 <main>
  88:	0c 94 78 00 	jmp	0xf0	; 0xf0 <_exit>

0000008c <__bad_interrupt>:
  8c:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

00000090 <__vector_1>:
  90:	1f 92       	push	r1
  92:	0f 92       	push	r0
  94:	0f b6       	in	r0, 0x3f	; 63
  96:	0f 92       	push	r0
  98:	11 24       	eor	r1, r1
  9a:	8f 93       	push	r24
  9c:	9f 93       	push	r25
  9e:	00 00       	nop
  a0:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <_edata>
  a4:	88 23       	and	r24, r24
  a6:	89 f0       	breq	.+34     	; 0xca <__vector_1+0x3a>
  a8:	28 98       	cbi	0x05, 0	; 5
  aa:	29 9a       	sbi	0x05, 1	; 5
  ac:	90 91 00 01 	lds	r25, 0x0100	; 0x800100 <_edata>
  b0:	81 e0       	ldi	r24, 0x01	; 1
  b2:	91 11       	cpse	r25, r1
  b4:	80 e0       	ldi	r24, 0x00	; 0
  b6:	80 93 00 01 	sts	0x0100, r24	; 0x800100 <_edata>
  ba:	00 00       	nop
  bc:	9f 91       	pop	r25
  be:	8f 91       	pop	r24
  c0:	0f 90       	pop	r0
  c2:	0f be       	out	0x3f, r0	; 63
  c4:	0f 90       	pop	r0
  c6:	1f 90       	pop	r1
  c8:	18 95       	reti
  ca:	29 98       	cbi	0x05, 1	; 5
  cc:	28 9a       	sbi	0x05, 0	; 5
  ce:	ee cf       	rjmp	.-36     	; 0xac <__vector_1+0x1c>

000000d0 <main>:
  d0:	84 b1       	in	r24, 0x04	; 4
  d2:	83 60       	ori	r24, 0x03	; 3
  d4:	84 b9       	out	0x04, r24	; 4
  d6:	85 b1       	in	r24, 0x05	; 5
  d8:	8c 7f       	andi	r24, 0xFC	; 252
  da:	85 b9       	out	0x05, r24	; 5
  dc:	52 98       	cbi	0x0a, 2	; 10
  de:	5a 9a       	sbi	0x0b, 2	; 11
  e0:	80 91 69 00 	lds	r24, 0x0069	; 0x800069 <__TEXT_REGION_LENGTH__+0x7e0069>
  e4:	83 60       	ori	r24, 0x03	; 3
  e6:	80 93 69 00 	sts	0x0069, r24	; 0x800069 <__TEXT_REGION_LENGTH__+0x7e0069>
  ea:	e8 9a       	sbi	0x1d, 0	; 29
  ec:	78 94       	sei
  ee:	ff cf       	rjmp	.-2      	; 0xee <main+0x1e>

000000f0 <_exit>:
  f0:	f8 94       	cli

000000f2 <__stop_program>:
  f2:	ff cf       	rjmp	.-2      	; 0xf2 <__stop_program>
