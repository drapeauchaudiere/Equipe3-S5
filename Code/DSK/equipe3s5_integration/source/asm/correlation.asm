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
	; A4 = return pointer,
	; B4 = signal pointer 1
	; A6 = signal pointer 2
	; B6 = samples number

	; A0 = signal 1 value / A register index
	; A1 = branch condition bit
	; A2 = loop counter
	; A3 = loop counter
	; A5 = summing register
	; B0 = signal 2 value

	ZERO A2			; clear the incrementLoop register

incrementLoop:

	ZERO A5		; clear the summing register
	ZERO A3		; clear the sumLoop index register

incrementSumLoop:

	MV A3,B0		; move the sumLoop index to a B register
||	SUB B6,A2,A0	; subtract the incrementLoop index from the samples number,
	ADD A3,A0,A0    ; than add the sumLoop : (N-i+j)

	LDW *A6[A0],A0	; load y(N-i+j) sample
||	LDW *B4[B0],B0  ; load x(j) sample
	NOP 4

	MPY A0,B0,A0	; multiply the samples
	ADD 1,A3,A3		; increment the sumLoop index (optimize to remove NOP)
	ADD A0,A5,A5	; add the product to the sum


||	CMPLTU A2,A3,A1	; if j <= i loop
	[!A1] B incrementSumLoop
	NOP 5
; end of incrementSumLoop

	STW A5, *A4++	; store the sum in the next output value

||	ADD 1,A2,A2		; increment the incrementLoop index
	CMPLTU B6,A2,A1	; if i <= N loop
	[!A1] B incrementLoop
	NOP 5
; end of incrementLoop

	MV B6,A2		; set the loop index to N,
||	MVK 1,A5
	SUB A2,A5,A2	; substract 1 : i = N-1

decrementLoop:

	ZERO A5		; clear the summing register
	ZERO A3		; clear the sumLoop index register

decrementSumLoop:

	SUB B6,A3,B0	; N-j
||	SUB A2,A3,A0	; i-j

	LDW *A6[A0],A0	; load x(N-j)
||	LDW *B4[B0],B0  ; load y(i-j)
	NOP 4

	MPY A0,B0,A0	; multiply the samples
	ADD 1,A3,A3		; increment the sumLoop index (optimize to remove NOP)
	ADD A0,A5,A5	; add the product to the sum

||	CMPGTU A3,A2,A1	; if j < i loop
	[!A1] B decrementSumLoop
	NOP 5
; end of decrementSumLoop

	STW A5, *A4++

||	MVK 1,A3
	SUB A2,A3,A2	; decrement the decrementLoop index
	CMPLTU A2,A3,A1 ; if i > 0 loop
	[!A1] B decrementLoop
	NOP 5
; end of decrementLoop

	B B3
	NOP 5

	.endasmfunc
