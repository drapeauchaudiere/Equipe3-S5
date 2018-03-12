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
	; A0 = signal 1 value / A register index
	; A1 = signal 2 value / branch condition bit
	; B3 = B register index
	; B0 = loop compare value, B1 = loop compare value

	ZERO A2			; clear the incrementLoop register
	MVK 1,A5
	SUB B6,A5,B6	; scale the numbre of samples for arrays starting at 0

incrementLoop:

	ZERO A5		; clear the summing register
	ZERO A3		; clear the sumLoop index register

incrementSumLoop:

	MV A3,B2
	SUB B6,A2,A0
	ADD B2,A0,A0

	LDW *A6[A0],A1
	NOP 4
	LDW *B4[B2],A0
	NOP 4

	MPY A0,A1,A0	; multiply the samples
	NOP
	ADD A0,A5,A5	; add the product to the sum

	ADD 1,A3,A3		; increment the sumLoop index register
	CMPLTU A2,A3,A1	;
	[!A1] B incrementSumLoop
	NOP 5
; end of incrementSumLoop

	STW A5, *A4++
	; Test GIT

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

	ZERO A5		; clear the summing register
	ZERO A3		; clear the sumLoop index register

decrementSumLoop:

	SUB B6,A3,B2
	SUB A2,A3,A0

	LDW *A6[A0],A1
	NOP 4
	LDW *B4[B2],A0
	NOP 4

	MPY A0,A1,A0	; multiply the samples
	NOP
	ADD A0,A5,A5	; add the product to the sum

	ADD 1,A3,A3
	CMPGTU A3,A2,A1
	[!A1] B decrementSumLoop
	NOP 5
; end of decrementSumLoop

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
