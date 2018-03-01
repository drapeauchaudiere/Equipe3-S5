;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;;
;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 	.def _correlateASM

 	.data


 	.text

_correlateASM
	.asmfunc
	; A4 = return pointer, B4 = signal pointer 1, A6 = signal pointer 2, B6 = samples number
	; A2 = loop counter, A3 = loop counter, A5 = summing register
	; A7 = signal 1 value
	; A8 = signal 2 value
	; A0 = signal 1 value / A register index
	; A1 = signal 2 value / branch condition bit
	; B3 = B register index
	; B0 = loop compare value, B1 = loop compare value

	ZERO A2		; clear the incrementLoop register
	MVK 1,A5
	MV A6,B5

incrementLoop:

	ZERO A6		; clear the summing register
	ZERO A7		; clear the summing register
	ZERO A3		; clear the sumLoop index register

incrementSumLoop:

	MV A3,B1
	SUB B6,A2,B2
	ADD B1,B2,B2

	LDH *B4[B1],A8
	NOP 4
	LDH *B5[B2],A9
	NOP 4

	MPYI A8,A9,A8	; multiply the samples
	NOP 9
	SADD A8,A7:A6,A7:A6

	ADD 1,A3,A3
	CMPLTU A2,A3,A1
	[!A1] B incrementSumLoop
	NOP 5
; end of incrementSumLoop

	SAT A7:A6,A5

	STW A5, *A4++

	ADD 1,A2,A2
	CMPLTU B6,A2,A1
	[!A1] B incrementLoop
	NOP 5
; end of incrementLoop

	ZERO A2		; clear the decrementLoop register
	MV B6,A2	;
	MVK 1,A5
	SUB A2,A5,A2

decrementLoop:

	ZERO A6		; clear the summing register
	ZERO A7		; clear the summing register
	ZERO A3		; clear the sumLoop index register

decrementSumLoop:

	SUB B6,A3,B1
	SUB A2,A3,A0
	MV A0,B2

	LDH *B4[B1],A8
	NOP 4
	LDH *B5[B2],A9
	NOP 4

	MPYI A8,A9,A8	; multiply the samples
	NOP 9
	SADD A8,A7:A6,A7:A6

	ADD 1,A3,A3
	CMPGTU A3,A2,A1
	[!A1] B decrementSumLoop
	NOP 5
; end of decrementSumLoop

	SAT A7:A6,A5

	STW A5, *A4++

	MVK 1,A5
	SUB A2,A5,A2
	CMPLTU A2,A5,A1
	[!A1] B decrementLoop
	NOP 5
; end of decrementLoop

	B B3
	NOP 5

	.endasmfunc
