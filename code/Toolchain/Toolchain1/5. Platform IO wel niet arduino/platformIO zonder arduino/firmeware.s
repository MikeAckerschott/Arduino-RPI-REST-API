
./.pio/build/uno/firmware.elf:     file format elf32-avr


Disassembly of section .text:

00000000 <__vectors>:
   0:	0c 94 34 00 	jmp	0x68	; 0x68 <__ctors_end>
   4:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
   8:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
   c:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  10:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  14:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  18:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  1c:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  20:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  24:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  28:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  2c:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  30:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  34:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  38:	0c 94 53 00 	jmp	0xa6	; 0xa6 <__vector_14>
  3c:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  40:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  44:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  48:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  4c:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  50:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  54:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  58:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  5c:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  60:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>
  64:	0c 94 51 00 	jmp	0xa2	; 0xa2 <__bad_interrupt>

00000068 <__ctors_end>:
  68:	11 24       	eor	r1, r1
  6a:	1f be       	out	0x3f, r1	; 63
  6c:	cf ef       	ldi	r28, 0xFF	; 255
  6e:	d8 e0       	ldi	r29, 0x08	; 8
  70:	de bf       	out	0x3e, r29	; 62
  72:	cd bf       	out	0x3d, r28	; 61

00000074 <__do_copy_data>:
  74:	11 e0       	ldi	r17, 0x01	; 1
  76:	a0 e0       	ldi	r26, 0x00	; 0
  78:	b1 e0       	ldi	r27, 0x01	; 1
  7a:	ec e5       	ldi	r30, 0x5C	; 92
  7c:	f1 e0       	ldi	r31, 0x01	; 1
  7e:	02 c0       	rjmp	.+4      	; 0x84 <__do_copy_data+0x10>
  80:	05 90       	lpm	r0, Z+
  82:	0d 92       	st	X+, r0
  84:	a2 30       	cpi	r26, 0x02	; 2
  86:	b1 07       	cpc	r27, r17
  88:	d9 f7       	brne	.-10     	; 0x80 <__do_copy_data+0xc>

0000008a <__do_clear_bss>:
  8a:	21 e0       	ldi	r18, 0x01	; 1
  8c:	a2 e0       	ldi	r26, 0x02	; 2
  8e:	b1 e0       	ldi	r27, 0x01	; 1
  90:	01 c0       	rjmp	.+2      	; 0x94 <.do_clear_bss_start>

00000092 <.do_clear_bss_loop>:
  92:	1d 92       	st	X+, r1

00000094 <.do_clear_bss_start>:
  94:	a5 30       	cpi	r26, 0x05	; 5
  96:	b2 07       	cpc	r27, r18
  98:	e1 f7       	brne	.-8      	; 0x92 <.do_clear_bss_loop>
  9a:	0e 94 7c 00 	call	0xf8	; 0xf8 <main>
  9e:	0c 94 ac 00 	jmp	0x158	; 0x158 <_exit>

000000a2 <__bad_interrupt>:
  a2:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

000000a6 <__vector_14>:
  a6:	1f 92       	push	r1
  a8:	0f 92       	push	r0
  aa:	0f b6       	in	r0, 0x3f	; 63
  ac:	0f 92       	push	r0
  ae:	11 24       	eor	r1, r1
  b0:	2f 93       	push	r18
  b2:	3f 93       	push	r19
  b4:	8f 93       	push	r24
  b6:	9f 93       	push	r25
  b8:	20 91 00 01 	lds	r18, 0x0100	; 0x800100 <__data_start>
  bc:	30 91 01 01 	lds	r19, 0x0101	; 0x800101 <__data_start+0x1>
  c0:	80 91 02 01 	lds	r24, 0x0102	; 0x800102 <__data_end>
  c4:	90 91 03 01 	lds	r25, 0x0103	; 0x800103 <__data_end+0x1>
  c8:	01 96       	adiw	r24, 0x01	; 1
  ca:	90 93 03 01 	sts	0x0103, r25	; 0x800103 <__data_end+0x1>
  ce:	80 93 02 01 	sts	0x0102, r24	; 0x800102 <__data_end>
  d2:	82 17       	cp	r24, r18
  d4:	93 07       	cpc	r25, r19
  d6:	38 f0       	brcs	.+14     	; 0xe6 <__vector_14+0x40>
  d8:	81 e0       	ldi	r24, 0x01	; 1
  da:	80 93 04 01 	sts	0x0104, r24	; 0x800104 <timer_flag>
  de:	10 92 03 01 	sts	0x0103, r1	; 0x800103 <__data_end+0x1>
  e2:	10 92 02 01 	sts	0x0102, r1	; 0x800102 <__data_end>
  e6:	9f 91       	pop	r25
  e8:	8f 91       	pop	r24
  ea:	3f 91       	pop	r19
  ec:	2f 91       	pop	r18
  ee:	0f 90       	pop	r0
  f0:	0f be       	out	0x3f, r0	; 63
  f2:	0f 90       	pop	r0
  f4:	1f 90       	pop	r1
  f6:	18 95       	reti

000000f8 <main>:
  f8:	84 b1       	in	r24, 0x04	; 4
  fa:	80 63       	ori	r24, 0x30	; 48
  fc:	84 b9       	out	0x04, r24	; 4
  fe:	54 98       	cbi	0x0a, 4	; 10
 100:	84 b5       	in	r24, 0x24	; 36
 102:	82 60       	ori	r24, 0x02	; 2
 104:	84 bd       	out	0x24, r24	; 36
 106:	85 b5       	in	r24, 0x25	; 37
 108:	85 60       	ori	r24, 0x05	; 5
 10a:	85 bd       	out	0x25, r24	; 37
 10c:	8e e0       	ldi	r24, 0x0E	; 14
 10e:	87 bd       	out	0x27, r24	; 39
 110:	80 91 6e 00 	lds	r24, 0x006E	; 0x80006e <__TEXT_REGION_LENGTH__+0x7e006e>
 114:	82 60       	ori	r24, 0x02	; 2
 116:	80 93 6e 00 	sts	0x006E, r24	; 0x80006e <__TEXT_REGION_LENGTH__+0x7e006e>
 11a:	78 94       	sei
 11c:	44 ef       	ldi	r20, 0xF4	; 244
 11e:	51 e0       	ldi	r21, 0x01	; 1
 120:	62 e3       	ldi	r22, 0x32	; 50
 122:	70 e0       	ldi	r23, 0x00	; 0
 124:	20 e2       	ldi	r18, 0x20	; 32
 126:	90 e1       	ldi	r25, 0x10	; 16
 128:	4c 99       	sbic	0x09, 4	; 9
 12a:	11 c0       	rjmp	.+34     	; 0x14e <main+0x56>
 12c:	70 93 01 01 	sts	0x0101, r23	; 0x800101 <__data_start+0x1>
 130:	60 93 00 01 	sts	0x0100, r22	; 0x800100 <__data_start>
 134:	80 91 04 01 	lds	r24, 0x0104	; 0x800104 <timer_flag>
 138:	88 23       	and	r24, r24
 13a:	b1 f3       	breq	.-20     	; 0x128 <main+0x30>
 13c:	85 b1       	in	r24, 0x05	; 5
 13e:	82 27       	eor	r24, r18
 140:	85 b9       	out	0x05, r24	; 5
 142:	85 b1       	in	r24, 0x05	; 5
 144:	89 27       	eor	r24, r25
 146:	85 b9       	out	0x05, r24	; 5
 148:	10 92 04 01 	sts	0x0104, r1	; 0x800104 <timer_flag>
 14c:	ed cf       	rjmp	.-38     	; 0x128 <main+0x30>
 14e:	50 93 01 01 	sts	0x0101, r21	; 0x800101 <__data_start+0x1>
 152:	40 93 00 01 	sts	0x0100, r20	; 0x800100 <__data_start>
 156:	ee cf       	rjmp	.-36     	; 0x134 <main+0x3c>

00000158 <_exit>:
 158:	f8 94       	cli

0000015a <__stop_program>:
 15a:	ff cf       	rjmp	.-2      	; 0x15a <__stop_program>
