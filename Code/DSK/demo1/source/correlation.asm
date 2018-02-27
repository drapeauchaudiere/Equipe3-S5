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


	MV A4,B3	; move the first input address out of the return register

	ZERO A2		; clear the incrementLoop register

incrementLoop:

	ZERO A5		; clear the summing register
	ZERO A3		; clear the sumLoop index register

incrementSummingLoop:

	STW A5, *A4++

; end of decrementSumLoop



; end of incrementLoop

	ZERO A2		; clear the decrementLoop register

decrementLoop:

	ZERO A5		; clear the summing register
	ZERO A3		; clear the sumLoop index register

decrementSumLoop:


; end of decrementSumLoop

; end of decrementLoop

	B B3
	NOP 5

	.endasmfunc
